/****************************************************************************
** Meta object code from reading C++ file 'oIRC_Th.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "oIRC_Th.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oIRC_Th.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_oIRC_Th_t {
    QByteArrayData data[15];
    char stringdata[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_oIRC_Th_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_oIRC_Th_t qt_meta_stringdata_oIRC_Th = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 12),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 13),
QT_MOC_LITERAL(4, 36, 16),
QT_MOC_LITERAL(5, 53, 18),
QT_MOC_LITERAL(6, 72, 19),
QT_MOC_LITERAL(7, 92, 19),
QT_MOC_LITERAL(8, 112, 19),
QT_MOC_LITERAL(9, 132, 7),
QT_MOC_LITERAL(10, 140, 7),
QT_MOC_LITERAL(11, 148, 13),
QT_MOC_LITERAL(12, 162, 11),
QT_MOC_LITERAL(13, 174, 10),
QT_MOC_LITERAL(14, 185, 8)
    },
    "oIRC_Th\0sUnhidePopup\0\0changeIRCData\0"
    "changeRedIRCData\0changeGreenIRCData\0"
    "changeYellowIRCData\0changeRawIRCDataInc\0"
    "changeRawIRCDataOut\0setNick\0AddNick\0"
    "ClearNickList\0IRCOfflined\0notifyPlay\0"
    "getTopic"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_oIRC_Th[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       3,    5,   84,    2, 0x06 /* Public */,
       4,    1,   95,    2, 0x06 /* Public */,
       5,    1,   98,    2, 0x06 /* Public */,
       6,    1,  101,    2, 0x06 /* Public */,
       7,    1,  104,    2, 0x06 /* Public */,
       8,    1,  107,    2, 0x06 /* Public */,
       9,    1,  110,    2, 0x06 /* Public */,
      10,    1,  113,    2, 0x06 /* Public */,
      11,    0,  116,    2, 0x06 /* Public */,
      12,    0,  117,    2, 0x06 /* Public */,
      13,    0,  118,    2, 0x06 /* Public */,
      14,    1,  119,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void oIRC_Th::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        oIRC_Th *_t = static_cast<oIRC_Th *>(_o);
        switch (_id) {
        case 0: _t->sUnhidePopup((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->changeIRCData((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 2: _t->changeRedIRCData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->changeGreenIRCData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->changeYellowIRCData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->changeRawIRCDataInc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->changeRawIRCDataOut((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setNick((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->AddNick((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->ClearNickList(); break;
        case 10: _t->IRCOfflined(); break;
        case 11: _t->notifyPlay(); break;
        case 12: _t->getTopic((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (oIRC_Th::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::sUnhidePopup)) {
                *result = 0;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(bool , bool , int , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::changeIRCData)) {
                *result = 1;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::changeRedIRCData)) {
                *result = 2;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::changeGreenIRCData)) {
                *result = 3;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::changeYellowIRCData)) {
                *result = 4;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::changeRawIRCDataInc)) {
                *result = 5;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::changeRawIRCDataOut)) {
                *result = 6;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::setNick)) {
                *result = 7;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::AddNick)) {
                *result = 8;
            }
        }
        {
            typedef void (oIRC_Th::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::ClearNickList)) {
                *result = 9;
            }
        }
        {
            typedef void (oIRC_Th::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::IRCOfflined)) {
                *result = 10;
            }
        }
        {
            typedef void (oIRC_Th::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::notifyPlay)) {
                *result = 11;
            }
        }
        {
            typedef void (oIRC_Th::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&oIRC_Th::getTopic)) {
                *result = 12;
            }
        }
    }
}

const QMetaObject oIRC_Th::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_oIRC_Th.data,
      qt_meta_data_oIRC_Th,  qt_static_metacall, 0, 0}
};


const QMetaObject *oIRC_Th::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *oIRC_Th::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_oIRC_Th.stringdata))
        return static_cast<void*>(const_cast< oIRC_Th*>(this));
    return QThread::qt_metacast(_clname);
}

int oIRC_Th::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void oIRC_Th::sUnhidePopup(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void oIRC_Th::changeIRCData(bool _t1, bool _t2, int _t3, QString _t4, QString _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void oIRC_Th::changeRedIRCData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void oIRC_Th::changeGreenIRCData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void oIRC_Th::changeYellowIRCData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void oIRC_Th::changeRawIRCDataInc(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void oIRC_Th::changeRawIRCDataOut(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void oIRC_Th::setNick(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void oIRC_Th::AddNick(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void oIRC_Th::ClearNickList()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void oIRC_Th::IRCOfflined()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void oIRC_Th::notifyPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void oIRC_Th::getTopic(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
