/**
 * File: DialogEditNodeTable.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#include "DialogEditNodeTable.h"

#include "../Data/XmlDocument.h"
#include "../Command/XmlEditCommandAggregator.h"
#include "../Command/XmlEditCommandSetNodeValue.h"
#include "../Command/XmlEditCommandAddAttribute.h"
#include "../Command/XmlEditCommandEditAttribute.h"
#include "../Command/XmlEditCommandRemoveAttribute.h"
#include "../Command/XmlEditCommandInvoker.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxlayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QMenu>

DialogEditNodeTable::DialogEditNodeTable(XmlEditCommandInvoker* xmlEditCommandInvoker, XmlDocument* xmlDocument, const QString& identifierNumber,
                                         QWidget *parent):
    DialogEditNodeBase(xmlEditCommandInvoker, xmlDocument, identifierNumber, parent)
{
    //Set window title
    this->setWindowTitle("Edit Element");

    this->resize(400, 300);

    //Make the bold font a real bolf
    m_boldFont.setBold(true);

    //Initialize layout
    m_pFormLayout = new QFormLayout;
    m_pHorizontalLayout = new QHBoxLayout;
    m_Vertlayout = new QVBoxLayout;

    //Initialize value widgets
    m_pValueLabel = new QLabel("Element textual value:");
    m_pValueLine = new QLineEdit;
    //Add to the form layout
    m_pFormLayout->addRow(m_pValueLabel, m_pValueLine);

    m_pAttributeTitle = new QLabel("Attributes:");

    //attribute table
    m_pTable = new QTableWidget(0, 2);

    //Set headers
    QStringList headers;
    headers << "Attribute Name" << "Value";
    m_pTable->setHorizontalHeaderLabels(headers);

    //Set the header bold
    m_pTable->verticalHeader()->setFont(m_boldFont);
    m_pTable->horizontalHeader()->setFont(m_boldFont);

    //Set columns width
    m_pTable->setColumnWidth(0, 120);// "Attribute Name"
    m_pTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //description

    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    //Set the custom contextual menu for the table
    m_pTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextual(QPoint)));

    //Initialize buttons
    m_pAddAttribute = new QPushButton(QIcon(":/icon/add.png"), "Add new attribute");
    m_pApplyBnt = new QPushButton(QIcon(":/icon/save.png"), "Apply");
    m_pCancelBnt = new QPushButton("Cancel");

    //Set the button layput
    m_pHorizontalLayout->addWidget(m_pAddAttribute, 1, Qt::AlignRight);
    m_pHorizontalLayout->addWidget(m_pApplyBnt, 0, Qt::AlignRight);
    m_pHorizontalLayout->addWidget(m_pCancelBnt, 0, Qt::AlignRight);

    //Connect the buttons
    connect(m_pAddAttribute,SIGNAL(clicked()),this,SLOT(AddAttributePressed()));
    connect(m_pApplyBnt,SIGNAL(clicked()),this,SLOT(ApplyButtonPressed()));
    connect(m_pCancelBnt,SIGNAL(clicked()),this,SLOT(reject()));

    //Setup the vertical layout
    m_Vertlayout->addLayout(m_pFormLayout);
    m_Vertlayout->addWidget(m_pAttributeTitle);
    m_Vertlayout->addWidget(m_pTable);
    m_Vertlayout->addItem(new QSpacerItem(20,20));
    m_Vertlayout->addLayout(m_pHorizontalLayout);

    //Set the main layout
    setLayout(m_Vertlayout);

    //Initialize the node
    m_node = m_pXmlDocument->GetDomNode(m_identifierNumber).toElement();

    LoadContent();
}



void DialogEditNodeTable::LoadContent()
{
    //Load the element value
    m_pValueLine->setText(m_node.toElement().text());

    //Get all the attributes
    QDomNamedNodeMap attributes = m_node.attributes();

    m_pTable->setRowCount(attributes.size());

    QTableWidgetItem* item = 0;

    //Add each attribute in to the table
    for(int indexRow=0; indexRow < attributes.size(); ++indexRow)
    {
        int columnIndex = 0;

        QDomAttr attributeNode = attributes.item(indexRow).toAttr();

        item = new QTableWidgetItem(attributeNode.nodeName());// << attribute name
        item->setToolTip(attributeNode.nodeName());
        m_pTable->setItem(indexRow, columnIndex, item);
        columnIndex++;

        item = new QTableWidgetItem(attributeNode.nodeValue());// << value
        item->setToolTip(attributeNode.nodeValue());
        m_pTable->setItem(indexRow, columnIndex, item);
        columnIndex++;
    }

}

void DialogEditNodeTable::ShowContextual(QPoint point)
{
    //Create a menu and fill with the actions
    QMenu menu;
    QAction* deleteAct = new QAction(QIcon(":/icon/delete.png"), "Delete", this);
    menu.addAction(deleteAct);

    //Disable the actions if none cell was selected
    QList<QTableWidgetItem*> selectedCellList = m_pTable->selectedItems();
    if(selectedCellList.size() < 1)
    {
        deleteAct->setDisabled(true);
    }

    //Show contextual menu
    QAction* itemSelected = menu.exec(m_pTable->mapToGlobal(point));

    //Execute what the user has requested
    if(itemSelected == deleteAct)
    {
        RemoveAttributePressed(selectedCellList.at(0)->row());
    }
}

void DialogEditNodeTable::ApplyButtonPressed()
{
    XmlEditCommandAggregator* commandAggregator = new XmlEditCommandAggregator(m_pXmlDocument);

    //Get all the attributes
    QDomNamedNodeMap attributes = m_node.attributes();

    //First remove the attributes the isn't present now
    for(int i=attributes.size()-1; i >= 0; i--)
    {
        bool attributePresent = false;
        QString attributeName = attributes.item(i).nodeName();

        //For each present attribute in the table
        for(int k=0; k < m_pTable->rowCount() && !attributePresent; ++k)
        {
            QString tableAttributeName = m_pTable->item(k, 0)->text();

            if(tableAttributeName != "" && attributeName == tableAttributeName)
            {
                attributePresent = true;
            }
        }

        //If the attribute is no longer present
        if(!attributePresent)
        {
            //Remove from the list
            attributes.removeNamedItem(attributeName);
            //Add an action to remove it
            commandAggregator->AddCommand(new XmlEditCommandRemoveAttribute(m_pXmlDocument, m_identifierNumber, attributeName));
        }
    }

    //Now the attributesList contains all the attribute that is present in the node
    //In the table it can be present more attribute (newly added ones)

    //Add the attributes that isn't present yet (new attribute) and modify the attributes already present
    for(int k=0; k < m_pTable->rowCount(); ++k)
    {
        bool attributeAlreadyPresent = false;
        QString tableAttributeName = m_pTable->item(k, 0)->text();
        QString tableAttributeValue = m_pTable->item(k, 1)->text();

        QString previousAttributeValue;

        //Not add the empty attribute name
        if(tableAttributeName != "")
        {
            //For each attribute already present
            for(int i=0;i < attributes.size() && !attributeAlreadyPresent; ++i)
            {
                if(attributes.item(i).nodeName() == tableAttributeName)
                {
                    attributeAlreadyPresent = true;
                    previousAttributeValue = attributes.item(i).nodeValue();
                }
            }

            //If the attribute is not present yet
            if(!attributeAlreadyPresent)
            {
                //Add an action to add it
                commandAggregator->AddCommand(new XmlEditCommandAddAttribute(m_pXmlDocument, m_identifierNumber, tableAttributeName,
                                                                             tableAttributeValue));
            }
            else //The attribute is yet present so maybe its value is changed
            {
                //If the value of the already present attribute is changed
                if(previousAttributeValue != tableAttributeValue)
                {
                    //Add an action to change its value
                    commandAggregator->AddCommand(new XmlEditCommandEditAttribute(m_pXmlDocument, m_identifierNumber, tableAttributeName,
                                                                                 tableAttributeValue));
                }
            }
        }
    }

    const QString& currentValue = m_pValueLine->text();

    //If the value is changed set the new
    if(m_node.toElement().text() != currentValue)
    {
        commandAggregator->AddCommand(new XmlEditCommandSetNodeValue(m_pXmlDocument, m_identifierNumber, currentValue));
    }

    if(commandAggregator->Size() > 0)
    {
        m_pXmlEditCommandInvoker->ExecuteACommand( commandAggregator );
    }
    QDialog::accept();
}

void DialogEditNodeTable::AddAttributePressed()
{
    m_pTable->insertRow(m_pTable->rowCount());
    m_pTable->setItem(m_pTable->rowCount()-1, 0, new QTableWidgetItem(""));
    m_pTable->setItem(m_pTable->rowCount()-1, 1, new QTableWidgetItem(""));
}

void DialogEditNodeTable::RemoveAttributePressed(int index)
{
    m_pTable->removeRow(index);
}

void DialogEditNodeTable::Show()
{
    QDialog::show();
}
void DialogEditNodeTable::Hide()
{
    QDialog::hide();
}

