/**
 * File: DialogXmlRelationManagementBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/21
*/

#include "DialogXmlRelationManagementBase.h"

DialogXmlRelationManagementBase::DialogXmlRelationManagementBase(XmlRelationCollection* xmlRelationCollection, QWidget *parent) :
    m_pXmlRelationCollection(xmlRelationCollection), QDialog(parent) {}

DialogXmlRelationManagementBase::~DialogXmlRelationManagementBase() {}
