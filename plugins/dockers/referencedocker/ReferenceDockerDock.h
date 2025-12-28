#ifndef REFERENCEDOCKERDOCK_H
#define REFERENCEDOCKERDOCK_H

#include <QDockWidget>
#include <QPointer>
#include <ui_WdgReferenceDocker.h>

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QEnterEvent>
#endif

#include "KisReferenceImagesLayer.h"
#include "kis_mainwindow_observer.h"

class ReferenceDockerDock : public QDockWidget, public KisMainwindowObserver
{
    Q_OBJECT
public:
    ReferenceDockerDock();
    QString observerName() override { return "ReferenceDockerDock"; }
    void setViewManager(KisViewManager *viewManager) override;
    void setCanvas(KoCanvasBase *canvas) override;
    void unsetCanvas() override {}
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void enterEvent(QEnterEvent *event) override;
#else
    void enterEvent(QEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;
public Q_SLOTS:
    void referenceImageChanged();
    void slotZoomSliderChanged(int value);
    void changeCurrentImage(int index);
    void slotViewScaleChanged(qreal value);
    void slotViewChanged();
    void slotColorSampled(const QColor &color);
private:
    QPointer<KisViewManager> m_vm = nullptr;
    QPointer<KisDocument> m_document = nullptr;
    QScopedPointer<Ui_WdgReferenceDocker> m_ui {nullptr};
    int m_index = 0;
};

#endif // REFERENCEDOCKERDOCK_H
