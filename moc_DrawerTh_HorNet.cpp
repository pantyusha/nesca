/****************************************************************************
** Meta object code from reading C++ file 'DrawerTh_HorNet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DrawerTh_HorNet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DrawerTh_HorNet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DrawerTh_HorNet_t {
    QByteArrayData data[5];
    char stringdata[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DrawerTh_HorNet_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DrawerTh_HorNet_t qt_meta_stringdata_DrawerTh_HorNet = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 14),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 8),
QT_MOC_LITERAL(4, 41, 9)
    },
    "DrawerTh_HorNet\0sAddDelimLines\0\0"
    "sAddLine\0sDrawGrid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DrawerTh_HorNet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    4,   30,    2, 0x06 /* Public */,
       4,    0,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void,

       0        // eod
};

void DrawerTh_HorNet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DrawerTh_HorNet *_t = static_cast<DrawerTh_HorNet *>(_o);
        switch (_id) {
        case 0: _t->sAddDelimLines(); break;
        case 1: _t->sAddLine((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->sDrawGrid(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DrawerTh_HorNet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DrawerTh_HorNet::sAddDelimLines)) {
                *result = 0;
            }
        }
        {
            typedef void (DrawerTh_HorNet::*_t)(int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DrawerTh_HorNet::sAddLine)) {
                *result = 1;
            }
        }
        {
            typedef void (DrawerTh_HorNet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DrawerTh_HorNet::sDrawGrid)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject DrawerTh_HorNet::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DrawerTh_HorNet.data,
      qt_meta_data_DrawerTh_HorNet,  qt_static_metacall, 0, 0}
};


const QMetaObject *DrawerTh_HorNet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DrawerTh_HorNet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DrawerTh_HorNet.stringdata))
        return static_cast<void*>(const_cast< DrawerTh_HorNet*>(this));
    return QThread::qt_metacast(_clname);
}

int DrawerTh_HorNet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DrawerTh_HorNet::sAddDelimLines()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DrawerTh_HorNet::sAddLine(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DrawerTh_HorNet::sDrawGrid()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
