/**
 * File: DialogInputStringForm.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "DialogInputStringForm.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

DialogInputStringForm::DialogInputStringForm(const QString& dialogTitle, const QString& labelText, const QString& applyBntText,
                                             const QString& initialString, QWidget* parent):
    DialogInputStringBase(parent)
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
    m_pInputStringLine = new QLineEdit;
    m_pInputStringLine->setFixedWidth(250);
    m_pInputStringLine->setText(initialString);

    //Set up the form layout
    m_pFormLayout->addRow(m_pTitleLabel, m_pInputStringLine);

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
    m_pVertlayout->addLayout(m_pFormLayout);
    m_pVertlayout->addItem(new QSpacerItem(20,20));
    m_pVertlayout->addLayout(m_pHorizontLayoutBnt);

    //Set the main layout
    setLayout(m_pVertlayout);
}

QString DialogInputStringForm::GetInputString() const
{
    return m_pInputStringLine->text();
}

bool DialogInputStringForm::CheckInput()
{

    //If the file hasn't an absolute path
    if(m_pInputStringLine->text().isEmpty())
    {
        QMessageBox::warning(this, "Input error", "The input string cannot be empty.");
        return false;
    }

    //No error found return true
    return true;
}

void DialogInputStringForm::ApplyButtonPressed()
{
    //Check the input
    bool inputOk = CheckInput();

    //If the input is ok cloase the dialog with accedpted
    if(inputOk)
    {
        QDialog::accept();
    }
}

void DialogInputStringForm::Show()
{
    QDialog::show();
}

void DialogInputStringForm::Hide()
{
    QDialog::hide();
}
