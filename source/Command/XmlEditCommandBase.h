/**
 * File: XmlEditCommand.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#ifndef XMLEDITCOMMANDDABASE_H
#define XMLEDITCOMMANDDABASE_H

#include <QString>

class XmlDocument;

/**
 * @brief The XmlEditCommand abstract base class that represent the execution of a command for edit a xml document
 */
class XmlEditCommandBase
{
protected:
    /**
     * @brief m_pXmlDocument to edit in the concrete class
     */
    XmlDocument* m_pXmlDocument;

public:
    /**
     * @brief XmlEditCommandBase constructor
     * @param XmlDocument to modify
     */
    XmlEditCommandBase(XmlDocument* xmlDocument);

    /**
     * @brief XmlEditCommandBase virtual destructor
     */
    ~XmlEditCommandBase();

    /**
     * @brief GetXmlDocument of the command
     * @return the xml document of the command
     */
    XmlDocument* GetXmlDocument() const;

    /**
     * @brief Execute of the command represented by the class
     */
    virtual void Execute() = 0;

    /**
     * @brief Undo the execution of the command
     */
    virtual void Undo() = 0;

    QString ComputeParentIdentifier(const QString& childIdentifier);
};

#endif // XMLEDITCOMMANDDABASE_H
