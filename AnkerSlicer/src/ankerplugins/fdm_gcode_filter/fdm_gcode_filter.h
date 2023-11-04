#ifndef FDM_GCODE_FILTER_H
#define FDM_GCODE_FILTER_H

#include <QTimer>
#include <QThread>
#include <QDebug>
#include "common/plugins/interfaces/anker_plugin.h"
#include "common/ak_const.h"


//  @2023-03-01 by ChunLian
class FdmGcodeFilter : public QObject, public AnkerPlugin
{
    Q_OBJECT
    MESHLAB_PLUGIN_IID_EXPORTER(Anker_PLUGIN_IID)
    Q_INTERFACES(AnkerPlugin)
    QString pluginName() const final{return AkConst::Plugin::FDM_GCODE_FILTER;}
public:
    FdmGcodeFilter(){}
    ~FdmGcodeFilter(){}
    void initialize(ControlInterface *, RichParameterList * globalParameterList) override final;
    static RichParameterList* s_globalParameterList;
public slots:
    void recMsgfromManager(PluginMessageData msgBody)override final;
signals:
    void sendMsg2Manager(PluginMessageData msgBody)override final;

};



#endif // FDM_GCODE_FILTER_H
