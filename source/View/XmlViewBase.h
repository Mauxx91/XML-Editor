/**
 * File: XmlViewBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef IXMLVIEW_H
#define IXMLVIEW_H

#include <QWidget>

class QAbstractItemModel;
class XmlRelationCheckerCoreBase;
class XmlEditCommandInvoker;

/**
 * @brief The IXmlView interface for a QWidget that show an Xml
 */
class XmlViewBase : public QWidget
{
private:
    /**
     * @brief m_pRelationCheckerCore: used to control the relation
     */
    XmlRelationCheckerCoreBase* m_pRelationCheckerCore;

    /**
     * @brief m_pXmlEditCommandInvoker: use to invoke command
     */
    XmlEditCommandInvoker* m_pXmlEditCommandInvoker;

public:
    /**
     * @brief XmlViewBase constructor that set the parent widget
     * @param relationCheckerCore: for use the relations
     * @param xmlEditCommandInvoker: for invoke command
     * @param parent widget of this widget
     */
    XmlViewBase(XmlRelationCheckerCoreBase* relationCheckerCore, XmlEditCommandInvoker* xmlEditCommandInvoker, QWidget *parent = 0);

    /**
     * @brief XmlViewBase virtual destructor
     */
    ~XmlViewBase();

    /**
     * @brief setModel of an xml to show
     * @param model that is wanted to show
     */
    virtual void SetModel(QAbstractItemModel * model) = 0;

    /**
     * @brief EspandToDepth: spand the tree to the given depth
     * @param depth: depth to expand
     */
    virtual void EspandToDepth(int depth) = 0;

    /**
     * @brief show: show the widget
     */
    virtual void Show() = 0;

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide() = 0;

    /**
     * @brief ShowAndSelectedItem: show and select the given item
     * @param identifierNumber: the identifier number if the item
     */
    virtual void ShowAndSelectedItem(const QString& identifierNumber) = 0;

signals:
    //Request the show and the select of an item in the given file name
    void ShowAndSelectedItemRequested(const QString& identifierNumber, const QString& absoluteFileName);
};

#endif // IXMLVIEW_H
