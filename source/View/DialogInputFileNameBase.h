/**
 * File: DialogInputFileNameBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/22
*/

#ifndef DIALOGINPUTFILENAMEBASE_H
#define DIALOGINPUTFILENAMEBASE_H

#include <QDialog>

/**
 * @brief The DialogInputFileNameBase abstract base class for a dialog that take input for a full path file name
 */
class DialogInputFileNameBase : public QDialog
{
protected:
    /**
     * @brief m_fileExtension of the fileName to take in input
     */
    QString m_fileExtension;

public:
    /**
     * @brief DialogInputFileNameBase constructor
     * @param parent widget
     */
    DialogInputFileNameBase(const QString& fileExtension, QWidget *parent = 0);

    /**
     * @brief ~DialogInputFileNameBase vitual destructor
     */
    virtual ~DialogInputFileNameBase();

    /**
     * @brief GetAbsoluteFileName: get the abosulte file name of a file with the inserted extension
     * @return the inserted absolute File Name
     */
    virtual QString GetAbsoluteFileName() const = 0;

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;

};

#endif // DIALOGINPUTFILENAMEBASE_H
