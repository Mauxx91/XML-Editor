/**
 * File: XmlRelationCollection.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#include "XmlRelationCollection.h"

#include "../Observer/IObserver.h"

#include <QDir>
#include <QDomNode>
#include <QDomText>

XmlRelationCollection::XmlRelationCollection(): m_relativeFileNameStandardConfFile("conf.xec"),
    m_rootTagName("xmlEditorConf"),
    m_relationCollectionTagName("xmlRelationCollection"),
    m_xmlRelationTagName("xmlRelation")
{
    //Activate autosave
    m_autoSave = true;

    //At the start set true
    m_updateEnabled = true;

    //Initialize the abosulte path using the current directory of the exe
    m_abosulteFileNameStandardConfFile = QDir::current().absolutePath() + "/" + m_relativeFileNameStandardConfFile;

    //Create the xml document used for manage the configuration file
    m_pXmlDocument = new XmlDocument();

    //Set the standardFileName
    m_pXmlDocument->ChangeFileName(m_abosulteFileNameStandardConfFile);

    //Set as observer of itself because it wants to know when the collection is updated
    this->AttachObserver(this);
}

XmlRelationCollection::~XmlRelationCollection()
{
    delete m_pXmlDocument;

    //Deactivate the update during the destruction
    m_updateEnabled = false;

    //Delete all the inserted relation
    Clear();
}

void XmlRelationCollection::ClearAndLoadXmlCollectionFromXmlDocumentOpened()
{
    //Clear all the collection
    Clear();

    //If an xml document is setted
    const QDomNode& rootNode = m_pXmlDocument->GetQDomDocument()->firstChildElement();

    //If  the root element is an element and the tag name is right go on
    if(rootNode.isElement() && rootNode.nodeName() == m_rootTagName)
    {
        //Get the fist child node called as passed to the function
        const QDomNode& firstChildNode = rootNode.firstChildElement(m_relationCollectionTagName);

        //If the element is not a null element
        if(!firstChildNode.isNull())
        {
            //Obtain all the child node of this node
            const QDomNodeList& childNodesList = firstChildNode.childNodes();

            //For each node check if it is an "xmlRelation" node and if it is call the function for insert the relation
            for(int i=0; i<childNodesList.size(); ++i)
            {
                //The examinated node
                const QDomNode& examinatedNode = childNodesList.at(i);

                //Check if it is an "xmlRelation"
                if(examinatedNode.isElement() && examinatedNode.nodeName() == m_xmlRelationTagName)
                {
                    //New relation
                    XmlRelation* xmlRelation = new XmlRelation;

                    //Fill the xmlRelation with the xml
                    xmlRelation->SetParseXmlRelationNode(&examinatedNode);

                    //Add itself as observer
                    xmlRelation->AttachObserver(this);

                    //Add the xmlRelation in the collection
                    m_xmlRelationList.append(xmlRelation);
                }
            }
        }
    }
}

void XmlRelationCollection::UpdateXmlDocumentWithRelationCollection()
{
    QDomDocument* xmlDocument = m_pXmlDocument->GetQDomDocument();

    //Clear the previous data
    xmlDocument->clear();

    //Create the root tag and append it to the document
    QDomElement rootNode = xmlDocument->createElement(m_rootTagName);
    xmlDocument->appendChild(rootNode);

    //Create the node for the xml collection
    QDomElement xmlCollectionNode = xmlDocument->createElement(m_relationCollectionTagName);
    rootNode.appendChild(xmlCollectionNode);

    //For each xml relation collection append a xmlRelation tag with the relation to the xmlCollection node
    for(int i=0; i<m_xmlRelationList.size(); ++i)
    {
        //Create a new xmlRelation node and append to the xmlCollection node
        QDomElement xmlRelationNode = xmlDocument->createElement(m_xmlRelationTagName);

        //Fill the xml relation node with the relation data
        m_xmlRelationList.at(i)->FillQDomNodeWithTheRelation(&xmlRelationNode, xmlDocument);

        //Add the filled node as child of the relation collection node
        xmlCollectionNode.appendChild(xmlRelationNode);
    }

    //Add the xml preable
    QDomNode& xmlPreambleNode = xmlDocument->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    xmlDocument->insertBefore( xmlPreambleNode, xmlDocument->firstChild() );

}

FILE_XML_ERR_CODE XmlRelationCollection::LoadSettedFileConf()
{
    //Open the configuration xml and store the result code
    FILE_XML_ERR_CODE errCode = m_pXmlDocument->OpenXmlFile();

    //If opened call the notify
    if(errCode == FILE_XML_NO_ERR)
    {
        m_updateEnabled = false;

        //Clear the current xml relation collection and refill with the content of the xml document
        ClearAndLoadXmlCollectionFromXmlDocumentOpened();

        Notify(UPDATE_XML_RELATIONS);

        m_updateEnabled = true;
    }

    return errCode;
}

FILE_XML_ERR_CODE XmlRelationCollection::SaveToSettedFileConf()
{
    //Update the xml document with the content of the collection
    UpdateXmlDocumentWithRelationCollection();

    //Save the modification in the file
    return m_pXmlDocument->SaveXmlFile();
}

FILE_XML_ERR_CODE XmlRelationCollection::SaveToAnotherFileConf(const QString& absoluteFileName)
{
    //Save to another file conf manteining the standard file conf

    //Save in the other path
    FILE_XML_ERR_CODE errCode = m_pXmlDocument->SaveXmlFileWithName(absoluteFileName);

    //Re set the standard conf fileName
    m_pXmlDocument->ChangeFileName(m_abosulteFileNameStandardConfFile);

    return errCode;
}

FILE_XML_ERR_CODE XmlRelationCollection::LoadNonStandardFileConf(const QString& absoluteFileName)
{
    //Load another file conf and after set the standard file conf for save the new modifies

    m_pXmlDocument->ChangeFileName(absoluteFileName);

    FILE_XML_ERR_CODE errCode = LoadSettedFileConf();

    //Set back the standard config file name
    m_pXmlDocument->ChangeFileName(m_abosulteFileNameStandardConfFile);

    SaveToSettedFileConf();

    return errCode;
}

const XmlDocument* XmlRelationCollection::GetXmlDocument() const
{
    return m_pXmlDocument;
}

void XmlRelationCollection::AddXmlRelation(XmlRelation* xmlRelation)
{
    m_xmlRelationList.append(xmlRelation);

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelationCollection::RemoveXmlRelation(int index)
{
    //if the index is correct
    if(index >=0 && index < m_xmlRelationList.size())
    {
        //Delete the xml relation
        delete m_xmlRelationList.at(index);

        //Remove from the list
        m_xmlRelationList.removeAt(index);

        Notify(UPDATE_XML_RELATIONS);
    }
}

int XmlRelationCollection::Size() const
{
    return m_xmlRelationList.size();
}

void XmlRelationCollection::Clear()
{
    //Delete all the inserted relation
    for(int i = m_xmlRelationList.size()-1;  i>=0; --i)
    {
        delete m_xmlRelationList.at(i);
        m_xmlRelationList.removeAt(i);
    }

    Notify(UPDATE_XML_RELATIONS);
}

XmlRelation* XmlRelationCollection::GetRelationAt(int index) const
{
    return m_xmlRelationList.at(index);
}

void XmlRelationCollection::Update(UPDATE_CODE updateCode)
{
    //if the relation has changed and the autosave is true
    if(updateCode == UPDATE_XML_RELATIONS && m_autoSave && m_updateEnabled)
    {
        SaveToSettedFileConf();
    }
}
