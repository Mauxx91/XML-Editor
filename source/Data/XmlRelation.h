/**
 * File: XmlRelation.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#ifndef XMLRELATION_H
#define XMLRELATION_H

#include "../Observer/Subject.h"

#include <QString>

class QDomDocument;
class QDomElement;
class QDomNode;

enum XML_RELATION_TYPE {SUB_TAG, ATTRIBUTE, TAG_VALUE};

/**
 * @brief The XmlRelation class describes a relation betwen a tag to another.
 *        It supports three different type of relation: as sub tag, as attribute value, as tag value
 */
class XmlRelation : public Subject
{
private:
    //Constant name of the config file tags
    const QString m_tagFromNameCTN;
    const QString m_attributeNameofTagFromCTN;
    const QString m_subTagNameOfTagFromCTN;
    const QString m_tagToNameCTN;
    const QString m_attributeNameOfTagToCTN;
    const QString m_relationTypeCTN;

    /**
     * @brief m_tagFromName: name of the tag from which the relation starts
     */
    QString m_tagFromName;

    /**
     * @brief m_attributeNameofTagFrom: the name of the attribute of the tag from or the sub tag(type 1 or 2, for 3 is not relevant)
     * in which it's written the attribute name of the tag too
     */
    QString m_attributeNameofTagFrom;

    /**
     * @brief m_subTagNameOfTagTo: the name of the sub tag in which it's written the attribute name of the tag to
     */
    QString m_subTagNameOfTagFrom;

    /**
     * @brief m_tagToName: the name of the tag that destination of the relation
     */
    QString m_tagToName;

    /**
     * @brief m_AttributeNameOfTagTo:it's the attribute name in whitch it's written the key of the tag
     */
    QString m_attributeNameOfTagTo;

    /**
     * @brief relationType
     *        1: SUB_TAG
     *        2: ATTRIBUTE
     *        3: TAG_VALUE
     */
    XML_RELATION_TYPE m_relationType;

public:
    /**
     * @brief XmlRelation that initializes all member attributes to the default value
     */
    XmlRelation();

    /**
     * @brief XmlRelation that initialize all the member attributes from the funtion input
     * @param tagFromName
     * @param attributeNameofTagFrom
     * @param subTagNameOfTagFrom
     * @param tagToName
     * @param attributeNameOfTagTo
     * @param type
     */
    XmlRelation(const QString& tagFromName, const QString& attributeNameofTagFrom, const QString& subTagNameOfTagFrom,
                const QString& tagToName, const QString& attributeNameOfTagTo, XML_RELATION_TYPE type = SUB_TAG);

    /**
     * @brief XmlRelation: copy constructor
     * @param xmlRelation: the object from which copy
     */
    XmlRelation(const XmlRelation& xmlRelation);

    //Setter all
    void SetAll(const QString& tagFromName, const QString& attributeNameofTagFrom, const QString& subTagNameOfTagFrom,
                const QString& tagToName, const QString& attributeNameOfTagTo, XML_RELATION_TYPE type = SUB_TAG);

    /**
     * @brief SetParseXmlRelationNode: parse an xmlRelation node and insert the relation in the collection
     * @param xmlRelationNode: the xmlRelation node of the xml to parse
     */
    void SetParseXmlRelationNode(const QDomNode *xmlRelationNode);

    /**
     * @brief FillQDomNodeWithTheRelation: fill the xml node in input with the content of the relation
     * @param m_poXmlRelation: the output.
     */
    void FillQDomNodeWithTheRelation(QDomElement* m_poXmlRelation, QDomDocument *document);

    //Setter
    void SetTagFromName(const QString& tagFromName);
    void SetAttributeNameofTagFrom(const QString& attributeNameofTagFrom);
    void SetSubTagNameOfTagFrom(const QString& subTagNameOfTagFrom);
    void SetTagToName(const QString& tagToName);
    void SetAttributeNameOfTagTo(const QString& attributeNameOfTagTo);
    void SetRelationType(XML_RELATION_TYPE type);

    //Getter
    const QString& GetTagFromName() const;
    const QString& GetAttributeNameofTagFrom() const;
    const QString& GetSubTagNameOfTagFrom() const;
    const QString& GetTagToName() const;
    const QString& GetAttributeNameOfTagTo() const;
    XML_RELATION_TYPE GetRelationType() const;
};

#endif // XMLRELATION_H
