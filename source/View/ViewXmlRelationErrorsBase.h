/**
 * File: ViewXmlRelationErrorsBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/23
*/

#ifndef VIEWXMLRELATIONERRORSBASE_H
#define VIEWXMLRELATIONERRORSBASE_H

#include "../Data/XmlRelationError.h"

#include <QList>

/**
 * @brief The ViewXmlRelationErrorsBase abstract base class that represent a Window where diplay the result of a relation check
 */
class ViewXmlRelationErrorsBase
{
protected:
    /**
     * @brief m_pXmlRelationErrorList: error list to display
     */
    QList<XmlRelationError*>* m_pXmlRelationErrorList;

public:

    /**
     * @brief DialoAssociatedFilesBase constructor
     */
    ViewXmlRelationErrorsBase(QList<XmlRelationError*>* xmlRelationErrorList);

    /**
     * @brief ~ViewXmlRelationErrorsBase vitual destructor
     */
    virtual ~ViewXmlRelationErrorsBase();

    /**
     * @brief SetNewXmlRelationErrorList: set a new error list and update the view
     * @param xmlRelationErrorList: the new error collection to display
     */
    virtual void SetNewXmlRelationErrorList(QList<XmlRelationError*>* xmlRelationErrorList) = 0;

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;
};

#endif // VIEWXMLRELATIONERRORSBASE_H
