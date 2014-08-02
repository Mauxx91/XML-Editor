/**
 * File: DraggableTreeView.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/25
*/

#ifndef DRAGGABLETREEVIEW_H
#define DRAGGABLETREEVIEW_H

#include <QTreeView>

class XmlDocument;
class XmlRelationCheckerCoreBase;
class XmlEditCommandInvoker;
class XmlTreeItem;
class QDomNode;

enum XML_TREE_ITEM_TYPE;

class DraggableTreeView: public QTreeView
{
    Q_OBJECT

private:
    /**
     * @brief m_DragStartPos: where the drag start. It's used to understand from which element the drag has started
     */
    QPoint m_DragStartPos;

    /**
     * @brief m_separator: the character used to separate the info in the string passed during the d&d
     */
    static const QString m_separator;

    /**
     * @brief m_pXmlRelationCheckerCore: for use the relations
     */
    XmlRelationCheckerCoreBase* m_pXmlRelationCheckerCore;

    /**
     * @brief m_pXmlEditCommandInvoker: for invoke a command to the xml document displayed
     */
    XmlEditCommandInvoker* m_pXmlEditCommandInvoker;

    /**
     * @brief m_expandedNodeIdentifierSet: list of the identifier if the expanded items
     */
    QSet<QString> m_expandedNodeIdentifierAndTypeSet;

    /**
     * @brief m_autoExpandTagNode: when it's true the node type it's auto expanded
     */
    bool m_autoExpandTagNode;

    //type separator the expanded identifier
    static const QString m_typeSeparator;

    //Store the copied node to past
    static QDomNode* m_pCopiedNodeToPast;

public:
    /**
     * @brief DraggableTreeView constructor
     * @param xmlRelationCheckerCore: for use the relations
     * @param xmlEditCommandInvoker: the invoker for invoke command
     */
    DraggableTreeView(XmlRelationCheckerCoreBase* xmlRelationCheckerCore, XmlEditCommandInvoker* xmlEditCommandInvoker);

    ~DraggableTreeView();

    //Getter for the xml document
    XmlDocument* GetXmlDocument() const;

private slots:
    /**
     * @brief OnContextMenuRequested: show the personalized context menu for the seleted item
     * @param point: where the context menu is requested
     */
    void OnContextMenuRequested(const QPoint& point);

    void FollowRelationActSelected(XmlTreeItem* itemXml);
    void NewActSelected(XmlTreeItem* itemXml);
    void EditActSelected(XmlTreeItem* itemXml);
    void DuplicateActSelected(XmlTreeItem* itemXml);
    void CopyActSelected(XmlTreeItem* itemXml);
    void PasteActSelected(XmlTreeItem* itemXml);
    void DeleteThisActSelected(XmlTreeItem* itemXml);
    void DeleteAllSubNodeActSelected(XmlTreeItem* itemXml);

    /**
     * @brief OnItemExpanded: expand all the node type children of the expanded node. Update the expanded item list
     * @param index of the expanded item
     */
    void OnItemExpanded(const QModelIndex& index);

    /**
     * @brief OnItemCollapsed: Update the expanded item list
     * @param index of the collapsed item
     */
    void OnItemCollapsed(const QModelIndex& index);

protected slots:
    //Invocated when the model changes and it call the restore of the expanded item
    virtual void rowsInserted(const QModelIndex &parent, int start, int end);

public:
    //Set the model as the QTreeWidget and restore the defalut expanded item
    virtual void setModel(QAbstractItemModel* model);
private:

    //Call the dialog for edit node
    void EditNodeRequested(const QString& identifierNumber);

    //Call the real recursive function for restore the expanded node
    void RestoreExpandedNode(bool autoExpandTagNode);

    //Recursive funtion that iterate on all the model items restoring the expanded item
    void RealRestoreExpandedNode(const QModelIndex& index);

public:
    //Make the item visible and selected
    void ShowAndSelectedItem(const QString& identifierNumber);

private:
    //Recursive, return true is the node was found in a child
    bool RealShowAndSelectItem(const QString& identifierNumber, const QModelIndex& index);

    //Perform the searching of the relation
    void PerformFollowRelation(const QString& starterIdentifierNumber);

    /**
     * @brief AddChildNode: add a new empty child node using the invoker
     * @param parentIdentifier: parent of the new node
     * @param nodeType: tag name of the new node
     */
    void AddChildNode(const QString& parentIdentifier, const QString& nodeType);

    /**
     * @brief DeleteNode: delete the node using the ivoker
     * @param identifierNumber of the node to delete
     */
    void DeleteNode(const QString& identifierNumber);

    /**
     * @brief DeleteAllChildNode: delete all the child node og the given node
     * @param parentIdentifier: parent node from which the childs will be removed
     * @param tagName: filter for delete only the child node with this tag name
     */
    void DeleteAllChildNode(const QString& parentIdentifier, const QString &tagName);

    //Copy a single existent node
    void CopySingleNodesAsChildNode(const QString& nodesFromIdentifier, const QString& nodeToidentifier,
                                    XmlDocument* sourceXmlDocument, XmlDocument* destinationXmlDocument);

    void ExecuteDragAndDropLinking(const QStringList& nodesToIdentifiersAndTagName, const QString& nodeFromIdentifier,
                              const QString& nodeFromTagName,
                        XmlDocument* sourceXmlDocument, XmlDocument* destinationXmlDocument);

    //Pste the current copied element
    void PasteElement(const QString& identifierNumber);

    /**
     * @brief AskNewChildTagName: ask the user for the tag name of the new node
     * @return the tag name of a new node
     */
    QString AskNewChildTagName() const;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void PerformDrag();

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

signals:
    //Request the show and the select of an item in the given file name
    void ShowAndSelectedItemRequested(const QString& identifierNumber, const QString& absoluteFileName);

private:
    QString ComputeParentIdentifier(const QString& childIdentifier) const;

};

#endif // DRAGGABLETREEVIEW_H
