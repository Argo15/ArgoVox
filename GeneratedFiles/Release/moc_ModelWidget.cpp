/****************************************************************************
** Meta object code from reading C++ file 'ModelWidget.h'
**
** Created: Sun Mar 24 20:39:45 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ModelWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModelWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModelWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      29,   23,   12,   12, 0x08,
      62,   58,   12,   12, 0x08,
      89,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ModelWidget[] = {
    "ModelWidget\0\0refresh()\0model\0"
    "currentModelChanged(QString)\0mat\0"
    "currentMatChanged(QString)\0addToScene()\0"
};

const QMetaObject ModelWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ModelWidget,
      qt_meta_data_ModelWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModelWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModelWidget))
        return static_cast<void*>(const_cast< ModelWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ModelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: refresh(); break;
        case 1: currentModelChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: currentMatChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: addToScene(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
