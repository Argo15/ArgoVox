/****************************************************************************
** Meta object code from reading C++ file 'chunkeditor2.h'
**
** Created: Sun Mar 24 20:31:43 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../chunkeditor2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chunkeditor2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChunkEditor2[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_ChunkEditor2[] = {
    "ChunkEditor2\0"
};

const QMetaObject ChunkEditor2::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ChunkEditor2,
      qt_meta_data_ChunkEditor2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChunkEditor2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChunkEditor2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChunkEditor2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChunkEditor2))
        return static_cast<void*>(const_cast< ChunkEditor2*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ChunkEditor2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
