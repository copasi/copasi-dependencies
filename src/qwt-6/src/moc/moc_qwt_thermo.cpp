/****************************************************************************
** Meta object code from reading C++ file 'qwt_thermo.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qwt_thermo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_thermo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtThermo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      10,   19, // properties
       2,   49, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x0a,

 // properties: name, type, flags
      48,   32, 0x0009510b,
      74,   60, 0x0009510b,
      99,   88, 0x0009510b,
     115,  110, 0x01095103,
     135,  128, 0x06095103,
     146,  128, 0x06095103,
     157,  153, 0x02095103,
     165,  153, 0x02095103,
     177,  153, 0x02095103,
     187,  128, 0x06095103,

 // enums: name, flags, count, data
      60, 0x0,    3,   57,
      88, 0x0,    3,   63,

 // enum data: key, value
     193, uint(QwtThermo::NoScale),
     201, uint(QwtThermo::LeadingScale),
     214, uint(QwtThermo::TrailingScale),
     228, uint(QwtThermo::OriginMinimum),
     242, uint(QwtThermo::OriginMaximum),
     256, uint(QwtThermo::OriginCustom),

       0        // eod
};

static const char qt_meta_stringdata_QwtThermo[] = {
    "QwtThermo\0\0val\0setValue(double)\0"
    "Qt::Orientation\0orientation\0ScalePosition\0"
    "scalePosition\0OriginMode\0originMode\0"
    "bool\0alarmEnabled\0double\0alarmLevel\0"
    "origin\0int\0spacing\0borderWidth\0pipeWidth\0"
    "value\0NoScale\0LeadingScale\0TrailingScale\0"
    "OriginMinimum\0OriginMaximum\0OriginCustom\0"
};

void QwtThermo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtThermo *_t = static_cast<QwtThermo *>(_o);
        switch (_id) {
        case 0: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtThermo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtThermo::staticMetaObject = {
    { &QwtAbstractScale::staticMetaObject, qt_meta_stringdata_QwtThermo,
      qt_meta_data_QwtThermo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtThermo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtThermo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtThermo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtThermo))
        return static_cast<void*>(const_cast< QwtThermo*>(this));
    return QwtAbstractScale::qt_metacast(_clname);
}

int QwtThermo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractScale::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt::Orientation*>(_v) = orientation(); break;
        case 1: *reinterpret_cast< ScalePosition*>(_v) = scalePosition(); break;
        case 2: *reinterpret_cast< OriginMode*>(_v) = originMode(); break;
        case 3: *reinterpret_cast< bool*>(_v) = alarmEnabled(); break;
        case 4: *reinterpret_cast< double*>(_v) = alarmLevel(); break;
        case 5: *reinterpret_cast< double*>(_v) = origin(); break;
        case 6: *reinterpret_cast< int*>(_v) = spacing(); break;
        case 7: *reinterpret_cast< int*>(_v) = borderWidth(); break;
        case 8: *reinterpret_cast< int*>(_v) = pipeWidth(); break;
        case 9: *reinterpret_cast< double*>(_v) = value(); break;
        }
        _id -= 10;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 1: setScalePosition(*reinterpret_cast< ScalePosition*>(_v)); break;
        case 2: setOriginMode(*reinterpret_cast< OriginMode*>(_v)); break;
        case 3: setAlarmEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 4: setAlarmLevel(*reinterpret_cast< double*>(_v)); break;
        case 5: setOrigin(*reinterpret_cast< double*>(_v)); break;
        case 6: setSpacing(*reinterpret_cast< int*>(_v)); break;
        case 7: setBorderWidth(*reinterpret_cast< int*>(_v)); break;
        case 8: setPipeWidth(*reinterpret_cast< int*>(_v)); break;
        case 9: setValue(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 10;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 10;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
