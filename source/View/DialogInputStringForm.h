/**
 * File: DialogInputStringForm.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef DIALOGINPUTSTRINGFORM_H
#define DIALOGINPUTSTRINGFORM_H

#include "DialogInputStringBase.h"

class QFormLayout;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief The DialogInputFileNameForm class that implements the Dialog with a form for take the input
 */
class DialogInputStringForm : public DialogInputStringBase
{
    Q_OBJECT

private:
    //Layouts
    QFormLayout* m_pFormLayout;
    QHBoxLayout* m_pHorizontLayoutBnt;
    QVBoxLayout* m_pVertlayout;

    //Title label
    QLabel* m_pTitleLabel;

    //Form Widgets
    QLineEdit* m_pInputStringLine;

    //Buttons
    QPushButton* m_pApplyBnt;
    QPushButton* m_pCancelBnt;

public:
    /**
     * @brief DialogInputStringForm
     * @param dialogTitle
     * @param labelText
     * @param applyBntText
     * @param initialString with fill the form at the begin
     * @param parent
     */
    DialogInputStringForm(const QString& dialogTitle, const QString& labelText, const QString& applyBntText,
                          const QString& initialString = "", QWidget* parent = 0);

    /**
     * @brief GetInputString: get the string in input
     * @return the string inserted by the user
     */
    QString GetInputString() const;

private:
    /**
     * @brief CheckInput: check if the string in input is not empty
     * @return true if the the input data is ok
     */
    bool CheckInput();

public slots:
    /**
     * @brief ApplyButtonPressed: check the input and if it's ok close the dialog
     */
    void ApplyButtonPressed();

    /**
     * @brief show: show the widget
     */
    virtual void Show();

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide();
};

#endif // DIALOGINPUTSTRINGFORM_H
