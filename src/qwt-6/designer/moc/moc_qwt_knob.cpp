/****************************************************************************
** Meta object code from reading C++ file 'qwt_knob.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qwt_knob.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_knob.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtKnob[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       8,   14, // properties
       2,   38, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      18,    8, 0x0009510b,
      32,   28, 0x02095103,
      56,   42, 0x0009510b,
      73,   66, 0x06095103,
      84,   28, 0x02095103,
     105,   93, 0x0009510b,
     117,   28, 0x02095103,
     128,   28, 0x02095103,

 // enums: name, flags, count, data
       8, 0x0,    4,   46,
      93, 0x0,    6,   54,

 // enum data: key, value
     140, uint(QwtKnob::Flat),
     145, uint(QwtKnob::Raised),
     152, uint(QwtKnob::Sunken),
     159, uint(QwtKnob::Styled),
     166, uint(QwtKnob::NoMarker),
     175, uint(QwtKnob::Tick),
     180, uint(QwtKnob::Triangle),
     189, uint(QwtKnob::Dot),
     193, uint(QwtKnob::Nub),
     197, uint(QwtKnob::Notch),

       0        // eod
};

static const char qt_meta_stringdata_QwtKnob[] = {
    "QwtKnob\0KnobStyle\0knobStyle\0int\0"
    "knobWidth\0Qt::Alignment\0alignment\0"
    "double\0totalAngle\0numTurns\0MarkerStyle\0"
    "markerStyle\0markerSize\0borderWidth\0"
    "Flat\0Raised\0Sunken\0Styled\0NoMarker\0"
    "Tick\0Triangle\0Dot\0Nub\0Notch\0"
};

void QwtKnob::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QwtKnob::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtKnob::staticMetaObject = {
    { &QwtAbstractSlider::staticMetaObject, qt_meta_stringdata_QwtKnob,
      qt_meta_data_QwtKnob, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtKnob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtKnob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtKnob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtKnob))
        return static_cast<void*>(const_cast< QwtKnob*>(this));
    return QwtAbstractSlider::qt_metacast(_clname);
}

int QwtKnob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< KnobStyle*>(_v) = knobStyle(); break;
        case 1: *reinterpret_cast< int*>(_v) = knobWidth(); break;
        case 2: *reinterpret_cast< Qt::Alignment*>(_v) = alignment(); break;
        case 3: *reinterpret_cast< double*>(_v) = totalAngle(); break;
        case 4: *reinterpret_cast< int*>(_v) = numTurns(); break;
        case 5: *reinterpret_cast< MarkerStyle*>(_v) = markerStyle(); break;
        case 6: *reinterpret_cast< int*>(_v) = markerSize(); break;
        case 7: *reinterpret_cast< int*>(_v) = borderWidth(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setKnobStyle(*reinterpret_cast< KnobStyle*>(_v)); break;
        case 1: setKnobWidth(*reinterpret_cast< int*>(_v)); break;
        case 2: setAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 3: setTotalAngle(*reinterpret_cast< double*>(_v)); break;
        case 4: setNumTurns(*reinterpret_cast< int*>(_v)); break;
        case 5: setMarkerStyle(*reinterpret_cast< MarkerStyle*>(_v)); break;
        case 6: setMarkerSize(*reinterpret_cast< int*>(_v)); break;
        case 7: setBorderWidth(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
