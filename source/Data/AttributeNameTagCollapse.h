/**
 * File: AttributeNameTagCollapse.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef ATTRIBUTENAMETAGCOLLAPSE_H
#define ATTRIBUTENAMETAGCOLLAPSE_H

#include "../Observer/Subject.h"

#include <QString>

class QSettings;

/**
 * @brief The AttributeNameTagCollapse class used for storage the attribute tag name filter.
 * It'a Subject and store this string in the registry  value.
 */
class AttributeNameTagCollapse : public Subject
{
private:
    /**
     * @brief m_attributeName: the filter attribute name with collpase the same tag name and show the content of this attribute
     */
    QString m_attributeName;

    /**
     * @brief m_pSettings: object for load and writes datas in the system registry
     */
    QSettings* m_pSettings;

    /**
     * @brief m_keyAttributeName: key used for access the value in the registry for the m_attributeName
     */
    const QString m_keyAttributeName;

public:
    /**
     * @brief AttributeNameTagCollapse constructor for initialize class members by load from the system registry
     */
    AttributeNameTagCollapse();

    /**
     * @brief AttributeNameTagCollapse destructor that delete the dynamic object used
     */
    ~AttributeNameTagCollapse();

    /**
     * @brief SetAttributeNameTag: setter for m_attributeName
     * @param attributeName: new attribute name tag collapse
     */
    void SetAttributeNameTag(const QString& attributeName);

    /**
     * @brief GetAttributeNameTag: getter for the m_attributeName
     */
    const QString& GetAttributeNameTag() const;


};

#endif // ATTRIBUTENAMETAGCOLLAPSE_H
