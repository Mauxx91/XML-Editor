/**
 * File: DialogXmlRelationManagementBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#ifndef DIALOGXMLRELATIONMANAGEMENTBASE_H
#define DIALOGXMLRELATIONMANAGEMENTBASE_H

#include "../Observer/IObserver.h"

#include <QDialog>

class XmlRelationCollection;

/**
 * @brief The DialogXmlRelationManagementBase astract base class for a dialog that permits to manage the xml relations
 */
class DialogXmlRelationManagementBase : public QDialog, public IObserver
{
protected:
    /**
     * @brief m_pXmlRelationCollection: the object that stores the collection of the xml relations
     */
    XmlRelationCollection* m_pXmlRelationCollection;

public:
    /**
     * @brief DialogXmlRelationManagementBase constructor that initializes the manber object
     * @param xmlRelationCollection: actual collection of the xml relation
     * @param parent widget of the dialog
     */
    DialogXmlRelationManagementBase(XmlRelationCollection* xmlRelationCollection, QWidget *parent = 0);

    /**
     * @brief ~DialogXmlRelationManagementBase virtual destructor
     */
    virtual ~DialogXmlRelationManagementBase();

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;

};

#endif // DIALOGXMLRELATIONMANAGEMENTBASE_H
