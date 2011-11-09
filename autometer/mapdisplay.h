#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H

#include <QWidget>
#include <QVBoxLayout>
#include "mainwindow.h"

class MapDisplay : public QWidget
{
    Q_OBJECT

    MainWindow *my_mw;
    QVBoxLayout *vb;

public:
    explicit MapDisplay(QWidget *parent = 0);

signals:

public slots:

};

#endif // MAPDISPLAY_H
