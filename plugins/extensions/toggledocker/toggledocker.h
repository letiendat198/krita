#ifndef TOGGLEDOCKER_H
#define TOGGLEDOCKER_H

#include <KisActionPlugin.h>
#include <kis_types.h>
#include <kis_node.h>
#include <QObject>

class ToggleDocker : public KisActionPlugin
{
    Q_OBJECT
public:
    ToggleDocker(QObject *parent, const QVariantList &);
    ~ToggleDocker() override;
public Q_SLOTS:
    void slotToggleLayer(bool show);
};

#endif // TOGGLEDOCKER_H
