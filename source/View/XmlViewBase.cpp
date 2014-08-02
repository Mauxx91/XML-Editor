/**
 * File: XmlViewBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#include "XmlViewBase.h"

XmlViewBase::XmlViewBase(XmlRelationCheckerCoreBase* relationCheckerCore, XmlEditCommandInvoker *xmlEditCommandInvoker, QWidget *parent):
    m_pRelationCheckerCore(relationCheckerCore), m_pXmlEditCommandInvoker(xmlEditCommandInvoker), QWidget(parent) {}

XmlViewBase::~XmlViewBase() {}
