/**
 * File: XmlRelationError.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#ifndef XMLRELATIONERROR_H
#define XMLRELATIONERROR_H

#include <QString>

class XmlRelation;

enum XML_RELATION_ERROR_TYPE {XML_RELATION_ERROR_TYPE_NO_INSTANCE_FOUND,
                              XML_RELATION_ERROR_TYPE_REFERENCED_NOT_FOUND,
                              XML_RELATION_ERROR_TYPE_REFERENCED_NEVER_REFERENCED};

enum XML_RELATION_ERROR_SERIOUSNESS_LEVEL {XML_RELATION_ERROR_SERIOUSNESS_LEVEL_INFO,
                                           XML_RELATION_ERROR_SERIOUSNESS_LEVEL_WARNING,
                                           XML_RELATION_ERROR_SERIOUSNESS_LEVEL_ERROR};

/**
 * @brief The XmlRelationError class represent an error in the xml compared to the xml relation
 */
class XmlRelationError
{
private:
    /**
     * @brief absoluteFileName of the file in which the error is found
     */
    QString m_absoluteFileName;

    /**
     * @brief m_pXmlRelation: pointer to the xml relation in which there's the error
     */
    XmlRelation* m_pXmlRelation;

    /**
     * @brief m_identifierNumber: hierarchical number of the tag in which the error was found
     */
    QString m_identifierNumber;

    /**
     * @brief m_key: value of the key attribute
     */
    QString m_key;

    /**
     * @brief m_errorType: type of the error
     */
    XML_RELATION_ERROR_TYPE m_errorType;

public:
    /**
     * @brief XmlRelationError constructor that initialize all the class parameters to the default value
     */
    XmlRelationError();

    /**
     * @brief XmlRelationError constructor that initialize all the class parameter with the input of the function
     * @param absoluteFileName of the file in which the error is found
     * @param xmlRelation: pointer to the xml relation in which there's the error
     * @param identifierNumber: hierarchical number of the tag in which the error was found
     * @param errorType: type of the error
     */
    XmlRelationError(const QString& absoluteFileName, XmlRelation* xmlRelation, const QString& identifierNumber,
                     const QString& key, XML_RELATION_ERROR_TYPE errorType);

    //Setter
    void SetAll(const QString& absoluteFileName, XmlRelation* xmlRelation, const QString& identifierNumber,
                const QString& key, XML_RELATION_ERROR_TYPE errorType);

    //Getter
    const QString& GetAbsoluteFileName() const;
    const XmlRelation* GetXmlRelation() const;
    const QString& GetIdentifierNumber() const;
    const QString& GetKey() const;
    XML_RELATION_ERROR_TYPE GetErrorType() const;
    XML_RELATION_ERROR_SERIOUSNESS_LEVEL GetSeriousnessLevel() const;
    QString GetErrorDescription() const;
};

#endif // XMLRELATIONERROR_H
