/**
 * File: IXmlRelationCheckerCore.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#ifndef XMLRELATIONCHECKERCOREBASE_H
#define XMLRELATIONCHECKERCOREBASE_H

#include <QList>
#include <QDomNode>

class XmlDocument;
class XmlRelationCollection;
class XmlRelationError;
class XmlRelation;

/**
 * @brief The IXmlRelationCheckerCore class represent the analyzer of the relations in a set of xmlDocument
 */
class XmlRelationCheckerCoreBase
{
protected:
    /**
     * @brief m_xmlDocumentsList: the collection of the xml document to check
     */
    QList<XmlDocument*>* m_xmlDocumentsList;

    /**
     * @brief m_pXmlRelationCollection: the collection of the xml relation to check
     */
    XmlRelationCollection* m_pXmlRelationCollection;

public:
    /**
     * @brief XmlRelationCheckerCoreBase constructor that initialize the class member with the data in input
     * @param xmlDocumentsList: the collection of the xml document to check
     * @param xmlRelationCollection: the collection of the xml relation to check
     */
    XmlRelationCheckerCoreBase(QList<XmlDocument *>* xmlDocumentsList, XmlRelationCollection* xmlRelationCollection);

    /**
     * @brief CheckAllRelationInAllXmlDocuments: check all the relations in all the xmlDocuments setted
     * @return the list of the found errors
     */
    virtual QList<XmlRelationError*>* CheckAllRelationInAllXmlDocuments() const = 0;

    /**
     * @brief CheckARelationPathExistence: return a relation path for the given start and end point
     * @param tagFromName: the name of the tag from which the path start
     * @param tagToName: the name of the tag destination of the path
     * @return the list of the xmlRelation that shape the path. If the list is empty then the path doesn't exist
     */
    virtual QList<XmlRelation*> CheckARelationPathExistence(const QString& tagFromName, const QString& tagToName) const = 0;

    virtual int GetIdentifierAndWhereOfFistNodeTo(QString* o_pIdentifierNumber, QString* o_pAbsoluteFileName,
                                                  bool& o_isASubNodeFromOfARelation, const QDomNode& candidateSubNodeFrom) const = 0;
};

#endif // XMLRELATIONCHECKERCOREBASE_H
