#include "ReferenceDockerDock.h"
#include "KoID.h"

ReferenceDockerDock::ReferenceDockerDock()
    : QDockWidget(i18n("Reference Docker"))
{
    QWidget *page = new QWidget(this);

    setWidget(page);
}

void ReferenceDockerDock::setViewManager(KisViewManager* vm) {
    m_vm = vm;
}

void ReferenceDockerDock::setCanvas(KoCanvasBase *canvas) {
    m_c = canvas;
}
