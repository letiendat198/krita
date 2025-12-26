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
    void mouseReleaseEvent(QMouseEvent *event) override;
Q_SIGNALS:
    void scaleChanged(qreal value);
    void colorSampled(QColor color);

private:
    QScopedPointer<QGraphicsScene> m_scene {nullptr};
    QGraphicsItem *m_item = nullptr;
    qreal m_baseFactor = 0;
};

#endif // REFERENCEVIEWWIDGET_H
