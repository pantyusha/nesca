/****************************************************************************
** Meta object code from reading C++ file 'CheckProxy_Th.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CheckProxy_Th.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CheckProxy_Th.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CheckProxy_Th_t {
    QByteArrayData data[7];
    char stringdata[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CheckProxy_Th_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CheckProxy_Th_t qt_meta_stringdata_CheckProxy_Th = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 16),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 18),
QT_MOC_LITERAL(4, 51, 19),
QT_MOC_LITERAL(5, 71, 19),
QT_MOC_LITERAL(6, 91, 19)
    },
    "CheckProxy_Th\0changeRedIRCData\0\0"
    "changeGreenIRCData\0changeYellowIRCData\0"
    "changeRawIRCDataInc\0changeRawIRCDataOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CheckProxy_Th[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    1,   42,    2, 0x06 /* Public */,
       4,    1,   45,    2, 0x06 /* Public */,
       5,    1,   48,    2, 0x06 /* Public */,
       6,    1,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void CheckProxy_Th::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CheckProxy_Th *_t = static_cast<CheckProxy_Th *>(_o);
        switch (_id) {
        case 0: _t->changeRedIRCData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->changeGreenIRCData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->changeYellowIRCData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->changeRawIRCDataInc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->changeRawIRCDataOut((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CheckProxy_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CheckProxy_Th::changeRedIRCData)) {
                *result = 0;
            }
        }
        {
            typedef void (CheckProxy_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CheckProxy_Th::changeGreenIRCData)) {
                *result = 1;
            }
        }
        {
            typedef void (CheckProxy_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CheckProxy_Th::changeYellowIRCData)) {
                *result = 2;
            }
        }
        {
            typedef void (CheckProxy_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CheckProxy_Th::changeRawIRCDataInc)) {
                *result = 3;
            }
        }
        {
            typedef void (CheckProxy_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CheckProxy_Th::changeRawIRCDataOut)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject CheckProxy_Th::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CheckProxy_Th.data,
      qt_meta_data_CheckProxy_Th,  qt_static_metacall, 0, 0}
};


const QMetaObject *CheckProxy_Th::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CheckProxy_Th::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CheckProxy_Th.stringdata))
        return static_cast<void*>(const_cast< CheckProxy_Th*>(this));
    return QThread::qt_metacast(_clname);
}

int CheckProxy_Th::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CheckProxy_Th::changeRedIRCData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CheckProxy_Th::changeGreenIRCData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CheckProxy_Th::changeYellowIRCData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CheckProxy_Th::changeRawIRCDataInc(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CheckProxy_Th::changeRawIRCDataOut(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
