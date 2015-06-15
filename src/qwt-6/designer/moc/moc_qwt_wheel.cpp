/****************************************************************************
** Meta object code from reading C++ file 'qwt_wheel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qwt_wheel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_wheel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtWheel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
      18,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   10,    9,    9, 0x05,
      37,    9,    9,    9, 0x05,
      52,    9,    9,    9, 0x05,
      68,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      87,    9,    9,    9, 0x0a,
     104,    9,    9,    9, 0x0a,
     126,    9,    9,    9, 0x0a,
     147,    9,    9,    9, 0x0a,

 // properties: name, type, flags
     179,  163, 0x0009510b,
      10,  191, 0x06095103,
     198,  191, 0x06095103,
     206,  191, 0x06095103,
     214,  191, 0x06095103,
     229,  225, 0x02095103,
     248,  243, 0x01095103,
     262,  243, 0x01095103,
     271,  243, 0x01095103,
     280,  243, 0x01095103,
     289,  191, 0x06095103,
     294,  225, 0x02095103,
     309,  191, 0x06095103,
     320,  191, 0x06095103,
     330,  225, 0x02095103,
     340,  225, 0x02095103,
     351,  225, 0x02095103,
     363,  225, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_QwtWheel[] = {
    "QwtWheel\0\0value\0valueChanged(double)\0"
    "wheelPressed()\0wheelReleased()\0"
    "wheelMoved(double)\0setValue(double)\0"
    "setTotalAngle(double)\0setViewAngle(double)\0"
    "setMass(double)\0Qt::Orientation\0"
    "orientation\0double\0minimum\0maximum\0"
    "singleStep\0int\0pageStepCount\0bool\0"
    "stepAlignment\0tracking\0wrapping\0"
    "inverted\0mass\0updateInterval\0totalAngle\0"
    "viewAngle\0tickCount\0wheelWidth\0"
    "borderWidth\0wheelBorderWidth\0"
};

void QwtWheel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtWheel *_t = static_cast<QwtWheel *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->wheelPressed(); break;
        case 2: _t->wheelReleased(); break;
        case 3: _t->wheelMoved((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setTotalAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setViewAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setMass((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtWheel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtWheel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QwtWheel,
      qt_meta_data_QwtWheel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtWheel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtWheel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtWheel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtWheel))
        return static_cast<void*>(const_cast< QwtWheel*>(this));
    return QWidget::qt_metacast(_clname);
}

int QwtWheel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt::Orientation*>(_v) = orientation(); break;
        case 1: *reinterpret_cast< double*>(_v) = value(); break;
        case 2: *reinterpret_cast< double*>(_v) = minimum(); break;
        case 3: *reinterpret_cast< double*>(_v) = maximum(); break;
        case 4: *reinterpret_cast< double*>(_v) = singleStep(); break;
        case 5: *reinterpret_cast< int*>(_v) = pageStepCount(); break;
        case 6: *reinterpret_cast< bool*>(_v) = stepAlignment(); break;
        case 7: *reinterpret_cast< bool*>(_v) = isTracking(); break;
        case 8: *reinterpret_cast< bool*>(_v) = wrapping(); break;
        case 9: *reinterpret_cast< bool*>(_v) = isInverted(); break;
        case 10: *reinterpret_cast< double*>(_v) = mass(); break;
        case 11: *reinterpret_cast< int*>(_v) = updateInterval(); break;
        case 12: *reinterpret_cast< double*>(_v) = totalAngle(); break;
        case 13: *reinterpret_cast< double*>(_v) = viewAngle(); break;
        case 14: *reinterpret_cast< int*>(_v) = tickCount(); break;
        case 15: *reinterpret_cast< int*>(_v) = wheelWidth(); break;
        case 16: *reinterpret_cast< int*>(_v) = borderWidth(); break;
        case 17: *reinterpret_cast< int*>(_v) = wheelBorderWidth(); break;
        }
        _id -= 18;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 1: setValue(*reinterpret_cast< double*>(_v)); break;
        case 2: setMinimum(*reinterpret_cast< double*>(_v)); break;
        case 3: setMaximum(*reinterpret_cast< double*>(_v)); break;
        case 4: setSingleStep(*reinterpret_cast< double*>(_v)); break;
        case 5: setPageStepCount(*reinterpret_cast< int*>(_v)); break;
        case 6: setStepAlignment(*reinterpret_cast< bool*>(_v)); break;
        case 7: setTracking(*reinterpret_cast< bool*>(_v)); break;
        case 8: setWrapping(*reinterpret_cast< bool*>(_v)); break;
        case 9: setInverted(*reinterpret_cast< bool*>(_v)); break;
        case 10: setMass(*reinterpret_cast< double*>(_v)); break;
        case 11: setUpdateInterval(*reinterpret_cast< int*>(_v)); break;
        case 12: setTotalAngle(*reinterpret_cast< double*>(_v)); break;
        case 13: setViewAngle(*reinterpret_cast< double*>(_v)); break;
        case 14: setTickCount(*reinterpret_cast< int*>(_v)); break;
        case 15: setWheelWidth(*reinterpret_cast< int*>(_v)); break;
        case 16: setBorderWidth(*reinterpret_cast< int*>(_v)); break;
        case 17: setWheelBorderWidth(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 18;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 18;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QwtWheel::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QwtWheel::wheelPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QwtWheel::wheelReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QwtWheel::wheelMoved(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
