/**
 * File: XmlEditCommandAggregator.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "XmlEditCommandAggregator.h"

XmlEditCommandAggregator::XmlEditCommandAggregator(XmlDocument *xmlDocument): XmlEditCommandBase(xmlDocument) {}

XmlEditCommandAggregator::~XmlEditCommandAggregator()
{
    //Delete all the included commands
    for(int i=0; i < m_commandsList.size(); ++i)
    {
        delete m_commandsList.at(i);
    }
}

void XmlEditCommandAggregator::AddCommand(XmlEditCommandBase* command)
{
    m_commandsList.push_back(command);
}

void XmlEditCommandAggregator::Execute()
{
    //For each command execute
    for(int i=0; i < m_commandsList.size(); ++i)
    {
        m_commandsList.at(i)->Execute();
    }
}

void XmlEditCommandAggregator::Undo()
{
    //For each command undo //The order is the opposite
    for(int i=m_commandsList.size()-1; i >= 0; --i)
    {
        m_commandsList.at(i)->Undo();
    }
}

int XmlEditCommandAggregator::Size() const
{
    return m_commandsList.size();
}
