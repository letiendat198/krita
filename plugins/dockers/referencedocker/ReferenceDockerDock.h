#ifndef REFERENCEDOCKERDOCK_H
#define REFERENCEDOCKERDOCK_H

#include <QDockWidget>
#include <ui_WdgReferenceDocker.h>

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
public Q_SLOTS:
    void referenceImageChanged();
    void slotZoomSliderChanged(int value);
    void changeCurrentImage(int index);
    void slotViewScaleChanged(qreal value);
    void slotViewChanged();
private:
    KisViewManager *m_vm = nullptr;
    KisDocument *m_document = nullptr;
    QScopedPointer<Ui_WdgReferenceDocker> m_ui {nullptr};
    int m_index = 0;
};

#endif // REFERENCEDOCKERDOCK_H
