/**
 * File: XmlEditCommandAddChildNode.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef XMLEDITCOMMANDADDCHILDNODE_H
#define XMLEDITCOMMANDADDCHILDNODE_H

#include "XmlEditCommandBase.h"

/**
 * @brief The XmlEditCommandAddChildNode class that add a child node to a node
 */
class XmlEditCommandAddChildNode : public XmlEditCommandBase
{
private:
    QString m_parentIdentifier;
    QString m_nodeType;

    QString m_addedNodeIdentifier;
public:
    XmlEditCommandAddChildNode(XmlDocument* xmlDocument, const QString& parentIdentifier, const QString& nodeType);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();

    const QString& GetAddedNodeIdentifier() const;
};

#endif // XMLEDITCOMMANDADDCHILDNODE_H
