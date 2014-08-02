#ifndef XMLEDITCOMMANDDELETEALLCHILDNODE_H
#define XMLEDITCOMMANDDELETEALLCHILDNODE_H

#include "XmlEditCommandBase.h"

#include <QList>
#include <QDomNode>

/**
 * @brief The XmlEditCommandDeleteAllChildNode class that delete all the child node of the given node
 */
class XmlEditCommandDeleteAllChildNode : public XmlEditCommandBase
{
private:
    QString m_parentIdentifier;
    QString m_nodeType;

    QList<QDomNode> m_deletedNodes;
public:
    XmlEditCommandDeleteAllChildNode(XmlDocument* xmlDocument, const QString& parentIdentifier, const QString& nodeType);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();
};

#endif // XMLEDITCOMMANDDELETEALLCHILDNODE_H
