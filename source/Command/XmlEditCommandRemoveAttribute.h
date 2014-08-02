/**
 * File: XmlEditCommandRemoveAttribute.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/31
*/

#ifndef XMLEDITCOMMANDREMOVEATTRIBUTE_H
#define XMLEDITCOMMANDREMOVEATTRIBUTE_H

#include "XmlEditCommandBase.h"

class XmlEditCommandRemoveAttribute : public XmlEditCommandBase
{
private:
    QString m_identifierNumber;

    QString m_attrName;
    QString m_previousValue;

public:
    XmlEditCommandRemoveAttribute(XmlDocument* xmlDocument, const QString& identifierNumber, const QString& attrName);

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute();

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo();
};

#endif // XMLEDITCOMMANDREMOVEATTRIBUTE_H
