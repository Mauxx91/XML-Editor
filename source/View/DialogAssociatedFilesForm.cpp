/**
 * File: DialogAssociatedFilesForm.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/18
*/

#include "DialogAssociatedFilesForm.h"

#include "../Data/AssociatedFiles.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

DialogAssociatedFilesForm::FormEntry::FormEntry(int numFile, const QString& absoluteFileName, QWidget *parent): QWidget(parent)
{
    m_currentIndex = numFile - 1;

    //Initialize the layout
    m_pFormLayout = new QFormLayout;
    m_pHorizontalLayout = new QHBoxLayout;

    //Initialize the widgets and set the initial path in the m_pAbsoluteFileNameLine
    m_pFileLabel = new QLabel("File " + QString::number(numFile) + " :" );
    m_pAbsoluteFileNameLine = new QLineEdit;
    m_pAbsoluteFileNameLine->setFixedWidth(500);
    m_pAbsoluteFileNameLine->setText(absoluteFileName);
    m_pBrowseButton = new QPushButton("browse");
    m_pDeleteEntry = new QPushButton(QIcon(":/icon/delete.png"), "remove");

    //Set the auxiliary horizontal layout
    m_pHorizontalLayout->addWidget(m_pAbsoluteFileNameLine);
    m_pHorizontalLayout->addWidget(m_pBrowseButton);
    m_pHorizontalLayout->addWidget(m_pDeleteEntry);

    //Set the form layout
    m_pFormLayout->addRow(m_pFileLabel, m_pHorizontalLayout);

    //Set the mai layout
    setLayout(m_pFormLayout);
}

void DialogAssociatedFilesForm::FormEntry::setCurrentIndex(int currentIndex)
{
    m_currentIndex = currentIndex;

    //Set the current index in the file label
    m_pFileLabel->setText("File " + QString::number(currentIndex + 1) + " :" );
}

DialogAssociatedFilesForm::DialogAssociatedFilesForm(AssociatedFiles* associatedFiles, QWidget* parent) :
    DialogAssociatedFilesBase(associatedFiles, parent)
{
    //Set window title
    this->setWindowTitle("Manage Associated XML");

    //Set the window size
    resize(900, 600);

    //Initialize the layouts
    m_pExternalLayout = new QVBoxLayout;
    m_pInternalLayout = new QVBoxLayout;
    m_pHorizontLayoutBnt = new QHBoxLayout;
    m_pExternalLayout->setSizeConstraint(QLayout::SetFixedSize);
    m_pInternalLayout->setSizeConstraint(QLayout::SetFixedSize);

    //Initializate the title label
    m_pTitleLabel = new QLabel;

    //Set the initial title
    SetTitleNoAssociatedFiles();

    //Initialize buttons
    m_pAddAssociatedFileBnt = new QPushButton(QIcon(":/icon/add.png"), "Add new associated file");
    m_pApplyBnt = new QPushButton(QIcon(":/icon/save.png"), "Apply");
    m_pCancelBnt = new QPushButton("Cancel");

    //Set the button layput
    m_pHorizontLayoutBnt->addWidget(m_pAddAssociatedFileBnt, 1, Qt::AlignRight);
    m_pHorizontLayoutBnt->addWidget(m_pApplyBnt, 0, Qt::AlignRight);
    m_pHorizontLayoutBnt->addWidget(m_pCancelBnt, 0, Qt::AlignRight);

    //Connect the buttons
    connect(m_pAddAssociatedFileBnt, SIGNAL(clicked()), this, SLOT(AddNewAssociatedFile()));
    connect(m_pApplyBnt,SIGNAL(clicked()),this,SLOT(ApplyButtonPressed()));
    connect(m_pCancelBnt,SIGNAL(clicked()),this,SLOT(reject()));

    //Setup the vertical layout
    m_pExternalLayout->addWidget(m_pTitleLabel);
    m_pExternalLayout->addLayout(m_pInternalLayout);
    m_pExternalLayout->addItem(new QSpacerItem(20,20));
    m_pExternalLayout->addLayout(m_pHorizontLayoutBnt);

    //Set the main layout
    setLayout(m_pExternalLayout);

    //Set the content to match the current values of m_pAssociatedFiles
    const QStringList& fileNamesList = m_pAssociatedFiles->GetAssociatedFileNamesList();

    //For every entry in the list create and display a QFormEntry in the internal layout
    for(int i=0; i<fileNamesList.size(); ++i)
    {
        //Current file name
        const QString& absoluteFileName = fileNamesList.at(i);

        AddNewAssociatedFile(absoluteFileName);
    }

    //Register as observer to the associated file because the main window could be add a file during the dialog is opened
    m_pAssociatedFiles->AttachObserver(this);
}

DialogAssociatedFilesForm::~DialogAssociatedFilesForm()
{
    m_pAssociatedFiles->DetachObserver(this);
}

void DialogAssociatedFilesForm::AddNewAssociatedFile(const QString& absoluteFileName)
{
    //Set the right text in the title label only at the fist insertion
    if(m_pFormEntryList.size() == 0)
    {
        SetTitleAlmostOneAssociatedFile();
    }

    //Create a new form entry
    FormEntry* formEntry = new FormEntry(m_pFormEntryList.size()+1, absoluteFileName, this);

    //Add in the list
    m_pFormEntryList.append(formEntry);

    //Display the form entry by adding to the internal layout
    m_pInternalLayout->addWidget(formEntry);

    //Connect the buttons
    connect(formEntry->m_pBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseButtonPressed()));
    connect(formEntry->m_pDeleteEntry, SIGNAL(clicked()), this, SLOT(DeleteButtonPressed()));

    //Hide the m_pNoAssociatedFilesLabel
    SetTitleAlmostOneAssociatedFile();
}

 void DialogAssociatedFilesForm::BrowseButtonPressed()
 {
     //Understand for which line the buttons has been pressed

     //Object that has emitted the signal
     QObject* objectThatHasCalledTheSignal = QObject::sender();

     //Only the button box is connected with this signal so trasform in a PushButton
     QPushButton* deleteButton = dynamic_cast<QPushButton*>(objectThatHasCalledTheSignal);

     //Obtain the parent widget of the delete box
     QObject* deleteButtonParent = deleteButton->parent();

     //The delete box has always as parent a FormEntry so it can be casted to
     FormEntry* formEntry = dynamic_cast<FormEntry*>(deleteButtonParent);

     //Index of the form entry to delete
     int indexFormEntryToDelete = formEntry->m_currentIndex;

     //Open new file
     const QString& candidateFileName = QFileDialog::getOpenFileName(this, tr("Chose a file"),
     m_pFormEntryList.at(indexFormEntryToDelete)->m_pAbsoluteFileNameLine->text() ,tr("XML files (*.xml);; Text files (*.txt);; All files (*)"));

     //If the selection is valid
     if( !candidateFileName.isEmpty() )
     {
         //Set the new path in the line edit
         m_pFormEntryList.at(indexFormEntryToDelete)->m_pAbsoluteFileNameLine->setText(candidateFileName);
     }
 }


void DialogAssociatedFilesForm::DeleteButtonPressed()
{
    //Understand for which line the buttons has been pressed

    //Object that has emitted the signal
    QObject* objectThatHasCalledTheSignal = QObject::sender();

    //Only the button box is connected with this signal so trasform in a PushButton
    QPushButton* deleteButton = dynamic_cast<QPushButton*>(objectThatHasCalledTheSignal);

    //Obtain the parent widget of the delete box
    QObject* deleteButtonParent = deleteButton->parent();

    //The delete box has always as parent a FormEntry so it can be casted to
    FormEntry* formEntry = dynamic_cast<FormEntry*>(deleteButtonParent);

    //Index of the form entry to delete
    int indexFormEntryToDelete = formEntry->m_currentIndex;

    //Delete the entry form at that line and after remove from the list
    delete m_pFormEntryList.at(indexFormEntryToDelete);
    m_pFormEntryList.removeAt(indexFormEntryToDelete);

    //Refactor the index of all the others form entry
    for(int i = indexFormEntryToDelete; i < m_pFormEntryList.size(); ++i)
    {
        m_pFormEntryList.at(i)->setCurrentIndex(i);
    }

    //Control if there aren't any files
    if(m_pFormEntryList.size() == 0)
    {
        //Set the text
        SetTitleNoAssociatedFiles();
    }
}

void DialogAssociatedFilesForm::ApplyButtonPressed()
{
    //Check the input, continue only if the input is ok
    if(checkInput())
    {
        //Apply the user modifies in the associated files object
        ApplyTheModifies();

        //Close the dialog
        QDialog::accept();
    }
}

bool DialogAssociatedFilesForm::checkInput()
{
    //As just an error is found return false

    //Check if the user hasn't inserted multiple times the same file name
    QStringList alreadyCheckedFiles; //list used for not check again an already check files
    for(int e=0; e <m_pFormEntryList.size(); ++e)
    {
        const QString& formFileName1 = m_pFormEntryList.at(e)->m_pAbsoluteFileNameLine->text();
        int fileOccurrence = 0; //how many time formFileName1 is present in the form

        //For avoid multiple waring for the same file check a file only if it isn't checked yet
        if(!alreadyCheckedFiles.contains(formFileName1, Qt::CaseInsensitive))
        {
            alreadyCheckedFiles.append(formFileName1); //To remember and not check again

            //Check if the file exists by comparing with all the filenames already present in the object
            for(int i=0; i <m_pFormEntryList.size(); ++i)
            {
                const QString& formFileName2 = m_pFormEntryList.at(i)->m_pAbsoluteFileNameLine->text();

                if(formFileName1.compare(formFileName2, Qt::CaseInsensitive) == 0)//0 if the compare result true
                {
                    fileOccurrence++;
                }
            }

            //If doens't exist insert the file in the object
            if(fileOccurrence > 1)
            {
                QMessageBox::warning(this, "Input error", "The associated file: \"" + formFileName1 + "\" has been inserted " + QString::number(fileOccurrence) + " times.\n\n"
                                    "Each associated file must be inserted at maximun in one row.");
                return false;
            }
        }
    }

    //Check if the file name is absolute and not empty and different from the main file
    for(int e=0; e <m_pFormEntryList.size(); ++e)
    {
        const QString& formFileName = m_pFormEntryList.at(e)->m_pAbsoluteFileNameLine->text();

        //If the file name is empty
        if(formFileName.isEmpty())
        {
            QMessageBox::warning(this, "Input error", QString::fromUtf8("The associated file n° ") + QString::number(e+1) + " is empty.\n\n"
                                "Each file must haven't a empty path.");
            return false;
        }
        //If the file hasn't an absolute path
        else if(!QFileInfo(formFileName).isAbsolute())
        {
            QMessageBox::warning(this, "Input error", "The associated file: \"" + formFileName + "\" hasn't an absolute path.\n\n"
                                "Each file must have an absolute path.");
            return false;
        }
        else if(formFileName.compare(m_pAssociatedFiles->GetMainFileAbsoluteFileName(), Qt::CaseInsensitive) == 0)//0 if the compare result true
        {
            QMessageBox::warning(this, "Input error", "The associated file: \"" + formFileName + "\" is equal to the main file.\n\n"
                                "Each associated file must be different from the main file.");
            return false;
        }

    }

    //Check if the files don't exist is postponed during the opening of the file

    //No error found return true
    return true;
}

void DialogAssociatedFilesForm::ApplyTheModifies()
{
    //Apply the user modifies in the associated files object

    //File actually present in the object
    const QStringList& fileNamesList = m_pAssociatedFiles->GetAssociatedFileNamesList();

    //Remove files present in the object and no longer present now in the form
    //For each file present in the object check if is present in the form now
    for(int i=0; i<fileNamesList.size(); ++i)
    {
        bool filePresent = false;

        //Check if the file is present by comparing with all the filenames
        for(int e=0; e <m_pFormEntryList.size() && !filePresent; ++e)
        {
            const QString& formFileName = m_pFormEntryList.at(e)->m_pAbsoluteFileNameLine->text();

            if(formFileName.compare(fileNamesList.at(i), Qt::CaseInsensitive) == 0) //0 if the compare result true
            {
                filePresent = true;
            }
        }

        //If the file is no longer present in the form remove from the object
        if(!filePresent)
        {
            m_pAssociatedFiles->RemoveAssociatedFile(i);
        }
    }

    //Add the file doesn't present yet in the object
    //For each file present in the form check if is not present and it is inserted
    for(int e=0; e <m_pFormEntryList.size(); ++e)
    {
        bool fileAlreadyPresent = false;

        const QString& formFileName = m_pFormEntryList.at(e)->m_pAbsoluteFileNameLine->text();

        //Check if the file exists by comparing with all the filenames already present in the object
        for(int i=0; i<fileNamesList.size() && !fileAlreadyPresent; ++i)
        {
            if(formFileName.compare(fileNamesList.at(i),Qt::CaseInsensitive) == 0) //0 if the compare result true
            {
                fileAlreadyPresent = true;
            }
        }

        //If doens't exist insert the file in the object
        if(!fileAlreadyPresent)
        {
            m_pAssociatedFiles->AddAssociatedFileName(formFileName);
        }
    }
}

void DialogAssociatedFilesForm::SetTitleAlmostOneAssociatedFile()
{
    m_pTitleLabel->setText("Click the 'Add' button below to add a new associated file.");
}

void DialogAssociatedFilesForm::SetTitleNoAssociatedFiles()
{
    m_pTitleLabel->setText("There aren't any associated files. Click on the button below to add one.");
}

void DialogAssociatedFilesForm::Update(UPDATE_CODE updateCode)
{
    //If a new associated file was added
    if(updateCode == UPDATE_NEW_ASSOCIATED_FILE)
    {
        //The associated file class ensure that the associated file that was addes is a new file not pre-existent in the collection
        AddNewAssociatedFile(m_pAssociatedFiles->GetLastModifiedFileName());
    }
}

void DialogAssociatedFilesForm::Show()
{
    QDialog::show();
}

void DialogAssociatedFilesForm::Hide()
{
    QDialog::hide();
}

