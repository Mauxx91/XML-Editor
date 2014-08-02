/**
 * File: QtXmlRelationCheckerCore.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#ifndef XMLRELATIONCHECKERCOREIMPL_H
#define XMLRELATIONCHECKERCOREIMPL_H

#include "XmlRelationCheckerCoreBase.h"

#include <QDomNode>

class XmlRelation;

/**
 * @brief The XmlRelationCheckerCoreImpl class that implements the XmlRelationCheckerCoreBase
 */
class XmlRelationCheckerCoreImpl : public XmlRelationCheckerCoreBase
{
public:
    /**
     * @brief XmlRelationCheckerCoreImpl constructor that initialize all the meber data
     * @param xmlDocumentsList: the collection of the xml document to check
     * @param xmlRelationCollection: the collection of the xml relation to check
     */
    XmlRelationCheckerCoreImpl(QList<XmlDocument *>* xmlDocumentsList, XmlRelationCollection* xmlRelationCollection);

    /**
     * @brief CheckAllRelationInAllXmlDocuments: check all the relations in all the xmlDocuments setted
     * @return the list of the found errors
     */
    virtual QList<XmlRelationError*>* CheckAllRelationInAllXmlDocuments() const;

    /**
     * @brief CheckARelationPathExistence: return a relation path for the given start and end point
     * @param tagFromName: the name of the tag from which the path start
     * @param tagToName: the name of the tag destination of the path
     * @return the list of the xmlRelation that shape the path. If the list is empty then the path doesn't exist
     */
    virtual QList<XmlRelation*> CheckARelationPathExistence(const QString& tagFromName, const QString& tagToName) const;

    /**
     * @brief GetIdentifierAndWhereOfFistNodeTo: return if the node is a starting point of a relation the first destination and
     *                                           the total number of the destination.
     * @param o_isASubNodeFromOfARelation: set if the node is a valide start of an existent relation
     * @param candidateSubNodeFrom: the node that is the potential sub node of the relation
     * @return the number of the destination of the relation: the output parameter is valid only if the function return a int > 0
     */
    virtual int GetIdentifierAndWhereOfFistNodeTo(QString* o_pIdentifierNumber, QString* o_pAbsoluteFileName,
                                                  bool& o_isASubNodeFromOfARelation, const QDomNode& candidateSubNodeFrom) const;

private:

    QList<XmlRelation*> SearchStartRelation(const QString& tagFromNameTarget, XmlRelation* currentRelation, int currentHop) const;

    /**
     * @brief CheckAllRelation: check all the relation in the QDomDocument starting from the given node
     * @param nodeToCheck: node from start the checking
     * @param errorCollection: where store the founded errors
     * @param absoluteFileName: the file name of the file owner of the QDomModel
     * @param parentNumberBase: hierarchical parent number
     * @param number: last level identifier number
     */
    void CheckAllRelation(QDomNode nodeToCheck, QList<XmlRelationError*>* errorCollection, const QString& absoluteFileName,
                          const QString& parentNumberBase, int number) const;

    //return the list of the key if it's a node from otherwise return the empty list
    QList<QString>* CheckIsNodeFrom(const QDomNode& nodeToCheck, XmlRelation* xmlRelation) const;

    //return the key if it's a node to otherwise return the empty string
    QString CheckIsNodeTo(const QDomNode& nodeToCheck, XmlRelation* xmlRelation) const;

    //return the key if it's a sub-node to otherwise return the empty string
    QString CheckIsSubNodeFrom(const QDomNode& nodeToCheck, XmlRelation* xmlRelation) const;

    //return the identifier number if exists the given node to or an empty string
    QString SearchGivenNodeTo(QDomNode nodeToCheck, const QString& key, XmlRelation* xmlRelation,
                              const QString &parentNumberBase, int number) const;

    //return true if exists the given node from
    bool SearchGivenNodeFrom(QDomNode nodeToCheck, const QString& key, XmlRelation* xmlRelation) const;

};

#endif // XMLRELATIONCHECKERCOREIMPL_H
