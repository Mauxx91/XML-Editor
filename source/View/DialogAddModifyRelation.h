/**
 * File: DialogAddModifyRelation.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#ifndef DIALOGADDMODIFYRELATION_H
#define DIALOGADDMODIFYRELATION_H

#include <QDialog>

class XmlRelation;

class QFormLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QComboBox;

/**
 * @brief The DialogAddModifyRelation class that permits to ask the input for modify a xml relation
 */
class DialogAddModifyRelation : public QDialog
{
    Q_OBJECT

private:
    /**
     * @brief m_pXmlRelation: where store the user input and load if the dialog is used for modify a pre-existent relation
     */
    XmlRelation* m_pXmlRelation;

    //Layouts
    QVBoxLayout* m_pVertLayout;
    QFormLayout* m_pFormLayout1;
    QFormLayout* m_pFormLayout2;
    QHBoxLayout* m_pHorizontLayoutBnt;

    //Central label
    QLabel* m_pCentralLabel;

    //Widgets for the input
    QLabel* m_pTagFromNameLabel;
    QLineEdit* m_pTagFromNameLine;
    QLabel* m_pAttributeNameOfTagFromLabel;
    QLineEdit* m_pAttributeNameOfTagFromLine;
    QLabel* m_pSubTagNameOfTagFromLabel;
    QLineEdit* m_pSubTagNameOfTagFromLine;
    QLabel* m_pTagToNameLabel;
    QLineEdit* m_pTagToNameLine;
    QLabel* m_pAttributeNameOfTagToLabel;
    QLineEdit* m_pAttributeNameOfTagToLine;
    QLabel* m_pRelationTypeLabel;
    QComboBox* m_pRelationTypeLine;

    //Buttons
    QPushButton* m_pSaveBnt;
    QPushButton* m_pCancelBnt;

public:
    /**
     * @brief DialogAddModifyRelation constructor that also initialize the value from a given xmlRelation if the isNewRelation bool is false
     * @param xmlRelation to fill with the user input
     * @param isNewRelation: if the dialog is used for a new xml relation or for modify a pre-existent xml relation
     * @param parent widget of this dialog
     */
    DialogAddModifyRelation(XmlRelation* xmlRelation, bool isNewRelation = true, QWidget *parent = 0);

private:
    /**
     * @brief InitializeValuesFromXmlRelation: initialize the input line from the xml relation object
     */
    void InitializeValuesFromXmlRelation();

    /**
     * @brief SaveTheModifiesInTheXmlObject: change the content of the xml relation object with the user input
     */
    void SaveTheModifiesInTheXmlObject();

    /**
     * @brief CheckInput: check the validiti of the input
     * @return true if the input is ok
     */
    bool CheckInput() const;

private slots:
    /**
     * @brief ChangedType: is executed when the user changes the type of the relation
     * @param newIndex selected by the user
     */
    //void ChangedType(int newIndex);

    /**
     * @brief SaveButtonPressed: check the user input and if it's ok call the QDialog::acceot slots
     */
    void SaveButtonPressed();

};

#endif // DIALOGADDMODIFYRELATION_H
