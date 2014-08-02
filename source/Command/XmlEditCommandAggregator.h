/**
 * File: XmlEditCommandAggregator.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef XMLEDITCOMMANDAGGREGATOR_H
#define XMLEDITCOMMANDAGGREGATOR_H

#include "XmlEditCommandBase.h"

#include <QList>

/**
 * @brief The XmlEditCommandAggregator class that permits to aggregate multiple commands and as a single command
 */
class XmlEditCommandAggregator : public XmlEditCommandBase
{
private:
    QList<XmlEditCommandBase*> m_commandsList;

public:
    /**
     * @brief XmlEditCommandAggregator constructor
     */
    XmlEditCommandAggregator(XmlDocument* xmlDocument);

    /**
     * @brief XmlEditCommandAggregator destructor that destroy all the commands added to that object
     */
    ~XmlEditCommandAggregator();

    /**
     * @brief AddCommand in the aggregator
     * @param command to add in the aggregator
     */
    void AddCommand(XmlEditCommandBase* command);

    /**
     * @brief Execute of the command executing all the included command
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command by executing the undo method of all the included commad
     */
    virtual void Undo();

    int Size() const;

};

#endif // XMLEDITCOMMANDAGGREGATOR_H
