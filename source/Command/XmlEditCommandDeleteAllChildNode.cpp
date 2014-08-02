#include "XmlEditCommandDeleteAllChildNode.h"

#include "XmlEditCommandAddExistentChildNode.h"

#include "../Data/XmlDocument.h"

XmlEditCommandDeleteAllChildNode::XmlEditCommandDeleteAllChildNode(XmlDocument* xmlDocument, const QString& parentIdentifier, const QString& nodeType):
    XmlEditCommandBase(xmlDocument)
{
    m_parentIdentifier = parentIdentifier;
    m_nodeType = nodeType;
}

void XmlEditCommandDeleteAllChildNode::Execute()
{
    m_deletedNodes = m_pXmlDocument->DeleteAllChildNode(m_parentIdentifier, m_nodeType);
}

void XmlEditCommandDeleteAllChildNode::Undo()
{
    //Restore all the eliminated nodes
    for(int i=0; i < m_deletedNodes.size(); ++i)
    {
        XmlEditCommandAddExistentChildNode(m_pXmlDocument, m_parentIdentifier, m_deletedNodes.at(i)).Execute();
    }
}
