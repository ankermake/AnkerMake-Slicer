#ifndef FDM_SLICER_H
#define FDM_SLICER_H

#include <common/plugins/interfaces/anker_plugin.h>

#include <QPushButton>
#include <QProgressBar>
#include "akslicer.h"
#include <QMainWindow>
#include <QSettings>
#include "common/controlInterface/controlInterface.h"
#include "../common/ak_const.h"
#include "akprogressbar.h"
#include "../common/controlInterface/progressdialog.h"
#include "akslicepanel.h"

using namespace control;

//class ControlInterface;
class FdmSlicer : public QObject, public AnkerPlugin
{
    Q_OBJECT
    MESHLAB_PLUGIN_IID_EXPORTER(Anker_PLUGIN_IID)
    Q_INTERFACES(AnkerPlugin)
    QString pluginName() const final{return AkConst::Plugin::FDM_SLICER;}; // MeshLabPlugin interface

public:
    void recMsgfromManager(PluginMessageData metaData) override;
signals:
    void sendMsg2Manager(PluginMessageData) override;
public:
    FdmSlicer();
    ~FdmSlicer();
    /**



     */
    void initialize(ControlInterface *controlmanager, RichParameterList * globalParameterList);

    /**

     */
    //void initPluginTemporary(PluginMessageData);

    void getGlobalSupportStatus();

signals:
    void sliceSucess();
    void sliceFailed();
    void appendSliceBtnStat(ESliceBtnStat btnStat);
    //void sliceEnable(bool enable);

private:
    //QPushButton * sliceBtn;
    //AkProgressBar *progressBar;
    ProgressDialog *progressBar= nullptr;
    ControlInterface *ctrlmanager= nullptr;
    AkSlicer* slicer= nullptr;
    AkSliceInfo sliceResult;
    QTimer *timer = nullptr;
    int fileLoadPercent = 0;
    int slicePercent = 0;
    AkSlicePanel* panel = nullptr;


private:
    void onAnalyzeBtnClicked();
    void doSliceSuccess(AkSliceInfo sliceInfo);
    void doSliceFailed(AkSliceInfo sliceInfo);
    void setProgressBar(AkSlicer::SliceStep step, AkSlicer::SliceStatus status, float percent, QString msg);
    void doSlice();
    void doPreview();
    void doPreviewBtnClicked();
    void saveStlFile();
    void trySlice();
    void getCustomSetting();
    void doSave();
};


#endif // FDM_SLICER_H
