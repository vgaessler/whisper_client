/****************************************************************************
** Meta object code from reading C++ file 'wViewerHandler.h'
**
** Created: Tue 2. Mar 10:17:48 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/
/*
#include "wViewerHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wViewerHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_whisper__ViewerHandler[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_whisper__ViewerHandler[] = {
    "whisper::ViewerHandler\0\0talkingChanged()\0"
    "isTalking()\0"
};

const QMetaObject whisper::ViewerHandler::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_whisper__ViewerHandler,
      qt_meta_data_whisper__ViewerHandler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &whisper::ViewerHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *whisper::ViewerHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *whisper::ViewerHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_whisper__ViewerHandler))
        return static_cast<void*>(const_cast< ViewerHandler*>(this));
    return QThread::qt_metacast(_clname);
}

int whisper::ViewerHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: talkingChanged(); break;
        case 1: isTalking(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void whisper::ViewerHandler::talkingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
*/
