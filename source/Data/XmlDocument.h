/**
 * File: AttributeNameTagCollapse.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include "../Observer/Subject.h"

#include <QString>
#include <QDomNode>

class QDomDocument;
class QFile;

enum FILE_XML_ERR_CODE {FILE_XML_FILE_NOT_EXIST,
                        FILE_XML_FILE_IS_NOT_FILE,
                        FILE_XML_FILE_IS_NOT_READABLE,
                        FILE_XML_FILE_IS_NOT_WRITABLE,
                        FILE_XML_UNABLE_OPEN_FILE,
                        FILE_XML_UNABlE_FILL_DOM_TREE,
                        FILE_XML_NO_ERR};

class XmlDocument: public Subject
{
private:
    /**
     * @brief m_isModified: if the file is different from the version saved on the disk
     */
    bool m_isModified;

    /**
     * @brief m_pDomDocument: the dom tree that represent the xml file opened
     */
    QDomDocument* m_pDomDocument;

    /**
     * @brief m_pFile: file used to read the xml file
     */
    QFile* m_pFile;

    /**
     * @brief absoluteFileName: file name complete of abosulte path.
     *                          If it's an empty string means that it's a new Xml without any file associated
     */
    QString m_absoluteFileName;

    /**
     * @brief m_errorString: used to store the error occurred
     */
    QString m_errorString;

    /**
     * @brief m_errorLine: used to store the line in which the error is occurred
     */
    int m_errorLine;

public:
    /**
     * @brief XmlDocument constructor that initialize the dynamc object
     */
    XmlDocument();

    /**
     * @brief XmlDocument destructor that deletes the dynamc object
     */
    ~XmlDocument();

    /**
     * @brief ChangeFileName: change the name file and the file of the document
     * @param abosulteFileName: nre name file
     */
    void ChangeFileName(const QString& absoluteFileName);

    /**
     * @brief NewXmlFile: Initilize the object for a empty xml file. The new file it's created on the disk yet
     * @param absoluteFileName: file name with absolute path of the new file
     */
    void NewXmlFile(const QString& absoluteFileName);

    /**
     * @brief OpenXmlFile. Open a xml file and fill the dom document with the content
     * @param absoluteFileName: file name with absolute path of the file
     * @return the errore code of the operation
     */
    FILE_XML_ERR_CODE OpenXmlFile(const QString& absoluteFileName = "");

    /**
     * @brief SaveXmlFile: save on the disk the modifies done in the xml tree
     * @return The result code
     */
    FILE_XML_ERR_CODE SaveXmlFile();

    /**
     * @brief SaveXmlFileWithName: save the xml in another files
     * @return The result code
     */
    FILE_XML_ERR_CODE SaveXmlFileWithName(const QString& absoluteFilePath);

    /**
     * @brief GetAbosulteName: get the absolute file name of the document
     * @return the abosulte file name of the document
     */
    QString GetAbsoluteFileName() const;

    /**
     * @brief GetQDomDocument of the xml file
     * @return the QDoomDocument of the XmlFile
     */
    QDomDocument* GetQDomDocument();

    //Getter for the errors
    const QString& GetErrorString() const;
    int GetErrorLine() const;

    /**
     * @brief IsModified: getter for the ssModified member
     * @return if the file is modified
     */
    bool IsModified() const;

    //count the valid element child of the given node
    int CountChildNode(const QDomNode& parentNode) const;

    //Method for edit the xmlDocument

    //return the identifier of the new inserted node
    QString AddChildNode(const QString& parentIdentifier, const QString& nodeType);

    //return the identifier of the new inserted node
    QString AddChildNode(const QString& parentIdentifier, const QDomNode& newNode);

    //return a copy of the deleted node
    QDomNode DeleteNode(const QString& identifierNumber);

    //tag name is a filter for deleted child nodes. Return the eliminated nodes
    QList<QDomNode> DeleteAllChildNode(const QString& parentIdentifier, const QString& tagName);

    //add a new attribute to the given identifier, or set if the attribute if already present, return the precedentValue
    QString AddOrSetAttribute(const QString& identifierNumber, const QString& attrName, const QString& value);

    //Remove the attribute attrName from the given numer, return the value of the removed attribute
    QString removeAttribute(const QString& identifierNumber, const QString& attrName);

    //Set the node value and return the previous value
    QString SetNodeValue(const QString& identifierNumber, const QString& value);

    //Get all the key of the first level child node of the given node
    QStringList GetAllKeyOfFistLevelChild(const QDomNode& node, const QString& tagName, const QString& attrName) const;

    //return the fist empty key from the given base key. It search through the all the given tag name with the key in the given attrName
    QString GetFirtEmptyKey(const QString& baseKey, const QString& tagName, const QString& attrName);

    //return all the key of the given tag from a base node
    QStringList GetAllKeyOfTag(const QDomNode& node, const QString& tagName, const QString& attrName) const;

    //return the key from the given identifier
    QString GetKeyFromIdentifierNumber(const QString& identifierNumber, const QString& attributeName) const;

    //return a node from the given identifier
    QDomNode GetDomNode(const QString& identifier) const;

private:
    //Perform the real serch of the node
    QDomNode* RealGetDomNode(QDomNode* nodeToCheck, const QString& idetifierNumberToSearch, const QString& parentNumberBase, int number) const;
};

#endif // XMLDOCUMENT_H
