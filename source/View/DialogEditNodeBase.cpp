/**
 * File: DialogEditNodeBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#include "DialogEditNodeBase.h"

DialogEditNodeBase::DialogEditNodeBase(XmlEditCommandInvoker* xmlEditCommandInvoker, XmlDocument* xmlDocument, const QString& identifierNumber, QWidget *parent): QDialog(parent)
{
    m_pXmlEditCommandInvoker = xmlEditCommandInvoker;
    m_pXmlDocument = xmlDocument;
    m_identifierNumber = identifierNumber;
}
