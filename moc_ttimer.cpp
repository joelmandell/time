/****************************************************************************
** Meta object code from reading C++ file 'ttimer.h'
**
** Created: Mon Jan 11 03:04:32 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ttimer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ttimer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TTimerThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_TTimerThread[] = {
    "TTimerThread\0"
};

const QMetaObject TTimerThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TTimerThread,
      qt_meta_data_TTimerThread, 0 }
};

const QMetaObject *TTimerThread::metaObject() const
{
    return &staticMetaObject;
}

void *TTimerThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TTimerThread))
        return static_cast<void*>(const_cast< TTimerThread*>(this));
    return QThread::qt_metacast(_clname);
}

int TTimerThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_TTimer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      19,    7,    7,    7, 0x0a,
      31,    7,    7,    7, 0x0a,
      43,    7,    7,    7, 0x0a,
      56,    7,    7,    7, 0x0a,
      69,    7,    7,    7, 0x0a,
      83,   79,    7,    7, 0x0a,
     107,    7,    7,    7, 0x0a,
     127,  119,    7,    7, 0x0a,
     143,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TTimer[] = {
    "TTimer\0\0endAlarm()\0playAlarm()\0"
    "setConfig()\0loadConfig()\0startClock()\0"
    "setTime()\0foo\0timeProperties(QString)\0"
    "stopClock()\0command\0moveIt(QString)\0"
    "configDialog()\0"
};

const QMetaObject TTimer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TTimer,
      qt_meta_data_TTimer, 0 }
};

const QMetaObject *TTimer::metaObject() const
{
    return &staticMetaObject;
}

void *TTimer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TTimer))
        return static_cast<void*>(const_cast< TTimer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TTimer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: endAlarm(); break;
        case 1: playAlarm(); break;
        case 2: setConfig(); break;
        case 3: loadConfig(); break;
        case 4: startClock(); break;
        case 5: setTime(); break;
        case 6: timeProperties((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: stopClock(); break;
        case 8: moveIt((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: configDialog(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
