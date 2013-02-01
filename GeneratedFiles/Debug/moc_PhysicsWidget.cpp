/****************************************************************************
** Meta object code from reading C++ file 'PhysicsWidget.h'
**
** Created: Thu Jan 31 19:44:40 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PhysicsWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PhysicsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhysicsWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      24,   14,   14,   14, 0x0a,
      36,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PhysicsWidget[] = {
    "PhysicsWidget\0\0addBox()\0addSphere()\0"
    "addCylinder()\0"
};

const QMetaObject PhysicsWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PhysicsWidget,
      qt_meta_data_PhysicsWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhysicsWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhysicsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhysicsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhysicsWidget))
        return static_cast<void*>(const_cast< PhysicsWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PhysicsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addBox(); break;
        case 1: addSphere(); break;
        case 2: addCylinder(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
