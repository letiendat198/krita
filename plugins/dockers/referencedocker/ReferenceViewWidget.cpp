#include "ReferenceViewWidget.h"

#include <QGraphicsItem>
#include <qevent.h>

ReferenceViewWidget::ReferenceViewWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene())
{
    this->setScene(m_scene.data());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);
}

void ReferenceViewWidget::setReferenceImage(KisReferenceImage *image) {
    if (!image) return;

    m_scene->clear();

    QPixmap pixmap = QPixmap::fromImage(image->getImage());
    m_item = m_scene->addPixmap(pixmap);
    m_item->setFlag(QGraphicsItem::ItemIsMovable, true);

    resetView();
}

void ReferenceViewWidget::resetView() {
    if (m_item) {
        m_item->setPos(0, 0);
        this->fitInView(m_item, Qt::KeepAspectRatio);
        this->centerOn(m_item);

        m_baseFactor = this->transform().m11();

        Q_EMIT scaleChanged(1);
    }
}

void ReferenceViewWidget::clearView() {
    m_scene->clear();
    m_item = nullptr;
}

void ReferenceViewWidget::setScale(qreal value) {
    qreal trueFactor = m_baseFactor + value - 1;

    // Won't work if we have rotate
    this->setTransform(QTransform().scale(trueFactor, trueFactor), false);

    Q_EMIT scaleChanged(trueFactor);
}

void ReferenceViewWidget::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);

    if (!m_item) return;

    QPoint mousePos = event->pos();
    QPointF scenePos = this->mapToScene(mousePos);
    QPointF itemPos = m_item->mapFromScene(scenePos);

    QColor color = static_cast<QGraphicsPixmapItem*>(m_item)->pixmap().toImage().pixel(itemPos.toPoint());

    Q_EMIT colorSampled(color);
    // dbgUI << "Color" << color.name();
}
