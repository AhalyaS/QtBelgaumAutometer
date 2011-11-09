#ifndef STACKEDWIDGETS_H
#define STACKEDWIDGETS_H

#include <QWidget>
#include <QStackedLayout>
#include <QTimer>

#include "mainwindow.h"
#include "startpage.h"

class StackedWidgets : public QWidget
{
    Q_OBJECT

    MainWindow *mw;
    StartPage *sp;
    QTimer *change_time;

    QStackedLayout *lay;

    int flag;

public:
    explicit StackedWidgets(QWidget *parent = 0);

signals:

public slots:
    void changeWidget();

};

#endif // STACKEDWIDGETS_H
