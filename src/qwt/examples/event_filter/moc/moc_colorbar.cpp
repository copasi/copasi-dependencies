/****************************************************************************
** Meta object code from reading C++ file 'colorbar.h'
**
** Created: Fri May 8 14:59:04 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../colorbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'colorbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ColorBar[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ColorBar[] = {
    "ColorBar\0\0selected(QColor)\0"
};

const QMetaObject ColorBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ColorBar,
      qt_meta_data_ColorBar, 0 }
};

const QMetaObject *ColorBar::metaObject() const
{
    return &staticMetaObject;
}

void *ColorBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColorBar))
        return static_cast<void*>(const_cast< ColorBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int ColorBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selected((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ColorBar::selected(const QColor & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
