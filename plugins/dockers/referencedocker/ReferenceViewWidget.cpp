#include "ReferenceViewWidget.h"

#include <QGraphicsItem>
#include <qevent.h>

#include <libs/widgetutils/kis_cursor.h>

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

    m_image = image;
    m_scene->clear();

    QPixmap pixmap = QPixmap::fromImage(image->getImage());
    m_item = m_scene->addPixmap(pixmap);
    m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_item->setCursor(KisCursor::openHandCursor());

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
    m_image = nullptr;
}

void ReferenceViewWidget::setScale(qreal value) {
    qreal trueFactor = m_baseFactor * value;
    // Won't work if we have rotate
    this->setTransform(QTransform().scale(trueFactor, trueFactor), false);
    Q_EMIT scaleChanged(trueFactor);
}

void ReferenceViewWidget::setColorSampleMode(bool value) {
    m_sampleMode = value;
    if (!m_item) return;

    if (value) m_item->setCursor(KisCursor::samplerCursor());
    else m_item->setCursor(KisCursor::openHandCursor());
}

void ReferenceViewWidget::mousePressEvent(QMouseEvent *event) {
    if (!m_sampleMode || !m_item || !m_image) {
        QGraphicsView::mousePressEvent(event);
        if (m_item) m_item->setCursor(KisCursor::closedHandCursor());
        return;
    }

    QPointF scenePos = this->mapToScene(event->pos());
    QPointF itemPos = m_item->mapFromScene(scenePos);
    QColor color = m_image->getImage().pixel(itemPos.toPoint());

    QSizeF scaledSize = SAMPLER_PREVIEW_SIZE / this->transform().m11();

    m_rectItem = m_scene->addRect(QRectF(QPointF(0,0), scaledSize), QPen(Qt::NoPen), QBrush(color));
    m_rectItem->setPos(scenePos - QPointF(m_rectItem->rect().width(), m_rectItem->rect().height()));
}

// Because of setCursor, mouseMoveEvent will fire even when not pressed
// Hence the check for m_rectItem
void ReferenceViewWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!m_sampleMode || !m_item || !m_image || !m_rectItem) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }

    QPointF scenePos = this->mapToScene(event->pos());
    QPointF itemPos = m_item->mapFromScene(scenePos);
    QColor color = m_image->getImage().pixel(itemPos.toPoint());

    m_rectItem->setPos(scenePos - QPointF(m_rectItem->rect().width(), m_rectItem->rect().height()));
    m_rectItem->setBrush(QBrush(color));
    m_rectItem->update();
}

void ReferenceViewWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (!m_sampleMode || !m_item || !m_image) {
        QGraphicsView::mouseReleaseEvent(event);
        if (m_item) m_item->setCursor(KisCursor::openHandCursor());
        return;
    }

    QPointF scenePos = this->mapToScene(event->pos());
    QPointF itemPos = m_item->mapFromScene(scenePos);
    QColor color = m_image->getImage().pixel(itemPos.toPoint());

    m_scene->removeItem(m_rectItem);
    delete m_rectItem;
    m_rectItem = nullptr;

    Q_EMIT colorSampled(color);
}
