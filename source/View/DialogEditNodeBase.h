/**
 * File: DialogEditNodeBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#ifndef DIALOGEDITNODEBASE_H
#define DIALOGEDITNODEBASE_H

#include <QDialog>

class XmlDocument;
class XmlEditCommandInvoker;

/**
 * @brief The DialogEditNodeBase abstract base class for a dialog to edit the attributes and the value of a node
 */
class DialogEditNodeBase : public QDialog
{
protected:
    /**
     * @brief m_pXmlDocument: the document in cui it's present the node to edit
     */
    XmlDocument* m_pXmlDocument;

    /**
     * @brief m_identifierNumber of the node to edit
     */
    QString m_identifierNumber;

    /**
     * @brief m_pXmlEditCommandInvoker: for invoke a command to the xml document displayed
     */
    XmlEditCommandInvoker* m_pXmlEditCommandInvoker;

public:
    DialogEditNodeBase(XmlEditCommandInvoker* xmlEditCommandInvoker, XmlDocument* xmlDocument, const QString& identifierNumber,
                       QWidget *parent = 0);

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;

};

#endif // DIALOGEDITNODEBASE_H
