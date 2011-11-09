/****************************************************************************
** Meta object code from reading C++ file 'startpage.h'
**
** Created: Thu 3. Nov 17:59:12 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../startpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'startpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StartPage[] = {

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
      11,   10,   10,   10, 0x0a,
      25,   10,   10,   10, 0x0a,
      43,   40,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StartPage[] = {
    "StartPage\0\0changeRates()\0startClicked()\0"
    "gp\0updateValues(QGeoPositionInfo)\0"
};

const QMetaObject StartPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_StartPage,
      qt_meta_data_StartPage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StartPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StartPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StartPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StartPage))
        return static_cast<void*>(const_cast< StartPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int StartPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeRates(); break;
        case 1: startClicked(); break;
        case 2: updateValues((*reinterpret_cast< QGeoPositionInfo(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
