#ifndef REFERENCEDOCKER_H
#define REFERENCEDOCKER_H

#include <QObject>

class ReferenceDockerPlugin : public QObject
{
    Q_OBJECT
public:
    ReferenceDockerPlugin(QObject *parent, const QVariantList &);
    ~ReferenceDockerPlugin() override;
};

#endif // REFERENCEDOCKER_H
