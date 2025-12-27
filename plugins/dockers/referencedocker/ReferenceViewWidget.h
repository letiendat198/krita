#ifndef REFERENCEVIEWWIDGET_H
#define REFERENCEVIEWWIDGET_H

#include <KisReferenceImage.h>
#include <QGraphicsView>

class ReferenceViewWidget : public QGraphicsView {
    Q_OBJECT
public:
    ReferenceViewWidget(QWidget *parent);
    void setReferenceImage(KisReferenceImage *image);
    void resetView();
    void clearView();
    void setScale(qreal value);
    void setColorSampleMode(bool value);
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
Q_SIGNALS:
    void scaleChanged(qreal value);
    void colorSampled(const QColor &color);

private:
    QScopedPointer<QGraphicsScene> m_scene {nullptr};
    QGraphicsItem *m_item = nullptr;
    QGraphicsRectItem *m_rectItem = nullptr;
    qreal m_baseFactor = 0; // The view scale so that item fits
    bool m_sampleMode = false;
    KisReferenceImage *m_image = nullptr;

    const QSizeF SAMPLER_PREVIEW_SIZE = QSizeF(50,50);
};

#endif // REFERENCEVIEWWIDGET_H
