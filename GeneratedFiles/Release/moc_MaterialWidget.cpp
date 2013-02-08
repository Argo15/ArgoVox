/****************************************************************************
** Meta object code from reading C++ file 'MaterialWidget.h'
**
** Created: Thu Feb 7 14:25:57 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MaterialWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MaterialWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MaterialWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   16,   15,   15, 0x0a,
      36,   16,   15,   15, 0x08,
      63,   15,   15,   15, 0x08,
      73,   15,   15,   15, 0x08,
      95,   90,   15,   15, 0x08,
     122,  116,   15,   15, 0x08,
     139,  116,   15,   15, 0x08,
     157,   15,   15,   15, 0x08,
     172,   15,   15,   15, 0x08,
     190,  186,   15,   15, 0x08,
     217,  186,   15,   15, 0x08,
     245,  116,   15,   15, 0x08,
     269,  116,   15,   15, 0x08,
     293,  116,   15,   15, 0x08,
     319,  116,   15,   15, 0x08,
     345,  116,   15,   15, 0x08,
     367,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MaterialWidget[] = {
    "MaterialWidget\0\0mat\0refresh(string)\0"
    "currentMatChanged(QString)\0saveAll()\0"
    "addNewMaterial()\0name\0nameChanged(QString)\0"
    "value\0specChanged(int)\0shineChanged(int)\0"
    "colorChanged()\0glowChanged()\0tex\0"
    "currentTexChanged(QString)\0"
    "currentNormChanged(QString)\0"
    "uTexOffChanged(QString)\0vTexOffChanged(QString)\0"
    "uTexScaleChanged(QString)\0"
    "vTexScaleChanged(QString)\0"
    "texRotateChanged(int)\0normalChanged()\0"
};

const QMetaObject MaterialWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaterialWidget,
      qt_meta_data_MaterialWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MaterialWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MaterialWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MaterialWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MaterialWidget))
        return static_cast<void*>(const_cast< MaterialWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaterialWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: refresh((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 1: currentMatChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: saveAll(); break;
        case 3: addNewMaterial(); break;
        case 4: nameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: specChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: shineChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: colorChanged(); break;
        case 8: glowChanged(); break;
        case 9: currentTexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: currentNormChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: uTexOffChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: vTexOffChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: uTexScaleChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: vTexScaleChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: texRotateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: normalChanged(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
