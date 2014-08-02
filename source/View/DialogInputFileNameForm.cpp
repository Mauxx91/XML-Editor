/**
 * File: DialogInputFileNameForm.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/22
*/

#include "DialogInputFileNameForm.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

DialogInputFileNameForm::DialogInputFileNameForm(const QString& dialogTitle, const QString& labelText, const QString& applyBntText,
                                                 const QString& fileExtension, const QString& extendedFileExtension, bool choseNewFile,
                                                 QWidget* parent):
    DialogInputFileNameBase(fileExtension,parent), m_extendedFileExtension(extendedFileExtension), m_chooseNewFile(choseNewFile)
{
    //Set the window title
    setWindowTitle(dialogTitle);

    //Initialize the layouts
    m_pFormLayout = new QFormLayout;
    m_pHorizontLayoutBnt = new QHBoxLayout;
    m_pVertlayout = new QVBoxLayout;

    //Set up the title label
    m_pTitleLabel = new QLabel(labelText);

    //Set up the form widgets
    m_pAbsoluteFileNameLine = new QLineEdit;
    m_pAbsoluteFileNameLine->setFixedWidth(500);
    m_pBrowseButton = new QPushButton("browse");

    //Connect the browse button
    connect(m_pBrowseButton,SIGNAL(clicked()),this,SLOT(BrowseButtonPressed()));

    //Set up the form layout
    m_pFormLayout->addRow(m_pAbsoluteFileNameLine, m_pBrowseButton);

    //Initialize buttons
    m_pApplyBnt = new QPushButton(applyBntText);
    m_pCancelBnt = new QPushButton("Cancel");

    //Set the button layput
    m_pHorizontLayoutBnt->addWidget(m_pApplyBnt, 1, Qt::AlignRight);
    m_pHorizontLayoutBnt->addWidget(m_pCancelBnt, 0, Qt::AlignRight);

    //Connect the buttons
    connect(m_pApplyBnt,SIGNAL(clicked()),this,SLOT(ApplyButtonPressed()));
    connect(m_pCancelBnt,SIGNAL(clicked()),this,SLOT(reject()));

    //Setup the vertical layout
    m_pVertlayout->addWidget(m_pTitleLabel);
    m_pVertlayout->addLayout(m_pFormLayout);
    m_pVertlayout->addItem(new QSpacerItem(20,20));
    m_pVertlayout->addLayout(m_pHorizontLayoutBnt);

    //Set the main layout
    setLayout(m_pVertlayout);
}

QString DialogInputFileNameForm::GetAbsoluteFileName() const
{
    QString fileName = m_pAbsoluteFileNameLine->text();

    //Fix the extension
    if(!fileName.endsWith(m_fileExtension))
    {
        fileName += m_fileExtension;
    }

    return fileName;
}

bool DialogInputFileNameForm::CheckInput()
{
    //If the file in input is empty
    if(m_pAbsoluteFileNameLine->text().isEmpty())
    {
        QMessageBox::warning(this, "Input error", "The input file cannot be empty");
        return false;
    }

    //If the file hasn't an absolute path
    if(!(QFileInfo(GetAbsoluteFileName()).isAbsolute()))
    {
        QMessageBox::warning(this, "Input error", "The input file hasn't an absolute path.\n\n"
                                "The file must have an absolute path.");
         return false;
    }

    //No error found return true
    return true;
}

void DialogInputFileNameForm::BrowseButtonPressed()
{
    QString candidateFileName;

    if(m_chooseNewFile) //Choose new file
    {
        candidateFileName = QFileDialog::getSaveFileName(this, tr("Chose a file"),
        m_pAbsoluteFileNameLine->text() ,m_extendedFileExtension);
    }
    else //Choose pre-existent file
    {
        candidateFileName = QFileDialog::getOpenFileName(this, tr("Chose a file"),
        m_pAbsoluteFileNameLine->text() ,m_extendedFileExtension);
    }

    //If the file name is not empty
    if(!candidateFileName.isEmpty())
    {
        //Fix the extension
        if(!candidateFileName.endsWith(m_fileExtension))
        {
            candidateFileName += m_fileExtension;
        }

        //Set the file in input
        m_pAbsoluteFileNameLine->setText(candidateFileName);
    }
}

void DialogInputFileNameForm::ApplyButtonPressed()
{
    //Check the input
    bool inputOk = CheckInput();

    //If the input is ok cloase the dialog with accedpted
    if(inputOk)
    {
        QDialog::accept();
    }
}

void DialogInputFileNameForm::Show()
{
    QDialog::show();
}

void DialogInputFileNameForm::Hide()
{
    QDialog::hide();
}
