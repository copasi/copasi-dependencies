/****************************************************************************
** Meta object code from reading C++ file 'qwt_plot_glcanvas.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qwt_plot_glcanvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_plot_glcanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtPlotGLCanvas[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       6,   24, // properties
       2,   42, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,

 // methods: signature, parameters, type, tag, flags
      39,   16,   26,   16, 0x02,

 // properties: name, type, flags
      64,   57, 0x0009510b,
      82,   76, 0x0009510b,
      97,   93, 0x02095103,
     107,   93, 0x02095103,
     120,   93, 0x02095001,
     137,  131, 0x13094001,

 // enums: name, flags, count, data
      57, 0x0,    3,   50,
      76, 0x0,    3,   56,

 // enum data: key, value
     147, uint(QwtPlotGLCanvas::Plain),
     153, uint(QwtPlotGLCanvas::Raised),
     160, uint(QwtPlotGLCanvas::Sunken),
     167, uint(QwtPlotGLCanvas::NoFrame),
     175, uint(QwtPlotGLCanvas::Box),
     179, uint(QwtPlotGLCanvas::Panel),

       0        // eod
};

static const char qt_meta_stringdata_QwtPlotGLCanvas[] = {
    "QwtPlotGLCanvas\0\0replot()\0QPainterPath\0"
    "borderPath(QRect)\0Shadow\0frameShadow\0"
    "Shape\0frameShape\0int\0lineWidth\0"
    "midLineWidth\0frameWidth\0QRect\0frameRect\0"
    "Plain\0Raised\0Sunken\0NoFrame\0Box\0Panel\0"
};

void QwtPlotGLCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtPlotGLCanvas *_t = static_cast<QwtPlotGLCanvas *>(_o);
        switch (_id) {
        case 0: _t->replot(); break;
        case 1: { QPainterPath _r = _t->borderPath((*reinterpret_cast< const QRect(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPainterPath*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtPlotGLCanvas::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtPlotGLCanvas::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_QwtPlotGLCanvas,
      qt_meta_data_QwtPlotGLCanvas, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtPlotGLCanvas::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtPlotGLCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtPlotGLCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtPlotGLCanvas))
        return static_cast<void*>(const_cast< QwtPlotGLCanvas*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int QwtPlotGLCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Shadow*>(_v) = frameShadow(); break;
        case 1: *reinterpret_cast< Shape*>(_v) = frameShape(); break;
        case 2: *reinterpret_cast< int*>(_v) = lineWidth(); break;
        case 3: *reinterpret_cast< int*>(_v) = midLineWidth(); break;
        case 4: *reinterpret_cast< int*>(_v) = frameWidth(); break;
        case 5: *reinterpret_cast< QRect*>(_v) = frameRect(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setFrameShadow(*reinterpret_cast< Shadow*>(_v)); break;
        case 1: setFrameShape(*reinterpret_cast< Shape*>(_v)); break;
        case 2: setLineWidth(*reinterpret_cast< int*>(_v)); break;
        case 3: setMidLineWidth(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
