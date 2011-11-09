#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QtCore>
#include <QtGui>
#include <QComboBox>
#include <QGeoPositionInfoSource>

#include "mainwindow.h"

using namespace QtMobility;

struct AutoRate{
    qreal min_rate;
    qreal rate_per_km;
    QString m_rate;
    QString r_perkm;

    AutoRate(int index)
    {
        calculateRates(index);
    }

    void calculateRates(int index)
    {
        switch(index)
        {
             case 0:
                 min_rate = 17.0;
                    rate_per_km = 9.0;
                    m_rate = "17.0";
                    r_perkm = "9.0";
                    break;
                case 1:
                    min_rate = 14.0;
                    rate_per_km = 6.0;
                    m_rate = "14.0";
                    r_perkm = "6.0";
                    break;
                case 2:
                    min_rate = 11.0;
                    rate_per_km = 7.0;
                    m_rate = "11.0";
                    r_perkm = "7.0";
                    break;
                case 3:
                    min_rate = 14.0;
                    rate_per_km = 8.0;
                    m_rate = "14.0";
                    r_perkm = "8.0";
                    break;
                case 4:
                    min_rate = 19.0;
                    rate_per_km = 6.5;
                    m_rate = "19.0";
                    r_perkm = "6.50";
                    break;
                case 5:
                    min_rate = 11.0;
                    rate_per_km = 9.0;
                    m_rate = "11.0";
                    r_perkm = "9.0";
                    break;
                case 6:
                    min_rate = 9.0;
                    rate_per_km = 6.0;
                    m_rate = "9.0";
                    r_perkm = "6.0";
                    break;
                case 7:
                    min_rate = 19.0;
                    rate_per_km = 6.5;
                    m_rate = "19.0";
                    r_perkm = "6.5";
                    break;
                case 8:
                    min_rate = 14.0;
                    rate_per_km = 7.0;
                    m_rate = "14.0";
                    r_perkm = "7.0";
                    break;
                case 9:
                    min_rate = 15.0;
                    rate_per_km = 10.0;
                    m_rate = "15.0";
                    r_perkm = "10.0";
                    break;
                case 10:
                    min_rate = 9.0;
                    rate_per_km = 5.0;
                    m_rate = "9.0";
                    r_perkm = "5.0";
                    break;
                case 11:
                    min_rate = 15.0;
                    rate_per_km = 8.0;
                    m_rate = "15.0";
                    r_perkm = "8.0";
                    break;
                case 12:
                    min_rate = 10.5;
                    rate_per_km = 7.0;
                    m_rate = "10.5";
                    r_perkm = "7.0";
                    break;
                case 13:
                    min_rate = 20.0;
                    rate_per_km = 10.0;
                    m_rate = "20.0";
                    r_perkm = "10.0";
                    break;
                }
    }
};


class StartPage : public QWidget
{
    Q_OBJECT

    QPushButton *click_me;
    QLineEdit *min_fare, *fare_per_km;
    QLabel *min, *per_km;
    QHBoxLayout *line1, *line2;
    QVBoxLayout *page;
    QFrame *frame;
    QMessageBox *final;
    QComboBox *cb;

    AutoRate *ar;

    QGeoPositionInfoSource *m_geo;

    int flag, flag2;
    qreal m_lat1, m_long1, m_lat2, m_long2;
    qreal m_dist, m_amt;

    QPalette startp, stopp;

    MainWindow mw;

public:
    QPushButton *my_toMap;

    explicit StartPage(QWidget *parent = 0);
    void calculateAmount();

signals:

public slots:
    void changeRates();
    void startClicked();
    void updateValues( QGeoPositionInfo gp) ;

};

#endif // STARTPAGE_H
