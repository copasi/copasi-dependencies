/****************************************************************************
** Meta object code from reading C++ file 'qwt_abstract_slider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qwt_abstract_slider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_abstract_slider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtAbstractSlider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       9,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,
      46,   18,   18,   18, 0x05,
      62,   18,   18,   18, 0x05,
      79,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     103,   99,   18,   18, 0x0a,

 // properties: name, type, flags
      19,  120, 0x06095103,
     132,  127, 0x03095103,
     143,  127, 0x03095103,
     155,  127, 0x03095103,
     170,  165, 0x01095103,
     184,  165, 0x01095103,
     193,  165, 0x01095103,
     202,  165, 0x01095103,
     211,  165, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_QwtAbstractSlider[] = {
    "QwtAbstractSlider\0\0value\0valueChanged(double)\0"
    "sliderPressed()\0sliderReleased()\0"
    "sliderMoved(double)\0val\0setValue(double)\0"
    "double\0uint\0totalSteps\0singleSteps\0"
    "pageSteps\0bool\0stepAlignment\0readOnly\0"
    "tracking\0wrapping\0invertedControls\0"
};

void QwtAbstractSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtAbstractSlider *_t = static_cast<QwtAbstractSlider *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->sliderPressed(); break;
        case 2: _t->sliderReleased(); break;
        case 3: _t->sliderMoved((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtAbstractSlider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtAbstractSlider::staticMetaObject = {
    { &QwtAbstractScale::staticMetaObject, qt_meta_stringdata_QwtAbstractSlider,
      qt_meta_data_QwtAbstractSlider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtAbstractSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtAbstractSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtAbstractSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtAbstractSlider))
        return static_cast<void*>(const_cast< QwtAbstractSlider*>(this));
    return QwtAbstractScale::qt_metacast(_clname);
}

int QwtAbstractSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractScale::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = value(); break;
        case 1: *reinterpret_cast< uint*>(_v) = totalSteps(); break;
        case 2: *reinterpret_cast< uint*>(_v) = singleSteps(); break;
        case 3: *reinterpret_cast< uint*>(_v) = pageSteps(); break;
        case 4: *reinterpret_cast< bool*>(_v) = stepAlignment(); break;
        case 5: *reinterpret_cast< bool*>(_v) = isReadOnly(); break;
        case 6: *reinterpret_cast< bool*>(_v) = isTracking(); break;
        case 7: *reinterpret_cast< bool*>(_v) = wrapping(); break;
        case 8: *reinterpret_cast< bool*>(_v) = invertedControls(); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setValue(*reinterpret_cast< double*>(_v)); break;
        case 1: setTotalSteps(*reinterpret_cast< uint*>(_v)); break;
        case 2: setSingleSteps(*reinterpret_cast< uint*>(_v)); break;
        case 3: setPageSteps(*reinterpret_cast< uint*>(_v)); break;
        case 4: setStepAlignment(*reinterpret_cast< bool*>(_v)); break;
        case 5: setReadOnly(*reinterpret_cast< bool*>(_v)); break;
        case 6: setTracking(*reinterpret_cast< bool*>(_v)); break;
        case 7: setWrapping(*reinterpret_cast< bool*>(_v)); break;
        case 8: setInvertedControls(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QwtAbstractSlider::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QwtAbstractSlider::sliderPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QwtAbstractSlider::sliderReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QwtAbstractSlider::sliderMoved(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
