/**
 * File: XmlEditCommandAddAttribute.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#include "XmlEditCommandAddAttribute.h"

#include "XmlEditCommandRemoveAttribute.h"

#include "../Data/XmlDocument.h"

XmlEditCommandAddAttribute::XmlEditCommandAddAttribute(XmlDocument* xmlDocument, const QString& identifierNumber,
                                                       const QString& attrName, const QString& value): XmlEditCommandBase(xmlDocument)
{
    m_identifierNumber = identifierNumber;
    m_attrName = attrName;
    m_value = value;
}

void XmlEditCommandAddAttribute::Execute()
{
    m_pXmlDocument->AddOrSetAttribute(m_identifierNumber, m_attrName, m_value);
}

void XmlEditCommandAddAttribute::Undo()
{
    //Delete the added node
    XmlEditCommandRemoveAttribute(m_pXmlDocument, m_identifierNumber, m_attrName).Execute();
}
