/**
 * File: DialogEditAttributeNameTagCollapseBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef DIALOGEDITATTRIBUTENAMETAGCOLLAPSEBASE_H
#define DIALOGEDITATTRIBUTENAMETAGCOLLAPSEBASE_H

#include <QDialog>

/**
 * @brief The DialogEditAttributeNameTagCollapseBase abstract base class of a dialog able to edit the attribute tag name string
 */
class DialogEditAttributeNameTagCollapseBase : public QDialog
{

public:
    /**
     * @brief DialogEditAttributeNameTagCollapseBase constructor that only set the parent widget to the base class
     * @param parent: parent widget
     */
    DialogEditAttributeNameTagCollapseBase(QWidget *parent = 0);

    /**
     * @brief ~DialogEditAttributeNameTagCollapseBase vitual destructor
     */
    virtual ~DialogEditAttributeNameTagCollapseBase();

    /**
     * @brief GetAttributeNameTag: Getter for the inserted attribute name tag collapse
     * @return The inserted attribute name tag collapse
     */
    virtual const QString GetAttributeNameTag() const = 0;

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;
};

#endif // DIALOGEDITATTRIBUTENAMETAGCOLLAPSEBASE_H
