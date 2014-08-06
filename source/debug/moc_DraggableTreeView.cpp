/****************************************************************************
** Meta object code from reading C++ file 'DraggableTreeView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../View/DraggableTreeView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DraggableTreeView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DraggableTreeView_t {
    QByteArrayData data[24];
    char stringdata[360];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DraggableTreeView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DraggableTreeView_t qt_meta_stringdata_DraggableTreeView = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 28),
QT_MOC_LITERAL(2, 47, 0),
QT_MOC_LITERAL(3, 48, 16),
QT_MOC_LITERAL(4, 65, 16),
QT_MOC_LITERAL(5, 82, 22),
QT_MOC_LITERAL(6, 105, 5),
QT_MOC_LITERAL(7, 111, 25),
QT_MOC_LITERAL(8, 137, 12),
QT_MOC_LITERAL(9, 150, 7),
QT_MOC_LITERAL(10, 158, 14),
QT_MOC_LITERAL(11, 173, 15),
QT_MOC_LITERAL(12, 189, 20),
QT_MOC_LITERAL(13, 210, 15),
QT_MOC_LITERAL(14, 226, 16),
QT_MOC_LITERAL(15, 243, 21),
QT_MOC_LITERAL(16, 265, 27),
QT_MOC_LITERAL(17, 293, 14),
QT_MOC_LITERAL(18, 308, 5),
QT_MOC_LITERAL(19, 314, 15),
QT_MOC_LITERAL(20, 330, 12),
QT_MOC_LITERAL(21, 343, 6),
QT_MOC_LITERAL(22, 350, 5),
QT_MOC_LITERAL(23, 356, 3)
    },
    "DraggableTreeView\0ShowAndSelectedItemRequested\0"
    "\0identifierNumber\0absoluteFileName\0"
    "OnContextMenuRequested\0point\0"
    "FollowRelationActSelected\0XmlTreeItem*\0"
    "itemXml\0NewActSelected\0EditActSelected\0"
    "DuplicateActSelected\0CopyActSelected\0"
    "PasteActSelected\0DeleteThisActSelected\0"
    "DeleteAllSubNodeActSelected\0OnItemExpanded\0"
    "index\0OnItemCollapsed\0rowsInserted\0"
    "parent\0start\0end"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DraggableTreeView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   84,    2, 0x08 /* Private */,
       7,    1,   87,    2, 0x08 /* Private */,
      10,    1,   90,    2, 0x08 /* Private */,
      11,    1,   93,    2, 0x08 /* Private */,
      12,    1,   96,    2, 0x08 /* Private */,
      13,    1,   99,    2, 0x08 /* Private */,
      14,    1,  102,    2, 0x08 /* Private */,
      15,    1,  105,    2, 0x08 /* Private */,
      16,    1,  108,    2, 0x08 /* Private */,
      17,    1,  111,    2, 0x08 /* Private */,
      19,    1,  114,    2, 0x08 /* Private */,
      20,    3,  117,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QModelIndex,   18,
    QMetaType::Void, QMetaType::QModelIndex,   18,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::Int, QMetaType::Int,   21,   22,   23,

       0        // eod
};

void DraggableTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DraggableTreeView *_t = static_cast<DraggableTreeView *>(_o);
        switch (_id) {
        case 0: _t->ShowAndSelectedItemRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->OnContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->FollowRelationActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 3: _t->NewActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 4: _t->EditActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 5: _t->DuplicateActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 6: _t->CopyActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 7: _t->PasteActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 8: _t->DeleteThisActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 9: _t->DeleteAllSubNodeActSelected((*reinterpret_cast< XmlTreeItem*(*)>(_a[1]))); break;
        case 10: _t->OnItemExpanded((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 11: _t->OnItemCollapsed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 12: _t->rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DraggableTreeView::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DraggableTreeView::ShowAndSelectedItemRequested)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DraggableTreeView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_DraggableTreeView.data,
      qt_meta_data_DraggableTreeView,  qt_static_metacall, 0, 0}
};


const QMetaObject *DraggableTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DraggableTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DraggableTreeView.stringdata))
        return static_cast<void*>(const_cast< DraggableTreeView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int DraggableTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void DraggableTreeView::ShowAndSelectedItemRequested(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
