/**
 * File: XmlRelation.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#include "XmlRelation.h"

#include "../Observer/IObserver.h"

#include <QDomDocument>

XmlRelation::XmlRelation():
    m_tagFromNameCTN("tagFromName"),
    m_attributeNameofTagFromCTN("attributeNameofTagFrom"),
    m_subTagNameOfTagFromCTN("subTagNameOfTagFrom"),
    m_tagToNameCTN("tagToName"),
    m_attributeNameOfTagToCTN("attributeNameOfTagTo"),
    m_relationTypeCTN("relationType")
{
    //Default value for the relationType
    m_relationType = SUB_TAG;
}

XmlRelation::XmlRelation(const QString& tagFromName, const QString& attributeNameofTagFrom, const QString& subTagNameOfTagFrom,
                         const QString& tagToName, const QString& attributeNameOfTagTo, XML_RELATION_TYPE type)
{
    //In order to initialize the const QString
    XmlRelation();

    SetAll(tagFromName, attributeNameofTagFrom, subTagNameOfTagFrom, tagToName, attributeNameOfTagTo, type);
}

XmlRelation::XmlRelation(const XmlRelation& xmlRelation)
{
    SetAll(xmlRelation.m_tagFromName, xmlRelation.m_attributeNameofTagFrom, xmlRelation.m_subTagNameOfTagFrom,
           xmlRelation.m_tagToName, xmlRelation.m_attributeNameOfTagTo, xmlRelation.m_relationType);
}

void XmlRelation::SetAll(const QString& tagFromName, const QString& attributeNameofTagFrom, const QString& subTagNameOfTagFrom,
                         const QString& tagToName, const QString& attributeNameOfTagTo, XML_RELATION_TYPE type)
{
    m_tagFromName = tagFromName;

    m_attributeNameofTagFrom = attributeNameofTagFrom;

    m_subTagNameOfTagFrom = subTagNameOfTagFrom;

    m_tagToName = tagToName;

    m_attributeNameOfTagTo = attributeNameOfTagTo;

    m_relationType = type;

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelation::SetParseXmlRelationNode(const QDomNode *xmlRelationNode)
{
    //Obtain all the child node of this node
    QDomNodeList childNodesList = xmlRelationNode->childNodes();

    //For each node check if it is an "xmlRelation" node and if it is call the function for insert the relation
    for(int i=0; i<childNodesList.size(); ++i)
    {
        //The examinated node
        const QDomNode& examinatedNode = childNodesList.at(i);

        //The child node that should have the text value
        const QDomNode childIfExaminatedNode = examinatedNode.firstChild();

        //If the examinated node is an element with the first child node is text
        if(examinatedNode.isElement() && childIfExaminatedNode.isText())
        {
            //Check if it is an "tagFromName" and set the value read in the xmlRelation
            if(examinatedNode.nodeName() == m_tagFromNameCTN)
            {
                const QString& nodeText = childIfExaminatedNode.nodeValue();
                this->SetTagFromName(nodeText);
            }
            //Check if it is an "attributeNameofTagFrom" and set the value read in the xmlRelation
            else if(examinatedNode.nodeName() == m_attributeNameofTagFromCTN)
            {
                const QString& nodeText = childIfExaminatedNode.nodeValue();
                this->SetAttributeNameofTagFrom(nodeText);
            }
            //Check if it is an "subTagNameOfTagFrom" and set the value read in the xmlRelation
            else if(examinatedNode.nodeName() == m_subTagNameOfTagFromCTN)
            {
                const QString& nodeText = childIfExaminatedNode.nodeValue();
                this->SetSubTagNameOfTagFrom(nodeText);
            }
            //Check if it is an "tagToName" and set the value read in the xmlRelation
            else if(examinatedNode.nodeName() == m_tagToNameCTN)
            {
                const QString& nodeText = childIfExaminatedNode.nodeValue();
                this->SetTagToName(nodeText);
            }
            //Check if it is an "attributeNameOfTagTo" and set the value read in the xmlRelation
            else if(examinatedNode.nodeName() == m_attributeNameOfTagToCTN)
            {
                const QString& nodeText = childIfExaminatedNode.nodeValue();
                this->SetAttributeNameOfTagTo(nodeText);
            }
            //Check if it is an "relationType" and set the value read in the xmlRelation
            else if(examinatedNode.nodeName() == m_relationTypeCTN)
            {
                const QString& nodeText = childIfExaminatedNode.nodeValue();
                this->SetRelationType(static_cast<XML_RELATION_TYPE>(nodeText.toInt()));
            }
        }
    }
}

void XmlRelation::FillQDomNodeWithTheRelation(QDomElement* m_poXmlRelation, QDomDocument* document)
{
    //For each element create a node and append in the m_poXmlRelation node

    //Element for tagFromName
    QDomNode tagFromNameNode = document->createElement(m_tagFromNameCTN); //Create node
    QDomNode tagFromNameNodeText = document->createTextNode(GetTagFromName()); //Create node text
    tagFromNameNode.appendChild(tagFromNameNodeText); //append the node Text in the node
    m_poXmlRelation->appendChild(tagFromNameNode); //append the node in the m_poXmlRelation node

    //Element for attributeNameofTagFrom
    QDomNode attributeNameofTagFromNode = document->createElement(m_attributeNameofTagFromCTN); //Create node
    QDomNode attributeNameofTagFromNodeText = document->createTextNode(GetAttributeNameofTagFrom()); //Create node text
    attributeNameofTagFromNode.appendChild(attributeNameofTagFromNodeText); //append the node Text in the node
    m_poXmlRelation->appendChild(attributeNameofTagFromNode); //append the node in the m_poXmlRelation node

    //Element for subTagNameOfTagFrom
    QDomNode subTagNameOfTagFromNode = document->createElement(m_subTagNameOfTagFromCTN); //Create node
    QDomNode subTagNameOfTagFromNodeText = document->createTextNode(GetSubTagNameOfTagFrom()); //Create node text
    subTagNameOfTagFromNode.appendChild(subTagNameOfTagFromNodeText); //append the node Text in the node
    m_poXmlRelation->appendChild(subTagNameOfTagFromNode); //append the node in the m_poXmlRelation node

    //Element for tagToName
    QDomNode tagToNameNode = document->createElement(m_tagToNameCTN); //Create node
    QDomNode tagToNameNodeText = document->createTextNode(GetTagToName()); //Create node text
    tagToNameNode.appendChild(tagToNameNodeText); //append the node Text in the node
    m_poXmlRelation->appendChild(tagToNameNode); //append the node in the m_poXmlRelation node

    //Element for attributeNameOfTagTo
    QDomNode attributeNameOfTagToNode = document->createElement(m_attributeNameOfTagToCTN); //Create node
    QDomNode attributeNameOfTagToNodeText = document->createTextNode(GetAttributeNameOfTagTo()); //Create node text
    attributeNameOfTagToNode.appendChild(attributeNameOfTagToNodeText); //append the node Text in the node
    m_poXmlRelation->appendChild(attributeNameOfTagToNode); //append the node in the m_poXmlRelation node

    //Element for relationType
    QDomNode relationTypeNode = document->createElement(m_relationTypeCTN); //Create node
    const QString& tagType = QString::number(static_cast<int>(GetRelationType())); //Cast from enum to QString of the internal rapr of the enum
    QDomNode relationTypeNodeText = document->createTextNode(tagType); //Create node text
    relationTypeNode.appendChild(relationTypeNodeText); //append the node Text in the node
    m_poXmlRelation->appendChild(relationTypeNode); //append the node in the m_poXmlRelation node
}

void XmlRelation::SetTagFromName(const QString& tagFromName)
{
    m_tagFromName = tagFromName;

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelation::SetAttributeNameofTagFrom(const QString& attributeNameofTagFrom)
{
    m_attributeNameofTagFrom = attributeNameofTagFrom;

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelation::SetSubTagNameOfTagFrom(const QString& subTagNameOfTagFrom)
{
    m_subTagNameOfTagFrom = subTagNameOfTagFrom;

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelation::SetTagToName(const QString& tagToName)
{
    m_tagToName = tagToName;

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelation::SetAttributeNameOfTagTo(const QString& attributeNameOfTagTo)
{
    m_attributeNameOfTagTo = attributeNameOfTagTo;

    Notify(UPDATE_XML_RELATIONS);
}

void XmlRelation::SetRelationType(XML_RELATION_TYPE type)
{
    m_relationType = type;

    Notify(UPDATE_XML_RELATIONS);
}

const QString& XmlRelation::GetTagFromName() const
{
    return m_tagFromName;
}
const QString& XmlRelation::GetAttributeNameofTagFrom() const
{
    return m_attributeNameofTagFrom;
}

const QString& XmlRelation::GetSubTagNameOfTagFrom() const
{
    return m_subTagNameOfTagFrom;
}

const QString& XmlRelation::GetTagToName() const
{
    return m_tagToName;
}

const QString& XmlRelation::GetAttributeNameOfTagTo() const
{
    return m_attributeNameOfTagTo;
}

XML_RELATION_TYPE XmlRelation::GetRelationType() const
{
    return m_relationType;
}
