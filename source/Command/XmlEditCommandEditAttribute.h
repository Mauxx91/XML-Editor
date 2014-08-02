/**
 * File: XmlEditCommandEditAttribute.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#ifndef XMLEDITCOMMANDEDITATTRIBUTE_H
#define XMLEDITCOMMANDEDITATTRIBUTE_H

#include "XmlEditCommandBase.h"

class XmlEditCommandEditAttribute : public XmlEditCommandBase
{
private:
    QString m_identifierNumber;

    QString m_attrName;
    QString m_value;
    QString m_previosValue;

public:
    XmlEditCommandEditAttribute(XmlDocument* xmlDocument, const QString& identifierNumber, const QString& attrName, const QString& value);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();
};

#endif // XMLEDITCOMMANDEDITATTRIBUTE_H
