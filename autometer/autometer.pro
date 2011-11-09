QT += xml network svg
TARGET = SimpleRouting
TEMPLATE = app
INCLUDEPATH += ../../src/global \
               ../../src/location \
               ../../src/location/maps
SOURCES += main.cpp \
           mapwidget.cpp \
           mainwindow.cpp \
           doodle.cpp \
    mainmenu.cpp \
    startpage.cpp \
    stackedwidgets.cpp

HEADERS += mapwidget.h \
           mainwindow.h \
           doodle.h \
    mainwindow.h \
    mapwidget.h \
    mainmenu.h \
    startpage.h \
    stackedwidgets.h

CONFIG += mobility
MOBILITY = location

equals(QT_MAJOR_VERSION, 4):lessThan(QT_MINOR_VERSION, 7){
    MOBILITY += bearer
    INCLUDEPATH += ../../src/bearer
}

symbian: {
    TARGET.CAPABILITY = Location \
                        NetworkServices \
                        ReadUserData \
                        WriteUserData
    ICON = icon.svg
}


















