/**
 * File: XmlRelationError.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#include "XmlRelationError.h"

#include "XmlRelation.h"

XmlRelationError::XmlRelationError()
{
    //Set the default value
    m_errorType = XML_RELATION_ERROR_TYPE_NO_INSTANCE_FOUND;
}

XmlRelationError::XmlRelationError(const QString& absoluteFileName, XmlRelation* xmlRelation, const QString& identifierNumber,
                                   const QString& key, XML_RELATION_ERROR_TYPE errorType)
{
    SetAll(absoluteFileName, xmlRelation, identifierNumber, key, errorType);
}


void XmlRelationError::SetAll(const QString& absoluteFileName, XmlRelation* xmlRelation, const QString& identifierNumber,
                              const QString& key, XML_RELATION_ERROR_TYPE errorType)
{
    m_absoluteFileName = absoluteFileName;
    m_pXmlRelation = xmlRelation;
    m_identifierNumber = identifierNumber;
    m_key = key;
    m_errorType = errorType;
}

const QString& XmlRelationError::GetAbsoluteFileName() const
{
    return m_absoluteFileName;
}

const XmlRelation* XmlRelationError::GetXmlRelation() const
{
    return m_pXmlRelation;
}

const QString& XmlRelationError::GetIdentifierNumber() const
{
    return m_identifierNumber;
}

const QString& XmlRelationError::GetKey() const
{
    return m_key;
}

XML_RELATION_ERROR_TYPE XmlRelationError::GetErrorType() const
{
    return m_errorType;
}

XML_RELATION_ERROR_SERIOUSNESS_LEVEL XmlRelationError::GetSeriousnessLevel() const
{
    switch(m_errorType)
    {
        case XML_RELATION_ERROR_TYPE_NO_INSTANCE_FOUND: return XML_RELATION_ERROR_SERIOUSNESS_LEVEL_INFO;

        case XML_RELATION_ERROR_TYPE_REFERENCED_NOT_FOUND: return XML_RELATION_ERROR_SERIOUSNESS_LEVEL_ERROR;

        case XML_RELATION_ERROR_TYPE_REFERENCED_NEVER_REFERENCED: return XML_RELATION_ERROR_SERIOUSNESS_LEVEL_WARNING;

    }
    return XML_RELATION_ERROR_SERIOUSNESS_LEVEL_INFO;
}

QString XmlRelationError::GetErrorDescription() const
{
    QString description;

    switch(m_errorType)
    {
        case XML_RELATION_ERROR_TYPE_NO_INSTANCE_FOUND:
            description = "XML_RELATION_ERROR_TYPE_NO_INSTANCE_FOUND";
            break;

        case XML_RELATION_ERROR_TYPE_REFERENCED_NOT_FOUND:
            //Old active description
            /*description.append("Tag: " +m_pXmlRelation->GetTagFromName()+ " with child tag: " +m_pXmlRelation->GetSubTagNameOfTagFrom());
            description.append(" reference element: " +m_pXmlRelation->GetTagToName()+ " with key: <b>" +m_key + "</b>");
            description.append(" but that element doesn't exist.");*/

            //New passive description that keep at the left the key for short view
            description.append("Nonexistent key: <b>"+m_key+"</b> of the tag: <i>" + m_pXmlRelation->GetTagToName() + "</i>"
                               " is referenced by tag: <i>" +m_pXmlRelation->GetTagFromName() + "</i>"
                               " with child tag: <i>" +m_pXmlRelation->GetSubTagNameOfTagFrom() + "</i>");

            break;

        case XML_RELATION_ERROR_TYPE_REFERENCED_NEVER_REFERENCED:
            //Old active description
            /*description.append("Tag: " +m_pXmlRelation->GetTagToName()+ " with key: " +m_key);
            description.append(" is present but never referenced by any tags.");*/

            //New passive description that keep at the left the key for short view
            description.append("Key: <b>"+m_key+"</b> of the tag: <i>" + m_pXmlRelation->GetTagToName() + "</i>"
                               " is never referenced by any tags.");

            break;

    }

    return description;
}
