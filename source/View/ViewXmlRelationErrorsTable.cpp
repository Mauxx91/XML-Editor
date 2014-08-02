/**
 * File: ViewXmlRelationErrorsTable.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/24
*/

#include "ViewXmlRelationErrorsTable.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <QMenu>

ViewXmlRelationErrorsTable::ViewXmlRelationErrorsTable(QList<XmlRelationError *>* xmlRelationErrorList):
    ViewXmlRelationErrorsBase(xmlRelationErrorList)
{
    //Set window title
    this->setWindowTitle("Check XML Relations results");

    //Resize
    this->resize(1280, 600);

    m_boldFont.setBold(true);

    //Label
    m_pSummaryLabel = new QLabel;

    //Initialize icons
    m_pErrorIcon = new QIcon(":/icon/error.png");
    m_pWarningIcon= new QIcon(":/icon/warning.png");
    m_pInfoIcon = new QIcon(":/icon/info.png");

    //Initialize uttons
    m_pErrorBnt = new QPushButton(*m_pErrorIcon, "");
    m_pWarningBnt = new QPushButton(*m_pWarningIcon, "");
    m_pInfoBnt = new QPushButton(*m_pInfoIcon, "");

    //Set the behaviour of the buttons
    m_pErrorBnt->setCheckable(true);
    m_pWarningBnt->setCheckable(true);
    m_pInfoBnt->setCheckable(true);

    //Initialize the state of the buttons
    m_pErrorBnt->setChecked(true);
    m_pWarningBnt->setChecked(true);
    m_pInfoBnt->setChecked(true);

    //Connect the buttons
    connect(m_pErrorBnt, SIGNAL(toggled(bool)), this, SLOT(ButtonToggled()));
    connect(m_pWarningBnt, SIGNAL(toggled(bool)), this, SLOT(ButtonToggled()));
    connect(m_pInfoBnt, SIGNAL(toggled(bool)), this, SLOT(ButtonToggled()));

    //Set the horizontal layout
    m_pTitleHorizLayout = new QHBoxLayout;
    m_pTitleHorizLayout->addWidget(m_pErrorBnt, 0);
    m_pTitleHorizLayout->addWidget(m_pWarningBnt, 0);
    m_pTitleHorizLayout->addWidget(m_pInfoBnt, 0);
    m_pTitleHorizLayout->addWidget(m_pSummaryLabel, 1);

    //Initialize the table widget to display the results
    m_pTable = new QTableWidget(0, 5);
    m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Set the header bold
    m_pTable->verticalHeader()->setFont(m_boldFont);
    m_pTable->horizontalHeader()->setFont(m_boldFont);

    //Connect the table
    connect(m_pTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(CellDoubleClicked(int, int)));

    //Set the custom contextual menu for the table
    m_pTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextualMenuTable(QPoint)));


    //Set headers
    QStringList headers;
    headers << "Ser." << "Type" << "Description" << "File name" << "Hided relation Error index";
    m_pTable->setHorizontalHeaderLabels(headers);

    //Set columns width
    m_pTable->setColumnWidth(0, 25);//Seriousness
    m_pTable->setColumnWidth(1, 190);//Type
    m_pTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); //description
    m_pTable->setColumnWidth(3, 160);//File name
    m_pTable->hideColumn(4);

    //Set layout and show the table
    m_pVertlayout = new QVBoxLayout;
    m_pVertlayout->addLayout(m_pTitleHorizLayout);
    m_pVertlayout->addWidget(m_pTable);
    setLayout(m_pVertlayout);

    ReLoadWidget(true);
}

void ViewXmlRelationErrorsTable::SetNewXmlRelationErrorList(QList<XmlRelationError*>* xmlRelationErrorList)
{
    if(m_pXmlRelationErrorList)
    {
        delete m_pXmlRelationErrorList;
    }

    m_pXmlRelationErrorList = xmlRelationErrorList;

    //Re set the original button state
    m_pErrorBnt->setChecked(true);
    m_pWarningBnt->setChecked(true);
    m_pInfoBnt->setChecked(true);

    ReLoadWidget(true);
}

void ViewXmlRelationErrorsTable::UpdateSummaryLabel()
{
    m_pSummaryLabel->setText(QString::number(m_pXmlRelationErrorList->size()) + " errors found. ");
}

void ViewXmlRelationErrorsTable::UpdateButtons(int errors, int warnings, int infos)
{
    //Update the button value and disable or enable

    m_pErrorBnt->setText(QString::number(errors) + " Erros");
    if(errors > 0)
    {
        m_pErrorBnt->setEnabled(true);
    }
    else
    {
        m_pErrorBnt->setEnabled(false);
    }

    m_pWarningBnt->setText(QString::number(warnings) + " Warnings");
    if(warnings > 0)
    {
        m_pWarningBnt->setEnabled(true);
    }
    else
    {
        m_pWarningBnt->setEnabled(false);
    }

    m_pInfoBnt->setText(QString::number(infos) + " Infos");
    if(infos > 0)
    {
        m_pInfoBnt->setEnabled(true);
    }
    else
    {
        m_pInfoBnt->setEnabled(false);
    }
}

void ViewXmlRelationErrorsTable::ReLoadWidget(bool notFilterAndUpdateButtons)
{
    m_pTable->setRowCount(0);

    UpdateSummaryLabel();

    FillTableAndButtons(notFilterAndUpdateButtons);
}

void ViewXmlRelationErrorsTable::FillTableAndButtons(bool notFilterAndUpdateButtons)
{
    //variable used for fill the table
    QTableWidgetItem* item;
    QLabel* descriptionLabel;

    int errorNum = 0;
    int warningNum = 0;
    int infoNum = 0;

    for(int i=0; i < m_pXmlRelationErrorList->size(); ++i)
    {
        XmlRelationError* xmlRelationError = m_pXmlRelationErrorList->at(i);

        //If show or not the error
        bool show = true;

        //if it has to use the severity filter
        if(!notFilterAndUpdateButtons)
        {
            XML_RELATION_ERROR_SERIOUSNESS_LEVEL serLevel = xmlRelationError->GetSeriousnessLevel();

            if(serLevel == XML_RELATION_ERROR_SERIOUSNESS_LEVEL_WARNING)
            {
                int k =0;
                k++;
            }

            show = (m_pErrorBnt->isChecked() && serLevel == XML_RELATION_ERROR_SERIOUSNESS_LEVEL_ERROR) ||
                   (m_pWarningBnt->isChecked() && serLevel == XML_RELATION_ERROR_SERIOUSNESS_LEVEL_WARNING) ||
                   (m_pInfoBnt->isChecked() && serLevel == XML_RELATION_ERROR_SERIOUSNESS_LEVEL_INFO);
        }

        //Filter the errors with the seriousness
        if(show)
        {
            int indexRow = m_pTable->rowCount();
            //Create a new row
            m_pTable->insertRow(indexRow);

            int columnIndex = 0;

            //headers << "Ser." << "Type" << "Description" << "File name" << "Hided relation Error index";

            item = new QTableWidgetItem("");// << "Ser."
            switch(xmlRelationError->GetSeriousnessLevel())
            {
                case XML_RELATION_ERROR_SERIOUSNESS_LEVEL_ERROR:
                    item->setIcon(*m_pErrorIcon);
                    item->setText("Error");
                    errorNum++;
                    break;
                case XML_RELATION_ERROR_SERIOUSNESS_LEVEL_WARNING:
                    item->setIcon(*m_pWarningIcon);
                    item->setText("Warning");
                    warningNum++;
                    break;
                case XML_RELATION_ERROR_SERIOUSNESS_LEVEL_INFO:
                    item->setIcon(*m_pInfoIcon);
                    item->setText("Info");
                    infoNum++;
                    break;

            }
            m_pTable->setItem(indexRow, columnIndex, item);
            columnIndex++;

            item = new QTableWidgetItem("");// << "Type."
            switch(xmlRelationError->GetErrorType())
            {
                case XML_RELATION_ERROR_TYPE_NO_INSTANCE_FOUND:
                    //item->setIcon(QIcon(":/icon/error.png"));
                    item->setText("Relation never found");
                    break;
                case XML_RELATION_ERROR_TYPE_REFERENCED_NOT_FOUND:
                    //item->setIcon(QIcon(":/icon/error.png"));
                    item->setText("Referenced nonexistent element");
                    break;
                case XML_RELATION_ERROR_TYPE_REFERENCED_NEVER_REFERENCED:
                    //item->setIcon(QIcon(":/icon/error.png"));
                    item->setText("Element never referenced");
                    break;

            }
            m_pTable->setItem(indexRow, columnIndex, item);
            columnIndex++;

            //Use a Qlabel for get the support for rich text
            descriptionLabel = new QLabel(xmlRelationError->GetErrorDescription());
            m_pTable->setCellWidget(indexRow, columnIndex, descriptionLabel);
            columnIndex++;

            item = new QTableWidgetItem(AbsoluteFileNameToRelativeFileName(xmlRelationError->GetAbsoluteFileName()));// << "File name"
            item->setToolTip(xmlRelationError->GetAbsoluteFileName());
            m_pTable->setItem(indexRow, columnIndex, item);
            columnIndex++;

            item = new QTableWidgetItem(AbsoluteFileNameToRelativeFileName(QString::number(i)));// "Hided relation Error index"
            m_pTable->setItem(indexRow, columnIndex, item);
            columnIndex++;

        }
    }

    if(notFilterAndUpdateButtons)
    {
        UpdateButtons(errorNum, warningNum, infoNum);
    }
}

QString ViewXmlRelationErrorsTable::AbsoluteFileNameToRelativeFileName(const QString& absoluteFileName)
{
    QFileInfo fileInfo(absoluteFileName);

    return fileInfo.fileName();
}

void ViewXmlRelationErrorsTable::ButtonToggled()
{
    //Reload the content without touch the buttons
    ReLoadWidget(false);
}

void ViewXmlRelationErrorsTable::ShowContextualMenuTable(QPoint point)
{
    //Create a menu and fill with the actions
    QMenu menu;
    QAction* showElementAct = new QAction(QIcon(":/icon/show.png"), "Show item location", this);
    menu.addAction(showElementAct);

    //Disable the actions if none cell was selected
    QList<QTableWidgetItem*> selectedCellList = m_pTable->selectedItems();
    if(selectedCellList.size() < 1)
    {
        showElementAct->setDisabled(true);
    }

    //Show contextual menu
    QAction* itemSelected = menu.exec(m_pTable->mapToGlobal(point));

    //Execute what the user has requested
    if(itemSelected == showElementAct)
    {
        EmitShowErrorNodeRequested(selectedCellList.at(0)->row());
    }
}

void ViewXmlRelationErrorsTable::CellDoubleClicked(int indexRow, int)
{
    //Call emit the signal
    EmitShowErrorNodeRequested(indexRow);
}

void ViewXmlRelationErrorsTable::EmitShowErrorNodeRequested(int rowIndex)
{
    //Get the index of the error by the last cell of the selected row

    int indexOfRelationErrorList = m_pTable->item(rowIndex, m_pTable->columnCount()-1)->text().toInt();

    //Get the relation error of the selected row
    XmlRelationError* xmlRelationError = m_pXmlRelationErrorList->at(indexOfRelationErrorList);

    emit ShowErrorNodeRequested(xmlRelationError->GetIdentifierNumber(), xmlRelationError->GetAbsoluteFileName());
}

void ViewXmlRelationErrorsTable::Show()
{
    QWidget::show();
}
void ViewXmlRelationErrorsTable::Hide()
{
    QWidget::hide();
}
