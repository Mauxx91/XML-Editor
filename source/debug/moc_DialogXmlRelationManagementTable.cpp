/****************************************************************************
** Meta object code from reading C++ file 'DialogXmlRelationManagementTable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../View/DialogXmlRelationManagementTable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DialogXmlRelationManagementTable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DialogXmlRelationManagementTable_t {
    QByteArrayData data[8];
    char stringdata[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DialogXmlRelationManagementTable_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DialogXmlRelationManagementTable_t qt_meta_stringdata_DialogXmlRelationManagementTable = {
    {
QT_MOC_LITERAL(0, 0, 32),
QT_MOC_LITERAL(1, 33, 18),
QT_MOC_LITERAL(2, 52, 0),
QT_MOC_LITERAL(3, 53, 15),
QT_MOC_LITERAL(4, 69, 17),
QT_MOC_LITERAL(5, 87, 8),
QT_MOC_LITERAL(6, 96, 23),
QT_MOC_LITERAL(7, 120, 5)
    },
    "DialogXmlRelationManagementTable\0"
    "AddRelationPressed\0\0OkButtonPressed\0"
    "CellDoubleClicked\0indexRow\0"
    "ShowContextualMenuTable\0point"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DialogXmlRelationManagementTable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    2,   36,    2, 0x08 /* Private */,
       6,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    2,
    QMetaType::Void, QMetaType::QPoint,    7,

       0        // eod
};

void DialogXmlRelationManagementTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DialogXmlRelationManagementTable *_t = static_cast<DialogXmlRelationManagementTable *>(_o);
        switch (_id) {
        case 0: _t->AddRelationPressed(); break;
        case 1: _t->OkButtonPressed(); break;
        case 2: _t->CellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->ShowContextualMenuTable((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DialogXmlRelationManagementTable::staticMetaObject = {
    { &DialogXmlRelationManagementBase::staticMetaObject, qt_meta_stringdata_DialogXmlRelationManagementTable.data,
      qt_meta_data_DialogXmlRelationManagementTable,  qt_static_metacall, 0, 0}
};


const QMetaObject *DialogXmlRelationManagementTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DialogXmlRelationManagementTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogXmlRelationManagementTable.stringdata))
        return static_cast<void*>(const_cast< DialogXmlRelationManagementTable*>(this));
    return DialogXmlRelationManagementBase::qt_metacast(_clname);
}

int DialogXmlRelationManagementTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DialogXmlRelationManagementBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
