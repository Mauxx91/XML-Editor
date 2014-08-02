/**
 * File: XmlTreeModel.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#include "XmlTreeModel.h"

#include "XmlDocument.h"
#include "AttributeNameTagCollapse.h"
#include "XmlTreeItem.h"

#include <QDomNode>
#include <QList>
#include <QMap>

XmlTreeModel::ExtendedDomNode::ExtendedDomNode(const QDomNode& node): QDomNode(node)
{
    m_number = 0;
}

XmlTreeModel::XmlTreeModel(AttributeNameTagCollapse* attributeNameTagCollapse, XmlDocument* xmlDocument, bool showTagIndentfiesNumbers) :QStandardItemModel(0)
{
    SetXmlDocument(xmlDocument);

    m_pAttributeNameTagCollapse = attributeNameTagCollapse;

    m_showTagIndentfiesNumbers = showTagIndentfiesNumbers;

    //Register to the tag name collapse Subject in order to update the model when it changes
    m_pAttributeNameTagCollapse->AttachObserver(this);

    //Register as observer to the xml document
    xmlDocument->AttachObserver(this);

    //Fill the model with the setted document
    FillTheModel();
}

void XmlTreeModel::Update(UPDATE_CODE updateCode)
{
    if(updateCode == UPDATE_ATTRIBUTE_NAME_TAG || updateCode == UPDATE_XML_DOCUMENT)
    {
        //Clear the model and fill again
        ReFillTheModel();
    }
}

void XmlTreeModel::SetXmlDocument(XmlDocument* xmlDocument)
{
    //Set xml documet
    m_pXmlDocument = xmlDocument;

    //Register as observer to the xmlDocument
    if(m_pXmlDocument) //If it's a valid document
        m_pXmlDocument->AttachObserver(this);
}

void XmlTreeModel::SetShowTagIndentfiesNumbers(bool show)
{
    //Only the boolean it's dirrent
    if(m_showTagIndentfiesNumbers != show)
    {
        //Set the new bool
        m_showTagIndentfiesNumbers = show;

        //Re fill the model to apply the changes
        ReFillTheModel();
    }
}

XmlDocument* XmlTreeModel::GetXmlDocument() const
{
    return m_pXmlDocument;
}

void XmlTreeModel::ReFillTheModel()
{
    //Clear the model and fill again
    this->clear();
    FillTheModel();
}

void XmlTreeModel::DivideNodesForTagName(QMap<QString, QList<XmlTreeModel::ExtendedDomNode>>& o_sameTagListMap, QDomNodeList childNodesList) const
{
    //Number of the element child of the normal node
    int numElementChild = 0;

    //To separate the element flow among the child list
    for(int i=0; i<childNodesList.size(); ++i)
    {
        //The examinated node
        const QDomNode& examinatedNode = childNodesList.at(i);

        //Insert the node only if it's a element
        if(examinatedNode.isElement())
        {
            numElementChild++;

            //Tag name of the node
            const QString& tagName = examinatedNode.nodeName();

            //Find if a list for this tag name already exists
            QMap<QString, QList<ExtendedDomNode>>::Iterator iteratorMapList = o_sameTagListMap.find(tagName);

            //If the list doesn't exist it will be created
            if(iteratorMapList == o_sameTagListMap.end())
            {
                //Create the list for this new tag name
                iteratorMapList = o_sameTagListMap.insert(tagName, QList<ExtendedDomNode>());
            }

            //Perdonalize the node
            ExtendedDomNode node(examinatedNode.cloneNode());
            node.m_number = numElementChild; //Set the last number of the indetifier number string

            //Insert in the list the node
            (*iteratorMapList).append(node);
        }
    }
}

QString XmlTreeModel::CheckNodeToDisplayUsingAttributeFilter(const QDomNode* domNode, const QString& attributeFilter) const
{
    QString nameToDisplay;

    const QDomNamedNodeMap& attributeNodesList = domNode->attributes();

    //For exit when the fist match is found
    bool firstMatchFound = false;

    //Flow the list searching the
    for(int i=0; i<attributeNodesList.size() && !firstMatchFound; ++i)
    {
        const QDomAttr& attritubeNode = attributeNodesList.item(i).toAttr();
        const QString& attributeName = attritubeNode.name();

        //If the attribute match set to display the value
        if( attributeName.contains(attributeFilter, Qt::CaseInsensitive) )
        {
            nameToDisplay = attritubeNode.value();
            //It's desiderate the first match so exit from the cicle
            firstMatchFound = true;
        }
    }
    //If no match was found set the tag name
    if(!firstMatchFound)
    {
        nameToDisplay = domNode->nodeName();
    }

    return nameToDisplay;
}

void XmlTreeModel::FillTheModel()
{
    //If an xml document is setted
    if(m_pXmlDocument)
    {
        //Create and sett the root item
        QStandardItem* item = new XmlTreeItem("root", "", XML_TREE_ITEM_TYPE_ROOT, "0");
        item->setIcon(QIcon(":/icon/root.png"));
        this->setItem(0, item);

        //Parse the document to fill the model
        AddElementNormalNode(&(m_pXmlDocument->GetQDomDocument()->firstChildElement()), item, "", 1);
    }
}

void XmlTreeModel::AddElementNormalNode(const QDomNode* domNode, QStandardItem *parentItem,
                                        const QString& parentNumberBase, int number)
{
    //If the node in not null. It's one of the exit condition of the recursion
    if(!domNode->isNull() && domNode->isElement() && !domNode->isComment())
    {
        //Separate all the child node in different list with all the nodes of the same tag name in the same list
        QMap<QString, QList<ExtendedDomNode>> sameTagListMap;

        DivideNodesForTagName(sameTagListMap, domNode->childNodes());

        QString nameToDisplay = domNode->nodeName();

        //This is a normal name so it has to be displayed with the attribute value that match most with the AtributeTagNameCollapse
        const QString& attributeFilter = m_pAttributeNameTagCollapse->GetAttributeNameTag();

        //Find that match first with the filter only If the filter is not empty, otherwise display the tag name
        if(attributeFilter != "")
        {
            nameToDisplay = CheckNodeToDisplayUsingAttributeFilter(domNode, attributeFilter);
        }

        //Initialize the identifiersNumber string with the base number
        QString identifierNumber = parentNumberBase;

        //If it's not empty is because it's the first node and it needn't the dot seprator
        if(!parentNumberBase.isEmpty())
        {
            identifierNumber.append('.');
        }

        //Attach the number
        identifierNumber.append(QString::number(number));

        //If it's need to show the identifiers number
        if(m_showTagIndentfiesNumbers)
        {
            //Prepend the identifier number in the item label
            nameToDisplay.prepend(identifierNumber + " ");
        }


        //Create a item for this node.
        QStandardItem* subItem = new XmlTreeItem(nameToDisplay, domNode->nodeName(), XML_TREE_ITEM_TYPE_NORMAL, identifierNumber);
        subItem->setIcon(QIcon(":/icon/instance.png"));
        parentItem->appendRow(subItem);

        //Call the function for the type node for every different tag name, therefore for every list in the map
        for(QMap<QString, QList<ExtendedDomNode>>::Iterator iteratorList = sameTagListMap.begin(); iteratorList != sameTagListMap.end(); ++iteratorList)
        {
            AddElementTypeNode(&(*iteratorList), subItem, identifierNumber);
        }
    }
}

void XmlTreeModel::AddElementTypeNode(QList<ExtendedDomNode>* sameTagNodeList, QStandardItem *parentItem, const QString& parentNumberBase)
{
    const QString& tagName = sameTagNodeList->first().nodeName();

    //First create a node for the type
    QStandardItem* subItem = new XmlTreeItem(tagName/* + " " + QString::number(sameTagNodeList->size())*/, tagName,
                                             XML_TREE_ITEM_TYPE_NODETYPE, parentNumberBase);
    //Set a different color for this items
    QBrush brush;
    brush.setColor(Qt::red);
    subItem->setForeground(brush);
    subItem->setIcon(QIcon(":/icon/type.jpg"));
    parentItem->appendRow(subItem);

    //Call a normal function for all the node in the list
    for(int i=0; i<sameTagNodeList->size(); ++i)
    {
        AddElementNormalNode(&sameTagNodeList->at(i), subItem, parentNumberBase, sameTagNodeList->at(i).m_number);
    }
}

/*QVariant XmlTreeModel::data(const QModelIndex &index, int role) const
{
    /*if ( index.isValid() && role == Qt::ForegroundRole )
        {
            if ( index.column() == 0 )
            {
                return QVariant( QColor( Qt::red ) );
            }
            return QVariant( QColor( Qt::black ) );
        }

    QVariant r = QStandardItemModel::data( index, role );
    QString y = r;*/

    //Qt::UserRole is 32
    /*if(role == 32)
    {
        return "personalizedRole";
    }

    return QStandardItemModel::data( index, role );
}*/

void XmlTreeModel::AddElement(QDomNode* domNode, QStandardItem *item)
{
    //If the node in not null. It's one of the exit condition of the recursion
    if(!domNode->isNull())
    {
        if(domNode->isElement())
        {
            domNode->childNodes();

            QStandardItem* subItem = new QStandardItem(domNode->nodeName());
            item->appendRow(subItem);

            AddElement(&domNode->firstChildElement(), subItem);
        }

        AddElement(&domNode->nextSiblingElement(), item);
    }
}

void XmlTreeModel::AddElementAllSons(QDomNode* domNode, QStandardItem *item)
{
    //If the node in not null. It's one of the exit condition of the recursion
    if(!domNode->isNull())
    {
        if(domNode->isElement())
        {
            QStandardItem* subItem = new QStandardItem(domNode->nodeName());
            item->appendRow(subItem);

            QDomNodeList childNodes = domNode->childNodes();

            for(int i=0; i<childNodes.size(); ++i)
                AddElementAllSons(&childNodes.at(i), subItem);
        }
    }
}




