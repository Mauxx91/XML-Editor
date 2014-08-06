/****************************************************************************
** Meta object code from reading C++ file 'DialogAssociatedFilesForm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../View/DialogAssociatedFilesForm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DialogAssociatedFilesForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DialogAssociatedFilesForm_t {
    QByteArrayData data[7];
    char stringdata[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DialogAssociatedFilesForm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DialogAssociatedFilesForm_t qt_meta_stringdata_DialogAssociatedFilesForm = {
    {
QT_MOC_LITERAL(0, 0, 25),
QT_MOC_LITERAL(1, 26, 20),
QT_MOC_LITERAL(2, 47, 0),
QT_MOC_LITERAL(3, 48, 16),
QT_MOC_LITERAL(4, 65, 19),
QT_MOC_LITERAL(5, 85, 19),
QT_MOC_LITERAL(6, 105, 18)
    },
    "DialogAssociatedFilesForm\0"
    "AddNewAssociatedFile\0\0absoluteFileName\0"
    "BrowseButtonPressed\0DeleteButtonPressed\0"
    "ApplyButtonPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DialogAssociatedFilesForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       1,    0,   42,    2, 0x28 /* Private | MethodCloned */,
       4,    0,   43,    2, 0x08 /* Private */,
       5,    0,   44,    2, 0x08 /* Private */,
       6,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DialogAssociatedFilesForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DialogAssociatedFilesForm *_t = static_cast<DialogAssociatedFilesForm *>(_o);
        switch (_id) {
        case 0: _t->AddNewAssociatedFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->AddNewAssociatedFile(); break;
        case 2: _t->BrowseButtonPressed(); break;
        case 3: _t->DeleteButtonPressed(); break;
        case 4: _t->ApplyButtonPressed(); break;
        default: ;
        }
    }
}

const QMetaObject DialogAssociatedFilesForm::staticMetaObject = {
    { &DialogAssociatedFilesBase::staticMetaObject, qt_meta_stringdata_DialogAssociatedFilesForm.data,
      qt_meta_data_DialogAssociatedFilesForm,  qt_static_metacall, 0, 0}
};


const QMetaObject *DialogAssociatedFilesForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DialogAssociatedFilesForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogAssociatedFilesForm.stringdata))
        return static_cast<void*>(const_cast< DialogAssociatedFilesForm*>(this));
    if (!strcmp(_clname, "IObserver"))
        return static_cast< IObserver*>(const_cast< DialogAssociatedFilesForm*>(this));
    return DialogAssociatedFilesBase::qt_metacast(_clname);
}

int DialogAssociatedFilesForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DialogAssociatedFilesBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
