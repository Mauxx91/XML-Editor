/**
 * File: XmlEditCommandRemoveAttribute.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#include "XmlEditCommandRemoveAttribute.h"

#include "../Data/XmlDocument.h"

XmlEditCommandRemoveAttribute::XmlEditCommandRemoveAttribute(XmlDocument* xmlDocument, const QString& identifierNumber,
                                                       const QString& attrName): XmlEditCommandBase(xmlDocument)
{
    m_identifierNumber = identifierNumber;
    m_attrName = attrName;
}

void XmlEditCommandRemoveAttribute::Execute()
{
    m_previousValue = m_pXmlDocument->removeAttribute(m_identifierNumber, m_attrName);
}

void XmlEditCommandRemoveAttribute::Undo()
{
    //Delete the added node
    m_pXmlDocument->AddOrSetAttribute(m_identifierNumber, m_attrName, m_previousValue);
}
