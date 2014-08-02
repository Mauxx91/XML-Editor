/**
 * File: IXmlRelationCheckerCore.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#include "XmlRelationCheckerCoreBase.h"

#include "../Data/XmlDocument.h"
#include "../Data/XmlRelationCollection.h"

XmlRelationCheckerCoreBase::XmlRelationCheckerCoreBase(QList<XmlDocument *>* xmlDocumentsList, XmlRelationCollection* xmlRelationCollection):
    m_xmlDocumentsList(xmlDocumentsList), m_pXmlRelationCollection(xmlRelationCollection)
{}
