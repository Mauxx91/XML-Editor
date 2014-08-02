/**
 * File: DialoAssociatedFilesBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/18
*/

#ifndef DIALOASSOCIATEDFILESBASE_H
#define DIALOASSOCIATEDFILESBASE_H

#include <QDialog>

class AssociatedFiles;

/**
 * @brief The DialoAssociatedFilesBase abstract base class of a dialog able to edit the associated files
 */
class DialogAssociatedFilesBase : public QDialog
{
protected:
    /**
     * @brief m_pAssociatedFiles: object that contains the list of the associated filenames
     */
    AssociatedFiles* m_pAssociatedFiles;

public:
    /**
     * @brief DialoAssociatedFilesBase constructor that set the parent widget to the base class and the AssociatedFiles data object
     * @param parent: parent widget
     */
    DialogAssociatedFilesBase(AssociatedFiles* associatedFiles, QWidget *parent = 0);

    /**
     * @brief ~DialoAssociatedFilesBase vitual destructor
     */
    virtual ~DialogAssociatedFilesBase();

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;
};

#endif // DIALOASSOCIATEDFILESBASE_H
