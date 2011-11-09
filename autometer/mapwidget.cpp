#include "mapwidget.h"
#include <QDebug>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QGeoMapPixmapObject>


QTM_USE_NAMESPACE



MapWidget::MapWidget(QGeoMappingManager *manager) : QGraphicsGeoMap(manager)
{
    m_panActive = false;
    m_dragging = false;

    m_currentObj = 0;
}

MapWidget::~MapWidget() {}


void MapWidget::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setFocus();
    if (event->button() == Qt::LeftButton) {
        m_panActive = true;
        m_currentObj = 0;

        QList<QGeoMapObject*> objList = mapObjectsAtScreenPosition(event->pos());
        for(int i=0; i < objList.count(); i++){
            if(QGeoMapObject::PixmapType == objList[i]->type()){
                m_currentObj = objList[i];
                break;
            }
        }
    }

    event->accept();
}

void MapWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {

        if(m_currentObj){
            if(m_dragging){
                if (QGeoMapPixmapObject *iccon = qobject_cast<QGeoMapPixmapObject *>(m_currentObj)) {
                    iccon->setCoordinate(screenPositionToCoordinate(event->pos()));
                }
            }
        }

        m_dragging = false;
        m_panActive = false;
        m_currentObj = 0;
    }

    event->accept();
}

void MapWidget::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentObj){
        m_dragging = true;
        if (QGeoMapPixmapObject *iccon = qobject_cast<QGeoMapPixmapObject *>(m_currentObj)) {
            iccon->setCoordinate(screenPositionToCoordinate(event->pos()));
        }
    }else if(m_panActive){
        pan(event->lastPos().x() - event->pos().x(), event->lastPos().y() - event->pos().y());
    }

    event->accept();
}

void MapWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setFocus();

    m_dragging = false;
    m_panActive = false;
    m_currentObj = 0;

    pan(event->lastPos().x() -  size().width() / 2.0, event->lastPos().y() - size().height() / 2.0);
    if (zoomLevel() < maximumZoomLevel())
        setZoomLevel(zoomLevel() + 1);

    event->accept();
}
