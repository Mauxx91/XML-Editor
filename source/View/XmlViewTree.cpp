/**
 * File: XmlViewTree.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/17
*/

#include "XmlViewTree.h"

#include "DraggableTreeView.h"

#include "../Core/XmlRelationCheckerCoreBase.h"
#include "../Data/XmlTreeItem.h"

#include <QStandardItemModel>
#include <QVBoxLayout>

XmlViewTree::XmlViewTree(XmlRelationCheckerCoreBase* xmlRelationCheckerCore, XmlEditCommandInvoker* xmlEditCommandInvoker, QWidget *parent):
    XmlViewBase(xmlRelationCheckerCore, xmlEditCommandInvoker, parent)
{
    //Initialize the real view
    m_pTreeView = new DraggableTreeView(xmlRelationCheckerCore, xmlEditCommandInvoker);

    //Connect the treeView signal as emitter of the signal
    connect(m_pTreeView, SIGNAL(ShowAndSelectedItemRequested(QString,QString)), this, SIGNAL(ShowAndSelectedItemRequested(QString,QString)));

    //Initialize the view
    m_pLayout = new QVBoxLayout;

    //Display the tree view
    m_pLayout->addWidget(m_pTreeView);
    setLayout(m_pLayout);
}

void XmlViewTree::SetModel(QAbstractItemModel * model)
{
    m_pTreeView->setModel(model);
}

void XmlViewTree::EspandToDepth(int depth)
{
     m_pTreeView->expandToDepth(depth);
}

void XmlViewTree::Show()
{
    QWidget::show();
}

void XmlViewTree::Hide()
{
    QWidget::hide();
}

void XmlViewTree::ShowAndSelectedItem(const QString& identifierNumber)
{
    m_pTreeView->ShowAndSelectedItem(identifierNumber);
}
