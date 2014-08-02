/**
 * File: XmlTreeItem.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/25
*/

#ifndef XMLTREEITEM_H
#define XMLTREEITEM_H

#include <QStandardItem>

/**
 * @brief The XML_TREE_ITEM_TYPE enum that indicates the type of a node in the xml tree model
 */
enum XML_TREE_ITEM_TYPE {XML_TREE_ITEM_TYPE_NORMAL,
                         XML_TREE_ITEM_TYPE_NODETYPE,
                         XML_TREE_ITEM_TYPE_ROOT};

/**
 * @brief The XmlTreeItem class that store additional data in a QStandardItem
 */
class XmlTreeItem : public QStandardItem
{
private:
    /**
     * @brief m_itemType: item of the item
     */
    XML_TREE_ITEM_TYPE m_itemType;

    /**
     * @brief m_identifierNumber of the node. If the type is a TYPENODE it's the identifier number of the parent
     */
    QString m_identifierNumber;

    /**
     * @brief m_tagName: tag name of the element
     */
    QString m_tagName;

public:
    /**
     * @brief XmlTreeItem constructor that initialize with the data in input
     * @param itemText to diplay
     * @param tagName: tag name of the item
     * @param itemType of the element
     * @param identifiersNumber of the node
     */
    XmlTreeItem(const QString& itemText, const QString& tagName, XML_TREE_ITEM_TYPE itemType, const QString& identifiersNumber);

    //Getter
    XML_TREE_ITEM_TYPE GetItemType() const;
    QString GetIdentifierNumber() const;
    QString GetTagName() const;

};

#endif // XMLTREEITEM_H
