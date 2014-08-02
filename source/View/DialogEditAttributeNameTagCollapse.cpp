/**
 * File: DialogEditAttributeNameTagCollapse.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#include "DialogEditAttributeNameTagCollapse.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QIcon>
#include <QPushButton>

DialogEditAttributeNameTagCollapse::DialogEditAttributeNameTagCollapse(const QString& attributeNameTag, QWidget* parent):
    DialogEditAttributeNameTagCollapseBase(parent)
{
    //Iniialiaze the layouts
    m_pFormLayout = new QFormLayout;
    m_pVertLayout = new QVBoxLayout;
    m_pHorizontLayoutBnt = new QHBoxLayout;

    //Attribute name tag input
    m_pAttributeNameTagLabel = new QLabel("Attribute name filter:");
    m_pAttributeNameTagLine = new QLineEdit();
    m_pFormLayout->addRow(m_pAttributeNameTagLabel, m_pAttributeNameTagLine);

    //Initialize the input lines
    m_pAttributeNameTagLine->setText(attributeNameTag);

    //Setup the button bar and connect the buttons with the QDialog signals
    m_pApplyBnt = new QPushButton(QIcon(":/icon/save.png"), "Apply");
    m_pApplyBnt ->setDefault(true);
    m_pCancelBnt = new QPushButton("Cancel");
    connect(m_pApplyBnt,SIGNAL(clicked()),this,SLOT(accept()));
    connect(m_pCancelBnt,SIGNAL(clicked()),this,SLOT(reject()));

    //Setup the horizontal layout
    m_pHorizontLayoutBnt->addWidget(m_pApplyBnt, 1, Qt::AlignRight);
    m_pHorizontLayoutBnt->addWidget(m_pCancelBnt, 0, Qt::AlignRight);

    //Setup the vertical layout
    m_pVertLayout->addLayout(m_pFormLayout);
    m_pVertLayout->addItem(new QSpacerItem(20,20));
    m_pVertLayout->addLayout(m_pHorizontLayoutBnt);

    //Set the vertical layout as the layout of the dialof
    setLayout(m_pVertLayout);
}

const QString DialogEditAttributeNameTagCollapse::GetAttributeNameTag() const
{
    return m_pAttributeNameTagLine->text();
}

void DialogEditAttributeNameTagCollapse::Show()
{
    QDialog::show();
}

void DialogEditAttributeNameTagCollapse::Hide()
{
    QDialog::hide();
}

