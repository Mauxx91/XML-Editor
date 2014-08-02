/**
 * File: DialoAssociatedFilesForm.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/18
*/

#ifndef DIALOASSOCIATEDFILESFORM_H
#define DIALOASSOCIATEDFILESFORM_H

#include "DialogAssociatedFilesBase.h"
#include "../Observer/IObserver.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QFormLayout;
class QHBoxLayout;
class QVBoxLayout;

/**
 * @brief The DialoAssociatedFilesForm class that implements DialoAssociatedFilesBase with a form layout
 */
class DialogAssociatedFilesForm : public DialogAssociatedFilesBase, public IObserver
{
    Q_OBJECT

private:

    /**
     * @brief The FormEntry class the represent a line for edit a associated file
     */
    class FormEntry: public QWidget{
    //All member is public because the class is simple used to collect the widget of the line
    public:
        //The current index to display the right number line
        int m_currentIndex;

        //layputs
        QFormLayout* m_pFormLayout;
        QHBoxLayout* m_pHorizontalLayout;

        //Line widgets
        QLabel* m_pFileLabel;
        QLineEdit* m_pAbsoluteFileNameLine;
        QPushButton* m_pBrowseButton;
        QPushButton* m_pDeleteEntry;

        //Constructor tha initialize the line
        FormEntry(int numFile, const QString& absoluteFileName = "", QWidget* parent = 0);

        //Change the current index and update the label
        void setCurrentIndex(int currentIndex);

    };

    //Layouts
    QVBoxLayout* m_pExternalLayout;
    QVBoxLayout* m_pInternalLayout;
    QHBoxLayout* m_pHorizontLayoutBnt;

    //List of input file, one for each associated files
    QList<FormEntry*> m_pFormEntryList;

    //Buttons
    QPushButton* m_pAddAssociatedFileBnt;
    QPushButton* m_pApplyBnt;
    QPushButton* m_pCancelBnt;

    //Title of the dialog
    QLabel* m_pTitleLabel;

public:
    /**
     * @brief DialoAssociatedFilesForm: create and initialize the dialog
     * @param parent widget
     */
    DialogAssociatedFilesForm(AssociatedFiles* associatedFiles, QWidget *parent = 0);

private slots:

    /**
     * @brief AddNewAssociatedFile: add a new line for an associated file
     * @param absoluteFileName: the file name of the new associated files
     */
    void AddNewAssociatedFile(const QString& absoluteFileName = "");

    /**
     * @brief BrowseButtonPressed: open the file explorer and fill the line of the button with the selected file
     */
    void BrowseButtonPressed();

    /**
     * @brief DeleteButtonPressed: delete the associated file of the line in which the delete button was pressed
     */
    void DeleteButtonPressed();

    /**
     * @brief ApplyButtonPressed: apply the user changes to the associated file object
     */
    void ApplyButtonPressed();

private:

    /**
     * @brief checkInput: for every line check if the input is valid
     * @return
     */
    bool checkInput();

    /**
     * @brief ApplyTheModifies: real apply the modifies to the associated file object
     */
    void ApplyTheModifies();

    /**
     * @brief SetTitleAlmostOneAssociatedFile: set the title that has to be dislpayed when almost one file is associated
     */
    void SetTitleAlmostOneAssociatedFile();

    /**
     * @brief SetTitleNoAssociatedFiles: set the title that has to be dislpayed when no files is associated
     */
    void SetTitleNoAssociatedFiles();

public:
    /**
     * @brief Update
     * @param updateCode: to know what time of update is needed
     */
    virtual void Update(UPDATE_CODE updateCode);

    /**
     * @brief show: show the widget
     */
    virtual void Show();

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide();
};

#endif // DIALOASSOCIATEDFILESFORM_H
