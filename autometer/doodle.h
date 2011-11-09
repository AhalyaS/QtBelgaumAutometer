#ifndef DOODLE_H
#define DOODLE_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>
#include <QImage>
#include <QHBoxLayout>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPushButton>

#include "mainmenu.h"

class Doodle : public QDialog
{
    Q_OBJECT

    QGraphicsScene *scene;
    QGraphicsRectItem *rect;
    QGraphicsView *view;
    QGraphicsPixmapItem *auto_image, *smoke1, *smoke2, *smoke3, *title;
    QGraphicsItemAnimation *anim_auto, *anim_sm1, *anim_sm2, *anim_sm3;
    QTimer *timer_create1, *timer_create2, *timer_create3, *end_anim, *first_over;
    QTimer *start_timer1, *start_timer2, *start_timer3;
    QTimeLine *timer;
    QPushButton *m_help;
    int i1, i2, i3;

public:
    explicit Doodle(QWidget *parent = 0);

signals:

public slots:
    void createSmoke1();
    void createSmoke2();
    void createSmoke3();
    void delTimer();
    void endAnimation();
    void switchScreen();

};

#endif // DOODLE_H
