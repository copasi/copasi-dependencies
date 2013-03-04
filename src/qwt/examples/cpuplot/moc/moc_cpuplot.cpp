/****************************************************************************
** Meta object code from reading C++ file 'cpuplot.h'
**
** Created: Fri May 8 14:58:43 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cpuplot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cpuplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CpuPlot[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,    9,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CpuPlot[] = {
    "CpuPlot\0\0,on\0showCurve(QwtPlotItem*,bool)\0"
};

const QMetaObject CpuPlot::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_CpuPlot,
      qt_meta_data_CpuPlot, 0 }
};

const QMetaObject *CpuPlot::metaObject() const
{
    return &staticMetaObject;
}

void *CpuPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CpuPlot))
        return static_cast<void*>(const_cast< CpuPlot*>(this));
    return QwtPlot::qt_metacast(_clname);
}

int CpuPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showCurve((*reinterpret_cast< QwtPlotItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
