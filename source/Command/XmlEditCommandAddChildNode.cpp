/**
 * File: XmlEditCommandAddChildNode.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "XmlEditCommandAddChildNode.h"

#include "XmlEditCommandDeleteNode.h"

#include "../Data/XmlDocument.h"

XmlEditCommandAddChildNode::XmlEditCommandAddChildNode(XmlDocument* xmlDocument, const QString& parentIdentifier, const QString& nodeType):
    XmlEditCommandBase(xmlDocument)
{
    m_parentIdentifier = parentIdentifier;
    m_nodeType = nodeType;
}

void XmlEditCommandAddChildNode::Execute()
{
    m_addedNodeIdentifier = m_pXmlDocument->AddChildNode(m_parentIdentifier, m_nodeType);
}

void XmlEditCommandAddChildNode::Undo()
{
    //Delete the added node
    XmlEditCommandDeleteNode(m_pXmlDocument, m_addedNodeIdentifier).Execute();
}

const QString& XmlEditCommandAddChildNode::GetAddedNodeIdentifier() const
{
    return m_addedNodeIdentifier;
}
