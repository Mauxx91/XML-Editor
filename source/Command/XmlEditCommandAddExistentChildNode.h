/**
 * File: XmlEditCommandAddExistentChildNode.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef XMLEDITCOMMANDADDEXISTENTCHILDNODE_H
#define XMLEDITCOMMANDADDEXISTENTCHILDNODE_H

#include "XmlEditCommandBase.h"

#include <QDomNode>

/**
 * @brief The XmlEditCommandAddChildNode class that add an existent child node to a node
 */
class XmlEditCommandAddExistentChildNode : public XmlEditCommandBase
{
private:
    QDomNode m_addedNode;
    QString m_parentIdentifier;

    QString m_addedNodeIdentifier;
public:
    XmlEditCommandAddExistentChildNode(XmlDocument* xmlDocument, const QString& parentIdentifier,
                                       const QDomNode& newNode);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();

    const QString& GetAddeNodeIdentifier() const;
};

#endif // XMLEDITCOMMANDADDEXISTENTCHILDNODE_H
