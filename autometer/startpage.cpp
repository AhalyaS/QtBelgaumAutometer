#include "startpage.h"

StartPage::StartPage(QWidget *parent) :
    QWidget(parent)
{
    QPixmap back_pix("C:\\QtSDK\\autometer\\Images\\back.png");
    back_pix.scaled(QSize(QApplication::desktop()->width(), QApplication::desktop()->height()), Qt::IgnoreAspectRatio);

    final = new QMessageBox(this);

    cb = new QComboBox();    
    cb->addItem("Bangalore, Karnataka");
    cb->addItem("Chennai, Tamil Nadu");
    cb->addItem("Mumbai, Maharashtra");
    cb->addItem("Hyderabad, Andhrra Pradesh");
    cb->addItem("Delhi, Delhi");
    cb->addItem("Pune, Maharashtra");
    cb->addItem("Ahmedebad, Gujurat");
    cb->addItem("Chandigarh, Haryana");
    cb->addItem("Coimbatore, Tamil Nadu");
    cb->addItem("Dharwad, Karnataka");
    cb->addItem("Jaipur, Rajasthan");
    cb->addItem("Nagpur, Madhya Pradesh");
    cb->addItem("Bhopal, Madhya Pradesh");
    cb->addItem("Patna, Bihar");
    ar = new AutoRate(cb->currentIndex());

    frame = new QFrame();

    frame->setStyleSheet("QPushButton, QLineEdit, QLabel, QComboBox {color: black; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #008000, stop: 0.15 #008b00, stop: 0.5 #00cd00, stop: 0.85 #008b00, stop: 1 #008000); border-width: 1px; border-color: #339; border-style: solid; border-radius: 7; padding: 3px; font-size: 30px; padding-left: 5px; padding-right: 5px; min-width: 150px; min-height: 50px;}");
    click_me = new QPushButton("START", this);
    my_toMap = new QPushButton("TO MAP", this);

    min_fare = new QLineEdit(ar->m_rate, this);
    fare_per_km = new QLineEdit(ar->r_perkm, this);

    min = new QLabel("Min Fare", this);
    per_km = new QLabel("Per KM", this);

    line1 = new QHBoxLayout();
    line1->addWidget(min);
    line1->addWidget(min_fare);

    line2 = new QHBoxLayout();
    line2->addWidget(per_km);
    line2->addWidget(fare_per_km);

    page = new QVBoxLayout();
    page->addWidget(cb);
    page->addItem(line1);
    page->addItem(line2);
    page->addWidget(click_me);

    page->addWidget(my_toMap);

    frame->setMinimumSize(QApplication::desktop()->width(), QApplication::desktop()->height()-50);
    frame->setAutoFillBackground(true);
    frame->setLayout(page);

    QPalette p = palette();
    p.setBrush(QPalette::Background, back_pix);
    frame->setPalette(p);

    m_geo = QGeoPositionInfoSource::createDefaultSource(this);
    m_geo->setUpdateInterval(100);
    connect(m_geo,SIGNAL(positionUpdated(QGeoPositionInfo)),this,SLOT(updateValues(QGeoPositionInfo)));
    m_geo->startUpdates();

    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRates()));
    connect(click_me, SIGNAL(clicked()), this, SLOT(startClicked()));

    frame->show();
}

void StartPage::changeRates()
{
    ar->calculateRates(cb->currentIndex());
    min_fare->setText(ar->m_rate);
    fare_per_km->setText(ar->r_perkm);
    update();
}

void StartPage::startClicked()
{
    if(click_me->text() == "START")
    {
        m_amt = 0;
        m_dist = 0;
        click_me->setText("STOP");
        flag = 1;
    }
    else
    {
        calculateAmount();
        click_me->setText("START");
        flag = 0;
    }
    update();
}

void StartPage::calculateAmount()
{
    if(m_dist > 2)
        m_amt = ar->min_rate + (m_dist - 2)*ar->rate_per_km;
    else
        m_amt = ar->min_rate;
    m_amt = qRound(m_amt);
    final->setText("The amount is "+QString::number(m_amt)+" and the distance travelled is "+QString::number(m_dist));
    final->exec();
}

void StartPage::updateValues(QGeoPositionInfo gp)
{
    if(flag2 == 0)
    {
        m_lat2 = gp.coordinate().latitude();
        m_long2 = gp.coordinate().longitude();
        flag2 = 1;
    }
    else
    {
        m_lat1 = m_lat2;
        m_long1 = m_long2;
        m_lat2 = gp.coordinate().latitude();
        m_long2 = gp.coordinate().longitude();
        qreal dLat = (m_lat2 - m_lat1) * 22/(7*180*2.0);
        qreal dLong = (m_long2 - m_long1) * 22/(7*180*2.0);
        qreal a = (qSin(dLat)*qSin(dLat)) + (qSin(dLong)*qSin(dLong)*qCos(m_lat1)*qCos(m_lat2));
        qreal c =2.0*(qAtan2(qSqrt(a), qSqrt(1.0-a)));
        m_dist += (6371*c);
    }
}
