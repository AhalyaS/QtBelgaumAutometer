#include "doodle.h"

Doodle::Doodle(QWidget *parent) :
    QDialog(parent)
{
    /* This creates the screen that first appears when the application is turned on
    ** A graphics view is created with a scen in it and the images are placed on this screen
    ** Property Animation is used to animate the images
    */

    i1 = 10;
    i2 = 10;
    i3 = 10;

    /*Create the view and scene*/
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);

    /*Fit the view to the screen*/

    view->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    QPixmap back_pix("C:\\QtSDK\\autometer\\Images\\bgrnd.png");
    back_pix.scaled(QSize(view->width(), view->height()), Qt::IgnoreAspectRatio);
    QBrush mybrush(back_pix);
    view->setBackgroundBrush(mybrush);

    /*Load images onto the application and scale them*/
    QPixmap auto_pix("C:\\QtSDK\\autometer\\Images\\auto1.png");
    auto_pix.scaled(QSize(view->width()/2.0, view->height()/2.0), Qt::KeepAspectRatioByExpanding);
    auto_image = new QGraphicsPixmapItem(auto_pix);
    auto_image->setPos(-10,75);
    auto_image->setVisible(false);

    QPixmap text_pix("C:\\QtSDK\\autometer\\Images\\text.png");
    text_pix.scaled(QSize(view->width()/2.0, view->height()/2.0), Qt::KeepAspectRatioByExpanding);
    title = new QGraphicsPixmapItem(text_pix);
    title->setPos(10, -100);
    title->setVisible(false);

    QPixmap cloud1("C:\\QtSDK\\autometer\\Images\\smoke3.png");
    cloud1.scaled(QSize(auto_pix.width()/8.0, auto_pix.height()/8.0));
    smoke1 = new QGraphicsPixmapItem(cloud1);
    smoke1->setPos(-(auto_pix.width()*0.15), 0);
    smoke1->setVisible(false);

    QPixmap cloud2("C:\\QtSDK\\autometer\\Images\\smoke2.png");
    cloud2.scaled(QSize(auto_pix.width()/8.0, auto_pix.height()/8.0));
    smoke2 = new QGraphicsPixmapItem(cloud2);
    smoke2->setPos(-(auto_pix.width()*0.23), 0);
    smoke2->setVisible(false);

    QPixmap cloud3("C:\\QtSDK\\autometer\\Images\\smoke1.png");
    cloud3.scaled(QSize(auto_pix.width()/8.0, auto_pix.height()/8.0));
    smoke3 = new QGraphicsPixmapItem(cloud3);
    smoke3->setPos(-(auto_pix.width()*0.28), 0);
    smoke3->setVisible(false);

    /*Set animation for the images*/
    timer = new QTimeLine(3000);
    timer->setFrameRange(0, 100);

    anim_auto = new QGraphicsItemAnimation();
    anim_auto->setItem(auto_image);
    anim_auto->setTimeLine(timer);

    first_over = new QTimer(this);
    first_over->setInterval(2000);

    timer_create1 = new QTimer(this);
    timer_create1->setInterval(150);
    timer_create2 = new QTimer(this);
    timer_create2->setInterval(150);
    timer_create3 = new QTimer(this);
    timer_create3->setInterval(150);

    start_timer2 = new QTimer(this);
    start_timer2->setInterval(1400);
    start_timer3 = new QTimer(this);
    start_timer3->setInterval(2300);
    start_timer1 = new QTimer(this);
    start_timer1->setInterval(1000);
    end_anim = new QTimer(this);
    end_anim->setInterval(1000);

    connect(timer_create1, SIGNAL(timeout()), this, SLOT(createSmoke1()));
    connect(timer_create2, SIGNAL(timeout()), this, SLOT(createSmoke2()));
    connect(timer_create3, SIGNAL(timeout()), this, SLOT(createSmoke3()));

    connect(start_timer2, SIGNAL(timeout()), timer_create2, SLOT(start()));
    connect(start_timer3, SIGNAL(timeout()), timer_create3, SLOT(start()));

    connect(start_timer1, SIGNAL(timeout()), timer, SLOT(start()));
    connect(start_timer1, SIGNAL(timeout()), start_timer2, SLOT(start()));
    connect(start_timer1, SIGNAL(timeout()), start_timer3, SLOT(start()));
    connect(start_timer1, SIGNAL(timeout()), timer_create1, SLOT(start()));
    connect(start_timer1, SIGNAL(timeout()), this, SLOT(delTimer()));

    connect(end_anim, SIGNAL(timeout()), this, SLOT(endAnimation()));

    connect(first_over, SIGNAL(timeout()), this, SLOT(switchScreen()));

    qreal wid = (QApplication::desktop()->width()) - (auto_pix.width()/2);
    for(int i=0; i<(wid/2.0); i++)
    {
        anim_auto->setPosAt((2.0*i)/(wid), QPointF(-i, -50));
    }
    /*Turn off the scroll bars*/
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    /*Add the items to the scene and add the scene to the view*/

    scene->addItem(smoke1);
    scene->addItem(smoke2);
    scene->addItem(smoke3);
    scene->addItem(auto_image);
    scene->addItem(title);
    view->setScene(scene);

    start_timer1->start();
}

void Doodle::createSmoke1()
{
    auto_image->setVisible(true);
    smoke1->setVisible(true);
    if(i1>=0)
    {
        i1--;
        smoke1->setOpacity(i1/10.0);
        smoke1->setPos(smoke1->x()-i1, smoke1->y()-i1);
    }
}

void Doodle::createSmoke2()
{
    smoke2->setVisible(true);
    if(i2>=0)
    {
        i2--;
        smoke2->setOpacity(i2/10.0);
        smoke2->setPos(smoke2->x()-i2, smoke2->y()-i2);
    }
}

void Doodle::createSmoke3()
{
    smoke3->setVisible(true);
    if(i3>=0)
    {
        i3--;
        smoke3->setOpacity(i3/10.0);
        smoke3->setPos(smoke3->x()-i3, smoke3->y()-i3);
    }
    if(i3 == 0)
        end_anim->start();

}

void Doodle::delTimer()
{
    start_timer1->stop();
}

void Doodle::endAnimation()
{
    end_anim->stop();
    start_timer2->stop();
    start_timer3->stop();
    timer_create1->stop();
    timer_create2->stop();
    timer_create3->stop();
    scene->removeItem(smoke1);
    scene->removeItem(smoke2);
    scene->removeItem(smoke3);
    title->setVisible(true);

    first_over->start();
/*
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    w.showMaximized();
#else
    w.show();
#endif/*/
}

void Doodle::switchScreen()
{
    first_over->stop();
    scene->removeItem(title);
    scene->removeItem(auto_image);

    MainMenu *mm=new MainMenu(this);
    mm->show();
}
