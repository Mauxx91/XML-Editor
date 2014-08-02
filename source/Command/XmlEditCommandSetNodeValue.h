/**
 * File: XmlEditCommandSetNodeValue.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#ifndef XMLEDITCOMMANDSETNODEVALUE_H
#define XMLEDITCOMMANDSETNODEVALUE_H

#include "XmlEditCommandBase.h"

class XmlEditCommandSetNodeValue : public XmlEditCommandBase
{
private:
    QString m_identifierNumber;

    QString m_value;
    QString m_previousValue;

public:
    XmlEditCommandSetNodeValue(XmlDocument* xmlDocument, const QString& identifierNumber, const QString& value);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();
};


#endif // XMLEDITCOMMANDSETNODEVALUE_H
