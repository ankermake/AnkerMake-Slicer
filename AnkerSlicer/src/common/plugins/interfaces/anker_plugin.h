#ifndef ANKER_PLUGIN_H
#define ANKER_PLUGIN_H

#include "meshlab_plugin_logger.h"
#include "meshlab_plugin.h"
#include <QObject>

class AnkerPlugin : public MeshLabPlugin
{

};

#define Anker_PLUGIN_IID  "Anker.AnkerPlugin/1.0"
Q_DECLARE_INTERFACE(AnkerPlugin, Anker_PLUGIN_IID)


class AnkerPluginBase : public QObject
{
    Q_OBJECT
public:
    friend class PluginManager;

    AnkerPluginBase() : enabled(true) {}
    virtual ~AnkerPluginBase()        {}

    virtual void initialize(ControlInterface* controlmanager, RichParameterList* globalParameterList) = 0;
    virtual QString pluginName() const = 0;

    
    virtual bool needSaveProject()           { return false; }
    virtual bool saveProject(const QString&) { return false; }
    virtual bool needLoadProject()           { return false; }
    virtual bool loadProject(const QString&) { return false; }

    bool isEnabled() const { return enabled; }
    QFileInfo pluginFileInfo() const { return plugFileInfo; }

    
    virtual void recMsgfromManager(PluginMessageData) = 0;
    
    virtual void sendMsg2Manager(PluginMessageData)   = 0;

private:
    void enable() { enabled = true; }
    void disable() { enabled = false; }
    bool enabled;
    QFileInfo plugFileInfo;
};















#endif // ANKER_PLUGIN_H
