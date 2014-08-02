/**
 * File: XmlEditCommandAddExistentChildNode.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "XmlEditCommandAddExistentChildNode.h"

#include "XmlEditCommandDeleteNode.h"

#include "../Data/XmlDocument.h"

XmlEditCommandAddExistentChildNode::XmlEditCommandAddExistentChildNode(XmlDocument* xmlDocument, const QString& parentIdentifier,
                                                                       const QDomNode &newNode):
    XmlEditCommandBase(xmlDocument)
{
    m_parentIdentifier = parentIdentifier;
    m_addedNode = newNode.cloneNode(true);
}

void XmlEditCommandAddExistentChildNode::Execute()
{
    m_addedNodeIdentifier = m_pXmlDocument->AddChildNode(m_parentIdentifier, m_addedNode);
}

void XmlEditCommandAddExistentChildNode::Undo()
{
    //Delete the added node
    XmlEditCommandDeleteNode(m_pXmlDocument, m_addedNodeIdentifier).Execute();
}

const QString& XmlEditCommandAddExistentChildNode::GetAddeNodeIdentifier() const
{
    return m_addedNodeIdentifier;
}
