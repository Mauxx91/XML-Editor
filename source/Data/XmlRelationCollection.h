/**
 * File: XmlRelationCollection.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#ifndef XMLRELATIONCOLLECTION_H
#define XMLRELATIONCOLLECTION_H

#include "XmlRelation.h"
#include "XmlDocument.h"

#include "../Observer/Subject.h"
#include "../Observer/IObserver.h"

#include <QList>

class QDomNode;

/**
 * @brief The XmlRelationCollection class that represent a colletion of XmlRelation
 */
class XmlRelationCollection : public Subject, public IObserver
{
private:
    //Constant name of the config file tags
    const QString m_rootTagName;
    const QString m_relationCollectionTagName;
    const QString m_xmlRelationTagName;

    /**
     * @brief m_autoSave: determine if auto save the configuration file every modification of the relations
     */
    bool m_autoSave;

    /**
     * @brief updateEnabled: used for prevent the save of an empty file during the loading
     */
    bool m_updateEnabled;

    /**
     * @brief configurationRelativeFileName: file name of the standard configuration file
     */
    const QString m_relativeFileNameStandardConfFile;

    /**
     * @brief m_abosulteFileNameStandardConfFile: absolute file name of the standard conf file
     */
    QString m_abosulteFileNameStandardConfFile;

    /**
     * @brief m_xmlRelationList: list of the xml relations
     */
    QList<XmlRelation*> m_xmlRelationList;

    /**
     * @brief m_pXmlDocument used for the persistence of the collection
     */
    XmlDocument* m_pXmlDocument;

public:
    /**
     * @brief XmlRelationCollection: construct a empty collection
     */
    XmlRelationCollection();

    /**
     * @brief XmlRelationCollection: destruct the content of the collection
     */
    ~XmlRelationCollection();

private:
    /**
     * @brief ClearAndLoadXmlCollectionFromXmlDocumentOpened: Clear the current xml relation collection and
     *                                                        refill with the content of the xml document
     */
    void ClearAndLoadXmlCollectionFromXmlDocumentOpened();

    /**
     * @brief UpdateXmlDocumentWithRelationCollection: update the state of the document with the content of the xmlCollection
     */
    void UpdateXmlDocumentWithRelationCollection();

public:
    /**
     * @brief LoadSettedFileConf: load a set of xml relation from the setted configuration file
     * @return the error code of the operation
     */
    FILE_XML_ERR_CODE LoadSettedFileConf();

    /**
     * @brief SaveToSettedFileConf: save the relation collection to the pre setted xml document
     * @return the result code of the operation
     */
    FILE_XML_ERR_CODE SaveToSettedFileConf();

    /**
     * @brief SaveToAnotherFileConf: save to an different file name keeping as settend after the operation the standard config file
     * @param absoluteFileName: the absolute file name where to save
     * @return the result code of the operation
     */
    FILE_XML_ERR_CODE SaveToAnotherFileConf(const QString& absoluteFileName);

    /**
     * @brief LoadNonStandardFileConf: load from an different file name keeping as settend after the operation the standard config file
     * @param absoluteFileName: the absolute file name where load from
     * @return the result code of the operation
     */
    FILE_XML_ERR_CODE LoadNonStandardFileConf(const QString& absoluteFileName);

    /**
     * @brief GetXmlDocument: get the XmlDocument used for the persistence of the collection
     * @return the XmlDocument used for the persistence of the collection
     */
    const XmlDocument* GetXmlDocument() const;

    /**
     * @brief AddXmlRelation: add the given relation in the collection
     * @param relation: the new relation to add in the collection
     */
    void AddXmlRelation(XmlRelation* xmlRelation);

    /**
     * @brief RemoveXmlRelation: remove the xml relation at the given index
     * @param index from which remove the xml relation
     */
    void RemoveXmlRelation(int index);

    /**
     * @brief Size: get the size of the collection
     * @return the number of the relation present in the collection
     */
    int Size() const;

    /**
     * @brief Clear: remove and delete all the xml relation in the collection
     */
    void Clear();

    /**
     * @brief GetRelationAt: get the xml relation at the given index
     * @param index of the relation: it has to be > 0 and < Size()
     * @return the xml relation at the given index
     */
    XmlRelation* GetRelationAt(int index) const;

    /**
     * @brief Update
     * @param updateCode: to know what time of update is needed
     */
    virtual void Update(UPDATE_CODE updateCode);
};

#endif // XMLRELATIONCOLLECTION_H
