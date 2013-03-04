/****************************************************************************
** Meta object code from reading C++ file 'bode.h'
**
** Created: Fri May 8 14:58:58 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWin[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      23,    8,    8,    8, 0x08,
      44,    8,    8,    8, 0x08,
      52,    8,    8,    8, 0x08,
      64,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWin[] = {
    "MainWin\0\0moved(QPoint)\0selected(QwtPolygon)\0"
    "print()\0exportSVG()\0enableZoomMode(bool)\0"
};

const QMetaObject MainWin::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWin,
      qt_meta_data_MainWin, 0 }
};

const QMetaObject *MainWin::metaObject() const
{
    return &staticMetaObject;
}

void *MainWin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWin))
        return static_cast<void*>(const_cast< MainWin*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: moved((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: selected((*reinterpret_cast< const QwtPolygon(*)>(_a[1]))); break;
        case 2: print(); break;
        case 3: exportSVG(); break;
        case 4: enableZoomMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
