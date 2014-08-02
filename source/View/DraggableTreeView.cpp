/**
 * File: DraggableTreeView.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/25
*/

#include "DraggableTreeView.h"

#include "DialogInputStringForm.h"
#include "DialogInputStringBase.h"
#include "DialogEditNodeBase.h"
#include "DialogEditNodeTable.h"

#include "../Data/XmlTreeItem.h"
#include "../Data/XmlTreeModel.h"
#include "../Data/XmlDocument.h"
#include "../Core/XmlRelationCheckerCoreBase.h"
#include "../Command/XmlEditCommandInvoker.h"
#include "../Command/XmlEditCommandAddChildNode.h"
#include "../Command/XmlEditCommandDeleteNode.h"
#include "../Command/XmlEditCommandDeleteAllChildNode.h"
#include "../Command/XmlEditCommandAddExistentChildNode.h"
#include "../Command/XmlEditCommandAggregator.h"

#include "../Data/XmlRelation.h"

#include <QStandardItemModel>
#include <QMessageBox>
#include <QModelIndex>
#include <QHeaderView>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QSet>

const QString DraggableTreeView::m_separator(';');
const QString DraggableTreeView::m_typeSeparator('T');
QDomNode* DraggableTreeView::m_pCopiedNodeToPast = 0;

DraggableTreeView::DraggableTreeView(XmlRelationCheckerCoreBase* xmlRelationCheckerCore, XmlEditCommandInvoker* xmlEditCommandInvoker):
    m_pXmlRelationCheckerCore(xmlRelationCheckerCore), m_pXmlEditCommandInvoker(xmlEditCommandInvoker)
{
    //Hide the header
    this->header()->hide();

    //Set the normal selection mode as in windows file explorer
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //Set at the start the not editable property
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Set the tree animation on
    //this->setAnimated(true); //Do not active because sometimes it crashes with this feature on

    //Set the using of a custom context menu for the tree widget
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &DraggableTreeView::OnContextMenuRequested);

    //Connect the expanded singnal
    connect(this, &QTreeView::expanded, this, &DraggableTreeView::OnItemExpanded);
    connect(this, &QTreeView::collapsed, this, &DraggableTreeView::OnItemCollapsed);

    //Set that the widget accept the drop
    this->setAcceptDrops(true);

    //Initialize the autoexpand
    m_autoExpandTagNode = true;

}

DraggableTreeView::~DraggableTreeView()
{
    if(m_pCopiedNodeToPast)
    {
        delete m_pCopiedNodeToPast;
        m_pCopiedNodeToPast = 0;
    }
}

XmlDocument* DraggableTreeView::GetXmlDocument() const
{
    XmlTreeModel* model = dynamic_cast<XmlTreeModel*>(this->model());

    return model->GetXmlDocument();
}

void DraggableTreeView::OnContextMenuRequested(const QPoint& point)
{
    QModelIndex index = this->indexAt(point);

    //Show the menu only if it's selected a valid index
    if(index.isValid())
    {
        //Create a menu and fill with the actions
        QMenu contextMenu;
        QAction* followRelationAct = new QAction(QIcon(":/icon/show.png"), "Follow relation", this);
        QAction* newAct = new QAction(QIcon(":/icon/newNode.png"), "New child", this);
        QAction* editAct = new QAction(QIcon(":/icon/edit.png"), "Edit", this);
        QAction* duplicateAct = new QAction(QIcon(":/icon/duplicate.png"), "Duplicate", this);
        QAction* copyAct = new QAction(QIcon(":/icon/copy.png"), "Copy", this);
        QAction* pasteAct = new QAction(QIcon(":/icon/paste.jpg"), "Paste", this);
        QAction* deleteThisAct = new QAction(QIcon(":/icon/delete.png"), "Delete", this);
        QAction* deleteAllSubNodeAct = new QAction(QIcon(":/icon/delete.png"), "Delete all childs", this);
        contextMenu.addAction(followRelationAct);
        contextMenu.addAction(newAct);
        contextMenu.addAction(editAct);
        contextMenu.addAction(duplicateAct);
        contextMenu.addAction(copyAct);
        contextMenu.addAction(pasteAct);
        contextMenu.addAction(deleteThisAct);
        contextMenu.addAction(deleteAllSubNodeAct);

        QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());

        //QMap<int, QVariant> itemsValues = model->itemData(index);
        //QString itemText = itemsValues.value(Qt::DisplayRole, QVariant("")).toString();

        XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

        if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NODETYPE)
        {
            followRelationAct->setEnabled(false);
            newAct->setText("New child of this type");
            editAct->setEnabled(false);
            duplicateAct->setEnabled(false);
            copyAct->setEnabled(false);
            pasteAct->setEnabled(false);
            deleteThisAct->setEnabled(false);
        }
        else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_ROOT)
        {
            followRelationAct->setEnabled(false);
            editAct->setEnabled(false);
            duplicateAct->setEnabled(false);
            copyAct->setEnabled(false);
            pasteAct->setEnabled(false);
            deleteThisAct->setEnabled(false);
        }
        else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NORMAL &&
                itemXml->GetIdentifierNumber() == "1")
        {
            duplicateAct->setEnabled(false);
        }

        //Let active the past action only if there's some to paste
        if( ! m_pCopiedNodeToPast)
        {
            pasteAct->setEnabled(false);
        }

        //Show contextual menu
        QAction* itemSelected = contextMenu.exec(this->mapToGlobal(point));

        //Execute what the user has requested
        if(itemSelected == followRelationAct)
        {
            FollowRelationActSelected(itemXml);
        }
        else if(itemSelected == newAct)
        {
            NewActSelected(itemXml);
        }
        else if(itemSelected == editAct)
        {
            EditActSelected(itemXml);
        }
        else if(itemSelected == duplicateAct)
        {
            DuplicateActSelected(itemXml);
        }
        else if(itemSelected == copyAct)
        {
            CopyActSelected(itemXml);
        }
        else if(itemSelected == pasteAct)
        {
            PasteActSelected(itemXml);
        }
        else if(itemSelected == deleteThisAct)
        {
            DeleteThisActSelected(itemXml);
        }
        else if(itemSelected == deleteAllSubNodeAct)
        {
            DeleteAllSubNodeActSelected(itemXml);
        }

    }
}

void DraggableTreeView::FollowRelationActSelected(XmlTreeItem* itemXml)
{
    PerformFollowRelation(itemXml->GetIdentifierNumber());
}

void DraggableTreeView::NewActSelected(XmlTreeItem* itemXml)
{
    if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NODETYPE)
    {
        AddChildNode(itemXml->GetIdentifierNumber(), itemXml->GetTagName());
    }
    else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_ROOT)
    {
        QString tagName = AskNewChildTagName();

        //If the user has press apply button
        if(tagName != "")
        {
            AddChildNode("1", tagName);
        }
    }
    else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NORMAL)
    {
        QString tagName = AskNewChildTagName();

        //If the user has press apply button
        if(tagName != "")
        {
            AddChildNode(itemXml->GetIdentifierNumber(), tagName);
        }
    }
}

void DraggableTreeView::EditActSelected(XmlTreeItem* itemXml)
{
    EditNodeRequested(itemXml->GetIdentifierNumber());
}

void DraggableTreeView::DuplicateActSelected(XmlTreeItem* itemXml)
{
    //Compute parent identifier
    QString parentIdentifier = ComputeParentIdentifier(itemXml->GetIdentifierNumber());

    CopySingleNodesAsChildNode(itemXml->GetIdentifierNumber(), parentIdentifier,
                               GetXmlDocument(), GetXmlDocument());
}

void DraggableTreeView::CopyActSelected(XmlTreeItem* itemXml)
{
    m_pCopiedNodeToPast = new QDomNode(GetXmlDocument()->GetDomNode(itemXml->GetIdentifierNumber()).cloneNode(true));
}

void DraggableTreeView::PasteActSelected(XmlTreeItem* itemXml)
{
    PasteElement(itemXml->GetIdentifierNumber());
}

void DraggableTreeView::DeleteThisActSelected(XmlTreeItem* itemXml)
{
    //remove the node
    DeleteNode(itemXml->GetIdentifierNumber());
}

void DraggableTreeView::DeleteAllSubNodeActSelected(XmlTreeItem* itemXml)
{
    if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NODETYPE)
    {
        //Remove all the child node
        DeleteAllChildNode(itemXml->GetIdentifierNumber(), itemXml->GetTagName());
    }
    else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_ROOT)
    {
        DeleteAllChildNode("1", "");
    }
    else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NORMAL)
    {
         DeleteAllChildNode(itemXml->GetIdentifierNumber(), "");
    }
}

void DraggableTreeView::OnItemExpanded(const QModelIndex& index)
{
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
    XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

    //If the auto expanded it's true
    //For each child node check if it's a type node and if it is auto expand the node
    for(int i=0; m_autoExpandTagNode && i < model->rowCount(itemXml->index()); ++i) //model->rowCount return the numeber of the row of the given index
    {
        //The current child node
        XmlTreeItem* childItemXml = dynamic_cast<XmlTreeItem*>(itemXml->child(i, 0));

        //If the child node it's a type node therefore auto expand
        if(childItemXml->GetItemType() == XML_TREE_ITEM_TYPE_NODETYPE)
        {
            if(childItemXml->index().isValid())
            {
                this->setExpanded(childItemXml->index(), true);
            }
        }
    }

    //Add the expanded item to the expanded item list
    m_expandedNodeIdentifierAndTypeSet.insert(QString(itemXml->GetIdentifierNumber())
                                              .append(m_typeSeparator).append(QString::number(itemXml->GetItemType()))
                                              .append(m_typeSeparator).append(itemXml->GetTagName()));
}

void DraggableTreeView::OnItemCollapsed(const QModelIndex& index)
{
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
    XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

    //Remove that item from the expanded list
    m_expandedNodeIdentifierAndTypeSet.remove(QString(itemXml->GetIdentifierNumber())
                                              .append(m_typeSeparator).append(QString::number(itemXml->GetItemType()))
                                              .append(m_typeSeparator).append(itemXml->GetTagName()));
}

void DraggableTreeView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    QTreeView::rowsInserted(parent, start, end);

    //Call the restore of the expanded items
    RestoreExpandedNode(false);
}

void DraggableTreeView::setModel(QAbstractItemModel* model)
{
    //Call the parent implementation
    QTreeView::setModel(model);

    m_expandedNodeIdentifierAndTypeSet.clear();

    this->expandToDepth(1);
}

void DraggableTreeView::EditNodeRequested(const QString& identifierNumber)
{
    DialogEditNodeTable(m_pXmlEditCommandInvoker, GetXmlDocument(), QString(identifierNumber)).exec();
}

void DraggableTreeView::RestoreExpandedNode(bool autoExpandTagNode)
{
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());

    if(model)
    {
        //Set the auto expanded property
        m_autoExpandTagNode = autoExpandTagNode;

        //Start the expansion from the root node
        RealRestoreExpandedNode(model->index(0, 0));

        //reset true the auto expanded propery
        m_autoExpandTagNode = true;
    }
}

void DraggableTreeView::RealRestoreExpandedNode(const QModelIndex& index)
{
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
    XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

    if(itemXml)
    {
        //Is the current item to Expand
        bool isItemToExpand = false;

        //Check if the node it's expanded or not, flow the expanded node list
        for(QSet<QString>::iterator iter=m_expandedNodeIdentifierAndTypeSet.begin();
            iter != m_expandedNodeIdentifierAndTypeSet.end() && !isItemToExpand;
            ++iter)
        {
            if( (
                    QString(itemXml->GetIdentifierNumber())
                    .append(m_typeSeparator).append(QString::number(itemXml->GetItemType()))
                    .append(m_typeSeparator).append(itemXml->GetTagName())
                )
                    .compare(*iter) == 0)
            {
                //Expand the node and exit the cycle
                this->setExpanded(index, true);
                isItemToExpand = true;
            }
        }

        //Call the function for each child node
        for(int i=0; i < model->rowCount(itemXml->index()); ++i) //model->rowCount return the number of the row of the given index
        {
            RealRestoreExpandedNode(itemXml->child(i, 0)->index());
        }
    }
}

void DraggableTreeView::ShowAndSelectedItem(const QString& identifierNumber)
{
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());

    if(model)
    {
        //Start the expansion from the root node
        RealShowAndSelectItem(identifierNumber, model->index(0, 0));
    }
}

bool DraggableTreeView::RealShowAndSelectItem(const QString& identifierNumber, const QModelIndex& index)
{
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
    XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

    if(itemXml)
    {
        //Item founded show
        if(itemXml->GetIdentifierNumber() == identifierNumber)
        {
            //Select the node
            setCurrentIndex(index);
            expand(index);

            return true;
        }
        else
        {
            bool childIsFound = false;

            //Call the function for each child node
            for(int i=0; i < model->rowCount(itemXml->index()); ++i) //model->rowCount return the number of the row of the given index
            {
                childIsFound = childIsFound || RealShowAndSelectItem(identifierNumber, itemXml->child(i, 0)->index());
            }

            //If a child is the searched node this node has to be expanded
            if(childIsFound)
            {
                this->setExpanded(index, true);
            }

            return childIsFound;
        }
    }
    return false;
}

void DraggableTreeView::PerformFollowRelation(const QString& starterIdentifierNumber)
{
    QString relationDestinationIdentifier = "";
    QString absoluteFileName = "";
    const QDomNode& clickedNode = GetXmlDocument()->GetDomNode(starterIdentifierNumber);
    bool isASubNodeFromOfARelation = false;

    int numDestination = m_pXmlRelationCheckerCore->GetIdentifierAndWhereOfFistNodeTo(&relationDestinationIdentifier,
                                                                                      &absoluteFileName,
                                                                                      isASubNodeFromOfARelation, clickedNode);

    //Depending on the num of the destinaion
    if(numDestination > 0)
    {
        if(numDestination > 1)
        {
            //Advert the user that was found more that on destination
            QMessageBox::warning(this, "Multiple destination found", "There are multiple destination for this element. \n\n"
                                       "Only the first will be shown.");
        }

        //Show the destination of the relation
        emit ShowAndSelectedItemRequested(relationDestinationIdentifier, absoluteFileName);
    }
    else //No destination was found
    {
        //if the element is a correct start element
        if(isASubNodeFromOfARelation)
        {
            //Advert the user that there was found not destination
            QMessageBox::warning(this, "No destination found", "There aren't any destination for this element. \n\n"
                                       "However this element is a correct start element for at least one relation.");
        }
        else
        {
            //Advert the user that the start point is not a valid start point for any relation
            QMessageBox::warning(this, "No destination found", "There aren't any destination for this element. \n\n"
                                       "This element isn't a correct start element for no one relation.");
        }
    }
}

void DraggableTreeView::AddChildNode(const QString& parentIdentifier, const QString& nodeType)
{
    XmlEditCommandAddChildNode* addCommand = new XmlEditCommandAddChildNode(GetXmlDocument(), parentIdentifier, nodeType);

    m_pXmlEditCommandInvoker->ExecuteACommand(addCommand);

    ShowAndSelectedItem(parentIdentifier);

    EditNodeRequested(addCommand->GetAddedNodeIdentifier());
}

void DraggableTreeView::DeleteNode(const QString& identifierNumber)
{
    m_pXmlEditCommandInvoker->ExecuteACommand(new XmlEditCommandDeleteNode(GetXmlDocument(), identifierNumber));
}

void DraggableTreeView::DeleteAllChildNode(const QString& parentIdentifier, const QString& tagName)
{
    m_pXmlEditCommandInvoker->ExecuteACommand(new XmlEditCommandDeleteAllChildNode(GetXmlDocument(), parentIdentifier, tagName));
}

void DraggableTreeView::CopySingleNodesAsChildNode(const QString& nodesFromIdentifier, const QString& nodeToidentifier,
                                                   XmlDocument* sourceXmlDocument, XmlDocument* destinationXmlDocument)
{
    const QDomNode& nodeToAdd = sourceXmlDocument->GetDomNode(nodesFromIdentifier);

    m_pXmlEditCommandInvoker->ExecuteACommand(new XmlEditCommandAddExistentChildNode(destinationXmlDocument, nodeToidentifier, nodeToAdd));

    ShowAndSelectedItem(nodeToidentifier);
}

void DraggableTreeView::ExecuteDragAndDropLinking(const QStringList& nodesToIdentifiersAndTagName, // start of the drag
                                                  const QString& nodeFromIdentifier, const QString& nodeFromTagName,//destination drag
                                             XmlDocument* sourceXmlDocument, XmlDocument* destinationXmlDocument)
{
    QList<XmlRelation*> relationPath;
    QString tagToName = "";

    //For each source node
    for(int i=0; i < nodesToIdentifiersAndTagName.size(); i=i+2)
    {
        //const QString& nodeToIdentifier = nodesToIdentifiersAndTagName.at(i);
        const QString& nodeToTagName = nodesToIdentifiersAndTagName.at(i+1);

        //First cycle
        if(i == 0)
        {
            tagToName = nodeToTagName;
            relationPath = m_pXmlRelationCheckerCore->CheckARelationPathExistence(nodeFromTagName, nodeToTagName);
        }
        else //Other cycle control that all the dragged elements is of the same type
        {
            //If current tag name is different say the error and return without do nothing
            if(tagToName != nodeToTagName)
            {
                QMessageBox::critical(this, "Drag and Drop Error", "it's impossible to drag elements of different type.");
                return;
            }
        }
    }

    //If exist a relation path that connect the tag from to the tag to
    if(relationPath.size() > 0)
    {
        //Create a new command aggregator
        XmlEditCommandAggregator* commandAggregator = new XmlEditCommandAggregator(destinationXmlDocument);

        const QString defaultKeyName = "defaultKey";
        QString precedingReferencerName = "";

        XmlRelation* previoslyXmlRelation = 0;

        for(int i=0; i <relationPath.size(); ++i)
        {
            XmlRelation* xmlRelation = relationPath.at(i);

            QDomElement nodeToAdd;

            QString identifierWhereAdd;

            //At the fist cycle add to the existent node
            if(i == 0)
            {
                //it has to add the child node to the node destination of the drag -> nodeFromIdentifier
                identifierWhereAdd = nodeFromIdentifier;

                //If it's the last cycle
                if(i == relationPath.size()-1)
                {
                    //Get the node where to add
                    QDomNode nodeWhereAdd = destinationXmlDocument->GetDomNode(identifierWhereAdd);

                    //Add all the start relation tag
                    for(int k=0; k < nodesToIdentifiersAndTagName.size(); k=k+2)
                    {
                        const QString& nodeToIdentifier = nodesToIdentifiersAndTagName.at(k);
                        const QString& key = sourceXmlDocument->GetKeyFromIdentifierNumber(nodeToIdentifier, xmlRelation->GetAttributeNameOfTagTo());

                        //Check if a node as the current already exists
                        QStringList existentKeys = destinationXmlDocument->GetAllKeyOfFistLevelChild(nodeWhereAdd, xmlRelation->GetSubTagNameOfTagFrom(),
                                                                                                    xmlRelation->GetAttributeNameofTagFrom());

                        bool keyIsNotAlreadyPresent = true;
                        //Check if the key already exist
                        for(int j=0; j < existentKeys.size() && keyIsNotAlreadyPresent; ++j)
                        {
                            if(key == existentKeys.at(j))
                            {
                                keyIsNotAlreadyPresent = false;
                            }
                        }

                        //Insert the node only id it's not present yet
                        if(keyIsNotAlreadyPresent)
                        {
                            QDomElement childNode = destinationXmlDocument->GetQDomDocument()->createElement(xmlRelation->GetSubTagNameOfTagFrom());
                            childNode.setAttribute(xmlRelation->GetAttributeNameofTagFrom(), key);

                            commandAggregator->AddCommand(new XmlEditCommandAddExistentChildNode(destinationXmlDocument, identifierWhereAdd, childNode));
                        }
                    }
                }
                else //add a pointer to a intermediate relation node
                {
                    //Compute the key fist empty defalut key for the to of the current relation
                    QString key = destinationXmlDocument->GetFirtEmptyKey(defaultKeyName, xmlRelation->GetTagToName(), xmlRelation->GetAttributeNameOfTagTo());
                    precedingReferencerName = key;

                    //Create the node to add
                    nodeToAdd = destinationXmlDocument->GetQDomDocument()->createElement(xmlRelation->GetSubTagNameOfTagFrom());

                    //Add the key to the nodeToAdd
                    nodeToAdd.setAttribute(xmlRelation->GetAttributeNameofTagFrom(), key);

                    commandAggregator->AddCommand(new XmlEditCommandAddExistentChildNode(destinationXmlDocument, identifierWhereAdd, nodeToAdd));
                }
            }
            else //Create a new itermediate node to peform the relation
            {
                //Add to the parent
                identifierWhereAdd = ComputeParentIdentifier(nodeFromIdentifier);

                //Create the node to add
                nodeToAdd = destinationXmlDocument->GetQDomDocument()->createElement(previoslyXmlRelation->GetTagToName());

                //Add the key to the nodeToAdd
                nodeToAdd.setAttribute(previoslyXmlRelation->GetAttributeNameOfTagTo(), precedingReferencerName);

                //If it's the last cycle
                if(i == relationPath.size()-1)
                {
                    //Get the node where to add
                    QDomNode nodeWhereAdd = destinationXmlDocument->GetDomNode(identifierWhereAdd);

                    //Add all the start relation tag
                    for(int k=0; k < nodesToIdentifiersAndTagName.size(); k=k+2)
                    {
                        const QString& nodeToIdentifier = nodesToIdentifiersAndTagName.at(k);
                        const QString& key = sourceXmlDocument->GetKeyFromIdentifierNumber(nodeToIdentifier, xmlRelation->GetAttributeNameOfTagTo());

                        //Check if a node as the current already exists
                        QStringList existentKeys = destinationXmlDocument->GetAllKeyOfFistLevelChild(nodeWhereAdd, xmlRelation->GetSubTagNameOfTagFrom(),
                                                                                                    xmlRelation->GetAttributeNameofTagFrom());

                        bool keyIsNotAlreadyPresent = true;
                        //Check if the key already exist
                        for(int j=0; j < existentKeys.size() && keyIsNotAlreadyPresent; ++j)
                        {
                            if(key == existentKeys.at(j))
                            {
                                keyIsNotAlreadyPresent = false;
                            }
                        }

                        //Insert the node only id it's not present yet
                        if(keyIsNotAlreadyPresent)
                        {

                            QDomElement childNode = destinationXmlDocument->GetQDomDocument()->createElement(xmlRelation->GetSubTagNameOfTagFrom());
                            childNode.setAttribute(xmlRelation->GetAttributeNameofTagFrom(), key);

                            nodeToAdd.appendChild(childNode);
                        }
                    }
                }
                else //add a pointer to a intermediate relation node
                {
                    //Compute the key fist empty defalut key for the to of the current relation
                    QString key = destinationXmlDocument->GetFirtEmptyKey(defaultKeyName, xmlRelation->GetTagToName(), xmlRelation->GetAttributeNameOfTagTo());
                    precedingReferencerName = key;

                    //Create the node to add
                    QDomElement childNode = destinationXmlDocument->GetQDomDocument()->createElement(xmlRelation->GetSubTagNameOfTagFrom());

                    //Add the key to the nodeToAdd
                    childNode.setAttribute(xmlRelation->GetAttributeNameofTagFrom(), key);

                    nodeToAdd.appendChild(childNode);
                }
                commandAggregator->AddCommand(new XmlEditCommandAddExistentChildNode(destinationXmlDocument, identifierWhereAdd, nodeToAdd));
            }

            //Set the current relation as the previolsy relation for the next cycle
            previoslyXmlRelation = xmlRelation;
        }

        if(commandAggregator->Size() >0)
        {
            //Execute more command as one
            m_pXmlEditCommandInvoker->ExecuteACommand(commandAggregator);
        }
        else
        {
            delete commandAggregator;
        }

        ShowAndSelectedItem(nodeFromIdentifier);
    }
    else //Advice the user that there isn't a relation path that permits the drag and drop action
    {
        QMessageBox::critical(this, "Drag and Drop Error", "It doesn't exist any relation path that permits the referencing action.");
    }
}

void DraggableTreeView::PasteElement(const QString& identifierNumber)
{
    XmlEditCommandAddExistentChildNode* command = new XmlEditCommandAddExistentChildNode(GetXmlDocument(), identifierNumber, *m_pCopiedNodeToPast);

    m_pXmlEditCommandInvoker->ExecuteACommand(command);

    ShowAndSelectedItem(command->GetAddeNodeIdentifier());
}

QString DraggableTreeView::AskNewChildTagName() const
{
    DialogInputStringBase* dialog = new DialogInputStringForm("Add child node", "Insert tag name of the new node:", "Apply", "");

    //Show the dialog
    dialog->exec();

    //If the result is positive export the relations in an external file conf
    const QString& inputString = dialog->GetInputString();

    delete dialog;

    return inputString;
}

void DraggableTreeView::mousePressEvent(QMouseEvent *event)
{
    //If the left button was pressed
    if (event->button() == Qt::LeftButton)
    {
        //Store the point in which the drag has started
        m_DragStartPos = event->pos();
    }
    //To permit the widget to also process normal mouse event
    QTreeView::mousePressEvent(event);
}

void DraggableTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //Calculate the distance from the drag start and the current mouse position
        int distance = (event->pos() - m_DragStartPos).manhattanLength();
        //If the distance is greather than the drag distance
        if (distance >= QApplication::startDragDistance())
        {
            //Perform a drag
            PerformDrag();
        }
    }
    //Don't let the widget perform the classic mouse event action, in order to take selected other selected items
    //QTreeView::mouseMoveEvent(event);
}

void DraggableTreeView::PerformDrag()
{
    //Index of the element from where the drag has started
    QModelIndex index = this->indexAt(m_DragStartPos);

    //If the drag started from a valid index
    if(index.isValid())
    {
        //List of all selected elements
        QModelIndexList selectedIndexes = this->selectedIndexes();

        QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
        XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

        //Start the drag only if it's not a root element
        if(itemXml->GetItemType() != XML_TREE_ITEM_TYPE_ROOT && itemXml->GetItemType() != XML_TREE_ITEM_TYPE_NODETYPE)
        {
            QString message;
            //Initialize the passed string: it will contains all the items identifier number separated by ';' character
            for(int i=0; i<selectedIndexes.size(); ++i)
            {
                //Get the item from the index
                itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(selectedIndexes.at(i)));

                //Pass only if it's a normal element
                if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NORMAL)
                {
                    //Add foreach item the identifier numner
                    message.append(itemXml->GetIdentifierNumber()+ m_separator + itemXml->GetTagName() + m_separator);
                }
            }

            //Uset to pass the data insed the drop
            QMimeData* mimeData = new QMimeData;
            mimeData->setText(message); //Pass the message

            //That pass the data contained in a mime data object and perform the drag
            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData); //set the mimedata

            //For this implementation for the widget start it's not importat knows if the drag has finished or not
            //If the drag was concluded and the type was the move action
            if (drag->exec(Qt::LinkAction) == Qt::LinkAction)
            {
                //Do nothing
            }
        }
    }
}

void DraggableTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    //Obtain the source widget of the drag and cast in a DraggableTreeView
    DraggableTreeView* source = dynamic_cast<DraggableTreeView*>(event->source());

    //Accept drag only from DraggableTreeView
    if (source)
    {
        //Set the drop action and accept the event
        event->setDropAction(Qt::LinkAction);
        event->accept();
    }
}

void DraggableTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    //Obtain the source widget of the drag and cast in a DraggableTreeView
    DraggableTreeView* source = dynamic_cast<DraggableTreeView*>(event->source());

    //Obtain the index in which the cursor is upon
    QModelIndex index = this->indexAt(event->pos());

    //item destination of the drag & drop
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
    XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

    //If the source si a valid destionation set as selected
    if (source && source != this && index.isValid() && itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NORMAL)
    {
        setCurrentIndex(index);
    }

    //Accept drag only from DraggableTreeView
    if (source)
    {
        //Set the drop action and accept the event
        event->setDropAction(Qt::LinkAction);
        event->accept();
    }
}

void DraggableTreeView::dropEvent(QDropEvent *event)
{
    //Obtain the source widget of the drag and cast in a DraggableTreeView
    DraggableTreeView* source = dynamic_cast<DraggableTreeView*>(event->source());

    XmlDocument* sourceXmlDocument = source->GetXmlDocument();

    QModelIndex index = this->indexAt(event->pos());

    //Accept drag only from DraggableTreeView and drag to valid index
    if (source && source != this && index.isValid())
    {
        //item destination of the drag & drop
        QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
        XmlTreeItem* itemXml = dynamic_cast<XmlTreeItem*>(model->itemFromIndex(index));

        if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NORMAL)
        {
            //Get the massage passed throgh the mime data
            const QString& message = event->mimeData()->text();

            //Separe the message
            QStringList identifiersAndTagNameToList = message.split(m_separator, QString::SkipEmptyParts);

            //Cal the function that execute the drag
            ExecuteDragAndDropLinking(identifiersAndTagNameToList, itemXml->GetIdentifierNumber(), itemXml->GetTagName(), sourceXmlDocument, GetXmlDocument());

            event->setDropAction(Qt::LinkAction);
            event->accept();
        }
        else
        {
            //Say to user that it's impossibile to drag to root elment
            if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_ROOT)
            {
                QMessageBox::warning(0, "Wrong destination", "It's impossible to drag & drop an element to the root element.\n\n"
                                 "The drag & drop is possible only to a normal element.");
            }
            //Say to user that it's impossible to draf to a type element
            else if(itemXml->GetItemType() == XML_TREE_ITEM_TYPE_NODETYPE)
            {
                QMessageBox::warning(0, "Wrong destination", "It's impossible to drag & drop an element to a type element.\n\n"
                                 "The drag & drop is possible only to a normal element.");
            }
        }
    }
}

QString DraggableTreeView::ComputeParentIdentifier(const QString& childIdentifier) const
{
    QString parentIdentifier = "";

    //Compute the identifier of the parent
    int indexOfLastDot = childIdentifier.lastIndexOf(".");

    //If thers's no dot
    if(indexOfLastDot == -1)
    {
        //The parent is the root node
        parentIdentifier = "0";
    }
    else //Remove the last number and the point
    {
        parentIdentifier = childIdentifier;
        parentIdentifier.remove(indexOfLastDot, childIdentifier.size()-indexOfLastDot);
    }

    return parentIdentifier;
}
