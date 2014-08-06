/**
 * File: XmlEditCommandInvoker.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef XMLEDITCOMMANDINVOKER_H
#define XMLEDITCOMMANDINVOKER_H

#include <QObject>
#include <QList>

class XmlEditCommandBase;
class XmlDocument;

/**
 * @brief The XmlEditCommandInvoker class that collect the story of the executed command
 */
class XmlEditCommandInvoker: public QObject
{
    Q_OBJECT

private:
    /**
     * @brief m_commandsList: the list of the executed commands
     */
    QList<XmlEditCommandBase*> m_commandsList;

    int m_indexOfLastValid;
public:

    XmlEditCommandInvoker();

    void Initialize();

    /**
     * @brief ExecuteACommand: exucute the given command and store the command to mantain a history in order to can get back the precedent state
     * @param xmlCommand to execute
     */
    void ExecuteACommand(XmlEditCommandBase* xmlCommand);

    //Undo the last command
    void Undo();

    //Redo the last command undoed
    void Redo();

    /**
     * @brief Clear: remove all the command stored in the class resetting the initial state
     */
    void Clear();

    /**
     * @brief ClearForSingleXmlDocument: clear all the command about the given document
     * @param absoluteFileName if the document
     */
    void ClearForSingleXmlDocument(XmlDocument* xmlDocument);

signals:
    void UndoEnabled(bool isEnabled);

    void RedoEnabled(bool isEnabled);
};

#endif // XMLEDITCOMMANDINVOKER_H
