/**
 * File: XmlEditCommandEditAttribute.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#include "XmlEditCommandEditAttribute.h"

#include "../Data/XmlDocument.h"

XmlEditCommandEditAttribute::XmlEditCommandEditAttribute(XmlDocument* xmlDocument, const QString& identifierNumber,
                                                       const QString& attrName, const QString& value): XmlEditCommandBase(xmlDocument)
{
    m_identifierNumber = identifierNumber;
    m_attrName = attrName;
    m_value = value;
}

void XmlEditCommandEditAttribute::Execute()
{
    m_previosValue = m_pXmlDocument->AddOrSetAttribute(m_identifierNumber, m_attrName, m_value);
}

void XmlEditCommandEditAttribute::Undo()
{
    //Delete the added node
    m_pXmlDocument->AddOrSetAttribute(m_identifierNumber, m_attrName, m_previosValue);
}
