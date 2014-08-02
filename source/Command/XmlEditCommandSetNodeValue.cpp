/**
 * File: XmlEditCommandSetNodeValue.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#include "XmlEditCommandSetNodeValue.h"

#include "../Data/XmlDocument.h"

XmlEditCommandSetNodeValue::XmlEditCommandSetNodeValue(XmlDocument* xmlDocument, const QString& identifierNumber, const QString& value):
    XmlEditCommandBase(xmlDocument)
{
    m_identifierNumber = identifierNumber;
    m_value = value;
}

void XmlEditCommandSetNodeValue::Execute()
{
    m_previousValue = m_pXmlDocument->SetNodeValue(m_identifierNumber, m_value);
}

void XmlEditCommandSetNodeValue::Undo()
{
    m_pXmlDocument->SetNodeValue(m_identifierNumber, m_previousValue);
}
