/****************************************************************************
** Meta object code from reading C++ file 'qwt_abstract_legend.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qwt_abstract_legend.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_abstract_legend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtAbstractLegend[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      33,   19,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QwtAbstractLegend[] = {
    "QwtAbstractLegend\0\0itemInfo,data\0"
    "updateLegend(QVariant,QList<QwtLegendData>)\0"
};

void QwtAbstractLegend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtAbstractLegend *_t = static_cast<QwtAbstractLegend *>(_o);
        switch (_id) {
        case 0: _t->updateLegend((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QList<QwtLegendData>(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtAbstractLegend::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtAbstractLegend::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QwtAbstractLegend,
      qt_meta_data_QwtAbstractLegend, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtAbstractLegend::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtAbstractLegend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtAbstractLegend::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtAbstractLegend))
        return static_cast<void*>(const_cast< QwtAbstractLegend*>(this));
    return QFrame::qt_metacast(_clname);
}

int QwtAbstractLegend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
