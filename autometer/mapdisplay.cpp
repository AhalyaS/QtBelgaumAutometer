#include "mapdisplay.h"

MapDisplay::MapDisplay(QWidget *parent) :
    QWidget(parent)
{
    vb = new QVBoxLayout();
    my_mw = new MainWindow();

    #if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
        my_mw->showMaximized();
    #else
        my_mw->show();
    #endif

    //vb->addItem(my_mw);
    setLayout(vb);
}
