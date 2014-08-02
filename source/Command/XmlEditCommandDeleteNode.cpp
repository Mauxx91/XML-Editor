/**
 * File: XmlEditCommandDeleteNode.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "XmlEditCommandDeleteNode.h"

#include "XmlEditCommandAddExistentChildNode.h"

#include "../Data/XmlDocument.h"

XmlEditCommandDeleteNode::XmlEditCommandDeleteNode(XmlDocument* xmlDocument, const QString& identifierNumber)
    :XmlEditCommandBase(xmlDocument)
{
    m_identifierNumber = identifierNumber;
}

void XmlEditCommandDeleteNode::Execute()
{
    m_deleteNodeCopy = m_pXmlDocument->DeleteNode(m_identifierNumber);
}

void XmlEditCommandDeleteNode::Undo()
{
    //Copute the parent identifier from the identifier of the deleted node
    QString parentIndentifierNumber = ComputeParentIdentifier(m_identifierNumber);

    XmlEditCommandAddExistentChildNode(m_pXmlDocument, parentIndentifierNumber, m_deleteNodeCopy).Execute();
}
