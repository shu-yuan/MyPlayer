/****************************************************************************
** Meta object code from reading C++ file 'mywidget.h'
**
** Created: Wed May 15 23:34:39 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mywidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      29,    9,    9,    9, 0x08,
      43,    9,    9,    9, 0x08,
      55,    9,    9,    9, 0x08,
      70,    9,    9,    9, 0x08,
      84,    9,    9,    9, 0x08,
     100,   95,    9,    9, 0x08,
     121,  119,    9,    9, 0x08,
     170,  163,    9,    9, 0x08,
     205,    9,    9,    9, 0x08,
     239,  221,    9,    9, 0x08,
     285,    9,    9,    9, 0x08,
     303,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MyWidget[] = {
    "MyWidget\0\0setPlaylistShown()\0setLrcShown()\0"
    "setPaused()\0skipBackward()\0skipForward()\0"
    "openFile()\0time\0updateTime(qint64)\0,\0"
    "stateChanged(Phonon::State,Phonon::State)\0"
    "source\0sourceChanged(Phonon::MediaSource)\0"
    "aboutToFinish()\0newState,oldState\0"
    "metaStateChanged(Phonon::State,Phonon::State)\0"
    "tableClicked(int)\0clearSources()\0"
};

const QMetaObject MyWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MyWidget,
      qt_meta_data_MyWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyWidget))
        return static_cast<void*>(const_cast< MyWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MyWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setPlaylistShown(); break;
        case 1: setLrcShown(); break;
        case 2: setPaused(); break;
        case 3: skipBackward(); break;
        case 4: skipForward(); break;
        case 5: openFile(); break;
        case 6: updateTime((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 7: stateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 8: sourceChanged((*reinterpret_cast< const Phonon::MediaSource(*)>(_a[1]))); break;
        case 9: aboutToFinish(); break;
        case 10: metaStateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 11: tableClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: clearSources(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
