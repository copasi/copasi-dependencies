/****************************************************************************
** Meta object code from reading C++ file 'qwt_slider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qwt_slider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_slider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtSlider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       7,   14, // properties
       1,   35, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      26,   10, 0x0009510b,
      52,   38, 0x0009510b,
      71,   66, 0x01095103,
      78,   66, 0x01095103,
      91,   85, 0x15095103,
     106,  102, 0x02095103,
     118,  102, 0x02095103,

 // enums: name, flags, count, data
      38, 0x0,    3,   39,

 // enum data: key, value
     126, uint(QwtSlider::NoScale),
     134, uint(QwtSlider::LeadingScale),
     147, uint(QwtSlider::TrailingScale),

       0        // eod
};

static const char qt_meta_stringdata_QwtSlider[] = {
    "QwtSlider\0Qt::Orientation\0orientation\0"
    "ScalePosition\0scalePosition\0bool\0"
    "trough\0groove\0QSize\0handleSize\0int\0"
    "borderWidth\0spacing\0NoScale\0LeadingScale\0"
    "TrailingScale\0"
};

void QwtSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QwtSlider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtSlider::staticMetaObject = {
    { &QwtAbstractSlider::staticMetaObject, qt_meta_stringdata_QwtSlider,
      qt_meta_data_QwtSlider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtSlider))
        return static_cast<void*>(const_cast< QwtSlider*>(this));
    return QwtAbstractSlider::qt_metacast(_clname);
}

int QwtSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt::Orientation*>(_v) = orientation(); break;
        case 1: *reinterpret_cast< ScalePosition*>(_v) = scalePosition(); break;
        case 2: *reinterpret_cast< bool*>(_v) = hasTrough(); break;
        case 3: *reinterpret_cast< bool*>(_v) = hasGroove(); break;
        case 4: *reinterpret_cast< QSize*>(_v) = handleSize(); break;
        case 5: *reinterpret_cast< int*>(_v) = borderWidth(); break;
        case 6: *reinterpret_cast< int*>(_v) = spacing(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 1: setScalePosition(*reinterpret_cast< ScalePosition*>(_v)); break;
        case 2: setTrough(*reinterpret_cast< bool*>(_v)); break;
        case 3: setGroove(*reinterpret_cast< bool*>(_v)); break;
        case 4: setHandleSize(*reinterpret_cast< QSize*>(_v)); break;
        case 5: setBorderWidth(*reinterpret_cast< int*>(_v)); break;
        case 6: setSpacing(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
