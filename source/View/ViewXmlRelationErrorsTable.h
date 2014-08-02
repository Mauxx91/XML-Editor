/**
 * File: ViewXmlRelationErrorsTable.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/24
*/

#ifndef VIEWXMLRELATIONERRORSTABLE_H
#define VIEWXMLRELATIONERRORSTABLE_H

#include "ViewXmlRelationErrorsBase.h"

#include <QWidget>
#include <QFont>

class QTableWidget;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;

/**
 * @brief The ViewXmlRelationErrorsTable class that implements the ViewXmlRelationErrorsBase with a table
 */
class ViewXmlRelationErrorsTable : public QWidget, public ViewXmlRelationErrorsBase
{
    Q_OBJECT

private:
    /**
     * @brief m_pTable: table used for show the errors
     */
    QTableWidget* m_pTable;

    QFont m_boldFont;

    //Layout
    QVBoxLayout* m_pVertlayout;
    QHBoxLayout* m_pTitleHorizLayout;

    //Label
    QLabel* m_pSummaryLabel;

    //Buttons
    QPushButton* m_pErrorBnt;
    QPushButton* m_pWarningBnt;
    QPushButton* m_pInfoBnt;

    //QIcons
    QIcon* m_pErrorIcon;
    QIcon* m_pWarningIcon;
    QIcon* m_pInfoIcon;

public:
    ViewXmlRelationErrorsTable(QList<XmlRelationError*>* xmlRelationErrorList);

    /**
     * @brief SetNewXmlRelationErrorList: set a new error list and update the view
     * @param xmlRelationErrorList: the new error collection to display
     */
    virtual void SetNewXmlRelationErrorList(QList<XmlRelationError*>* xmlRelationErrorList);

private:

    /**
     * @brief UpdateSummaryLabel: update the summary label
     */
    void UpdateSummaryLabel();

    /**
     * @brief UpdateButtons: update the content of the buttons
     * @param errors number
     * @param warnings number
     * @param infos number
     */
    void UpdateButtons(int errors, int warnings, int infos);

    /**
     * @brief ReLoadWidget: update the content of the widget
     * @param notFilterAndUpdateButtons: if is true non filter the output on the serevity and reload the buttons,
     *                                   otherwise filter on the severty and not update the buttons
     */
    void ReLoadWidget(bool notFilterAndUpdateButtons);

    /**
     * @brief FillTableAndButtons: fill the table with the error and the buttons content
     * @param notFilterAndUpdateButtons: if is true non filter the output on the serevity and reload the buttons,
     *                                   otherwise filter on the severty and not update the buttons
     */
    void FillTableAndButtons(bool notFilterAndUpdateButtons);

    /**
     * @brief AbsoluteFileNameToRelativeFileName: get the file name from a full path file name
     * @param absoluteFileName: the obsolute file name to convert
     * @return the file name
     */
    QString AbsoluteFileNameToRelativeFileName(const QString& absoluteFileName);

private slots:
    /**
     * @brief ButtonToggled: reload the widget when a button is toggled
     */
    void ButtonToggled();

    /**
     * @brief ShowContextualMenuTable: show the contextual menu
     * @param point: where show the menu
     */
    void ShowContextualMenuTable(QPoint point);

    /**
     * @brief CellDoubleClicked: call the show of the tag with the error in the view
     * @param indexRow: indexRow clicked
     */
    void CellDoubleClicked(int indexRow, int);

signals:

    void ShowErrorNodeRequested(const QString& identifierNumber, const QString& absoluteFileName);

private:
    /**
     * @brief EmitShowErrorNodeRequested: find the identifier of the selected row and the file name and emit the signal
     * @param rowIndex: selected index of the table
     */
    void EmitShowErrorNodeRequested(int rowIndex);

public:
    /**
     * @brief show: show the widget
     */
    virtual void Show();

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide();
};

#endif // VIEWXMLRELATIONERRORSTABLE_H
