#include "stackedwidgets.h"
#include <QDebug>

StackedWidgets::StackedWidgets(QWidget *parent) :
    QWidget(parent)
{
    QStackedLayout *st = new QStackedLayout();
    setLayout(st);

    mw = new MainWindow();
    sp = new StartPage();

    flag = 0;

    layout()->addWidget(sp);

    connect(sp->my_toMap, SIGNAL(clicked()), this, SLOT(changeWidget()));
    connect(mw->my_back, SIGNAL(clicked()), this, SLOT(changeWidget()));
}

void StackedWidgets::changeWidget()
{
    QStackedLayout *lay = qobject_cast<QStackedLayout*>(layout());

    qDebug()<<"changeWidget()";

    if(flag == 0)
    {
        sp->hide();

        #if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
            mw->showMaximized();
        #else
            mw->show();
        #endif

        qDebug()<<"if";
        flag = 1;
    }
    else
    {
        mw->hide();
        sp->show();
        qDebug()<<"Else";
        flag = 0;
    }
}
