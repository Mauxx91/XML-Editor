/**
 * File: AttributeNameTagCollapse.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#include "XmlDocument.h"

#include "../Observer/IObserver.h"

#include <QDomDocument>
#include <QFileInfo>
#include <QTextStream>
#include <QString>

XmlDocument::XmlDocument(): m_absoluteFileName("")
{
    m_isModified = false;

    //Initialize dom document
    m_pDomDocument = new QDomDocument;

    //Initialize the file
    m_pFile = new QFile;
}

XmlDocument::~XmlDocument()
{
    delete m_pDomDocument;
    delete m_pFile;
}

void XmlDocument::ChangeFileName(const QString& absoluteFileName)
{
    //Initiliaze the file
    m_pFile->setFileName(absoluteFileName);

    //Set the file name
    m_absoluteFileName = absoluteFileName;
}

void XmlDocument::NewXmlFile(const QString& absoluteFileName)
{
    //Set as modified because as new it need to has saved
    m_isModified = true;

    //Reinitialize the object to a clear state that well represent a new empty xml
    m_pDomDocument->clear(); //empty the dom document

    ChangeFileName(absoluteFileName);
}

FILE_XML_ERR_CODE XmlDocument::OpenXmlFile(const QString& absoluteFileName)
{
    //Set as not modified
    m_isModified = false;

    //empty the dom document
    m_pDomDocument->clear();

    //If the absoluteFileName in input is not empty set as new file
    if(absoluteFileName != "")
    {
        ChangeFileName(absoluteFileName);
    }

    //Used for read the property of the file
    QFileInfo fileInfo;
    fileInfo.setFile(*m_pFile);

    //Check if the file exist
    if(!fileInfo.exists())
    {
        return FILE_XML_FILE_NOT_EXIST;
    }

    if(!fileInfo.isFile())
    {
        return FILE_XML_FILE_IS_NOT_FILE;
    }

    if(!fileInfo.isReadable())
    {
        FILE_XML_FILE_IS_NOT_READABLE;
    }

    //Try to open the file
    if( !m_pFile->open(QIODevice::ReadOnly) )
    {
        return FILE_XML_UNABLE_OPEN_FILE;
    }

    //Try to load the file content in the Dom tree, false is for no namesace processing
    if( !m_pDomDocument->setContent(m_pFile, false, &m_errorString, &m_errorLine))
    {
        return FILE_XML_UNABlE_FILL_DOM_TREE;
    }

    //Close the file
    m_pFile->close();

    //Update the file name
    m_absoluteFileName = absoluteFileName;

    Notify(UPDATE_XML_DOCUMENT);

    //All ok return no error
    return FILE_XML_NO_ERR;
}

FILE_XML_ERR_CODE XmlDocument::SaveXmlFile()
{    
    //Empty the file
    m_pFile->resize(0);

    //Used for read the property of the file
    QFileInfo fileInfo;
    fileInfo.setFile(*m_pFile);

    if(!fileInfo.isFile())
    {
        return FILE_XML_FILE_IS_NOT_FILE;
    }

    if(!fileInfo.isWritable())
    {
        return FILE_XML_FILE_IS_NOT_WRITABLE;
    }

    //Open the file for write
    if( !m_pFile->open(QIODevice::WriteOnly) )
    {
        return FILE_XML_UNABLE_OPEN_FILE;
    }

    //Set as not modified
    m_isModified = false;

    //Create a stream for write
    QTextStream stream;
    stream.setDevice(m_pFile); //Set the where the stream will write
    stream.setCodec("UTF-8"); //Set encoding

    //Write the content of the xml document with the stream
    m_pDomDocument->save(stream, QDomNode::EncodingFromTextStream); //use the encoding setted in the stream

    //Close the file
    m_pFile->close();

    //All ok return no error
    return FILE_XML_NO_ERR;
}

FILE_XML_ERR_CODE XmlDocument::SaveXmlFileWithName(const QString& absoluteFilePath)
{
    //Change file name
    ChangeFileName(absoluteFilePath);

    //Save with the new file name
    return SaveXmlFile();
}

QString XmlDocument::GetAbsoluteFileName() const
{
    QString ab = m_absoluteFileName;
    return ab.replace(QString("\\"), QString("/"), Qt::CaseInsensitive);
}

QDomDocument *XmlDocument::GetQDomDocument()
{
    return m_pDomDocument;
}

const QString& XmlDocument::GetErrorString() const
{
    return m_errorString;
}

int XmlDocument::GetErrorLine() const
{
    return m_errorLine;
}

bool XmlDocument::IsModified() const
{
    return m_isModified;
}

int XmlDocument::CountChildNode(const QDomNode& parentNode) const
{
    //Count the valid child node
    QDomNodeList childList = parentNode.childNodes();
    int numChildNode = 0;
    for(int i=0; i < childList.size(); ++i)
    {
        if(childList.at(i).isElement() && !childList.at(i).isComment())
        {
            numChildNode++;
        }
    }
    return numChildNode;
}

QString XmlDocument::AddChildNode(const QString& parentIdentifier, const QString& nodeType)
{
    QDomElement newNode = m_pDomDocument->createElement(nodeType);

    return AddChildNode(parentIdentifier, newNode);
}

QString XmlDocument::AddChildNode(const QString& parentIdentifier, const QDomNode& newNode)
{
    //Get the parent node of the new empty node
    QDomNode parentNode = GetDomNode(parentIdentifier);

    int numChildNode = CountChildNode(parentNode);

    //Compute the child identifier number
    QString indentifierNumber = parentIdentifier + "." + QString::number(numChildNode+1);

    //Add the new node to the parent node
    QDomNode insertedNode = parentNode.appendChild(newNode.cloneNode(true));

    //notify of the change and set as modfies
    m_isModified = true;
    Notify(UPDATE_XML_DOCUMENT);

    return indentifierNumber;
}

QDomNode XmlDocument::DeleteNode(const QString& identifierNumber)
{
    //Get the node to delete
    QDomNode nodeTodelete = GetDomNode(identifierNumber);

    QDomNode deletedNodeCopy = nodeTodelete.cloneNode(true);

    //Remove the node
    nodeTodelete.parentNode().removeChild(nodeTodelete);

    //notify of the change and set as modfies
    m_isModified = true;
    Notify(UPDATE_XML_DOCUMENT);

    return deletedNodeCopy;
}

QList<QDomNode> XmlDocument::DeleteAllChildNode(const QString& parentIdentifier, const QString& tagName)
{
    //Get the node to delete
    QDomNode parentNode = GetDomNode(parentIdentifier);

    QList<QDomNode> deletedNode;

    //If has to delete all child node
    if(tagName == "")
    {
        QDomNodeList parentChilds = parentNode.childNodes();
        for(int i=0; i < parentChilds.size(); ++i)
        {
            deletedNode.append(parentChilds.at(i).cloneNode(true));
        }
        //Clone the current node without the child, and replace the old node with the new
        QDomNode clonedParentNode = parentNode.cloneNode(false);
        parentNode.parentNode().replaceChild(clonedParentNode, parentNode);
    }
    else //If has to delete all child node that match the tag name given
    {
        //Obtain all the child node of this node
        QDomNodeList childNodesList = parentNode.childNodes();

        //Remove all the child node
        for(int i=childNodesList.size(); i >= 0 ; --i)
        {
            //If the child has the same tag name
            if(childNodesList.at(i).nodeName() == tagName)
            {
                //save the node
                deletedNode.append(childNodesList.at(i).cloneNode(true));

                //remove the node
                parentNode.removeChild(childNodesList.at(i));
            }
        }
    }

    //notify of the change and set as modfies
    m_isModified = true;
    Notify(UPDATE_XML_DOCUMENT);

    return deletedNode;
}

QString XmlDocument::AddOrSetAttribute(const QString& identifierNumber, const QString& attrName, const QString& value)
{
    QDomElement node = GetDomNode(identifierNumber).toElement();

    QString previousValue = node.attribute(attrName);

    node.setAttribute(attrName, value);

    //notify of the change and set as modfies
    m_isModified = true;
    Notify(UPDATE_XML_DOCUMENT);

    return previousValue;
}

QString XmlDocument::removeAttribute(const QString& identifierNumber, const QString& attrName)
{
    QDomElement node = GetDomNode(identifierNumber).toElement();

    QString previousValue = node.attribute(attrName);

    node.removeAttribute(attrName);

    //notify of the change and set as modfies
    m_isModified = true;
    Notify(UPDATE_XML_DOCUMENT);

    return previousValue;
}

QString XmlDocument::SetNodeValue(const QString& identifierNumber, const QString& value)
{
    QDomElement node = GetDomNode(identifierNumber).toElement();

    QString previousValue = node.toElement().text();

    bool hasATextNode = false;

    //Flow the child node in search of the text node
    QDomNodeList chidNodes = node.childNodes();
    for(int i=0; i < chidNodes.size() && !hasATextNode; ++i)
    {
        QDomText childNode= chidNodes.at(i).toText();

        if(!childNode.isNull())
        {
            hasATextNode = true;
            //Set the value
            childNode.setData(value);
        }
    }

    //If a node text is not present add a new one
    if(!hasATextNode)
    {
        QDomText newNodeText = GetQDomDocument()->createTextNode(value);
        node.appendChild(newNodeText);
    }

    //notify of the change and set as modfies
    m_isModified = true;
    Notify(UPDATE_XML_DOCUMENT);

    return previousValue;
}

QStringList XmlDocument::GetAllKeyOfFistLevelChild(const QDomNode& node, const QString& tagName, const QString& attrName) const
{
    QStringList localKeys;

    if(!node.isNull() && node.isElement() && !node.isComment())
    {
        QDomNodeList childNodes = node.childNodes();

        for(int i=0; i < childNodes.size(); ++i)
        {
            const QDomNode childNode = childNodes.at(i);

            if(childNode.isElement() && tagName == childNode.nodeName() && childNode.toElement().hasAttribute(attrName))
            {
                localKeys << childNode.toElement().attribute(attrName);
            }
        }
    }
    return localKeys;
}

QStringList XmlDocument::GetAllKeyOfTag(const QDomNode& node, const QString& tagName, const QString& attrName) const
{
    QStringList localKeys;

    if(!node.isNull())
    {
        if(node.isElement() && !node.isComment())
        {
            //If the tag is correct and has an attribute that contains the key
            if(tagName == node.nodeName() && node.toElement().hasAttribute(attrName))
            {
                localKeys << node.toElement().attribute(attrName);
            }

            //Recursive call on the childs node
            localKeys += GetAllKeyOfTag(node.firstChildElement(), tagName, attrName);
        }

        //Resursive call on the sibling node
        localKeys += GetAllKeyOfTag(node.nextSiblingElement(), tagName, attrName);
    }
    return localKeys;
}

QString XmlDocument::GetFirtEmptyKey(const QString& baseKey, const QString& tagName, const QString& attrName)
{
    //Get all the key
    QStringList keysList = GetAllKeyOfTag(GetQDomDocument()->firstChildElement(), tagName, attrName);

    bool keyFound = false;
    QString key;

    //Search the firt empty key
    for(int i=1; !keyFound; ++i)
    {
        //try this key
        key = baseKey + QString::number(i);

        bool keyIsEmpty = true;

        for(int k=0; k < keysList.size() && keyIsEmpty; ++k)
        {
            //If key is already used
            if(key == keysList.at(k))
            {
                keyIsEmpty = false;
            }
        }

        //If the key is empty stop the cycle in order to return that key
        if(keyIsEmpty)
        {
            keyFound = true;
        }
    }

    return key;
}

QString XmlDocument::GetKeyFromIdentifierNumber(const QString& identifierNumber, const QString& attributeName) const
{
    //Get the node
    QDomElement node = GetDomNode(identifierNumber).toElement();

    //return the attribute
    return node.attribute(attributeName);
}

QDomNode XmlDocument::GetDomNode(const QString& identifier) const
{
    QDomNode* node = RealGetDomNode(&(m_pDomDocument->firstChildElement()), identifier, "", 1);

    QDomNode nodeToReturn;

    if(node)
    {
        nodeToReturn = *node;
        delete node;
    }
    else
    {
        nodeToReturn = m_pDomDocument->createElement("RootElement");
        m_pDomDocument->appendChild(nodeToReturn);
    }
    return nodeToReturn;
}

QDomNode* XmlDocument::RealGetDomNode(QDomNode* nodeToCheck, const QString& idetifierNumberToSearch,
                                      const QString& parentNumberBase, int number) const
{
    if(!nodeToCheck->isNull())
    {
        if(nodeToCheck->isElement() && !nodeToCheck->isComment())
        {
            //Calcolate the hierarchical number
            QString completeIdentifiersNumber = parentNumberBase;
            //If it's not empty is because it's the first node and it needn't the dot seprator
            if(!parentNumberBase.isEmpty())
            {
                completeIdentifiersNumber.append('.');
            }
            //Attach the number
            completeIdentifiersNumber.append(QString::number(number));

            //if the current node is the searched node return this
            if(idetifierNumberToSearch == completeIdentifiersNumber)
            {
                return new QDomNode(*nodeToCheck);
            }

            //Recursive call that ensure to explore all the document element
            QDomNode* result = RealGetDomNode(&nodeToCheck->firstChildElement(), idetifierNumberToSearch, completeIdentifiersNumber, 1);

            //If the node was found in the child nodes return
            if(result != 0)
            {
                return result;
            }
        }

        //The node was not found in the child node so has to be present in a sibling node
        return RealGetDomNode(&nodeToCheck->nextSiblingElement(), idetifierNumberToSearch, parentNumberBase, number+1);
    }
    //The node was not found in this path
    return 0;
}

