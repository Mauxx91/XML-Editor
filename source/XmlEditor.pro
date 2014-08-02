#-------------------------------------------------
#
# Project created by Biancucci Maurizio 2014-07-16T16:32:29
#
#-------------------------------------------------

QT       += core \
            gui \
            xml \
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XmlEditor
TEMPLATE = app


SOURCES += main.cpp \
    View/MainWindowView.cpp \
    Observer/Subject.cpp \
    Observer/IObserver.cpp \
    Data/AttributeNameTagCollapse.cpp \
    View/DialogEditAttributeNameTagCollapseBase.cpp \
    View/DialogEditAttributeNameTagCollapse.cpp \
    Data/XmlDocument.cpp \
    Data/XmlTreeModel.cpp \
    View/XmlViewTree.cpp \
    View/XmlViewBase.cpp \
    Data/AssociatedFiles.cpp \
    View/DialogAssociatedFilesBase.cpp \
    View/DialogAssociatedFilesForm.cpp \
    Data/XmlRelation.cpp \
    Data/XmlRelationCollection.cpp \
    View/DialogXmlRelationManagementBase.cpp \
    View/DialogXmlRelationManagementTable.cpp \
    View/DialogAddModifyRelation.cpp \
    View/DialogInputFileNameBase.cpp \
    View/DialogInputFileNameForm.cpp \
    Data/XmlRelationError.cpp \
    Core/XmlRelationCheckerCoreImpl.cpp \
    Core/XmlRelationCheckerCoreBase.cpp \
    View/ViewXmlRelationErrorsBase.cpp \
    View/ViewXmlRelationErrorsTable.cpp \
    Data/XmlTreeItem.cpp \
    View/DraggableTreeView.cpp \
    Command/XmlEditCommandBase.cpp \
    Command/XmlEditCommandAddChildNode.cpp \
    Command/XmlEditCommandInvoker.cpp \
    Command/XmlEditCommandAggregator.cpp \
    View/DialogInputStringBase.cpp \
    View/DialogInputStringForm.cpp \
    Command/XmlEditCommandDeleteNode.cpp \
    Command/XmlEditCommandDeleteAllChildNode.cpp \
    Command/XmlEditCommandAddExistentChildNode.cpp \
    View/DialogEditNodeBase.cpp \
    View/DialogEditNodeTable.cpp \
    Command/XmlEditCommandAddAttribute.cpp \
    Command/XmlEditCommandEditAttribute.cpp \
    Command/XmlEditCommandRemoveAttribute.cpp \
    Command/XmlEditCommandSetNodeValue.cpp

HEADERS  += version.h\
    View/MainWindowView.h \
    Observer/Subject.h \
    Observer/IObserver.h \
    Data/AttributeNameTagCollapse.h \
    View/DialogEditAttributeNameTagCollapseBase.h \
    View/DialogEditAttributeNameTagCollapse.h \
    Data/XmlDocument.h \
    Data/XmlTreeModel.h \
    View/XmlViewTree.h \
    View/XmlViewBase.h \
    Data/AssociatedFiles.h \
    View/DialogAssociatedFilesBase.h \
    View/DialogAssociatedFilesForm.h \
    Data/XmlRelation.h \
    Data/XmlRelationCollection.h \
    View/DialogXmlRelationManagementBase.h \
    View/DialogXmlRelationManagementTable.h \
    View/DialogAddModifyRelation.h \
    View/DialogInputFileNameBase.h \
    View/DialogInputFileNameForm.h \
    Data/XmlRelationError.h \
    Core/XmlRelationCheckerCoreImpl.h \
    Core/XmlRelationCheckerCoreBase.h \
    View/ViewXmlRelationErrorsBase.h \
    View/ViewXmlRelationErrorsTable.h \
    Data/XmlTreeItem.h \
    View/DraggableTreeView.h \
    Command/XmlEditCommandBase.h \
    Command/XmlEditCommandAddChildNode.h \
    Command/XmlEditCommandInvoker.h \
    Command/XmlEditCommandAggregator.h \
    View/DialogInputStringForm.h \
    View/DialogInputStringBase.h \
    Command/XmlEditCommandDeleteNode.h \
    Command/XmlEditCommandDeleteAllChildNode.h \
    Command/XmlEditCommandAddExistentChildNode.h \
    View/DialogEditNodeBase.h \
    View/DialogEditNodeTable.h \
    Command/XmlEditCommandAddAttribute.h \
    Command/XmlEditCommandEditAttribute.h \
    Command/XmlEditCommandRemoveAttribute.h \
    Command/XmlEditCommandSetNodeValue.h

RC_FILE = resources.rc

RESOURCES += \
    icon.qrc
