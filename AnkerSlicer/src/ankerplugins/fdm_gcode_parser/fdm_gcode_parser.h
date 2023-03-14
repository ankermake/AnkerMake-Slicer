#ifndef FDM_GCODE_PARSER_H
#define FDM_GCODE_PARSER_H

#include <common/plugins/interfaces/anker_plugin.h>
#include "common/controlInterface/controlInterface.h"
#include "../common/controlInterface/progressdialog.h"
#include "common/utilities/tlogger.h"
#include "common/GeoAndShow/SceneParam.h"
#include "../common/ak_const.h"
#include <QMainWindow>
#include <QProcess>
#include <QRemoteObjectHost>
#include "fdm_rpc_wrapper.h"
#include "fdmgcodepreviewentry.h"
/*




*/



class FdmGcodeParser : public QObject, public AnkerPlugin
{
    Q_OBJECT
        MESHLAB_PLUGIN_IID_EXPORTER(Anker_PLUGIN_IID)
        Q_INTERFACES(AnkerPlugin)
        QString pluginName() const final { return AkConst::Plugin::FDM_GCODE_PARSER; } // MeshLabPlugin interface
    enum {
        DP_DOCK_FDM_GCODE_PASER,
    };

    //QMainWindow* MW;

public:
    FdmGcodeParser();
    virtual ~FdmGcodeParser();
    inline void initialize(ControlInterface* controlmanager, RichParameterList* globalParameterList) {
        qDebug() << "controlmanager " <<controlmanager;
        if (globalParameterList) globalParams = globalParameterList; }
//    std::unique_ptr<Anker::GCodeProcessor> processor;
//    QList<viewWidget*>  mviewer;

    // MeshLabPlugin interface
public:
    void recMsgfromManager(PluginMessageData) override final;
signals:
    void sendMsg2Manager(PluginMessageData) override final;

public:
    void messageProcessing(PluginMessageData msgBody);
    void reOpenGcodePreview(PluginMessageData msgBody);
    void openGcodePreviewWithoutFile();
    void openGcodePreviewInnetwork(const QString& file,const QString& m_hostAdress);
    
    bool checkOpenFile(QString gcodePath);
    void queryLoggingStatus();

//    void exportMessageProcessing(PluginMessageData msgBody);  //no need

    fdmRpcWrapper *m_rpc = nullptr;
    QRemoteObjectHost *m_host = nullptr;

    fdmRpcWrapper *m_rpc_inner = nullptr;
    QRemoteObjectHost *m_host_inner = nullptr;

private:
    RichParameterList* globalParams;
    int useTimes = 0;
    QProcess *pIn = nullptr;

//    QList<QProcess *> pInList;
    ProgressDialog* m_pDlg;
    QString CurrentShowFile;

    FdmGcodePreviewEntry* preview = nullptr;
    QList<FdmGcodePreviewEntry *> previewNetworkList;
private slots:
    void setUseTimes(int ut);
    void loggingStausChange(bool status);
};


#endif // FDM_SETTING_H
