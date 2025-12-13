#ifndef REFERENCEDOCKERDOCK_H
#define REFERENCEDOCKERDOCK_H

#include "kis_mainwindow_observer.h"
#include <QDockWidget>
#include <Ui_WdgReferenceDocker.h>


class ReferenceDockerDock : public QDockWidget, public KisMainwindowObserver, public Ui_WdgReferenceDocker
{
public:
    ReferenceDockerDock();
    QString observerName() override { return "ReferenceDockerDock"; }
    void setViewManager(KisViewManager *viewManager) override;
    void setCanvas(KoCanvasBase *canvas) override;
    void unsetCanvas() override {}
private:
    KisViewManager *m_vm;
    KoCanvasBase *m_c;
};

#endif // REFERENCEDOCKERDOCK_H
