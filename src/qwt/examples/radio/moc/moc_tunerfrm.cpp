/****************************************************************************
** Meta object code from reading C++ file 'tunerfrm.h'
**
** Created: Fri May 8 14:59:09 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tunerfrm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tunerfrm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TunerFrame[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   35,   11,   11, 0x0a,
      55,   35,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TunerFrame[] = {
    "TunerFrame\0\0f\0fieldChanged(double)\0"
    "frq\0setFreq(double)\0adjustFreq(double)\0"
};

const QMetaObject TunerFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_TunerFrame,
      qt_meta_data_TunerFrame, 0 }
};

const QMetaObject *TunerFrame::metaObject() const
{
    return &staticMetaObject;
}

void *TunerFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TunerFrame))
        return static_cast<void*>(const_cast< TunerFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int TunerFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fieldChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: setFreq((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: adjustFreq((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TunerFrame::fieldChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
