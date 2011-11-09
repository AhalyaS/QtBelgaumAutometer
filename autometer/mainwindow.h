/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qgeomappingmanager.h>

#include <QMainWindow>
#include <QGraphicsView>
#include <QGeoServiceProvider>
#include <QGeoRoutingManager>
#include <qnetworksession.h>
#include <QGeoMapPixmapObject>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include <QPushButton>
#include <QTimer>

class QResizeEvent;
class QShowEvent;

QTM_USE_NAMESPACE
class MapWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int flag;
    QTimer *flagTimer;
    QAction *doRouteAct;

public:
    QPushButton *my_back;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event);
    void showEvent(QShowEvent *);

private:
    void setupMap();
    void setupMenu();
    void setProvider(QString providerId);
    QPair<QPixmap, QPoint> makeStandardMarker(int shapeType, const QString & text, const QPen & pen, const QPen & textPen, const QBrush & brush);


private slots:
    void networkSessionOpened();
    void error(QNetworkSession::SessionError error);
    void startRoute();

    void resultsError(QGeoRouteReply::Error errorCode, QString errorString);
    void routeFinished();

    void timerCalled();
public slots:
    void updateValues(QGeoPositionInfo gp);

private:
    QGeoPositionInfoSource *my_geo;
    QGeoCoordinate my_coord1;
    QGeoCoordinate my_coord2;

    QGeoServiceProvider *my_serviceProvider;
    QGeoMappingManager *my_mapManager;
    QGeoRoutingManager *my_routingManager;
    MapWidget *my_mapWidget;

    QGraphicsView *my_view;

    QNetworkSession *my_session;
    QGeoMapPixmapObject *my_startIcon;
    QGeoMapPixmapObject *my_endIcon;
};

#endif // MAINWINDOW_H
