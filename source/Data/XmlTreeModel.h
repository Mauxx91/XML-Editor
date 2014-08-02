/**
 * File: XmlTreeModel.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include "../Observer/IObserver.h"

#include <QStandardItemModel>
#include <QDomNode>

class XmlDocument;
class AttributeNameTagCollapse;


/**
 * @brief The XmlTreeModel class represent the model of an xml instance organized as a three
 */
class XmlTreeModel : public QStandardItemModel, public IObserver
{
    Q_OBJECT

private:

    /**
     * @brief The ExtendedDomNode class that is used as extension of QDomNode class for store addiotional data
     */
    class ExtendedDomNode : public QDomNode
    {
    public:
        //number of the child
        int m_number;

        //Construct only by a pre-existent QDomNode
        ExtendedDomNode(const QDomNode& node);
    };

    /**
     * @brief m_pXmlDocument: the document that stores the xml tree
     */
    XmlDocument* m_pXmlDocument;

    /**
     * @brief attributeNameTagCollapse: contains the tag name to display when collapse the tag with the same name
     */
    AttributeNameTagCollapse* m_pAttributeNameTagCollapse;

    /**
     * @brief m_showTagIndentfiesNumbers: if it's true a identfier number is showed at the left of each normal node
     */
    bool m_showTagIndentfiesNumbers;

public:
    /**
     * @brief XmlTreeModel constructor that try to fill the model with the xml document in input
     * @param xmlDocument: the document with fill the model
     */
    XmlTreeModel(AttributeNameTagCollapse* attributeNameTagCollapse, XmlDocument* xmlDocument = 0,
                 bool showTagIndentfiesNumbers = false);

    /**
     * @brief Update the model to reflect the data changes
     * @param updateCode: what has changed
     */
    void Update(UPDATE_CODE updateCode);

    /**
     * @brief SetXmlDocument: set a new xml document in the model
     * @param document: new document with fill the model
     */
    void SetXmlDocument(XmlDocument* xmlDocument);

    /**
     * @brief SetShowTagIndentfiesNumbers: show or hide the identifiers number at the left of each normal node
     * @param show: a boolean that indicates if show the identiers number or not
     */
    void SetShowTagIndentfiesNumbers(bool show);

    /**
     * @brief GetXmlDocument: get the xml document
     * @return the xml document
     */
    XmlDocument* GetXmlDocument() const;

private:

    void DivideNodesForTagName(QMap<QString, QList<XmlTreeModel::ExtendedDomNode>>& o_sameTagListMap, QDomNodeList childNodesList) const;

    //Search for the name to diplay using the attribute filter setted
    QString CheckNodeToDisplayUsingAttributeFilter(const QDomNode* domNode, const QString& attributeFilter) const;

    /**
     * @brief ReFillTheModel: clear the model and re fill the model strating from the setted xml document
     */
    void ReFillTheModel();

    /**
     * @brief FillTheModel from the xml document setted
     */
    void FillTheModel();

    /**
     * @brief AddElementNormalNode: add a normal elements using with the attribute name filter
     * @param domNode: normal node to insert
     * @param parentItem: item in which this node will be inserted as last child
     * @param numberBase
     * @param number
     */
    void AddElementNormalNode(const QDomNode *domNode, QStandardItem *parentItem, const QString &parentNumberBase, int number);

    /**
     * @brief AddElementTypeNode: add a type node with the tag name displayed
     * @param sameTagNodeList: list of normal node that will be inserted as child of this node
     * @param parentItem: item in which this node will be inserted as last child
     * @param parentNumberBase
     */
    void AddElementTypeNode(QList<ExtendedDomNode>* sameTagNodeList, QStandardItem *parentItem, const QString& parentNumberBase);

    /**
     * @brief data: override the data funtion in order to change the apparence of the element.
     * @param index: if the item
     * @param role: the role in whitch the index is used
     * @return the data stored under the given role for the item referred to by the index
     */
    //QVariant data(const QModelIndex &index, int role) const;

//Not used funtion:

    //Pure recursive add alements function that insert all the elements with the tag name
    void AddElement(QDomNode* domNode, QStandardItem *item);
    //Pure recursive add alements function that insert all the elements with the tag name
    void AddElementAllSons(QDomNode* domNode, QStandardItem *item);

};

#endif // XMLTREEMODEL_H
