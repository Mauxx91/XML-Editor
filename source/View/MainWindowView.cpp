/**
 * File: MainWindowView.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/16
*/

#include "MainWindowView.h"

#include "DialogEditAttributeNameTagCollapseBase.h"
#include "DialogEditAttributeNameTagCollapse.h"
#include "DialogAssociatedFilesForm.h"
#include "DialogAssociatedFilesBase.h"
#include "DialogXmlRelationManagementTable.h"
#include "DialogXmlRelationManagementBase.h"
#include "DialogInputFileNameBase.h"
#include "DialogInputFileNameForm.h"
#include "ViewXmlRelationErrorsBase.h"
#include "ViewXmlRelationErrorsTable.h"
#include "XmlViewBase.h"
#include "XmlViewTree.h"

#include "../version.h"
#include "../Data/AttributeNameTagCollapse.h"
#include "../Data/AssociatedFiles.h"
#include "../Data/XmlTreeModel.h"
#include "../Data/XmlRelationCollection.h"
#include "../Core/XmlRelationCheckerCoreBase.h"
#include "../Core/XmlRelationCheckerCoreImpl.h"
#include "../Command/XmlEditCommandInvoker.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QCoreApplication>
#include <QStatusBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QPushButton>

#include <QDragEnterEvent>
#include <QMimeData>

MainWindowView::MainWindowView(): QMainWindow(0)
{
    //Set the window title and version
    setWindowTitle(APPLICATION_NAME " v" VER_FILEVERSION_STR);

    //Set application
    QCoreApplication::setOrganizationName("Milestone S.r.l");
    QCoreApplication::setOrganizationDomain("milestone.it");
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    //Create actions and fill the main menu bar
    CreateAction();
    CreateApplicationMenuBar();
    createStatusBar();

    //Resize the window
    this->resize(1024, 768);
    //Set the icon
    this->setWindowIcon(QIcon(":/icon/iconaexe.png"));

    //Set accept drop events
    this->setAcceptDrops(true);

    //Initialize and load from the system registry the previously value
    m_pAttributeNameTagCollapse = new AttributeNameTagCollapse;

    //Initialize the command invoker
    m_pXmlEditorCommandInvoker = new XmlEditCommandInvoker;

    //Connect the command invoker
    connect(m_pXmlEditorCommandInvoker, SIGNAL(UndoEnabled(bool)), this, SLOT(SetEnableUndo(bool)));
    connect(m_pXmlEditorCommandInvoker, SIGNAL(RedoEnabled(bool)), this, SLOT(SetEnableRedo(bool)));

    //Set a zero the error view
    m_pXmlErrorWidget = 0;

    //Initialize the associated file names
    m_pAssociatedFiles = new AssociatedFiles;
    m_pAssociatedFiles->AttachObserver(this);

    //Initialize layouts
    m_pHorizMainLayout = new QHBoxLayout;
    m_pLeftVertLayout = new QVBoxLayout;
    m_pRightVertLayout = new QVBoxLayout;

    //Initialize combo boxes
    m_pLeftComboBox = new QComboBox;
    m_pRightComboBox = new QComboBox;

    //Connect the combo box with the right actions
    connect(m_pLeftComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ExchangeLeftXmlShown(int)));
    connect(m_pRightComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ExchangeRightXmlShown(int)));

    //Initialize and load the relation collection from the standard configuration file
    m_pXmlRelationCollection = new XmlRelationCollection;
    FILE_XML_ERR_CODE errCode = m_pXmlRelationCollection->LoadSettedFileConf();
    //If an error has occurred show the box with the error
    if(errCode != FILE_XML_NO_ERR)
    {
        ShowOpenSaveXmlError("Impossible to load the standard configuration file. When the first new relation will be inserted a new one configuration file will be created.\n\n", errCode, m_pXmlRelationCollection->GetXmlDocument());
    }

    //Initialize the core
    m_pXmlRelationCheckerCore = new XmlRelationCheckerCoreImpl(&m_openedXmlDocuments, m_pXmlRelationCollection);

    //Initialize the view
    m_pLeftXmlViewBase = new XmlViewTree(m_pXmlRelationCheckerCore, m_pXmlEditorCommandInvoker);
    m_pRightXmlViewBase = new XmlViewTree(m_pXmlRelationCheckerCore, m_pXmlEditorCommandInvoker);

    //connect the view
    connect(m_pLeftXmlViewBase, SIGNAL(ShowAndSelectedItemRequested(QString,QString)), this, SLOT(ShowAndSelectedItemOnLeftTree(QString,QString)));
    connect(m_pRightXmlViewBase, SIGNAL(ShowAndSelectedItemRequested(QString,QString)), this, SLOT(ShowAndSelectedItemOnRightTree(QString,QString)));

    //Combine the widget and the layouts
    m_pLeftVertLayout->addWidget(m_pLeftComboBox);
    m_pLeftVertLayout->addWidget(m_pLeftXmlViewBase);
    m_pLeftVertLayout->setAlignment(Qt::AlignTop);
    m_pRightVertLayout->addWidget(m_pRightComboBox);
    m_pRightVertLayout->addWidget(m_pRightXmlViewBase);
    m_pRightVertLayout->setAlignment(Qt::AlignTop);
    m_pHorizMainLayout->addItem(m_pLeftVertLayout);
    m_pHorizMainLayout->addItem(m_pRightVertLayout);

    //Set the main layout
    m_pCentralWidget = new QWidget;
    m_pCentralWidget->setLayout(m_pHorizMainLayout);
    setCentralWidget(m_pCentralWidget);

    //Initialize the file watcher
    m_pFileSystemWatcher = new QFileSystemWatcher;
    connect(m_pFileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(OpenedFileChanged(QString)));

    //Disable the save action
    DisableSaveActions();
}

MainWindowView::~MainWindowView()
{
    delete m_pAttributeNameTagCollapse;

    delete m_pXmlRelationCollection;

    if(m_pXmlErrorWidget)
    {
        delete m_pXmlErrorWidget;
    }

    delete m_pFileSystemWatcher;

    delete m_pXmlEditorCommandInvoker;

    delete m_pXmlRelationCheckerCore;
}

void MainWindowView::Update(UPDATE_CODE updateCode)
{
    if(updateCode == UPDATE_NEW_ASSOCIATED_FILE)
    {
        //File name of the added associated file
        const QString& fileNameAdded = m_pAssociatedFiles->GetLastModifiedFileName();

        //Open the file in the aditor as a non mai file
        OpenXmlFile(fileNameAdded, false);

    }
    else if(updateCode == UPDATE_REMOVED_ASSOCIATED_FILE)
    {
        const QString& fileNameRemoved = m_pAssociatedFiles->GetLastModifiedFileName();

        //Remove the file from the editor
        ClearSingleXml(fileNameRemoved);
    }
    else if(updateCode == UPDATE_XML_DOCUMENT)
    {
        //Set the start in both the combo box
        SetModifiedStarInComboBox();

        //Activate the save action
        EnableSaveActions();
    }
}

void MainWindowView::CreateAction()
{
    //Create Action for create a new XML
    m_pNewXmlAct = new QAction(QIcon(":/icon/new.png"), "&New XML", this);
    m_pNewXmlAct->setShortcut(QKeySequence::New);
    m_pNewXmlAct->setStatusTip("Create a new XML file. The current work will be closed");
    connect(m_pNewXmlAct, SIGNAL(triggered()), this, SLOT(NewMainXmlFile()));

    //Create Action for open a new XML
    m_pOpenXmlAct = new QAction(QIcon(":/icon/open.png"), "&Open XML", this);
    m_pOpenXmlAct->setShortcut(QKeySequence::Open);
    m_pOpenXmlAct->setStatusTip("Open a preexisting XML file. The current work will be closed");
    connect(m_pOpenXmlAct, SIGNAL(triggered()), this, SLOT(OpenXmlFile()));

    //Create Action for save the left XML
    m_pSaveLeftXmlAct = new QAction(QIcon(":/icon/save.png"), "Save &Left XML", this);
    m_pSaveLeftXmlAct->setShortcut(tr("CTRL+S"));
    m_pSaveLeftXmlAct->setStatusTip("Save the xml document current shown in the left tree");
    connect(m_pSaveLeftXmlAct, SIGNAL(triggered()), this, SLOT(SaveLeftXmlFile()));

    //Create Action for save the right XML
    m_pSaveRightXmlAct = new QAction(QIcon(":/icon/save.png"), "Save &Right XML", this);
    m_pSaveRightXmlAct->setShortcut(tr("CTRL+D"));
    m_pSaveRightXmlAct->setStatusTip("Save the xml document current shown in the right tree");
    connect(m_pSaveRightXmlAct, SIGNAL(triggered()), this, SLOT(SaveRightXmlFile()));

    //Create Action for save all XML
    m_pSaveAllXmlAct = new QAction(QIcon(":/icon/saveAll.png"), "Save All XML", this);
    m_pSaveAllXmlAct->setShortcut(tr("CTRL+SHIFT+S"));
    m_pSaveAllXmlAct->setStatusTip("Save the xml document current shown in the right tree");
    connect(m_pSaveAllXmlAct, SIGNAL(triggered()), this, SLOT(SaveAllXmlFile()));

    //Create Action for exit from the program
    m_pExitProgrammAct = new QAction(QIcon(":/icon/exit.png"), tr("&Exit"), this);
    m_pExitProgrammAct->setShortcut(tr("CTRL+Q"));
    m_pExitProgrammAct->setStatusTip(QString::fromUtf8("Quit the program"));
    connect(m_pExitProgrammAct, SIGNAL(triggered()), this, SLOT(close()));



    //Create Action for undo
    m_pUndoAct = new QAction(QIcon(":/icon/undo.png"), tr("&Undo"), this);
    m_pUndoAct->setShortcut(tr("CTRL+Z"));
    m_pUndoAct->setStatusTip(QString::fromUtf8("Undo the last action"));
    connect(m_pUndoAct, SIGNAL(triggered()), this, SLOT(Undo()));

    //Create Action for redo
    m_pRedoAct = new QAction(QIcon(":/icon/redo.png"), tr("&Redo"), this);
    m_pRedoAct->setShortcut(tr("CTRL+Y"));
    m_pRedoAct->setStatusTip(QString::fromUtf8("Redo the last undo action"));
    connect(m_pRedoAct, SIGNAL(triggered()), this, SLOT(Redo()));




    //Create Action for manage tag filter
    m_setAttributeNameTagCollapseAct = new QAction(QIcon(":/icon/collapse.png"), "Set Collapse &Tag Filter", this);
    m_setAttributeNameTagCollapseAct->setShortcut(tr("ALT+T"));
    m_setAttributeNameTagCollapseAct->setStatusTip("Set the attribute name to display when collapse same tags");
    connect(m_setAttributeNameTagCollapseAct, SIGNAL(triggered()), this, SLOT(EditAttributeNameTagCollapse()));

    //Create Action for manage associate XML files
    m_pManageAssociateXmlFilesAct = new QAction(QIcon(":/icon/associate.png"), "Manage &Associated XML", this);
    m_pManageAssociateXmlFilesAct->setShortcut(tr("ALT+M"));
    m_pManageAssociateXmlFilesAct->setStatusTip("Manage the associated xml files");
    connect(m_pManageAssociateXmlFilesAct, SIGNAL(triggered()), this, SLOT(EditAssociatedFileNames()));

    //Create Action for manage XML relations
    m_pManageXmlRelationAct = new QAction(QIcon(":/icon/relation.jpg"), "Manage XML &Relations", this);
    m_pManageXmlRelationAct->setShortcut(tr("ALT+R"));
    m_pManageXmlRelationAct->setStatusTip("Manage the associated xml files");
    connect(m_pManageXmlRelationAct, SIGNAL(triggered()), this, SLOT(ManageXmlRelations()));

    //Create Action for import XML relations
    m_pInportXmlRelationAct = new QAction(QIcon(":/icon/import.png"), "Import XML &Relations", this);
    m_pInportXmlRelationAct->setShortcut(tr("ALT+I"));
    m_pInportXmlRelationAct->setStatusTip("Manage the associated xml files");
    connect(m_pInportXmlRelationAct, SIGNAL(triggered()), this, SLOT(ImportXmlRelations()));

    //Create Action for export XML relations
    m_pExportXmlRelationAct = new QAction(QIcon(":/icon/export.png"), "Export XML &Relations", this);
    m_pExportXmlRelationAct->setShortcut(tr("ALT+E"));
    m_pExportXmlRelationAct->setStatusTip("Manage the associated xml files");
    connect(m_pExportXmlRelationAct, SIGNAL(triggered()), this, SLOT(ExportXmlRelations()));



    //Create Action for Check the xml relations
    m_pCheckXmlRelationAct = new QAction(QIcon(":/icon/check.png"), "&Check XML Relations", this);
    m_pCheckXmlRelationAct->setShortcut(tr("ALT+SHIFT+C"));
    m_pCheckXmlRelationAct->setStatusTip("Control the consistence of the relations in the main XML and in all associated files");
    connect(m_pCheckXmlRelationAct, SIGNAL(triggered()), this, SLOT(CheckXmlRelation()));



    //Create Action for show the about dialog
    m_pAboutAct = new QAction(QIcon(":/icon/iconaexe.png"), "&About " APPLICATION_NAME, this);
    m_pAboutAct->setShortcut(QKeySequence::WhatsThis);
    m_pAboutAct->setStatusTip(tr("Shows info about the application"));
    connect(m_pAboutAct, SIGNAL(triggered()), this, SLOT(About()));
}

void MainWindowView::CreateApplicationMenuBar()
{
    //Add a menu in the main menu bar and fill with the actions
    m_pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pFileMenu->addAction(m_pNewXmlAct);
    m_pFileMenu->addAction(m_pOpenXmlAct);
    m_pFileMenu->addAction(m_pSaveLeftXmlAct);
    m_pFileMenu->addAction(m_pSaveRightXmlAct);
    m_pFileMenu->addAction(m_pSaveAllXmlAct);
    m_pFileMenu->addAction(m_pExitProgrammAct);

    //Add a menu in the main menu bar and fill with the actions
    m_pEditMenu = menuBar()->addMenu(tr("E&dit"));
    m_pEditMenu->addAction(m_pUndoAct);
    m_pEditMenu->addAction(m_pRedoAct);

    //Add a menu in the main menu bar and fill with the actions
    m_pSettingsMenu = menuBar()->addMenu(tr("&Settings"));
    m_pSettingsMenu->addAction(m_setAttributeNameTagCollapseAct);
    m_pSettingsMenu->addAction(m_pManageAssociateXmlFilesAct);
    m_pSettingsMenu->addAction(m_pManageXmlRelationAct);
    m_pSettingsMenu->addAction(m_pInportXmlRelationAct);
    m_pSettingsMenu->addAction(m_pExportXmlRelationAct);

    //Add a menu in the main menu bar and fill with the actions
    m_pChecksMenu = menuBar()->addMenu(tr("&Checks"));
    m_pChecksMenu->addAction(m_pCheckXmlRelationAct);

    //Add a menu in the main menu bar and fill with the actions
    m_pAboutMenu = menuBar()->addMenu(tr("&About"));
    m_pAboutMenu->addAction(m_pAboutAct);
}

void MainWindowView::createStatusBar()
{
    //Show the status bar and show this message
    statusBar()->showMessage(tr("Ready"));
}

void MainWindowView::SetModifiedStarInComboBox()
{
    //For every document opened set if it's modifies with adding a star in the combo box
    for(int i=0; i < m_openedXmlDocuments.size(); ++i)
    {
        //If the examined document is modified
        if(m_openedXmlDocuments.at(i)->IsModified())
        {
            m_pLeftComboBox->setItemText(i, m_openedXmlDocuments.at(i)->GetAbsoluteFileName() + "*");
            m_pRightComboBox->setItemText(i, m_openedXmlDocuments.at(i)->GetAbsoluteFileName() + "*");
        }
        else
        {
            m_pLeftComboBox->setItemText(i, m_openedXmlDocuments.at(i)->GetAbsoluteFileName());
            m_pRightComboBox->setItemText(i, m_openedXmlDocuments.at(i)->GetAbsoluteFileName());
        }
    }
}

void MainWindowView::EnableSaveActions()
{
    m_pSaveLeftXmlAct->setEnabled(true);
    m_pSaveRightXmlAct->setEnabled(true);
    m_pSaveAllXmlAct->setEnabled(true);
}

void MainWindowView::DisableSaveActions()
{
    m_pSaveLeftXmlAct->setEnabled(false);
    m_pSaveRightXmlAct->setEnabled(false);
    m_pSaveAllXmlAct->setEnabled(false);
}

int MainWindowView::CountModifiedFile() const
{
    int numModifiedFile = 0;

    for(int i=0; i < m_openedXmlDocuments.size(); ++i)
    {
        if(m_openedXmlDocuments.at(i)->IsModified())
        {
            numModifiedFile++;
        }
    }

    return numModifiedFile;
}

void MainWindowView::ExchangeLeftXmlShown(int newIndex)
{
    //If newIndex is valid set the model otherWise set 0 to display a empty tree
    if(newIndex >= 0 && newIndex < m_openedXmlModels.size())
    {
        m_pLeftXmlViewBase->SetModel(m_openedXmlModels.at(newIndex));
    }
    else
    {
        m_pLeftXmlViewBase->SetModel(0);
    }
}

void MainWindowView::ExchangeRightXmlShown(int newIndex)
{
    //If newIndex is valid set the model otherWise set 0 to display a empty tree
    if(newIndex >= 0 && newIndex < m_openedXmlModels.size())
    {
        m_pRightXmlViewBase->SetModel(m_openedXmlModels.at(newIndex));
    }
    else
    {
        m_pRightXmlViewBase->SetModel(0);
    }
}

void MainWindowView::SaveLeftXmlFile()
{
    //The document to save
    XmlDocument* xmlDocument = m_openedXmlDocuments.at(m_pLeftComboBox->currentIndex());

    //Save the xmlDocument
    RealSaveXml(xmlDocument);
}

void MainWindowView::SaveRightXmlFile()
{
    //The document to save
    XmlDocument* xmlDocument = m_openedXmlDocuments.at(m_pRightComboBox->currentIndex());

    //Save the xmlDocument
    RealSaveXml(xmlDocument);
}

void MainWindowView::SaveAllXmlFile()
{
    //For each openedDocument lauch the save
    for(int i=0; i < m_openedXmlDocuments.size(); ++i)
    {
        RealSaveXml(m_openedXmlDocuments.at(i));
    }
}

void MainWindowView::Undo()
{
    m_pXmlEditorCommandInvoker->Undo();
}

void MainWindowView::Redo()
{
    m_pXmlEditorCommandInvoker->Redo();
}

void MainWindowView::SetEnableUndo(bool enable)
{
    m_pUndoAct->setEnabled(enable);
}

void MainWindowView::SetEnableRedo(bool enable)
{
    m_pRedoAct->setEnabled(enable);
}

void MainWindowView::RealSaveXml(XmlDocument *xmlDocument)
{
    //Save only if the file is modified
    if(xmlDocument->IsModified())
    {
        //To prevent that the program see the save as external modify
        m_pFileSystemWatcher->removePath(xmlDocument->GetAbsoluteFileName());

        //Save and get the error code
        FILE_XML_ERR_CODE errCode = xmlDocument->SaveXmlFile();

        if(errCode == FILE_XML_NO_ERR)
        {
            SetModifiedStarInComboBox();
        }
        else
        {
            //Initial string error
            QString error = "It was impossible save the file: \""+ xmlDocument->GetAbsoluteFileName() +"\".\n\n";

            //Show the error
            ShowOpenSaveXmlError(error, errCode, xmlDocument);
        }

        //Disable or enable the save action
        if(CountModifiedFile() > 0)
        {
            EnableSaveActions();
        }
        else
        {
            DisableSaveActions();
        }

        //To re enable the check of external modifies
        m_pFileSystemWatcher->addPath(xmlDocument->GetAbsoluteFileName());
    }
}

void MainWindowView::AddNewXml(XmlDocument* xmlDocument)
{
    //Append to the list of documents
    m_openedXmlDocuments.append(xmlDocument);

    //Create and append to the list a new xml model
    //QAbstractItemModel* model = ;
    m_openedXmlModels.append(new XmlTreeModel(m_pAttributeNameTagCollapse, xmlDocument));

    //Append to both the combo box
    m_pLeftComboBox->addItem(xmlDocument->GetAbsoluteFileName());
    m_pRightComboBox->addItem(xmlDocument->GetAbsoluteFileName());

    //Add the xml file in the watcher
    m_pFileSystemWatcher->addPath(xmlDocument->GetAbsoluteFileName());

    //Attach as obsever of the xml document
    xmlDocument->AttachObserver(this);
}

void MainWindowView::ClearAllXml()
{
    //Delete all the items of both combo boxes
    m_pLeftComboBox->clear();
    m_pRightComboBox->clear();

    //Delete all the opened files and before remove from the file watcher
    for(int i=0; i<m_openedXmlDocuments.size(); ++i)
    {
        m_pFileSystemWatcher->removePath(m_openedXmlDocuments.at(i)->GetAbsoluteFileName());
        delete m_openedXmlDocuments.at(i);
    }
    m_openedXmlDocuments.clear();

    //Remove all the models
    m_openedXmlModels.clear();

    //Remove all the commands from the invoker
    m_pXmlEditorCommandInvoker->Clear();

}

void MainWindowView::ClearSingleXml(const QString& removedFileName)
{
    //Search the index of the removed file
    int indexRemoved = -1;

    //remove from the file watcher
    m_pFileSystemWatcher->removePath(removedFileName);

    //The document of the file that it's being removed
    XmlDocument* xmlDocument;

    //search in the opened file because even if all the list evolves in the same way the combo can have the final *
    for(int i=1; i<m_openedXmlDocuments.size() && indexRemoved == -1; ++i)
    {
        const QString& fileNameCombo = m_openedXmlDocuments.at(i)->GetAbsoluteFileName();

        if(removedFileName.compare(fileNameCombo, Qt::CaseInsensitive) == 0)// zero means it's the same
        {
            indexRemoved = i;
            xmlDocument = m_openedXmlDocuments.at(i);
        }
    }

    //Remove only if the file is present in the editor
    if(indexRemoved != -1)
    {
        //Remove the the command about this xmlDocument
        m_pXmlEditorCommandInvoker->ClearForSingleXmlDocument(xmlDocument);

        //Delete items of this file in both the combo box
        m_pLeftComboBox->removeItem(indexRemoved);
        m_pRightComboBox->removeItem(indexRemoved);

        //Delete the file from the opened list
        delete m_openedXmlDocuments.at(indexRemoved);
        m_openedXmlDocuments.removeAt(indexRemoved);

        //Remove the model of the file
        m_openedXmlModels.removeAt(indexRemoved);
    }
}

void MainWindowView::NewMainXmlFile()
{
    //Call the dialog for chose the file name and the path for the new xml
    QString& candidateFileName = QFileDialog::getSaveFileName(this, tr("Chose name and path for the new xml"),
                                                             "." ,tr("XML files (*.xml);; Text files (*.txt);; All files (*)"));

    //If the selection is valid
    if( !candidateFileName.isEmpty() )
    {
        //Add .xml extension if no extension is given by the user
        int lastIndexOfDot = candidateFileName.lastIndexOf('.');
        if(lastIndexOfDot == -1) //If has no extension
            candidateFileName.append(".xml");

        //Create a new XmlDocument
        XmlDocument* xmlDocument = new XmlDocument();
        xmlDocument->NewXmlFile(candidateFileName);

        //Clear all and add in the editor the new file
        ClearAllXml();

        //Add the correct open xml file in the editor
        AddNewXml(xmlDocument);

        //Change main file in order to give loaded all the previosly inserted associated files
        m_pAssociatedFiles->ChangeMainFile(candidateFileName);
    }
}

void MainWindowView::OpenXmlFile(const QString& candidateFileNameNoDialog, bool isMainFile)
{
    QString candidateFileName = "";

    //If candidate file name in input is == "" it's needed to open a dialog for choose the file, otherwise there's no need to show it
    if( candidateFileNameNoDialog == "" )
    {
        candidateFileName = QFileDialog::getOpenFileName(this, tr("Chose a file to open"),
                                                                 "." ,tr("XML files (*.xml);; Text files (*.txt);; All files (*)"));
    }
    else //if the string it's already given
    {
        candidateFileName = candidateFileNameNoDialog;
    }

    //If the selection is valid
    if( !candidateFileName.isEmpty() )
    {
        //Open an xml document
        XmlDocument* xmlDocument = new XmlDocument();
        FILE_XML_ERR_CODE errCode = xmlDocument->OpenXmlFile(candidateFileName);

        if(errCode == FILE_XML_NO_ERR)
        {
            if(isMainFile)
            {
                ClearAllXml(); //Clear all and add in the editor the new file
            }

            //Add the correct open xml file in the editor
            AddNewXml(xmlDocument);

            //Change main file in order to give loaded all the previosly inserted associated files
            if(isMainFile)
            {
                m_pAssociatedFiles->ChangeMainFile(candidateFileName);
            }
        }
        else
        {
            //Initial string error
            QString error = "It was impossible open the file: \""+ candidateFileName +"\".\n\n";

            //Show the error
            ShowOpenSaveXmlError(error, errCode, xmlDocument);

            delete xmlDocument;
        }
    }

}

void MainWindowView::ShowOpenSaveXmlError(QString baseError, FILE_XML_ERR_CODE errCode, const XmlDocument* xmlDocument = 0) const
{
    if(errCode == FILE_XML_FILE_NOT_EXIST)
    {
        baseError += "The file doesn't exist.";
    }
    else if(errCode == FILE_XML_FILE_IS_NOT_FILE)
    {
        baseError += "The path inserted doesn't corripond to a file.";
    }
    else if(errCode == FILE_XML_FILE_IS_NOT_READABLE)
    {
        baseError += "The file is't readable.";
    }
    else if(errCode == FILE_XML_FILE_IS_NOT_WRITABLE)
    {
        baseError += "The file is't writable.";
    }
    else if(errCode == FILE_XML_UNABLE_OPEN_FILE)
    {
        baseError += "Error during the file's opening.";
    }
    else if(errCode == FILE_XML_UNABlE_FILL_DOM_TREE)
    {
        //If xml Document is given
        if(xmlDocument)
        {
            //Obtain the error occurred during the xml parsing
            QString errorString = xmlDocument->GetErrorString();
            errorString.replace(0, 1, errorString.at(0).toUpper());

            baseError += errorString + " at line: " + QString::number(xmlDocument->GetErrorLine()) + ".";
        }
        else
        {
            baseError += "Error during the parsing of the XML.";
        }
    }

    QMessageBox::critical(0, "Critical error", baseError, QMessageBox::Ok);
}

void MainWindowView::EditAttributeNameTagCollapse()
{
    DialogEditAttributeNameTagCollapseBase* dialog = new DialogEditAttributeNameTagCollapse(m_pAttributeNameTagCollapse->GetAttributeNameTag(), this);

    //Show the dialog and store the result
    int result = dialog->exec();

    if(result==QDialog::Accepted) //If the user has pressed the apply button
    {
        //Update with the new value
        m_pAttributeNameTagCollapse->SetAttributeNameTag(dialog->GetAttributeNameTag());
    }

    delete dialog;
}

void MainWindowView::EditAssociatedFileNames()
{
    DialogAssociatedFilesBase* dialog = new DialogAssociatedFilesForm(m_pAssociatedFiles, this);

    //Show the dialog
    dialog->exec();

    delete dialog;
}

void MainWindowView::ManageXmlRelations()
{
    DialogXmlRelationManagementTable dialog(m_pXmlRelationCollection, this);

    //Show the dialog
    dialog.exec();
}

void MainWindowView::ImportXmlRelations()
{
    DialogInputFileNameBase* dialog = new DialogInputFileNameForm("Import XML Relations", "Chose a file name from which import the relations (.xec)", "Import",
                                                                  ".xec", "XML Editor Config files (*.xec)", false, this);

    //Show the dialog and store the result
    int result = dialog->exec();

    //If the result is positive import the relations from an external file conf
    if(result == QDialog::Accepted)
    {
         FILE_XML_ERR_CODE errCode = m_pXmlRelationCollection->LoadNonStandardFileConf(dialog->GetAbsoluteFileName());

         //Initial string error
         QString error = "It was impossible import the relations.\n\n";

         //If the operation is done without errors
         if(errCode == FILE_XML_NO_ERR)
         {
             //Show that the operation is complete
             QMessageBox::information(this, "Success", "Relations imported with successful.");
         }
         else//Show the error
         {
            ShowOpenSaveXmlError(error, errCode, 0);
         }
    }

    delete dialog;
}

void MainWindowView::ExportXmlRelations()
{
    DialogInputFileNameBase* dialog = new DialogInputFileNameForm("Export XML Relations", "Chose a file name where export the relations (.xec)", "Export",
                                                                  ".xec", "XML Editor Config files (*.xec)", true, this);

    //Show the dialog and store the result
    int result = dialog->exec();

    //If the result is positive export the relations in an external file conf
    if(result == QDialog::Accepted)
    {
         FILE_XML_ERR_CODE errCode = m_pXmlRelationCollection->SaveToAnotherFileConf(dialog->GetAbsoluteFileName());

         //Initial string error
         QString error = "It was impossible export the relations.\n\n";

         //If the operation is done without errors
         if(errCode == FILE_XML_NO_ERR)
         {
             //Show that the operation is complete
             QMessageBox::information(this, "Success", "Relations exported with successful.");
         }
         else//Show the error
         {
            ShowOpenSaveXmlError(error, errCode, 0);
         }
    }

    delete dialog;
}

void MainWindowView::CheckXmlRelation()
{
    QList<XmlRelationError*>* xmlRelationErrorList = m_pXmlRelationCheckerCore->CheckAllRelationInAllXmlDocuments();

    if(!m_pXmlErrorWidget)
    {
        m_pXmlErrorWidget = new ViewXmlRelationErrorsTable(xmlRelationErrorList);

        //If the dynamic type is correct connect
        ViewXmlRelationErrorsTable* errorWidgetTable = dynamic_cast<ViewXmlRelationErrorsTable*>(m_pXmlErrorWidget);
        if(errorWidgetTable)
        {
            connect(errorWidgetTable, SIGNAL(ShowErrorNodeRequested(QString, QString)), this, SLOT(ShowAndSelectedItemOnAnyTree(QString,QString)));
        }
        m_pXmlErrorWidget->Show();
    }
    else
    {
        m_pXmlErrorWidget->SetNewXmlRelationErrorList(xmlRelationErrorList);
        m_pXmlErrorWidget->Show();
    }

    //delete xmlRelationErrorList; //It's needn't because it's deleted by the result table widget when it will be closed or destroyed
}

void MainWindowView::About()
{
    QMessageBox::about(this, tr("About"),
                       "Developed by Biancucci Maurizio for Milestone S.r.l\n\n"

                       "version: v" VER_FILEVERSION_STR);
}

void MainWindowView::OpenedFileChanged(const QString& absoluteFileName)
{
    //If no others function is ask for reload
    if(!m_isAskingReloadSet.contains(absoluteFileName))
    {
        //Add this function
        m_isAskingReloadSet.insert(absoluteFileName);

        //Search the index of the modified files
        int indexFileToReload = -1;

        //search in a combo box but it's the same for every list because it evolves in same way
        for(int i=0; i<m_openedXmlDocuments.size() && indexFileToReload == -1; ++i) //1 because at zero there's the main file
        {
            const QString& fileNameCombo = m_openedXmlDocuments.at(i)->GetAbsoluteFileName();

            if(absoluteFileName.compare(fileNameCombo, Qt::CaseInsensitive) == 0)// zero means it's the same
            {
                indexFileToReload = i;
            }
        }

        //Ask only if the file is real present in the editor
        if(indexFileToReload != -1)
        {
            //Before delete ask to the user if the is he real secure
            QMessageBox msgBox;
            msgBox.setText(absoluteFileName + "\n\n This file has been modified by another program.\nDo you want to reload it?");
            msgBox.setIcon(QMessageBox::Question);

            //Buttons
            QPushButton* yesBnt = new QPushButton("Yes");
            msgBox.addButton(yesBnt, QMessageBox::AcceptRole);
            connect(yesBnt, SIGNAL(clicked()), &msgBox, SLOT(accept()));
            QPushButton* cancelBnt = msgBox.addButton("No", QMessageBox::RejectRole);
            connect(cancelBnt, SIGNAL(clicked()), &msgBox, SLOT(reject()));

            //Set yes as defalut button
            msgBox.setDefaultButton(yesBnt);

            //Show the dialog and store the result
            int result = msgBox.exec();

            //If the user has confirmed
            if(result == QMessageBox::Accepted)
            {
                //Reload the modified file
                m_openedXmlDocuments.at(indexFileToReload)->OpenXmlFile(absoluteFileName);
            }
        }

        //Remove this function
        m_isAskingReloadSet.remove(absoluteFileName);
    }
}

void MainWindowView::closeEvent(QCloseEvent* event)
{
    //per fermare la chiusura utilizzare event->ignore();

    //Check if there's some file modified and ask to the user what he wanna do
    for(int i=0; i < m_openedXmlDocuments.size(); ++i)
    {
        //If it's modified
        if(m_openedXmlDocuments.at(i)->IsModified())
        {
            //Ask to the user
            QMessageBox msgBox;
            msgBox.setText(m_openedXmlDocuments.at(i)->GetAbsoluteFileName() + "\n\n This file has been modified. Do you want to save the modifies?");
            msgBox.setIcon(QMessageBox::Question);

            //Buttons
            QPushButton* saveBnt = new QPushButton("Save");
            msgBox.addButton(saveBnt, QMessageBox::AcceptRole);
            connect(saveBnt, SIGNAL(clicked()), &msgBox, SLOT(accept()));
            QPushButton* cancelBnt = msgBox.addButton("Discard", QMessageBox::RejectRole);
            connect(cancelBnt, SIGNAL(clicked()), &msgBox, SLOT(reject()));
            QPushButton* abortBnt = msgBox.addButton("Cancel", QMessageBox::RejectRole);
            connect(abortBnt, SIGNAL(clicked()), &msgBox, SLOT(reject()));

            //Set yes as defalut button
            msgBox.setDefaultButton(saveBnt);

            //Show the dialog and store the result
            msgBox.exec();

            //If the user has confirmed
            if(msgBox.clickedButton() == saveBnt)
            {
                //Save the file and continue
                m_openedXmlDocuments.at(i)->SaveXmlFile();
            }
            //Stop the closing of the programm
            else if(msgBox.clickedButton() == abortBnt)
            {
                event->ignore();
                return;
            }
        }
    }

    //Also close the error result window if it's
    if(m_pXmlErrorWidget)
    {
        delete m_pXmlErrorWidget;
        m_pXmlErrorWidget = 0;
    }

    ClearAllXml();
}

void MainWindowView::dragEnterEvent(QDragEnterEvent *event)
{
    //If it's a drag of a file or a link to a file
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void MainWindowView::dropEvent(QDropEvent *event)
{
    //Take the ulrs
    const QList<QUrl>& urls = event->mimeData()->urls();

    //Controlf if the url list is not empty
    if (urls.isEmpty())
    {
        return;
    }

    //If it's not a valid file in the local file system
    const QString& fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
    {
        return;
    }

    //All it's ok try to open the new file asking the action that the user wants

    QMessageBox msgBox;
    msgBox.setText("Do you want to open the file as main file or add as associate file?\n\n"
                   + fileName);
    msgBox.setIcon(QMessageBox::Question);

    //Buttons
    QPushButton* openAsMainFileBnt = msgBox.addButton(tr("Open as main file"), QMessageBox::ActionRole);
    QPushButton* openAsAssociateFileBnt = msgBox.addButton(tr("Open as associate file"), QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Abort);

    //Set yes as defalut button
    msgBox.setDefaultButton(openAsMainFileBnt);

    //Show the dialog
    msgBox.exec();

    if(msgBox.clickedButton() == openAsMainFileBnt)
    {
        //Open as main file
        OpenXmlFile(fileName, true);
    }
    else if(msgBox.clickedButton() == openAsAssociateFileBnt)
    {
        //Add to the associate files
        bool inserted = m_pAssociatedFiles->AddAssociatedFileName(fileName);

        //Open as associate file only it it was not present yet
        if(inserted)
        {
            OpenXmlFile(fileName, false);
        }
    }
}

void MainWindowView::ShowAndSelectedItem(const QString& identifierNumber, const QString& absoluteFileName, bool useOnlyOneTree, bool leftSide)
{
    XmlViewBase* xmlView = 0;

    //First check if the current left or right showed files is the correct one
    if((m_pLeftComboBox->currentText()).replace("*", "").compare(absoluteFileName) == 0 && !useOnlyOneTree)
    {
        xmlView = m_pLeftXmlViewBase;
    }
    else if((m_pRightComboBox->currentText()).replace("*", "").compare(absoluteFileName) == 0 && !useOnlyOneTree)
    {
        xmlView = m_pRightXmlViewBase;
    }

    //If no current view is showing the right file then search over all the opened file
    if(xmlView == 0)
    {
        for(int i=0; i < m_openedXmlDocuments.size(); ++i)
        {
            //If the it's the searched file
            if(m_openedXmlDocuments.at(i)->GetAbsoluteFileName().compare(absoluteFileName) == 0)
            {
                //If it's normal show in the left, or is it's selected the left side
                if( !useOnlyOneTree || (useOnlyOneTree && leftSide) )
                {
                    //Show the file in the left view
                    m_pLeftComboBox->setCurrentIndex(i);

                    //Set the xmlView where to show the item
                    xmlView = m_pLeftXmlViewBase;
                }
                //Show in the right side
                else
                {
                    //Show the file in the right view
                    m_pRightComboBox->setCurrentIndex(i);

                    //Set the xmlView where to show the item
                    xmlView = m_pRightXmlViewBase;
                }
            }
        }
    }

    //If the file was foud call the show
    if(xmlView)
    {
        //Bring the focus to this windows
        this->activateWindow();

        //Select the item
        xmlView->ShowAndSelectedItem(identifierNumber);
    }
    else //Say to the user that the file is not opened in the editor
    {
        QMessageBox::warning(this, "File not open", "It's impossible to show the error location"
                             " because the file it's not yet opened in the editor");
    }
}

void MainWindowView::ShowAndSelectedItemOnLeftTree(const QString& identifierNumber, const QString& absoluteFileName)
{
    ShowAndSelectedItem(identifierNumber, absoluteFileName, true, true);
}

void MainWindowView::ShowAndSelectedItemOnRightTree(const QString& identifierNumber, const QString& absoluteFileName)
{
    ShowAndSelectedItem(identifierNumber, absoluteFileName, true, false);
}

void MainWindowView::ShowAndSelectedItemOnAnyTree(const QString& identifierNumber, const QString& absoluteFileName)
{
    ShowAndSelectedItem(identifierNumber, absoluteFileName, false, false);
}
