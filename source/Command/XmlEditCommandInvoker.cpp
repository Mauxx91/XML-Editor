/**
 * File: XmlEditCommandInvoker.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "XmlEditCommandInvoker.h"

#include "XmlEditCommandBase.h"

XmlEditCommandInvoker::XmlEditCommandInvoker()
{
    m_indexOfLastValid = 0;
    emit UndoEnabled(false);
    emit RedoEnabled(false);
}

void XmlEditCommandInvoker::ExecuteACommand(XmlEditCommandBase* xmlCommand)
{
    //Execute the command
    xmlCommand->Execute();

    //Erase all the command after the indexOfthelastValidCommand
    for(int i=m_commandsList.size()-1; i>m_indexOfLastValid; --i)
    {
        m_commandsList.removeAt(i);
    }

    //Store the command
    m_commandsList.push_back(xmlCommand);

    m_indexOfLastValid = m_commandsList.size()-1;

    //Almost a command is insered do enable the undo
    emit UndoEnabled(true);
    //The index is on the last command so disable the redo
    emit RedoEnabled(false);
}

void XmlEditCommandInvoker::Undo()
{
    //Execute the undo on the last valid command
    m_commandsList.at(m_indexOfLastValid)->Undo();

    //Decrease the last valid command
    m_indexOfLastValid += -1;

    //If there's no other command to undo
    if(m_indexOfLastValid == -1)
    {
        emit UndoEnabled(false);
    }

    //There at least one command to redo so enable it
    emit RedoEnabled(true);
}

void XmlEditCommandInvoker::Redo()
{
    //Execute the redo on the fist non valid command
    m_commandsList.at(m_indexOfLastValid+1)->Execute();

    //Increase the last valid command
    m_indexOfLastValid += 1;

    //If there's no other command to redo
    if(m_indexOfLastValid == m_commandsList.size()-1)
    {
        emit RedoEnabled(false);
    }

    //There at least one command to undo so enable it
    emit UndoEnabled(true);
}

void XmlEditCommandInvoker::Clear()
{
    m_commandsList.clear();
    emit UndoEnabled(false);
    emit RedoEnabled(false);
}

void XmlEditCommandInvoker::ClearForSingleXmlDocument(XmlDocument* xmlDocument)
{
    //For every command check if it's about the xmlDocument
    int size = m_commandsList.size();

    for(int i=size-1; i >= 0; ++i)
    {
        m_commandsList.at(i);
        if(m_commandsList.at(i)->GetXmlDocument() == xmlDocument)
        {
            m_commandsList.removeAt(i);
            m_indexOfLastValid--;
        }
    }

    //Set undo and redo corence to the new state
    if(m_indexOfLastValid == 0)
    {
        //No undo
        emit UndoEnabled(false);

        if(m_commandsList.size() > 0)
        {
            emit RedoEnabled(true);
        }
        else
        {
            emit RedoEnabled(false);
        }
    }
    else
    {
        if(m_indexOfLastValid == m_commandsList.size()-1)
        {
            emit RedoEnabled(false);

            if(m_commandsList.size() > 0)
            {
                emit UndoEnabled(true);
            }
            else
            {
                emit UndoEnabled(false);
            }
        }
        else
        {
            emit UndoEnabled(true);
            emit RedoEnabled(true);
        }
    }
}
