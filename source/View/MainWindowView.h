/**
 * File: MainWindowView.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/16
*/

#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include "../Observer/IObserver.h"
#include "../Data/XmlDocument.h"

#include <QMainWindow>
#include <QStringList>
#include <QSet>

class QAction;
class QMenu;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QFileSystemWatcher;

class AttributeNameTagCollapse;
class AssociatedFiles;
class XmlViewBase;
class XmlTreeModel;
class XmlRelationCollection;
class ViewXmlRelationErrorsBase;
class XmlEditCommandInvoker;
class XmlRelationCheckerCoreBase;

enum XML_RELATION_ERROR_SERIOUSNESS_LEVEL;

/**
 * @brief The MainWindowView class that initialize all the needed application component
 */
class MainWindowView : public QMainWindow, public IObserver
{
    Q_OBJECT

private:

    //Actions
    QAction* m_pNewXmlAct;
    QAction* m_pOpenXmlAct;
    QAction* m_pSaveLeftXmlAct;
    QAction* m_pSaveRightXmlAct;
    QAction* m_pSaveAllXmlAct;
    QAction* m_pExitProgrammAct;
    QAction* m_pUndoAct;
    QAction* m_pRedoAct;
    QAction* m_setAttributeNameTagCollapseAct;
    QAction* m_pManageAssociateXmlFilesAct;
    QAction* m_pManageXmlRelationAct;
    QAction* m_pInportXmlRelationAct;
    QAction* m_pExportXmlRelationAct;
    QAction* m_pCheckXmlRelationAct;
    QAction* m_pAboutAct;

    //Menu
    QMenu* m_pFileMenu;
    QMenu* m_pEditMenu;
    QMenu* m_pSettingsMenu;
    QMenu* m_pChecksMenu;
    QMenu* m_pAboutMenu;

    //Layouts
    QHBoxLayout* m_pHorizMainLayout; //Main layout
    QVBoxLayout* m_pLeftVertLayout; //Left vertical layout
    QVBoxLayout* m_pRightVertLayout; //Right vertical layout

    //Combo boxes
    QComboBox* m_pLeftComboBox; //left combo box
    QComboBox* m_pRightComboBox; //right combo box

    //Central widget auxiliare in order to set up a layout as central widget
    QWidget* m_pCentralWidget;

    /**
     * @brief m_pXmlErrorWidget: view used to show xml errors found after a check
     */
    ViewXmlRelationErrorsBase* m_pXmlErrorWidget;

    /**
     * @brief m_pXmlRelationCheckerCore: class used for chack and use the relations
     */
    XmlRelationCheckerCoreBase* m_pXmlRelationCheckerCore;

    /**
     * @brief m_attributeNameTagCollapse: store the filter for waht to show when collapse tag whit same name in the same level
     */
    AttributeNameTagCollapse* m_pAttributeNameTagCollapse;

    /**
     * @brief m_pAssociatedFiles: store the name of the associated file names
     */
    AssociatedFiles* m_pAssociatedFiles;

    /**
     * @brief m_pXmlRelationCollection: store the known relations
     */
    XmlRelationCollection* m_pXmlRelationCollection;

    /**
     * @brief m_openedXmlDocuments: list of all the xml document opened. The fist is always the main file
     */
    QList<XmlDocument*> m_openedXmlDocuments;

    /**
     * @brief m_openedXmlModels: list of all the model of the xml document opened.
     */
    QList<XmlTreeModel*> m_openedXmlModels;

    //Two XmlView for each column
    XmlViewBase* m_pLeftXmlViewBase;
    XmlViewBase* m_pRightXmlViewBase;

    /**
     * @brief m_pFileSystemWatcher: used for monitor the modifies of the opened file
     */
    QFileSystemWatcher* m_pFileSystemWatcher;

    /**
     * @brief m_isAskingReloadSet: a set of the file for which id current asking the reload question
     *                             it's used to prevent to open multiple time the question for the same file
     */
    QSet<QString> m_isAskingReloadSet;

    /**
     * @brief m_pXmlEditorCommandInvoker: invoker for
     */
    XmlEditCommandInvoker* m_pXmlEditorCommandInvoker;

public:
    /**
     * @brief MainWindowView constructor. Istantiate and initialize all object needed.
     */
    MainWindowView();

    /**
     * @brief MainWindowView destructor. Delete all except Qt stuff that is eliminated by the parental destructor.
     */
    ~MainWindowView();

    /**
     * @brief Update the view after a modify of the observed subjects
     * @param updateCode: what was updated
     */
    void Update(UPDATE_CODE updateCode);

private:
    /**
     * @brief createAction: create the actions that will be used to create the main menu.
     *                      Every action is connected with the corrisponding slot.
     */
    void CreateAction();

    /**
     * @brief CreateApplicationMenuBar: create the application main menu bar.
     */
    void CreateApplicationMenuBar();

    /**
     * @brief createStatusBar: initialize the status bar in order to be shown
     */
    void createStatusBar();

    void EnableSaveActions();

    void DisableSaveActions();

    int CountModifiedFile() const;

    /**
     * @brief AddNewXml: add an xmlDocument in the editor
     * @param xmlDocument: the new xmlDocument loaded
     */
    void AddNewXml(XmlDocument* xmlDocument);

    /**
     * @brief ClearAllXml: delete all the document and model loaded
     */
    void ClearAllXml();

    /**
     * @brief ClearSingleXml: clear the xml opened file at the index
     * @param index of the file to close
     */
    void ClearSingleXml(const QString &removedFileName);

    void SetModifiedStarInComboBox();

private slots:

    /**
     * @brief ExchangeLeftXmlShown: change the xml model diplayed in the left column
     * @param newIndex: new index of the left combo box that indicates which model set in the left xml view
     */
    void ExchangeLeftXmlShown(int newIndex);

    /**
     * @brief ExchangeRightXmlShown: change the xml model diplayed in the right column
     * @param newIndex: new index of the right combo box that indicates which model set in the right xml view
     */
    void ExchangeRightXmlShown(int newIndex);

    void SaveLeftXmlFile();

    void SaveRightXmlFile();

    void SaveAllXmlFile();

    void Undo();

    void Redo();

    void SetEnableUndo(bool enable);

    void SetEnableRedo(bool enable);

private:

    void RealSaveXml(XmlDocument* xmlDocument);

public slots:

    /**
     * @brief NewXmlFile: open a new empty XML file in the editor as main file
     */
    void NewMainXmlFile();

    /**
     * @brief OpenMainXmlFile open a preexisting xml file as main file
     */
    void OpenXmlFile(const QString &candidateFileNameNoDialog = "", bool isMainFile = true);

private:
    void ShowOpenSaveXmlError(QString baseError, FILE_XML_ERR_CODE errCode, const XmlDocument *xmlDocument) const;

private slots:
    /**
     * @brief EditAttributeNameTagCollapse: show the dialog and modify the name tag attribute
     */
    void EditAttributeNameTagCollapse();

    /**
     * @brief EditAssociatedFileNames: show the dialog for modify the associated file names
     */
    void EditAssociatedFileNames();

    /**
     * @brief ManageXmlRelations: show the dialog for manage the xml relations
     */
    void ManageXmlRelations();

    /**
     * @brief ImportXmlRelations: show the dialog for inport xml relations
     */
    void ImportXmlRelations();

    /**
     * @brief ExportXmlRelations: show the dialog for export xml relations
     */
    void ExportXmlRelations();

    /**
     * @brief CheckXmlRelation: launch the check of all xml relations in every opened xml
     */
    void CheckXmlRelation();

    /**
     * @brief about: shows the about dialog
     */
    void About();

    /**
     * @brief OpenedFileChanged: ask for reload a file that has been changed outside this program
     * @param absoluteFileName: file that is changed
     */
    void OpenedFileChanged(const QString& absoluteFileName);

protected:
    /**
     * @brief closeEvent: event triggered when the user close the windows and quit the program
     */
    void closeEvent(QCloseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    /**
     * @brief ShowAndSelectedItem: show and select the given item
     * @param identifierNumber: the identifier number if the item
     * @param useOnlyOneTree: show only in one side
     * @param leftSide: show in left side, false shoe in right side
     */
    void ShowAndSelectedItem(const QString& identifierNumber, const QString& absoluteFileName, bool useOnlyOneTree, bool leftSide);

private slots:
    void ShowAndSelectedItemOnLeftTree(const QString& identifierNumber, const QString& absoluteFileName);

    void ShowAndSelectedItemOnRightTree(const QString& identifierNumber, const QString& absoluteFileName);

    void ShowAndSelectedItemOnAnyTree(const QString& identifierNumber, const QString& absoluteFileName);
};

#endif // MAINWINDOWVIEW_H
