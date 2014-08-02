/**
 * File: ViewXmlRelationErrorsBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#include "ViewXmlRelationErrorsBase.h"

#include <QSettings>

ViewXmlRelationErrorsBase::ViewXmlRelationErrorsBase(QList<XmlRelationError*>* xmlRelationErrorList)
{
    m_pXmlRelationErrorList = xmlRelationErrorList;
}

ViewXmlRelationErrorsBase::~ViewXmlRelationErrorsBase()
{
   if(m_pXmlRelationErrorList)
   {
        delete m_pXmlRelationErrorList;
   }
}

//m_pSettings->setValue(m_keySeriousnessLevelFilter, QString::number(static_cast<int>(newSeriousmessFilter)));
