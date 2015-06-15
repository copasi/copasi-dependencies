/****************************************************************************
** Meta object code from reading C++ file 'qwt_counter.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qwt_counter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_counter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtCounter[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
      10,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x05,
      41,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   11,   11,   11, 0x0a,
      79,   11,   11,   11, 0x08,
      93,   11,   11,   11, 0x08,
     106,   11,   11,   11, 0x08,

 // properties: name, type, flags
      12,  120, 0x06095103,
     127,  120, 0x06095103,
     135,  120, 0x06095103,
     143,  120, 0x06095103,
     158,  154, 0x02095103,
     169,  154, 0x02095103,
     181,  154, 0x02095103,
     193,  154, 0x02095103,
     210,  205, 0x01095103,
     219,  205, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_QwtCounter[] = {
    "QwtCounter\0\0value\0buttonReleased(double)\0"
    "valueChanged(double)\0setValue(double)\0"
    "btnReleased()\0btnClicked()\0textChanged()\0"
    "double\0minimum\0maximum\0singleStep\0int\0"
    "numButtons\0stepButton1\0stepButton2\0"
    "stepButton3\0bool\0readOnly\0wrapping\0"
};

void QwtCounter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtCounter *_t = static_cast<QwtCounter *>(_o);
        switch (_id) {
        case 0: _t->buttonReleased((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->btnReleased(); break;
        case 4: _t->btnClicked(); break;
        case 5: _t->textChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtCounter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtCounter::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QwtCounter,
      qt_meta_data_QwtCounter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtCounter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtCounter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtCounter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtCounter))
        return static_cast<void*>(const_cast< QwtCounter*>(this));
    return QWidget::qt_metacast(_clname);
}

int QwtCounter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = value(); break;
        case 1: *reinterpret_cast< double*>(_v) = minimum(); break;
        case 2: *reinterpret_cast< double*>(_v) = maximum(); break;
        case 3: *reinterpret_cast< double*>(_v) = singleStep(); break;
        case 4: *reinterpret_cast< int*>(_v) = numButtons(); break;
        case 5: *reinterpret_cast< int*>(_v) = stepButton1(); break;
        case 6: *reinterpret_cast< int*>(_v) = stepButton2(); break;
        case 7: *reinterpret_cast< int*>(_v) = stepButton3(); break;
        case 8: *reinterpret_cast< bool*>(_v) = isReadOnly(); break;
        case 9: *reinterpret_cast< bool*>(_v) = wrapping(); break;
        }
        _id -= 10;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setValue(*reinterpret_cast< double*>(_v)); break;
        case 1: setMinimum(*reinterpret_cast< double*>(_v)); break;
        case 2: setMaximum(*reinterpret_cast< double*>(_v)); break;
        case 3: setSingleStep(*reinterpret_cast< double*>(_v)); break;
        case 4: setNumButtons(*reinterpret_cast< int*>(_v)); break;
        case 5: setStepButton1(*reinterpret_cast< int*>(_v)); break;
        case 6: setStepButton2(*reinterpret_cast< int*>(_v)); break;
        case 7: setStepButton3(*reinterpret_cast< int*>(_v)); break;
        case 8: setReadOnly(*reinterpret_cast< bool*>(_v)); break;
        case 9: setWrapping(*reinterpret_cast< bool*>(_v)); break;
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

// SIGNAL 0
void QwtCounter::buttonReleased(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QwtCounter::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
