/**
 * File: QtXmlRelationCheckerCore.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#include "XmlRelationCheckerCoreImpl.h"

#include "../Data/XmlRelationError.h"
#include "../Data/XmlRelationCollection.h"

#include <QDomDocument>

#include <climits>

XmlRelationCheckerCoreImpl::XmlRelationCheckerCoreImpl(QList<XmlDocument *>* xmlDocumentsList,
                                                       XmlRelationCollection* xmlRelationCollection):
    XmlRelationCheckerCoreBase(xmlDocumentsList, xmlRelationCollection)
{}

QList<XmlRelationError*>* XmlRelationCheckerCoreImpl::CheckAllRelationInAllXmlDocuments() const
{
    QList<XmlRelationError*>* errorCollection = new QList<XmlRelationError*>;

    //Flow the xml collection
    for(int i=0; i < m_xmlDocumentsList->size(); ++i)
    {
        QDomNode rootNode = m_xmlDocumentsList->at(i)->GetQDomDocument()->firstChildElement();

        CheckAllRelation(rootNode, errorCollection, m_xmlDocumentsList->at(i)->GetAbsoluteFileName(), "", 1);
    }

    return errorCollection;
}

QList<XmlRelation*> XmlRelationCheckerCoreImpl::CheckARelationPathExistence(const QString& tagFromName, const QString& tagToName) const
{
    //Initialize the path
    QList<XmlRelation*> path;

    //The relation destination. If it's different from zero than a relation that end to te tagTo was found
    XmlRelation* xmlDestinationRelation = 0;

    //For every relation check if there's a relation that match the destination tag
    for(int i=0; i < m_pXmlRelationCollection->Size() && !xmlDestinationRelation; ++i)
    {
        //It's the right relation
        if(tagToName == m_pXmlRelationCollection->GetRelationAt(i)->GetTagToName())
        {
            xmlDestinationRelation = m_pXmlRelationCollection->GetRelationAt(i);
        }
    }

    //If the is a destination of a relation, search if a path to the tagFromName exist
    if(xmlDestinationRelation)
    {
        path = SearchStartRelation(tagFromName, xmlDestinationRelation, 0);
    }

    return path;
}

QList<XmlRelation*>  XmlRelationCheckerCoreImpl::SearchStartRelation(const QString& tagFromNameTarget, XmlRelation* currentRelation,
                                                                     int currentHop) const
{
    //Initialize the path
    QList<XmlRelation*> path;

    //It's the right relation
    if(tagFromNameTarget == currentRelation->GetTagFromName())
    {
        //Append the current relation in the path and return
        path.append(currentRelation);
        return path;
    }

    QString currentTagFromName = currentRelation->GetTagFromName();
    QString currentTagToName = currentRelation->GetTagToName();

    //Anti-graph cycle condition
    if(currentHop < m_pXmlRelationCollection->Size())
    {
        QList<XmlRelation*> bestPath;
        int bestPathSize = INT_MAX;

        //For every relation check with the recursive call, while no one relation is return a non-empty valid path
        for(int i=0; i < m_pXmlRelationCollection->Size() && path.size() == 0; ++i)
        {
            XmlRelation* cycleRelation = m_pXmlRelationCollection->GetRelationAt(i);

            //For debug
            //QString cycleTagFromName = cycleRelation->GetTagFromName();
            //QString cycleTagToName = cycleRelation->GetTagToName();

            //If exist a walk from the cycle relation to the current relation
            if(cycleRelation->GetTagToName() == currentRelation->GetTagFromName())
            {
                QList<XmlRelation*> potentialPath = SearchStartRelation(tagFromNameTarget, cycleRelation, currentHop+1);

                //If the start relation was found and it's the best path (less long)
                if(potentialPath.size() > 0 && potentialPath.size() <= bestPathSize)
                {
                    bestPath = potentialPath;
                    bestPathSize = potentialPath.size();
                }
            }
        }

        if(bestPath.size() > 0)
        {
            path += bestPath;
            path += currentRelation;
        }
    }

    return path;
}

int XmlRelationCheckerCoreImpl::GetIdentifierAndWhereOfFistNodeTo(QString* o_pIdentifierNumber, QString* o_pAbsoluteFileName,
                                                                  bool& o_isASubNodeFromOfARelation, const QDomNode& candidateSubNodeFrom) const
{
    int numDestination = 0;
    QString key = "";
    o_isASubNodeFromOfARelation = false;
    XmlRelation* xmlRelation = 0;

    //Check if the node is a correct start point of a relation
    for(int i=0; i < m_pXmlRelationCollection->Size() && !o_isASubNodeFromOfARelation; ++i)
    {
        key = CheckIsSubNodeFrom(candidateSubNodeFrom, m_pXmlRelationCollection->GetRelationAt(i));

        if( ! key.isEmpty())
        {
            //Set the output for the parameter o_isASubNodeFromOfARelation
            o_isASubNodeFromOfARelation = true;

            //Set the found relation
            xmlRelation = m_pXmlRelationCollection->GetRelationAt(i);
        }
    }

    if(o_isASubNodeFromOfARelation)
    {
        bool fistDestinationFound = false;

        //If the node is a correct start point search for the reference and count them all
        //For each document opened
        for(int i=0; i < m_xmlDocumentsList->size(); ++i)
        {
            QDomNode rootNode = m_xmlDocumentsList->at(i)->GetQDomDocument()->firstChildElement();

            //If the current node is a destination
            QString destinationIdentifierNumber = SearchGivenNodeTo(rootNode, key, xmlRelation, "", 1);
            if( ! destinationIdentifierNumber.isEmpty() )
            {
                numDestination++;

                //If the firt destination was not found set the output
                if( ! fistDestinationFound)
                {
                    fistDestinationFound = true;

                    *o_pAbsoluteFileName = m_xmlDocumentsList->at(i)->GetAbsoluteFileName();
                    *o_pIdentifierNumber = destinationIdentifierNumber;
                }
            }
        }
    }

    return numDestination;
}

void XmlRelationCheckerCoreImpl::CheckAllRelation(QDomNode nodeToCheck, QList<XmlRelationError*>* errorCollection,
                                                  const QString& absoluteFileName, const QString& parentNumberBase, int number) const
{
    if(!nodeToCheck.isNull())
    {
        if(nodeToCheck.isElement() && !nodeToCheck.isComment())
        {

            //Calcolate the hierarchical number
            QString completeIdentifiersNumber = parentNumberBase;
            //If it's not empty is because it's the first node and it needn't the dot seprator
            if(!parentNumberBase.isEmpty())
            {
                completeIdentifiersNumber.append('.');
            }
            //Attach the number
            completeIdentifiersNumber.append(QString::number(number));

            //For debug
            //const QString& nodeName = nodeToCheck.nodeName();

            //For every xml relations
            for(int i=0; i < m_pXmlRelationCollection->Size(); ++i)
            {
                //Current analized xml relation
                XmlRelation* xmlRelation = new XmlRelation(*m_pXmlRelationCollection->GetRelationAt(i));

                //If the relation is a SUB_TAG relation
                if(xmlRelation->GetRelationType() == SUB_TAG)
                {

                    //It would be better if it's mantained a list of the finded tag to in order to avoid the research of
                    //the tag from of this tag
                    //It's not inplemented because even without thid optimization it's fast enough

                    //If the current tag is a node from for the current relation then check for the existence of the node to
                    //The current tag is a nofe from is the list of the key contained is at least 1
                    QList<QString>* keyFound = CheckIsNodeFrom(nodeToCheck, xmlRelation);
                    if(keyFound->size() > 0)
                    {
                        //For each key
                        for(int k=0; k < keyFound->size(); ++k)
                        {
                            bool referenceFound = false;

                            //Search in all the files the referenced node to
                            for(int j=0; j < m_xmlDocumentsList->size() && !referenceFound; ++j)
                            {
                                const QDomNode& rootNode = m_xmlDocumentsList->at(j)->GetQDomDocument()->firstChildElement();

                                //Search if this reference is valid
                                referenceFound = referenceFound || ! SearchGivenNodeTo(rootNode, keyFound->at(k), xmlRelation, "", 1).isEmpty();
                            }

                            //If the reference not exist add an error
                            if(!referenceFound)
                            {
                                XmlRelationError* xmlRelationError = new XmlRelationError(absoluteFileName, xmlRelation, completeIdentifiersNumber,
                                                                              keyFound->at(k), XML_RELATION_ERROR_TYPE_REFERENCED_NOT_FOUND);

                                //Add the error in the list
                                errorCollection->append(xmlRelationError);
                            }
                        }
                    }
                    delete keyFound;
                    //If the current tag is a node to for the current relation then check for the existence of the node to
                    const QString& tagToKey = CheckIsNodeTo(nodeToCheck, xmlRelation);
                    if(tagToKey != "") //then is a node to
                    {
                        bool referenceFound = false;

                        //Search in all the files the referencer node from
                        for(int j=0; j < m_xmlDocumentsList->size()  && !referenceFound; ++j)
                        {
                            const QDomNode& rootNode = m_xmlDocumentsList->at(j)->GetQDomDocument()->firstChildElement();

                            //Search if this reference is valid
                            referenceFound = referenceFound || SearchGivenNodeFrom(rootNode, tagToKey, xmlRelation);
                        }

                        //If the reference not exist add an error
                        if(!referenceFound)
                        {
                            XmlRelationError* xmlRelationError = new XmlRelationError(absoluteFileName, xmlRelation, completeIdentifiersNumber,
                                                                          tagToKey, XML_RELATION_ERROR_TYPE_REFERENCED_NEVER_REFERENCED);

                            //Add the error in the list
                            errorCollection->append(xmlRelationError);
                        }
                    }
                }
            }
            //Recursive call that ensure to explore all the document element
            CheckAllRelation(nodeToCheck.firstChildElement(), errorCollection, absoluteFileName, completeIdentifiersNumber, 1);
        }

        //Recursive call that ensure to explore all the document element
        CheckAllRelation(nodeToCheck.nextSiblingElement(), errorCollection, absoluteFileName, parentNumberBase, number+1);
    }
}

QList<QString>* XmlRelationCheckerCoreImpl::CheckIsNodeFrom(const QDomNode& nodeToCheck, XmlRelation* xmlRelation) const
{
    //Precondition: the passed node is an element

    //For debug
    //const QString& nodeName = nodeToCheck.nodeName();

    //List of the key of the referenced value
    QList<QString>* keyFound = new QList<QString>;

    //If the relation is a SUB_TAG relation, otherwise is not supported and always return false
    if(xmlRelation->GetRelationType() == SUB_TAG)
    {
        //Check the tag name corresponds with the tag from name
        if(nodeToCheck.nodeName() == xmlRelation->GetTagFromName())
        {
            //Now it has to search all the child node with the given tag name with the right attribute

            //Obtain all the child node of this node
            QDomNodeList childNodesList = nodeToCheck.childNodes();

            //For debug
            //int childNodesListSize = childNodesList.size();

            for(int i=0; i<childNodesList.size(); ++i)
            {
                //The examinated node
                const QDomNode& subNode = childNodesList.at(i);

                //Controll only the element node
                if(subNode.isElement())
                {
                    //For debug
                    //const QString& subNodeName = subNode.nodeName();

                    //If it's the right sub-node
                    if(subNode.nodeName() == xmlRelation->GetSubTagNameOfTagFrom())
                    {
                        //Check the presence of the right attribute

                        const QDomNamedNodeMap& attributeNodesList = subNode.attributes();

                        //For exit when the fist match is found
                        bool attributeFound = false;

                        //Flow the list searching the
                        for(int i=0; i<attributeNodesList.size() && !attributeFound; ++i)
                        {
                            const QDomAttr& attritubeNode = attributeNodesList.item(i).toAttr();
                            const QString& attributeName = attritubeNode.name();

                            //If the attribute match set to display the value
                            if( attributeName == xmlRelation->GetAttributeNameofTagFrom() )
                            {
                                //Attribute found exit the cycle
                                attributeFound = true;

                                //Add the key in the list
                                keyFound->append(attritubeNode.value());
                            }
                        }
                    }
                }
            }
        }
    }
    return keyFound;
}

QString XmlRelationCheckerCoreImpl::CheckIsNodeTo(const QDomNode& nodeToCheck, XmlRelation* xmlRelation) const
{
    //Precondition: the passed node is an element

    QString key = "";

    //For debug
    //const QString& nodeName = nodeToCheck.nodeName();

    //If the relation is a SUB_TAG relation, otherwise is not supported and always return false
    if(xmlRelation->GetRelationType() == SUB_TAG)
    {
        //Check the tag name corresponds with the tag to name
        if(nodeToCheck.nodeName() == xmlRelation->GetTagToName())
        {
            //Check the presence of the right attribute

            const QDomNamedNodeMap& attributeNodesList = nodeToCheck.attributes();

            //For exit when the fist match is found
            bool attributeFound = false;

            //Flow the list searching the
            for(int i=0; i<attributeNodesList.size() && !attributeFound; ++i)
            {
                const QDomAttr& attritubeNode = attributeNodesList.item(i).toAttr();
                const QString& attributeName = attritubeNode.name();

                //If the attribute match set to display the value
                if( attributeName == xmlRelation->GetAttributeNameOfTagTo() )
                {
                    //Attribute found exit the cycle
                    attributeFound = true;

                    //Set the key value with the attribute value
                    key = attritubeNode.value();
                }
            }          
        } 
    }

    //Return the key
    return key;
}

QString XmlRelationCheckerCoreImpl::CheckIsSubNodeFrom(const QDomNode& nodeToCheck, XmlRelation* xmlRelation) const
{
    //Precondition: the passed node is an element

    //For debug
    //const QString& nodeName = nodeToCheck.nodeName();

    //List of the key of the referenced value
    QString key = "";

    //If the relation is a SUB_TAG relation, otherwise is not supported and always return false
    if(xmlRelation->GetRelationType() == SUB_TAG)
    {
        //For debug
        //QString subTagName = xmlRelation->GetSubTagNameOfTagFrom();

        //Check the tag name corresponds with the subtag from name
        if(nodeToCheck.nodeName() == xmlRelation->GetSubTagNameOfTagFrom())
        {
            //Now it has to search all the child node with the given tag name with the right attribute

            //Obtain the parent node
            const QDomNode& parentNode = nodeToCheck.parentNode();

            //If the node is the correct child of the right node from
            if(parentNode.isElement() && parentNode.nodeName() == xmlRelation->GetTagFromName())
            {
                //Check the presence of the right attribute

                const QDomNamedNodeMap& attributeNodesList = nodeToCheck.attributes();

                //For exit when the fist match is found
                bool attributeFound = false;

                //Flow the list searching the
                for(int i=0; i<attributeNodesList.size() && !attributeFound; ++i)
                {
                    const QDomAttr& attritubeNode = attributeNodesList.item(i).toAttr();
                    const QString& attributeName = attritubeNode.name();

                    //If the attribute match set to display the value
                    if( attributeName == xmlRelation->GetAttributeNameofTagFrom() )
                    {
                        //Attribute found exit the cycle
                        attributeFound = true;

                        //Add the key in the list
                        key = attritubeNode.value();
                    }
                }
            }
        }
    }

    return key;
}

QString XmlRelationCheckerCoreImpl::SearchGivenNodeTo(QDomNode nodeToCheck, const QString& key, XmlRelation* xmlRelation,
                                                   const QString& parentNumberBase, int number) const
{
    if(!nodeToCheck.isNull())
    {
        if(nodeToCheck.isElement() && !nodeToCheck.isComment())
        {
            //Initialize the identifiersNumber string with the base number
            QString identifierNumber = parentNumberBase;

            //If it's not empty is because it's the first node and it needn't the dot seprator
            if(!parentNumberBase.isEmpty())
            {
                identifierNumber.append('.');
            }

            //Attach the number
            identifierNumber.append(QString::number(number));

            //Controll if the node is a node to key
            const QString& nodeToKey = CheckIsNodeTo(nodeToCheck, xmlRelation);

            //Check this is the wanted node to
            if(key.compare(nodeToKey, Qt::CaseInsensitive) == 0)
            {
                return identifierNumber;
            }
            //Check the son
            else
            {
                const QString& sonIdentifier = SearchGivenNodeTo(nodeToCheck.firstChildElement(), key, xmlRelation, identifierNumber, 1);

                //If the son is the node searched
                if( ! sonIdentifier.isEmpty())
                {
                    //return the child identifier number
                    return sonIdentifier;
                }
            }
        }
        //Recursive call on the sibling
        return SearchGivenNodeTo(nodeToCheck.nextSiblingElement(), key, xmlRelation, parentNumberBase, number+1);
    }
    return "";
}

bool XmlRelationCheckerCoreImpl::SearchGivenNodeFrom(QDomNode nodeToCheck, const QString& key, XmlRelation* xmlRelation) const
{
    if(!nodeToCheck.isNull())
    {
        if(nodeToCheck.isElement() && !nodeToCheck.isComment())
        {
            //For debug
            //const QString& nodeName = nodeToCheck.nodeName();

            //Controll if the node is a node to key
            QList<QString>* keyList = CheckIsNodeFrom(nodeToCheck, xmlRelation);

            bool found = false;

            for(int i=0; i < keyList->size() && !found; ++i)
            {
                if(key.compare(keyList->at(i), Qt::CaseInsensitive) == 0)
                {
                    found = true;
                }
            }

            delete keyList;

            //Check this is the wanted node to
            if(found || SearchGivenNodeFrom(nodeToCheck.firstChildElement(), key, xmlRelation))
            {
                return true;
            }
        }
        //Recursive call
        return SearchGivenNodeFrom(nodeToCheck.nextSiblingElement(), key, xmlRelation);
    }
    return false;
}
