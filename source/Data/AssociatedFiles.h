/**
 * File: AssociatedFiles.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/18
*/

#ifndef ASSOCIATEDFILES_H
#define ASSOCIATEDFILES_H

#include "../Observer/Subject.h"

#include <QStringList>

class QSettings;

/**
 * @brief The AssociatedFiles class is a string list of the obsolute file name of the associated file and a osservable Subject.
 *        For every file name (with absolute path) it store and load the associated file name in the system registry for persistence
 */
class AssociatedFiles : public Subject
{
private:
    /**
     * @brief m_mainAbsoluteFileName: name of the main file
     */
    QString m_mainAbsoluteFileName;

    /**
     * @brief m_associatedAbsolutedFileNameList: list of the absoluted file name of the associated files
     */
    QStringList m_associatedAbsolutedFileNameList;

    /**
     * @brief m_pSettings: object for load and writes datas in the system registry
     */
    QSettings* m_pSettings;

    /**
     * @brief m_baseKeyAssociatedFileName: base key used for formate a personalized key for every file
     */
    const QString m_baseKeyAssociatedFileName;

    /**
     * @brief m_keyAssociatedFileName: key used for access the value in the registry for the m_attributeName
     */
    QString m_keyAssociatedFileName;

    /**
     * @brief separator character used to distinguish every file name when all files is joined in a unique string
     */
    const QChar m_separator;

    /**
     * @brief m_lastModifiedFileName: file name of the last modified file
     */
    QString m_lastModifiedFileName;

public:
    /**
     * @brief AssociatedFiles constructor that load the associated file name of the main file in input if isn't equal to the empty string
     * @param mainFileName: absolute file name of the main file
     */
    AssociatedFiles(const QString& mainFileName = "");

    /**
     * @brief AssociatedFiles destructor that delete the dynamic object used
     */
    ~AssociatedFiles();

    /**
     * @brief ChangeMainFile: reinitialize the object with a new main file
     * @param mainFileName: new Main file name complete of absolute path
     */
    void ChangeMainFile(QString newMainAbsoluteFileName);

    /**
     * @brief GetMainFileAbsoluteFileName: get the main file abosulute path
     * @return the main file abosulte path
     */
    const QString& GetMainFileAbsoluteFileName() const;

    /**
     * @brief GetAssociatedFileNamesList: get the associated file names list with absolute path
     * @return the list of the absolute file names of the associated file names
     */
    const QStringList& GetAssociatedFileNamesList() const;

    /**
     * @brief GetAssociatedFileName: get associated file name at the given index
     * @param index of desidered associated file name
     * @return the associated file name at the given index
     */
    const QString& GetAssociatedFileName(int index) const;

    /**
     * @brief GetLastModifiedIndex: get the index of the last modified index
     * @return the index of the last modified index
     */
    const QString& GetLastModifiedFileName() const;

    /**
     * @brief RemoveAssociatedFile: remove the file at the index position
     * @param index: index of the list in which is present the file to remove
     */
    void RemoveAssociatedFile(int index);

    /**
     * @brief AddAssociatedFileName: add a new associated file name, if the file is already present it won't be inserted
     * @param associatedAbsoluteFileName: name of the new associated file
     * @param index: index of the position in which insert the new filename, if < 0 it will be appended on back
     * @return tru if the file was inserted
     */
    bool AddAssociatedFileName(const QString& associatedAbsoluteFileName, int index = -1);

private:

    /**
     * @brief WriteAssociatedFileName: write to the registry the associated file names of the main file
     */
    void WriteAssociatedFileName();

    /**
     * @brief LoadAssociateFileName: load from the registry the associated file names of the main file
     */
    void LoadAssociatedFileName();
};

#endif // ASSOCIATEDFILES_H
