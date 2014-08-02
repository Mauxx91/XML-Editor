/**
 * File: DialogXmlRelationManagementTable.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#include "DialogAddModifyRelation.h"
#include "../Data/XmlRelation.h"
#include "../Data/XmlRelationCollection.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QMenu>

#include "DialogXmlRelationManagementTable.h"

DialogXmlRelationManagementTable::DialogXmlRelationManagementTable(XmlRelationCollection *xmlRelationCollection, QWidget *parent) :
    DialogXmlRelationManagementBase(xmlRelationCollection, parent)
{
    //Set window title
    this->setWindowTitle("Manage XML Relations");

    //Resize the dialog
    this->resize(890, 500);

    //Make the bold font a real bold font
    m_boldFont.setBold(true);

    //Initialize the title label
    m_pTitleLabel = new QLabel("Relations list:");

    //Initialize the table
    m_pTable = new QTableWidget(0,6);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Set the table hoizontal header
    QStringList headers;
    headers << "Tag" << "has child-tag" << "with key attribute"
            << "" << "tag" << "with key attribute" ;
    m_pTable->setHorizontalHeaderLabels(headers);

    //Set the header bold
    m_pTable->verticalHeader()->setFont(m_boldFont);
    m_pTable->horizontalHeader()->setFont(m_boldFont);

    //Set the right column width
    m_pTable->setColumnWidth(0, 140);
    m_pTable->setColumnWidth(1, 140);
    m_pTable->setColumnWidth(2, 160);
    m_pTable->setColumnWidth(3, 80);
    m_pTable->setColumnWidth(4, 140);
    m_pTable->setColumnWidth(5, 160);

    //Connect the table
    connect(m_pTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(CellDoubleClicked(int, int)));

    //Set the custom contextual menu for the table
    m_pTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextualMenuTable(QPoint)));

    //Initialize the buttons
    m_pAddRelationBnt =  new QPushButton(QIcon(":/icon/add.png"), "Add new relation");
    m_pAddRelationBnt->setStatusTip("Add a new XML relation");
    m_pOkBnt = new QPushButton("Ok");

    //Connect the buttons
    connect(m_pAddRelationBnt, SIGNAL(clicked()), this, SLOT(AddRelationPressed()));
    connect(m_pOkBnt, SIGNAL(clicked()), this, SLOT(OkButtonPressed()));

    //Initialize the layouts
    m_pVerticalMainLayout = new QVBoxLayout;
    m_pHorizontalBntLayout = new QHBoxLayout;

    //Fill the horizontal layout
    m_pHorizontalBntLayout->addWidget(m_pAddRelationBnt, 1, Qt::AlignRight);
    m_pHorizontalBntLayout->addWidget(m_pOkBnt, 0, Qt::AlignRight);

    //Fill the main vertical layout
    m_pVerticalMainLayout->addWidget(m_pTitleLabel);
    m_pVerticalMainLayout->addWidget(m_pTable);
    m_pVerticalMainLayout->addLayout(m_pHorizontalBntLayout);

    //Attach as Observer in the XmlRelationCollection
    m_pXmlRelationCollection->AttachObserver(this);

    //Attach as observer in all the existent relations in the xml relation collection
    for(int i=0; i<m_pXmlRelationCollection->Size(); ++i)
    {
        m_pXmlRelationCollection->GetRelationAt(i)->AttachObserver(this);
    }

    //Force a reload of the existent relation
    ReFillTheTable();

    //Set the main layout
    setLayout(m_pVerticalMainLayout);
}

DialogXmlRelationManagementTable::~DialogXmlRelationManagementTable()
{
    //Detach from the colletion
    m_pXmlRelationCollection->DetachObserver(this);

    //Detach as observer from all the existent relations in the xml relation collection
    for(int i=0; i<m_pXmlRelationCollection->Size(); ++i)
    {
        m_pXmlRelationCollection->GetRelationAt(i)->DetachObserver(this);
    }
}

void DialogXmlRelationManagementTable::ModifyRelationPressed(int row)
{
    //The xml object that has to be updated with the user input
    XmlRelation* newXmlRelation = m_pXmlRelationCollection->GetRelationAt(row);

    DialogAddModifyRelation dialog(newXmlRelation, false); //false is for the modify

    //shows it, it mmodify by itself the relation
    dialog.exec();
}

void DialogXmlRelationManagementTable::DeleteRelationPressed(int row)
{
    //Before delete ask to the user if the is he real secure
    QMessageBox msgBox;
    msgBox.setText("The procedure is irreversible. Are you real sure to remove the relation number "+ QString::number(row+1) +"?");
    msgBox.setIcon(QMessageBox::Question);

    //Buttons
    QPushButton* deleteBnt = new QPushButton(QIcon(":/icon/delete.png"), "Delete");
    msgBox.addButton(deleteBnt, QMessageBox::AcceptRole);
    connect(deleteBnt, SIGNAL(clicked()), &msgBox, SLOT(accept()));
    QPushButton* cancelBnt = msgBox.addButton("Cancel", QMessageBox::RejectRole);
    connect(cancelBnt, SIGNAL(clicked()), &msgBox, SLOT(reject()));

    //Set cancel as defalut button
    msgBox.setDefaultButton(cancelBnt);

    //Show the dialog and store the result
    int result = msgBox.exec();

    //If the user has confirmed
    if(result == QMessageBox::Accepted)
    {
        //Delete the xml relation from the xml collection
        m_pXmlRelationCollection->RemoveXmlRelation(row);
    }
}

void DialogXmlRelationManagementTable::AddRelationPressed()
{
    //The xml object that will be filled with the user input
    XmlRelation* newXmlRelation = new XmlRelation;

    DialogAddModifyRelation dialog(newXmlRelation, true); //tru is for new relation

    //take the result of the dialog and shows it
    int result = dialog.exec();

    //If the user has clicked tha save button
    if(result == QDialog::Accepted)
    {
        //Set the new xml relation in the relation collection
        m_pXmlRelationCollection->AddXmlRelation(newXmlRelation);

        //Attach as observer of the new xml relation
        newXmlRelation->AttachObserver(this);
    }
}

void DialogXmlRelationManagementTable::ReFillTheTable()
{
    //Set the row count as the number of the relation
    m_pTable->setRowCount(m_pXmlRelationCollection->Size());

    //variable used for fill the table
    QTableWidgetItem* item;

    //Fill a row for each relation
    for(int rowIndex=0; rowIndex<m_pXmlRelationCollection->Size(); ++rowIndex)
    {
        int columnIndex = 0;

        XmlRelation* xmlRelation = m_pXmlRelationCollection->GetRelationAt(rowIndex);

        //headers << "This tag" << "has this child-tag" << "with this attribute"
        //        << ""         << "this tag"           << "with this attribute" ;

        item = new QTableWidgetItem(xmlRelation->GetTagFromName()); //Tag from name <<"This tag"
        item->setToolTip(xmlRelation->GetTagFromName());
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;

        item = new QTableWidgetItem(xmlRelation->GetSubTagNameOfTagFrom()); //Tag name of sub tag from <<"has this child-tag"
        item->setToolTip(xmlRelation->GetSubTagNameOfTagFrom());
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;

        item = new QTableWidgetItem(xmlRelation->GetAttributeNameofTagFrom()); //Attribute name of tag from <<"with this attribute"
        item->setToolTip(xmlRelation->GetAttributeNameofTagFrom());
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;

        item = new QTableWidgetItem("which refers"); //<< ""
        item->setFont(m_boldFont); //Set the bold font to diplay this item
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;

        item = new QTableWidgetItem(xmlRelation->GetTagToName()); //Tag to name <<"this tag"
        item->setToolTip(xmlRelation->GetTagToName());
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;

        item = new QTableWidgetItem(xmlRelation->GetAttributeNameOfTagTo()); //Attribute name of tag to <<"with this attribute"
        item->setToolTip(xmlRelation->GetAttributeNameOfTagTo());
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;

        /*QString relationType;
        switch(xmlRelation->GetRelationType())
        {
            case SUB_TAG: relationType = "SUB_TAG";
                break;
            case ATTRIBUTE: relationType = "ATTRIBUTE";
                break;
            case TAG_VALUE: relationType = "TAG_VALUE";
                break;
        }

        item = new QTableWidgetItem(relationType); //Relation type
        item->setToolTip(relationType);
        m_pTable->setItem(rowIndex, columnIndex, item);
        columnIndex++;*/
    }
}

void DialogXmlRelationManagementTable::OkButtonPressed()
{
    QDialog::accept();
}

void DialogXmlRelationManagementTable::CellDoubleClicked(int indexRow, int)
{
    //Call for modify the relation
    ModifyRelationPressed(indexRow);
}

void DialogXmlRelationManagementTable::ShowContextualMenuTable(QPoint point)
{
    //Create a menu and fill with the actions
    QMenu menu;
    QAction* modifyAct = new QAction(QIcon(":/icon/edit.png"), "Modify", this);
    QAction* deleteAct = new QAction(QIcon(":/icon/delete.png"), "Delete", this);
    menu.addAction(modifyAct);
    menu.addAction(deleteAct);

    //Disable the actions if none cell was selected
    QList<QTableWidgetItem*> selectedCellList = m_pTable->selectedItems();
    if(selectedCellList.size() < 1)
    {
        modifyAct->setDisabled(true);
        deleteAct->setDisabled(true);
    }

    //Show contextual menu
    QAction* itemSelected = menu.exec(m_pTable->mapToGlobal(point));

    //Execute what the user has requested
    if(itemSelected == modifyAct)
    {
        ModifyRelationPressed(selectedCellList.at(0)->row());
    }
    else if(itemSelected == deleteAct)
    {
        DeleteRelationPressed(selectedCellList.at(0)->row());
    }
}

void DialogXmlRelationManagementTable::Update(UPDATE_CODE updateCode)
{
    //If it was modified the relations refill the table in order to show the modifies
    if(updateCode == UPDATE_XML_RELATIONS)
    {
        ReFillTheTable();
    }
}

void DialogXmlRelationManagementTable::Show()
{
    QDialog::show();
}

void DialogXmlRelationManagementTable::Hide()
{
    QDialog::hide();
}
