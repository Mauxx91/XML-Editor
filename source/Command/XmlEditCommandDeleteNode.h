/**
 * File: XmlEditCommandDeleteNode.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef XMLEDITCOMMANDDELETENODE_H
#define XMLEDITCOMMANDDELETENODE_H

#include "XmlEditCommandBase.h"

#include <QDomNode>

/**
 * @brief The XmlEditCommandDeleteNode class that delete a node
 */
class XmlEditCommandDeleteNode : public XmlEditCommandBase
{
private:
    QString m_identifierNumber;

    QDomNode m_deleteNodeCopy;
public:
    XmlEditCommandDeleteNode(XmlDocument* xmlDocument, const QString& identifierNumber);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();
};

#endif // XMLEDITCOMMANDDELETENODE_H
