#ifndef FDM_SETTING_H
#define FDM_SETTING_H

#include <common/plugins/interfaces/anker_plugin.h>
#include <common/controlInterface/controlInterface.h>
#include "../../common/ak_const.h"
#include "materialwidget.h"
#include "machinewidget.h"
#include "parameterswidget.h"
using namespace control;

namespace fdmsettings {
class FdmParamSettingsWidget;

class FdmSettingPlugin : public QObject, public AnkerPlugin
{
    Q_OBJECT
    MESHLAB_PLUGIN_IID_EXPORTER(Anker_PLUGIN_IID)
    Q_INTERFACES(AnkerPlugin)
    QString pluginName() const final{return AkConst::Plugin::FDM_SETTING;}

    
    bool needSaveProject() override { return true; }
    bool saveProject(const QString& proPath) override;
    bool needLoadProject() override { return true; }
    bool loadProject(const QString& proPath) override;

    
public:
    void recMsgfromManager(PluginMessageData metaData) override;
signals:
    void sendMsg2Manager(PluginMessageData) override;
    void savePanelShow();
    void savePanelHide();
public:
    FdmSettingPlugin(){
        initPlugin(nullptr, nullptr);
    }
    ~FdmSettingPlugin();

public:
    FdmParamSettingsWidget *fdmParamSettingsWidget{nullptr};
    RichParameterList *globalParameterList{nullptr};

    /**



     */
    void initialize(ControlInterface *controlmanager, RichParameterList * globalParameterList) override
    {
        initGui   (controlmanager, globalParameterList);
    }
    void initPlugin(ControlInterface *, RichParameterList *);
    void initGui   (ControlInterface *, RichParameterList *);   
    void _initParam();

private slots:
    void slotOpenPreferenceIndex(int index) {if(m_controlmanager) m_controlmanager->openPreferences(index);}
    
    void doSupportEnabled(bool enable);
    
private:
    ControlInterface *m_controlmanager = nullptr;

    void sendSliceResetMsg();
    void getGlobalSupportSetting();
    QString getConfig(QString configPath);
    QString getSupportConfig();
    bool loadRealTimeProfile(const QString &filePath);
    QString extractRealTimeParamFile(QString gcodeFile);
};


}
#endif // FDM_SETTING_H
