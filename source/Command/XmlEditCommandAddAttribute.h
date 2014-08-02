/**
 * File: XmlEditCommandAddAttribute.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#ifndef XMLEDITCOMMANDADDATTRIBUTE_H
#define XMLEDITCOMMANDADDATTRIBUTE_H

#include "XmlEditCommandBase.h"

class XmlEditCommandAddAttribute : public XmlEditCommandBase
{
private:
    QString m_identifierNumber;

    QString m_attrName;
    QString m_value;

public:
    XmlEditCommandAddAttribute(XmlDocument* xmlDocument, const QString& identifierNumber, const QString& attrName, const QString& value);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();
};

#endif // XMLEDITCOMMANDADDATTRIBUTE_H
