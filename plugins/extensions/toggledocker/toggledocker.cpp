#include "toggledocker.h"

#include "KisViewManager.h"
#include "KisMainWindow.h"
#include <kpluginfactory.h>
#include <kis_action.h>
#include <KoDockRegistry.h>

K_PLUGIN_FACTORY_WITH_JSON(ToggleDockerFactory, "krita_toggledocker.json", registerPlugin<ToggleDocker>();)

ToggleDocker::ToggleDocker(QObject *parent, const QVariantList &) : KisActionPlugin(parent)
{
    KisAction *action = createAction("toggle_layer");
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slotToggleLayer(bool)));
}

ToggleDocker::~ToggleDocker() {

}

void ToggleDocker::slotToggleLayer(bool show) {
    KisMainWindow *main = viewManager()->mainWindow();
    main->dockWidget("KisLayerBox")->setVisible(show);
}


#include "toggledocker.moc"
