/****************************************************************************
** Meta object code from reading C++ file 'DisplayWidget.h'
**
** Created: Mon Feb 4 02:34:15 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DisplayWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DisplayWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DisplayWidget[] = {

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
      15,   14,   14,   14, 0x08,
      30,   14,   14,   14, 0x08,
      47,   14,   14,   14, 0x08,
      67,   62,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DisplayWidget[] = {
    "DisplayWidget\0\0sceneChecked()\0"
    "physicsChecked()\0tilesChecked()\0mode\0"
    "currentModeChanged(QString)\0"
};

const QMetaObject DisplayWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DisplayWidget,
      qt_meta_data_DisplayWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DisplayWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DisplayWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DisplayWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayWidget))
        return static_cast<void*>(const_cast< DisplayWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DisplayWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sceneChecked(); break;
        case 1: physicsChecked(); break;
        case 2: tilesChecked(); break;
        case 3: currentModeChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
