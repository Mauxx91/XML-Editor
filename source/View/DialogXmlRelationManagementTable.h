/**
 * File: DialogXmlRelationManagementTable.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#ifndef DIALOGXMLRELATIONMANAGEMENTTABLE_H
#define DIALOGXMLRELATIONMANAGEMENTTABLE_H

#include "DialogXmlRelationManagementBase.h"

#include <QFont>

class QVBoxLayout;
class QHBoxLayout;
class QTableWidget;
class QPushButton;
class QLabel;

/**
 * @brief The DialogXmlRelationManagementTable class that implements the dialog for manage the xml relations with a table
 */
class DialogXmlRelationManagementTable : public DialogXmlRelationManagementBase
{
    Q_OBJECT

private:
    //Layouts
    QVBoxLayout* m_pVerticalMainLayout;
    QHBoxLayout* m_pHorizontalBntLayout;

    //Bold font
    QFont m_boldFont;

    //Title label
    QLabel* m_pTitleLabel;

    /**
     * @brief m_pTable: table for show the relations
     */
    QTableWidget* m_pTable;

    //Buttons
    QPushButton* m_pAddRelationBnt;
    QPushButton* m_pOkBnt;

public:
    /**
     * @brief DialogXmlRelationManagementTable constructor that initialize all
     * @param xmlRelationCollection: to display and modify
     * @param parent widget of the dialog
     */
    DialogXmlRelationManagementTable(XmlRelationCollection* xmlRelationCollection, QWidget *parent = 0);

    /**
     * @brief DialogXmlRelationManagementTable destructor: detach itself from all the Subject (collection and all the relation)
     */
    ~DialogXmlRelationManagementTable();

private:
    /**
     * @brief ModifyRelationPressed: show the interface that permits to modify a single relationo
     * @param row of the relation to modify
     */
    void ModifyRelationPressed(int row);

    /**
     * @brief DeleteRelationPressed: ask for confirm and then delete a relation
     * @param row of the relation to delete
     */
    void DeleteRelationPressed(int row);

private slots:
    /**
     * @brief AddRelationPressed: call the dialog to take the input in order to insert a new relation
     */
    void AddRelationPressed();

    /**
     * @brief OkButtonPressed: it's executed when the Ok button is pressed and simply close the dialog with the accepted signal
     */
    void OkButtonPressed();

    /**
     * @brief CellDoubleClicked: when a user double click of a cell open the modify xml relation dialog
     * @param indexRow: the row of the clicked cell
     */
    void CellDoubleClicked(int indexRow, int);

    /**
     * @brief ShowContextualMenuTable: show the contextual menu of the table
     * @param point: where the user has clicked
     */
    void ShowContextualMenuTable(QPoint point);

private:
    /**
     * @brief ReFillTheTable: clear and re-fill the table with the latest version of the xml relation collection
     */
    void ReFillTheTable();

public:

    /**
     * @brief Update
     * @param updateCode: to know what time of update is needed
     */
    virtual void Update(UPDATE_CODE updateCode);

    /**
     * @brief show: show the widget
     */
    virtual void Show();

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide();
};

#endif // DIALOGXMLRELATIONMANAGEMENTTABLE_H
