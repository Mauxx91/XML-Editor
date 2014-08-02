/**
 * File: XmlTreeItem.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/25
*/

#include "XmlTreeItem.h"

XmlTreeItem::XmlTreeItem(const QString& itemText, const QString& tagName, XML_TREE_ITEM_TYPE itemType, const QString& identifiersNumber):
    QStandardItem(itemText)
{
    m_itemType = itemType;
    m_tagName = tagName;
    m_identifierNumber = identifiersNumber;

}

XML_TREE_ITEM_TYPE XmlTreeItem::GetItemType() const
{
    return m_itemType;
}

QString XmlTreeItem::GetIdentifierNumber() const
{
    return m_identifierNumber;
}

QString XmlTreeItem::GetTagName() const
{
    return m_tagName;
}
