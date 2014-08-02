/**
 * File: XmlViewTree.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#ifndef XMLVIEWTREE_H
#define XMLVIEWTREE_H

#include "XmlViewBase.h"

class QTreeView;
class QVBoxLayout;
class DraggableTreeView;

class XmlEditCommandInvoker;

/**
 * @brief The XmlViewTree class implements IXmlView as a tree
 */
class XmlViewTree : public XmlViewBase
{
    Q_OBJECT

private:

    /**
     * @brief m_pTreeView for show the xml as a tree
     */
    DraggableTreeView* m_pTreeView;

    /**
     * @brief m_pLayout used to show another widget
     */
    QVBoxLayout* m_pLayout;

public:
    /**
     * @brief XmlViewTree that set up the widget
     * @param parent widget
     */
    XmlViewTree(XmlRelationCheckerCoreBase* xmlRelationCheckerCore,
                XmlEditCommandInvoker* xmlEditCommandInvoker, QWidget *parent = 0);

signals:
    //Request the show and the select of an item in the given file name
    void ShowAndSelectedItemRequested(const QString& identifierNumber, const QString& absoluteFileName);

public:

    /**
     * @brief SetModel that will be shown
     * @param model wanted to be shown
     */
    virtual void SetModel(QAbstractItemModel * model);

    /**
     * @brief EspandToDepth: spand the tree to the given depth
     * @param depth: depth to expand
     */
    virtual void EspandToDepth(int depth);

    /**
     * @brief show: show the widget
     */
    virtual void Show();

    /**
     * @brief hide: hide the widget
     */
    virtual void Hide();

    /**
     * @brief ShowAndSelectedItem: show and select the given item
     * @param identifierNumber: the identifier number if the item
     */
    virtual void ShowAndSelectedItem(const QString& identifierNumber);
};

#endif // XMLVIEWTREE_H
