/**
 * File: DialogInputFileNameForm.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/22
*/

#ifndef DIALOGINPUTFILENAMEFORM_H
#define DIALOGINPUTFILENAMEFORM_H

#include "DialogInputFileNameBase.h"

class QFormLayout;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief The DialogInputFileNameForm class that implements the Dialog with a form for take the input
 */
class DialogInputFileNameForm : public DialogInputFileNameBase
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
    QLineEdit* m_pAbsoluteFileNameLine;
    QPushButton* m_pBrowseButton;

    //Buttons
    QPushButton* m_pApplyBnt;
    QPushButton* m_pCancelBnt;

    //File extension for validate the input in the browse dialog
    QString m_extendedFileExtension;

    /**
     * @brief m_chooseNewFile: is it has to choose a new file or an existent one
     */
    bool m_chooseNewFile;

public:
    /**
     * @brief DialogInputFileNameForm: initiali the dialog
     * @param fileExtension of the input file
     * @param parent widget
     */
    DialogInputFileNameForm(const QString& dialogTitle, const QString& labelText, const QString& applyBntText,
                            const QString& fileExtension, const QString& extendedFileExtension, bool choseNewFile, QWidget* parent = 0);

    /**
     * @brief GetAbsoluteFileName: get the abosulte file name of a file with the inserted extension
     * @return the inserted absolute File Name
     */
    QString GetAbsoluteFileName() const;

private:
    /**
     * @brief CheckInput: check if the file name inserted is absolute and not empty and show the error to the user
     * @return true if the the input data is ok
     */
    bool CheckInput();

public slots:
    /**
     * @brief BrowseButtonPressed: show the dialog to explore the file and choose a file
     */
    void BrowseButtonPressed();

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

#endif // DIALOGINPUTFILENAMEFORM_H
