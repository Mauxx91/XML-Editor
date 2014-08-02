/**
 * File: AssociatedFiles.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/18
*/

#include "AssociatedFiles.h"

#include "../Observer/IObserver.h"

#include <QSettings>

AssociatedFiles::AssociatedFiles(const QString& mainFileName): m_baseKeyAssociatedFileName("k_AssociatedFileName"), m_separator('?')
{
    //Set initial value
    m_lastModifiedFileName = "";

    //Initialize the QSettings object
    m_pSettings = new QSettings;

    //Initialize the main file name
    m_mainAbsoluteFileName = mainFileName;
}

AssociatedFiles::~AssociatedFiles()
{
    delete m_pSettings;
}

void AssociatedFiles::ChangeMainFile(QString newMainAbsoluteFileName)
{
    //Only if it's different
    if(m_mainAbsoluteFileName != newMainAbsoluteFileName)
    {
        //Change the file name
        m_mainAbsoluteFileName = newMainAbsoluteFileName;

        //Change the key for the new file name
        m_keyAssociatedFileName = m_baseKeyAssociatedFileName + m_mainAbsoluteFileName;

        //Clear all the loaded associated file names
        m_associatedAbsolutedFileNameList.clear();
        for(int i = m_associatedAbsolutedFileNameList.size()-1; i>=0; --i)
        {
            RemoveAssociatedFile(i);
        }

        //Reload all the associated file names
        LoadAssociatedFileName();
    }
}

const QString& AssociatedFiles::GetMainFileAbsoluteFileName() const
{
    return m_mainAbsoluteFileName;
}

const QStringList& AssociatedFiles::GetAssociatedFileNamesList() const
{
    return m_associatedAbsolutedFileNameList;
}

const QString& AssociatedFiles::GetAssociatedFileName(int index) const
{
    //If the index is valid
    if(index >= 0 && index < m_associatedAbsolutedFileNameList.size())
    {
        return m_associatedAbsolutedFileNameList.at(index);
    }
    else
    {
        static QString emptyString;
        return emptyString;
    }
}

const QString& AssociatedFiles::GetLastModifiedFileName() const
{
    return m_lastModifiedFileName;
}

void AssociatedFiles::RemoveAssociatedFile(int index)
{
    //Check if it's a valid index
    if(index >= 0 && index < m_associatedAbsolutedFileNameList.size())
    {
        //Set the last modified file name
        m_lastModifiedFileName = m_associatedAbsolutedFileNameList.at(index);

        m_associatedAbsolutedFileNameList.removeAt(index);

        //Update the registry
        WriteAssociatedFileName();

        Notify(UPDATE_REMOVED_ASSOCIATED_FILE);
    }
}

bool AssociatedFiles::AddAssociatedFileName(const QString& associatedAbsoluteFileName, int index)
{
    bool fileWasNotAlreadyPresent = false;

    //Check if the file is already present
    for(int i=0; i < m_associatedAbsolutedFileNameList.size() && !fileWasNotAlreadyPresent; ++i)
    {
        if(m_associatedAbsolutedFileNameList.at(i) == associatedAbsoluteFileName)
        {
            fileWasNotAlreadyPresent = true;
        }
    }

    //Insert only if the file was not present yet
    if( ! fileWasNotAlreadyPresent)
    {
        //Set the new file name
        if( index < 0)
        {
            m_associatedAbsolutedFileNameList.append(associatedAbsoluteFileName);
        }
        else
        {
            m_associatedAbsolutedFileNameList.insert(index, associatedAbsoluteFileName);
        }

        //Set the last modified file name
        m_lastModifiedFileName = associatedAbsoluteFileName;

        //Update the registry
        WriteAssociatedFileName();

        Notify(UPDATE_NEW_ASSOCIATED_FILE);
    }

    return !fileWasNotAlreadyPresent;
}

void AssociatedFiles::WriteAssociatedFileName()
{
    //All filenames is joined in a unique string with m_separator as separator character
    const QString& allFileNames = m_associatedAbsolutedFileNameList.join(m_separator);

    //Update the value in the registry
    m_pSettings->setValue(m_keyAssociatedFileName, allFileNames);
}


void AssociatedFiles::LoadAssociatedFileName()
{
    //Load the all filenames string and get a empty string if the value doesn't exist
    const QString& allFileNames = m_pSettings->value(m_keyAssociatedFileName, QVariant("")).toString();

    //Obtain associate filenames list by splitting the string every occurrence of the separator character
    const QStringList& listAssociatedFiles = allFileNames.split(m_separator, QString::SkipEmptyParts);

    //Flow the list and insert one file at time in order to call the notify for each file
    for(int i=0; i < listAssociatedFiles.size(); ++i)
    {
        AddAssociatedFileName(listAssociatedFiles.at(i));
    }
}
