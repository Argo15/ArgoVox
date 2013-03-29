/****************************************************************************
** Meta object code from reading C++ file 'ActorWidget.h'
**
** Created: Fri Mar 29 03:48:33 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ActorWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ActorWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ActorWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      32,   23,   12,   12, 0x0a,
      65,   59,   12,   12, 0x0a,
      94,   12,   12,   12, 0x0a,
     111,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ActorWidget[] = {
    "ActorWidget\0\0refresh()\0material\0"
    "currentMatChanged(QString)\0model\0"
    "currentModelChanged(QString)\0"
    "changePosition()\0changeScale()\0"
};

const QMetaObject ActorWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ActorWidget,
      qt_meta_data_ActorWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ActorWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ActorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ActorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ActorWidget))
        return static_cast<void*>(const_cast< ActorWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ActorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: refresh(); break;
        case 1: currentMatChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: currentModelChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: changePosition(); break;
        case 4: changeScale(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
