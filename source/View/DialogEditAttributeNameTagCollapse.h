/**
 * File: DialogEditAttributeNameTagCollapse.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef DIALOGEDITATTRIBUTENAMETAGCOLLAPSE_H
#define DIALOGEDITATTRIBUTENAMETAGCOLLAPSE_H

#include "DialogEditAttributeNameTagCollapseBase.h"

class QFormLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief The DialogEditAttributeNameTagCollapse concrete class of a dialog able to edit the attribute tag name string
 */
class DialogEditAttributeNameTagCollapse : public DialogEditAttributeNameTagCollapseBase
{
    Q_OBJECT

private:
    //Layouts
    QFormLayout* m_pFormLayout; //For the form
    QVBoxLayout* m_pVertLayout; //Highest level layout that include all the others
    QHBoxLayout* m_pHorizontLayoutBnt; //For diplay horizontaly the buttons

    //Form components
    QLabel* m_pAttributeNameTagLabel;
    QLineEdit* m_pAttributeNameTagLine;

    //Buttons
    QPushButton* m_pApplyBnt;
    QPushButton* m_pCancelBnt;

public:
    /**
     * @brief DialogEditAttributeNameTagCollapse constructor tha initialize all the stuff needed
     * @param attributeNameTag: the current value for the attribute name tag
     * @param parent: parent widget
     */
    DialogEditAttributeNameTagCollapse(const QString& attributeNameTag = "", QWidget* parent = 0);

    /**
     * @brief GetAttributeNameTag: getter for waht the user has inserted in the input line
     * @return What the user has inserted in the input line
     */
    const QString GetAttributeNameTag() const;

    /**
     * @brief show: show the widget
     */
    virtual void Show();

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide();

};

#endif // DIALOGEDITATTRIBUTENAMETAGCOLLAPSE_H
