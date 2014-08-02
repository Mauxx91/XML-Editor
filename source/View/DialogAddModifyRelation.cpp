/**
 * File: DialogAddModifyRelation.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#include "DialogAddModifyRelation.h"

#include "../Data/XmlRelation.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

DialogAddModifyRelation::DialogAddModifyRelation(XmlRelation* xmlRelation, bool isNewRelation, QWidget *parent): QDialog(parent)
{
    //Initialize the xml relation
    m_pXmlRelation = xmlRelation;

    //Set the right window title
    if(isNewRelation)
    {
        setWindowTitle("Add new relation");
    }
    else
    {
        setWindowTitle("Modify relation");
    }

    //Initialize the layouts
    m_pVertLayout = new QVBoxLayout;
    m_pFormLayout1 = new QFormLayout;
    m_pFormLayout2 = new QFormLayout;
    m_pHorizontLayoutBnt = new QHBoxLayout;

    //Widgets for the input

    //Name of the tag from
    m_pTagFromNameLabel = new QLabel("Tag:");
    m_pTagFromNameLabel->setStatusTip("Name of the tag from which the relation start");
    m_pTagFromNameLine = new QLineEdit();
    m_pTagFromNameLine->setStatusTip("Name of the tag from which the relation start");
    m_pFormLayout1->addRow(m_pTagFromNameLabel, m_pTagFromNameLine);

    //Name of the sub tag that contains the attribute that refers tha key of the tag to
    m_pSubTagNameOfTagFromLabel = new QLabel("has child-tag:");
    m_pSubTagNameOfTagFromLabel->setStatusTip("Name of the sub tag that contains the attribute that refers tha key of the tag to");
    m_pSubTagNameOfTagFromLine = new QLineEdit();
    m_pSubTagNameOfTagFromLine->setStatusTip("Name of the sub tag that contains the attribute that refers tha key of the tag to");
    m_pFormLayout1->addRow(m_pSubTagNameOfTagFromLabel, m_pSubTagNameOfTagFromLine);

    //Name of the attribute that containt the key of the tag to
    m_pAttributeNameOfTagFromLabel = new QLabel("with key attribute:");
    m_pAttributeNameOfTagFromLabel->setStatusTip("Name of the attribute that containt the key of the tag to");
    m_pAttributeNameOfTagFromLine = new QLineEdit();
    m_pAttributeNameOfTagFromLine->setStatusTip("Name of the attribute that containt the key of the tag to");
    m_pFormLayout1->addRow(m_pAttributeNameOfTagFromLabel, m_pAttributeNameOfTagFromLine);

    //Initialize central label
    m_pCentralLabel = new QLabel("<b>which refers</b>");

    //Name of the tag destination if the relation
    m_pTagToNameLabel = new QLabel("tag:");
    m_pTagToNameLabel->setStatusTip("Name of the tag destination of the relation");
    m_pTagToNameLine = new QLineEdit();
    m_pTagToNameLine->setStatusTip("Name of the tag destination of the relation");
    m_pFormLayout2->addRow(m_pTagToNameLabel, m_pTagToNameLine);

    //Name of the attribute of the tag to that defines the key for the tag to
    m_pAttributeNameOfTagToLabel = new QLabel("with key attribute:");
    m_pAttributeNameOfTagToLabel->setStatusTip("Name of the attribute of the tag to that defines the key for the tag to");
    m_pAttributeNameOfTagToLine = new QLineEdit();
    m_pAttributeNameOfTagToLine->setStatusTip("Name of the attribute of the tag to that defines the key for the tag to");
    m_pFormLayout2->addRow(m_pAttributeNameOfTagToLabel, m_pAttributeNameOfTagToLine);

    //Relation type
    /*m_pRelationTypeLabel = new QLabel("Relation type:");
    m_pRelationTypeLabel->setStatusTip("Type of the relation is model in the XML");
    m_pRelationTypeLine = new QComboBox();
    m_pRelationTypeLine->setStatusTip("Type of the relation is model in the XML");
    //Initialize the combo Box
    QStringList items;
    items << "SUB_TAG" << "ATTRIBUTE" << "TAG_VALUE";
    m_pRelationTypeLine->addItems(items);
    m_pFormLayout->addRow(m_pRelationTypeLabel, m_pRelationTypeLine);
    connect(m_pRelationTypeLine,SIGNAL(currentIndexChanged(int)),this, SLOT(ChangedType(int)));*/

    //Initialize the input lines if it's fro modify a relation
    if(!isNewRelation)
    {
        InitializeValuesFromXmlRelation();
    }

    //Setup the button bar and connect the buttons with the QDialog signals
    m_pSaveBnt = new QPushButton(QIcon(":/icon/save.png"), "Save");
    m_pSaveBnt ->setDefault(true);
    m_pCancelBnt = new QPushButton("Cancel");
    connect(m_pSaveBnt,SIGNAL(clicked()),this, SLOT(SaveButtonPressed()));
    connect(m_pCancelBnt,SIGNAL(clicked()),this, SLOT(reject()));

    //Setup the horizontal layout
    m_pHorizontLayoutBnt->addWidget(m_pSaveBnt, 1, Qt::AlignRight);
    m_pHorizontLayoutBnt->addWidget(m_pCancelBnt, 0, Qt::AlignRight);

    //Setup the vertical layout
    m_pVertLayout->addLayout(m_pFormLayout1);
    m_pVertLayout->addItem(new QSpacerItem(10,10));
    m_pVertLayout->addWidget(m_pCentralLabel, 0, Qt::AlignCenter);
    m_pVertLayout->addItem(new QSpacerItem(10,10));
    m_pVertLayout->addLayout(m_pFormLayout2);
    m_pVertLayout->addItem(new QSpacerItem(20,20));
    m_pVertLayout->addLayout(m_pHorizontLayoutBnt);

    //Set the vertical layout as the layout of the dialof
    setLayout(m_pVertLayout);
}

void DialogAddModifyRelation::InitializeValuesFromXmlRelation()
{
    m_pTagFromNameLine->setText(m_pXmlRelation->GetTagFromName());
    m_pAttributeNameOfTagFromLine->setText(m_pXmlRelation->GetAttributeNameofTagFrom());
    m_pSubTagNameOfTagFromLine->setText(m_pXmlRelation->GetSubTagNameOfTagFrom());
    m_pTagToNameLine->setText(m_pXmlRelation->GetTagToName());
    m_pAttributeNameOfTagToLine->setText(m_pXmlRelation->GetAttributeNameOfTagTo());
    //m_pRelationTypeLine->setCurrentIndex(static_cast<int>(m_pXmlRelation->GetRelationType()));
}

void DialogAddModifyRelation::SaveTheModifiesInTheXmlObject()
{
    m_pXmlRelation->SetAll(m_pTagFromNameLine->text(), m_pAttributeNameOfTagFromLine->text(), m_pSubTagNameOfTagFromLine->text(),
                           m_pTagToNameLine->text(), m_pAttributeNameOfTagToLine->text(),
                           static_cast<XML_RELATION_TYPE>(0));
}

bool DialogAddModifyRelation::CheckInput() const
{
    //Chek that all the input line ins't empty
    if(m_pTagFromNameLine->text().isEmpty() || m_pAttributeNameOfTagFromLine->text().isEmpty() ||
       m_pSubTagNameOfTagFromLine->text().isEmpty() ||
       m_pTagToNameLine->text().isEmpty() || m_pAttributeNameOfTagToLine->text().isEmpty())
    {
        //Tell to the user the error
        QMessageBox::critical(0, "Input error", "All the input files must have a value.");

        return false;
    }
    else
    {
        //input is ok
        return true;
    }
}

/*
void DialogAddModifyRelation::ChangedType(int newIndex)
{
    //If is selected other that isn't SUB_TAG so hide disable the m_pSubTagNameOfTagFromLine
    if(newIndex > 0)
    {
        m_pSubTagNameOfTagFromLine->setEnabled(false);
    }
    else
    {
        m_pSubTagNameOfTagFromLine->setEnabled(true);
    }
}*/

void DialogAddModifyRelation::SaveButtonPressed()
{
    /*//If is selected other that isn't SUB_TAG say that it isn't supported
    if(m_pRelationTypeLine->currentIndex() > 0)
    {
        QMessageBox::warning(this, "Input error", "The \""+ m_pRelationTypeLine->currentText() +"\" type isn't supported yet. Please choose \"SUB_TAG\" type.");

        //return and not close the dialog
        return;
    }*/
    if(!CheckInput())
    {
        //return and not close the dialog
        return;
    }

    //It's all ok continue

    //Save the user input in the xml object
    SaveTheModifiesInTheXmlObject();

    //Close the dialog accepting the result
    QDialog::accept();
}

