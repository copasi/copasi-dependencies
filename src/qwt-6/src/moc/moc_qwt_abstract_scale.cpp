/****************************************************************************
** Meta object code from reading C++ file 'qwt_abstract_scale.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qwt_abstract_scale.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_abstract_scale.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtAbstractScale[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      24,   17, 0x06095103,
      35,   17, 0x06095103,
      50,   46, 0x02095103,
      64,   46, 0x02095103,
      78,   17, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_QwtAbstractScale[] = {
    "QwtAbstractScale\0double\0lowerBound\0"
    "upperBound\0int\0scaleMaxMajor\0scaleMaxMinor\0"
    "scaleStepSize\0"
};

void QwtAbstractScale::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QwtAbstractScale::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtAbstractScale::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QwtAbstractScale,
      qt_meta_data_QwtAbstractScale, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtAbstractScale::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtAbstractScale::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtAbstractScale::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtAbstractScale))
        return static_cast<void*>(const_cast< QwtAbstractScale*>(this));
    return QWidget::qt_metacast(_clname);
}

int QwtAbstractScale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = lowerBound(); break;
        case 1: *reinterpret_cast< double*>(_v) = upperBound(); break;
        case 2: *reinterpret_cast< int*>(_v) = scaleMaxMajor(); break;
        case 3: *reinterpret_cast< int*>(_v) = scaleMaxMinor(); break;
        case 4: *reinterpret_cast< double*>(_v) = scaleStepSize(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLowerBound(*reinterpret_cast< double*>(_v)); break;
        case 1: setUpperBound(*reinterpret_cast< double*>(_v)); break;
        case 2: setScaleMaxMajor(*reinterpret_cast< int*>(_v)); break;
        case 3: setScaleMaxMinor(*reinterpret_cast< int*>(_v)); break;
        case 4: setScaleStepSize(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
