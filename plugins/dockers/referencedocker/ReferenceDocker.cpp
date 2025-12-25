#include "ReferenceDocker.h"

#include <KoDockRegistry.h>
#include <ReferenceDockerDock.h>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY_WITH_JSON(ReferenceDockerPluginFactory,
                           "krita_referencedocker.json",
                           registerPlugin<ReferenceDockerPlugin>();)

class ReferenceDockerDockFactory : public KoDockFactoryBase {
 public:
    ReferenceDockerDockFactory()
    {
    }

    virtual ~ReferenceDockerDockFactory()
    {
    }

    QString id() const override
    {
      return QString( "ReferenceDocker" );
    }

    virtual Qt::DockWidgetArea defaultDockWidgetArea() const
    {
        return Qt::RightDockWidgetArea;
    }

    QDockWidget* createDockWidget() override
    {
        ReferenceDockerDock *dockWidget = new ReferenceDockerDock();
        dockWidget->setObjectName(id());
        return dockWidget;
    }

    DockPosition defaultDockPosition() const override
    {
        return DockMinimized;
    }
private:
};

ReferenceDockerPlugin::ReferenceDockerPlugin(QObject *parent, const QVariantList &)
    : QObject(parent)
{
    KoDockRegistry::instance()->add(new ReferenceDockerDockFactory());
}

ReferenceDockerPlugin::~ReferenceDockerPlugin() {

}


#include "ReferenceDocker.moc"
