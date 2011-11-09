#include "mainwindow.h"
#include "mapwidget.h"

#include <QApplication>
#include <QMessageBox>
#include <QGeoCoordinate>
#include <QProcessEnvironment>
#include <QUrl>
#include <QNetworkProxy>
#include <QNetworkProxyFactory>
#include <QTimer>
#include <QAction>
#include <QMenuBar>
#include <qnetworkconfigmanager.h>
#include <QSvgRenderer>
#include <QPainter>
#include <QGeoMapRouteObject>

QTM_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        my_serviceProvider(0),
        my_mapWidget(0)
{
    flag = 0;

    my_geo = QGeoPositionInfoSource::createDefaultSource(this);
    my_geo->setUpdateInterval(1);
    connect(my_geo, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(updateValues(QGeoPositionInfo)));
    my_geo->startUpdates();

    flagTimer = new QTimer(this);
    flagTimer->setInterval(3500);
    flagTimer->start();

    connect(flagTimer, SIGNAL(timeout()), this, SLOT(timerCalled()));

    QGraphicsScene *sc = new QGraphicsScene;

    my_view = new QGraphicsView(sc, this);
    my_view->setVisible(true);
    my_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    my_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCentralWidget(my_view);

    this->setStyleSheet("QPushButton {color: black; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #008000, stop: 0.15 #008b00, stop: 0.5 #00cd00, stop: 0.85 #008b00, stop: 1 #008000); border-width: 1px; border-color: #339; border-style: solid; border-radius: 7; padding: 3px; font-size: 30px; padding-left: 5px; padding-right: 5px; min-width: 150px; min-height: 50px;}");
    my_back = new QPushButton("BACK", this);

}

MainWindow::~MainWindow()
{
    delete my_serviceProvider;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    my_view->setSceneRect(QRectF(QPointF(0.0, 0.0), my_view->size()));
    if (my_mapWidget)
        my_mapWidget->resize(my_view->size());
}

void MainWindow::showEvent(QShowEvent* event)
{
    resizeEvent(0);
}

void MainWindow::networkSessionOpened()
{
    QString urlEnv = QProcessEnvironment::systemEnvironment().value("http_proxy");
    if (!urlEnv.isEmpty()) {
        QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(url.host());
        proxy.setPort(url.port(8080));
        QNetworkProxy::setApplicationProxy(proxy);
    } else
        QNetworkProxyFactory::setUseSystemConfiguration(true);

    //get provider, we are hardcoding it to nokia
    setProvider("nokia");
    //set up the map widget
    setupMap();
}

void MainWindow::error(QNetworkSession::SessionError error)
{
    if (error == QNetworkSession::UnknownSessionError) {
        if (my_session->state() == QNetworkSession::Connecting) {
            QMessageBox msgBox(qobject_cast<QWidget *>(parent()));
            msgBox.setText("This application requires network access to function.");
            msgBox.setInformativeText("Press Cancel to quit the application.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Retry);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Retry) {
                QTimer::singleShot(0, my_session, SLOT(open()));
            } else if (ret == QMessageBox::Cancel) {
                close();
            }
        }
    } else if (error == QNetworkSession::SessionAbortedError) {
        QMessageBox msgBox(qobject_cast<QWidget *>(parent()));
        msgBox.setText("Out of range of network.");
        msgBox.setInformativeText("Move back into range and press Retry, or press Cancel to quit the application.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Retry);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Retry) {
            QTimer::singleShot(0, my_session, SLOT(open()));
        } else if (ret == QMessageBox::Cancel) {
            close();
        }
    }
}

void MainWindow::setProvider(QString providerId)
{
    if (my_serviceProvider)
        delete my_serviceProvider;
    my_serviceProvider = new QGeoServiceProvider(providerId);
    if (my_serviceProvider->error() != QGeoServiceProvider::NoError) {
        QMessageBox::information(this, tr("MapViewer Example"), tr(
                                     "Unable to find the %1 geoservices plugin.").arg(providerId));
        qApp->quit();
        return;
    }

    my_mapManager = my_serviceProvider->mappingManager();
    my_routingManager = my_serviceProvider->routingManager();
}

void MainWindow::setupMap()
{
    my_mapWidget = new MapWidget(my_mapManager);
    my_view->scene()->addItem(my_mapWidget);
    my_view->scene()->addWidget(my_back);
    my_mapWidget->setCenter(my_coord1);
    my_mapWidget->setZoomLevel(15);

    setupMenu();
    resizeEvent(0);

    QPair<QPixmap, QPoint> markerPair = makeStandardMarker(1,"Start", QPen(), QPen(QColor(Qt::white)), QBrush(QColor(Qt::green)));
    my_startIcon = new QGeoMapPixmapObject(my_coord2,markerPair.second,markerPair.first);
    my_mapWidget->addMapObject(my_startIcon);

    QPair<QPixmap, QPoint> markerPair2 = makeStandardMarker(1,"End", QPen(), QPen(QColor(Qt::white)), QBrush(QColor(Qt::yellow)));
    my_endIcon = new QGeoMapPixmapObject(my_coord1,markerPair2.second,markerPair2.first);
    my_mapWidget->addMapObject(my_endIcon);

}

void MainWindow::setupMenu()
{
    connect(my_geo, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(startRoute()));
}

QPair<QPixmap, QPoint> MainWindow::makeStandardMarker(int shapeType, const QString & text, const QPen & pen, const QPen & textPen, const QBrush & brush)
{
    qDebug() << "makeStandardMarker: " + QString::number(shapeType);

    QString svgTemplate;
    QPoint offset;
    switch (shapeType) {
        case 0:
            svgTemplate = "<svg>"
                          "<path "
                          "style=\"fill:#000000;fill-opacity:.4\" "
                          "d=\"m 18.948,33.432051 c 0,1.41694 -2.238,2.567949 -5,2.567949 -2.762,0 -5,-1.151009 -5,-2.567949 0,-1.416939 2.238,-2.567948 5,-2.567948 2.762,-0.002 5,1.149994 5,2.567948\" "
                          "/>"
                          "<path "
                          "style=\"fill:#666\" "
                          "d=\"M 14 0 C 6.28 0 0 6.3348796 0 14.125 C 0 16.209809 0.4425 18.209911 1.3125 20.09375 L 1.84375 21.125 C 2.96275 23.084964 12.2565 32.190555 13.3125 33.21875 L 14 33.875 L 14.6875 33.21875 C 15.7435 32.188525 25.026 23.109909 26.125 21.1875 L 26.65625 20.15625 C 27.54825 18.225721 28 16.204734 28 14.125 C 28.001 6.3348796 21.72 0 14 0 z M 14 0.90625 C 21.175 0.90625 27 6.7640687 27 14 C 27 15.982294 26.55825 17.873541 25.78125 19.5625 L 25.28125 20.53125 C 24.22125 22.386669 14 32.375 14 32.375 C 14 32.375 3.77875 22.387684 2.71875 20.53125 L 2.21875 19.5625 C 1.43975 17.875571 1 15.982294 1 14 C 1 6.7640687 6.823 0.90625 14 0.90625 z \" "
                          "/>"
                          "<path "
                          "style=\"fill:#fff\" "
                          "d=\"M 14 0.90625 C 6.823 0.90625 1 6.7640687 1 14 C 1 15.982294 1.43975 17.875571 2.21875 19.5625 L 2.71875 20.53125 C 3.77875 22.387684 14 32.375 14 32.375 C 14 32.375 24.22125 22.386669 25.28125 20.53125 L 25.78125 19.5625 C 26.55825 17.873541 27 15.982294 27 14 C 27 6.7640687 21.175 0.90625 14 0.90625 z M 14 1.90625 C 20.616 1.90625 26 7.3294234 26 14 C 25.910217 16.115917 25.491329 18.196844 24.40625 20.03125 C 23.776565 21.248455 17.937 27.075427 14 30.9375 C 10.063 27.075427 4.206579 21.245609 3.59375 20.0625 C 2.614452 18.171896 1.90079 16.186259 2 14 C 2 7.3294234 7.383 1.90625 14 1.90625 z \" "
                          "/>"
                          "<path "
                          "style=\"fill:%brush%;fill-opacity:%brushOpacity%\" "
                          "d=\"m 13.996268,1.9105251 c -6.617,0 -12.001,5.4261873 -12.001,12.0967639 -0.09921,2.186259 0.631702,4.174018 1.611,6.064622 0.612829,1.183109 6.453,7.017707 10.39,10.87978 3.937,-3.862073 9.777315,-9.70013 10.407,-10.917335 1.085079,-1.834406 1.503217,-3.91115 1.593,-6.027067 0,-6.6705766 -5.384,-12.0967639 -12,-12.0967639 l 0,0 z\" "
                          "/>"
                          "<path style=\"fill:#ffffff;fill-opacity:0.7\" "
                          "d=\"M 20.968528,6.089997 C 17.785074,3.5240085 13.725682,2.5379787 11.919627,3.8924751 11.436514,4.2548035 11.151528,4.750748 11.073071,5.3665525 c 4.1092,0.6284223 7.111478,2.6511761 8.521564,4.4853881 2.388133,3.1064364 1.967904,3.9515754 1.967904,3.9515754 0.774274,-0.0387 1.422004,-0.242256 1.904023,-0.603757 1.806055,-1.354497 0.684944,-4.5441591 -2.498034,-7.109762 l 0,0 z\" "
                          "/>"
                          "<text "
                          "x=\"14\" y=\"19\" "
                          "font-size=\"10pt\" font-family=\"arial\" font-weight=\"bold\" text-anchor=\"middle\" "
                          "fill=\"%textPen%\" "
                          "textContent = \"%textContent%\""
                          ">"
                          "%text%"
                          "</text>"
                          "</svg>";
            offset = QPoint(-14, -34);
            break;

        case 1:
            svgTemplate = "<svg><path style=\"fill:%brush%;fill-opacity:%brushOpacity%\" d=\"M 16.00663,.0000019037492 C 7.1692812,.0000019037492 0,7.1692841 0,16.006632 0,24.843981 7.1692812,32 16.00663,32 24.84398,32 32,24.843981 32,16.006632 32,7.1692841 24.84398,.0000019037492 16.00663,.0000019037492 z M 15.847492,0.39784686 L 19.136344,11.484461 30.766679,10.993786 21.178615,17.544966 25.236635,28.379612 16.00663,21.324494 6.8959804,28.512225 10.794861,17.624536 1.1007042,11.232493 12.73104,11.537507 15.847492,0.39784686 z\" /><path style=\"fill:%pen%;fill-opacity:%penOpacity%\" d=\"M 15.847492,0.39784686 L 19.136344,11.484461 30.766679,10.993786 21.178615,17.544966 25.236635,28.379612 16.00663,21.324494 6.8959804,28.512225 10.794861,17.624536 1.1007042,11.232493 12.73104,11.537507 15.847492,0.39784686 z\" /><text x=\"16\" y=\"20\" font-size=\"10pt\" font-family=\"arial\" font-weight=\"bold\" text-anchor=\"middle\" fill=\"%textPen%\" textContent = \"%textContent%\">%text%</text></svg>";
            offset = QPoint(-16, -16);
            break;
    }

    svgTemplate.replace("%text%", text, Qt::CaseSensitive);
    svgTemplate.replace("%pen%", pen.color().name(), Qt::CaseSensitive);
    svgTemplate.replace("%penOpacity%", QString::number(pen.color().alpha() / 255.0), Qt::CaseSensitive);
    svgTemplate.replace("%textPen%", textPen.color().name(), Qt::CaseSensitive);
    svgTemplate.replace("%brush%", brush.color().name(), Qt::CaseSensitive);
    svgTemplate.replace("%brushOpacity%", QString::number(brush.color().alpha() / 255.0), Qt::CaseSensitive);
    svgTemplate.replace("%textContent%", "", Qt::CaseSensitive);

    QSvgRenderer renderer(svgTemplate.toAscii());
    QPixmap pixmap(renderer.defaultSize());
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);
    renderer.render(&painter);

    return qMakePair(pixmap, offset);
}
void MainWindow::startRoute()
{
    if(my_coord1 == my_coord2)
        return;
    qDebug()<<"start route";
    QList<QGeoCoordinate> waypoints;

    waypoints.append(my_startIcon->coordinate());
    waypoints.append(my_coord1);

    QGeoRouteRequest req(waypoints);
    QGeoRouteReply *reply = my_routingManager->calculateRoute(req);

    QObject::connect(reply, SIGNAL(finished()), this,SLOT(routeFinished()));
    QObject::connect(reply,SIGNAL(error(QGeoRouteReply::Error, QString)), this,SLOT(resultsError(QGeoRouteReply::Error, QString)));
}

void MainWindow::routeFinished()
{
    QGeoRouteReply *reply = static_cast<QGeoRouteReply*>(sender());

    if (reply->error() != QGeoRouteReply::NoError) {
        // Errors are handled in a different slot (resultsError)
        return;
    }

    if (!reply)
        return;

    if (reply->routes().size() < 1)
        return;

    QGeoMapRouteObject *route = new QGeoMapRouteObject(reply->routes().at(0));
    QColor routeColor(Qt::blue);
    routeColor.setAlpha(127); //semi-transparent
    QPen pen(routeColor);
    pen.setWidth(7);
    pen.setCapStyle(Qt::RoundCap);
    route->setPen(pen);
    my_mapWidget->addMapObject(route);
    my_mapWidget->setCenter(my_coord1);

    disconnect(reply, SIGNAL(finished()), this,SLOT(routeFinished()));
    disconnect(reply,SIGNAL(error(QGeoRouteReply::Error, QString)), this,SLOT(resultsError(QGeoRouteReply::Error, QString)));
    reply->deleteLater();
}

void MainWindow::resultsError(QGeoRouteReply::Error errorCode, QString errorString)
{
    QObject* reply = static_cast<QGeoRouteReply *>(sender());

    (new QMessageBox(
         QMessageBox::Information,
         tr("Revgeo Example"),
         tr("Error #%1 while trying to process your search query.\n\"%2\"").arg(errorCode).arg(errorString),
         0,
         this
     ))->show();

    disconnect(reply, SIGNAL(finished()), this,SLOT(routeFinished()));
    disconnect(reply,SIGNAL(error(QGeoRouteReply::Error, QString)), this,SLOT(resultsError(QGeoRouteReply::Error, QString)));
    reply->deleteLater();
}

void MainWindow::updateValues(QGeoPositionInfo gp)
{
    if(flag == 0) {
        my_coord2 = my_coord1;
        flag = 1;
    }
    else if(flag == 1) {
        my_coord2 = my_coord1;
        //doRouteAct->trigger();
        flag = 2;
    }
    my_coord1 = gp.coordinate();

    qDebug()<<QString::number((gp.coordinate().latitude()));
    qDebug()<<QString::number((gp.coordinate().longitude()));
}

void MainWindow::timerCalled()
{
    setWindowTitle(tr("Simple route"));

    // Set Internet Access Point
    QNetworkConfigurationManager manager;
    const bool canStartIAP = (manager.capabilities()
                              & QNetworkConfigurationManager::CanStartAndStopInterfaces);

    // Is there default access point, use it
    QNetworkConfiguration cfg = manager.defaultConfiguration();
    if (!cfg.isValid() || (!canStartIAP && cfg.state() != QNetworkConfiguration::Active)) {
        QMessageBox::information(this, tr("Map Viewer Demo"), tr("Available Access Points not found."));
        return;
    }

    //set callbacks and start internet connection
    my_session = new QNetworkSession(cfg, this);
    connect(my_session, SIGNAL(opened()), this, SLOT(networkSessionOpened()));
    connect(my_session,SIGNAL(error(QNetworkSession::SessionError)),this,SLOT(error(QNetworkSession::SessionError)));
    my_session->open();


    flagTimer->stop();
}
