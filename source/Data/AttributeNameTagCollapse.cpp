/**
 * File: AttributeNameTagCollapse.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#include "AttributeNameTagCollapse.h"

#include "../Observer/IObserver.h"

#include <QSettings>

AttributeNameTagCollapse::AttributeNameTagCollapse(): m_keyAttributeName("k_AttributeNameTag")
{
    //Initialize the settings to access the stored input of the previously session
    m_pSettings = new QSettings();

    //Initialize the members from the previosly inserted values or insert default value -> The QVariant("") if the key is not found
    m_attributeName = m_pSettings->value(m_keyAttributeName, QVariant("name")).toString();
}

AttributeNameTagCollapse::~AttributeNameTagCollapse()
{
    delete m_pSettings;
}

void AttributeNameTagCollapse::SetAttributeNameTag(const QString& attributeName)
{
    m_attributeName = attributeName;

    //Update the value in the registry
    m_pSettings->setValue(m_keyAttributeName, m_attributeName);

    //Notify all the observers
    Notify(UPDATE_ATTRIBUTE_NAME_TAG);
}

const QString &AttributeNameTagCollapse::GetAttributeNameTag() const
{
    return m_attributeName;
}
