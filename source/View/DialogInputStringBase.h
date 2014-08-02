/**
 * File: DialogInputStringBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef DIALOGINPUTSTRINGBASE_H
#define DIALOGINPUTSTRINGBASE_H

#include <QDialog>

/**
 * @brief The DialogInputFileNameBase abstract base class for a dialog that take input for a full path file name
 */
class DialogInputStringBase : public QDialog
{
public:
    /**
     * @brief DialogInputStringBase constructor
     * @param parent widget
     */
    DialogInputStringBase(QWidget *parent = 0);

    /**
     * @brief ~DialogInputStringBase vitual destructor
     */
    virtual ~DialogInputStringBase();

    /**
     * @brief GetAbsoluteFileName: get the abosulte file name of a file with the inserted extension
     * @return the inserted absolute File Name
     */
    virtual QString GetInputString() const = 0;

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;

};

#endif // DIALOGINPUTSTRINGBASE_H
