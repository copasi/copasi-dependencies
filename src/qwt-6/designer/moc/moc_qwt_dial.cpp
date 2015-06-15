/****************************************************************************
** Meta object code from reading C++ file 'qwt_dial.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qwt_dial.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_dial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtDial[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       2,   32, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      12,    8, 0x02095103,
      29,   22, 0x0009510b,
      46,   41, 0x0009510b,
      58,   51, 0x06095103,
      65,   51, 0x06095103,
      77,   51, 0x06095103,

 // enums: name, flags, count, data
      22, 0x0,    3,   40,
      41, 0x0,    2,   46,

 // enum data: key, value
      89, uint(QwtDial::Plain),
      95, uint(QwtDial::Raised),
     102, uint(QwtDial::Sunken),
     109, uint(QwtDial::RotateNeedle),
     122, uint(QwtDial::RotateScale),

       0        // eod
};

static const char qt_meta_stringdata_QwtDial[] = {
    "QwtDial\0int\0lineWidth\0Shadow\0frameShadow\0"
    "Mode\0mode\0double\0origin\0minScaleArc\0"
    "maxScaleArc\0Plain\0Raised\0Sunken\0"
    "RotateNeedle\0RotateScale\0"
};

void QwtDial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QwtDial::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtDial::staticMetaObject = {
    { &QwtAbstractSlider::staticMetaObject, qt_meta_stringdata_QwtDial,
      qt_meta_data_QwtDial, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtDial::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtDial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtDial::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtDial))
        return static_cast<void*>(const_cast< QwtDial*>(this));
    return QwtAbstractSlider::qt_metacast(_clname);
}

int QwtDial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = lineWidth(); break;
        case 1: *reinterpret_cast< Shadow*>(_v) = frameShadow(); break;
        case 2: *reinterpret_cast< Mode*>(_v) = mode(); break;
        case 3: *reinterpret_cast< double*>(_v) = origin(); break;
        case 4: *reinterpret_cast< double*>(_v) = minScaleArc(); break;
        case 5: *reinterpret_cast< double*>(_v) = maxScaleArc(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLineWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: setFrameShadow(*reinterpret_cast< Shadow*>(_v)); break;
        case 2: setMode(*reinterpret_cast< Mode*>(_v)); break;
        case 3: setOrigin(*reinterpret_cast< double*>(_v)); break;
        case 4: setMinScaleArc(*reinterpret_cast< double*>(_v)); break;
        case 5: setMaxScaleArc(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
