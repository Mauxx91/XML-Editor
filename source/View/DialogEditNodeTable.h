/**
 * File: DialogEditNodeTable.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#ifndef DIALOGEDITNODETABLE_H
#define DIALOGEDITNODETABLE_H

#include "DialogEditNodeBase.h"

#include <QDomElement>
#include <QFont>

class QFormLayout;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QTableWidget;

class DialogEditNodeTable : public DialogEditNodeBase
{
    Q_OBJECT

private:
    QFont m_boldFont;

    QFormLayout* m_pFormLayout;
    QHBoxLayout* m_pHorizontalLayout;
    QVBoxLayout* m_Vertlayout;

    //Tag value widget
    QLabel* m_pValueLabel;
    QLineEdit* m_pValueLine;

    //attribute table
    QLabel* m_pAttributeTitle;
    QTableWidget* m_pTable;

    //Buttons
    QPushButton* m_pAddAttribute;
    QPushButton* m_pApplyBnt;
    QPushButton* m_pCancelBnt;

    //node to edit
    QDomElement m_node;

public:
    DialogEditNodeTable(XmlEditCommandInvoker* xmlEditCommandInvoker, XmlDocument* xmlDocument, const QString& identifierNumber,
                        QWidget *parent = 0);

private:
    //Fill the content from the node
    void LoadContent();

private slots:
    void ShowContextual(QPoint point);

    void ApplyButtonPressed();

    void AddAttributePressed();

    void RemoveAttributePressed(int index);

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

#endif // DIALOGEDITNODETABLE_H
