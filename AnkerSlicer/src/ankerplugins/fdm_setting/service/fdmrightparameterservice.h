#ifndef FDMRIGHTPARAMETERSERVICE_H
#define FDMRIGHTPARAMETERSERVICE_H
#include <QObject>
#include "fdmparameterprofileservice.h"
#include "common/ak_const.h"
#include <QTimer>
#include "../profilemanage/fdmrealtimeprofile.h"
#include "../profilemanage/operatelog.h"


class FdmRightParameterService : public QObject
{

    Q_OBJECT
    
    Q_PROPERTY(QString machineName READ getMachineName  NOTIFY machineNameChanged)
    Q_PROPERTY(QStringList machineList READ getMachineList NOTIFY machineListChanged)
    
    Q_PROPERTY(QString materialName READ getMaterialName NOTIFY materialNameChanged)
    Q_PROPERTY(QStringList materialList READ getMaterialList NOTIFY materialListChanged)
    
    Q_PROPERTY(QString currentParameterName READ getCurrentSelectedName  NOTIFY currentParameterNameChanged)
    Q_PROPERTY(QStringList allParameterList READ getAllParameterList NOTIFY allParameterListChanged)
    
    Q_PROPERTY(int extruderCount READ getExtruderCount NOTIFY extruderCountChanged)

    
    Q_PROPERTY(int aiQualityCurrentIdx READ getAiQualityCurrentIdx NOTIFY aiQualityCurrentIdxChanged)
    Q_PROPERTY(QStringList aiQualityList READ getAiQualityStrList NOTIFY aiQualityListChanged)
    Q_PROPERTY(int aiInfillDensityCurrentIdx READ getAiInfillDensityCurrentIdx NOTIFY aiInfillDensityCurrentIdxChanged)
    Q_PROPERTY(QStringList aiInfillDensityList READ getAiInfillDensityStrList  NOTIFY aiInfillDensityListChanged)
    
    Q_PROPERTY(int globalSupportTextCode READ getGlobalSupportTextCode NOTIFY globalSupportTextCodeChanged)
    Q_PROPERTY(int globalSupportState READ getGlobalSupportState NOTIFY globalSupportStateChanged)
    Q_PROPERTY(int generateAdhesionState READ getGenerateAdhesionState NOTIFY generateAdhesionStateChanged)

    
    Q_PROPERTY(QString nozzleSize READ getNozzleSize NOTIFY nozzleSizeChanged)
    Q_PROPERTY(QStringList nozzleSizeList READ getNozzleSizeList NOTIFY nozzleSizeListChanged)

    //speed
    Q_PROPERTY(QString printMode READ getPrintMode NOTIFY printModeChanged)
    Q_PROPERTY(QStringList printModeList READ getPrintModeList NOTIFY printModeListChanged)
    //Q_PROPERTY(QVariant printModeList READ getPrintModeList NOTIFY printModeListChanged)

    //test
    Q_PROPERTY(QString infillThickness READ getInfillThicknesse NOTIFY infillThicknessChanged)

public:
    FdmRightParameterService(){}
public:
    static FdmRightParameterService* instance()
    {
        static FdmRightParameterService manager;
        return &manager;
    }
    
    void init() ;
    
    void reload(FdmRealTimeProfile *newProfile);

    
    QString getMachineName() ;
    QStringList getMachineList() ;
    QString getMaterialName() ;
    QStringList getMaterialList() ;

    
    const QString getCurrentSelectedName() ;
    QStringList getAllParameterList() ;

    
    int getExtruderCount();

    
    void refreshUI();

    
    int getAiQualityCurrentIdx() ;
    QStringList getAiQualityStrList();

    int getAiInfillDensityCurrentIdx() ;
    QStringList getAiInfillDensityStrList() ;

    int getGlobalSupportState();
    int getGenerateAdhesionState();
    int getGlobalSupportTextCode();

    
    QString getNozzleSize() ;
    QStringList getNozzleSizeList() ;

    void setSupportMeshCount(int newSupportMeshCount);
    void setGlobalSupportForbidden(bool forbidden);

    //test
    QString getInfillThicknesse()
    {
        return QString::number(m_realTimeProfile->getSetting(AkConst::Category::BASE_PARAM, "infill_sparse_thickness").toFloat(), 'g', 2);
    }
    void syncRightTreeToRealtimeprofile();

    
    void selectProfile(QString name);
    void selectMachine(QString name);
    void selectMaterial(QString name);
    void selectNozzle(QString name);

    void applySimpleModeData();
    void setDefaultValueFromExpertMode();

    QString getPrintMode();
    QStringList getPrintModeList();
    QStringList getPrintModeOriginList();

public slots:
    
    void onSaveBtnClicked();
    
    void onSaveAsBtnClicked(const QString profileName);
    
    //void currentParameterChanged(QString parameterName) ;
    
    void onExtruderSwitched(int extruderIdx);
    
    void onSourceTreeApiNodeValueChanged(const QString &categoryName, const FdmSettingItem & item);

    
    void onMachineNameChanged(const QString name);
    void onMaterialNameChanged(const QString name);
    void onCurrentSelectedNameChanged(const QString &parameterName);
    //void onAiQualityChanged(float newAiQuality);
    void onAiQualityCurrentIdxChanged(int idx);
    
    void onNozzleSizeChanged(const QString name);

    //void onAiInfillDensityChanged(double newAiInfillDensity);
    void onAiInfillDensityIdxChanged(int idx);

    
    void onSetBtnClicked(AkConst::EWidgetType widgetType, QString name);
    
    void onResetBtnClicked();

    void receiveOperLog(OperateLogPtr operLogPtr);

    
    void onGlobalSupportStateChanged(int state);
    void onGenerateAdhesionStateChanged(int state);
    void doOperateComplete();

    void doMainWindowInitFinished();

    //change speed mode
    void onPrintModeChanged(const QString name);

    void onPrintModeIdxChanged(int index);

    void checkPrintMode();

signals:
    void currentParameterNameChanged();
    void machineNameChanged();
    void materialNameChanged();
    void machineListChanged();
    void materialListChanged();
    void extruderCountChanged();
    void nozzleSizeChanged();
    void nozzleSizeListChanged();

    
    void allParameterListChanged();
    //void aiNormalQualityClicked();
    //void aiHighQualityClicked();
    void aiInfillDensityCurrentIdxChanged();
    void aiInfillDensityListChanged();

    void aiQualityCurrentIdxChanged();
    void aiQualityListChanged();
    
    void currentParameterSwitched(QString name) ;
    void currentMachineSwitched(QString name);
    void currentMaterialSwitched(QString name);

    void globalSupportStateChanged();
    void generateAdhesionStateChanged();
    void globalSupportTextCodeChanged();

    
    void setSupportEnabled(bool enable);


    void printModeListChanged();
    void printModeChanged();

    void printModeVisibleChanged();
    void componentModeChanged();

    void infillThicknessChanged();

private:
    QList<float> getAiQualityList();
    QList<int> getAiInfillDensityList() ;
    void calcGlobalSupportTextCode() ;

    void onExtruderSwitched(int extruderIdx, FdmQmlTreeApi & treeApi);
    
    void refreshTree();
    void setTreeNodeValue(const QString & nodeName, const QVariant value); //  add @2023-04-13 by ChunLian

    //void updateRealtimeProfile(FdmParameterProfile* destProfile);
    void selectDefaultParamFromMachine();

    FdmRealTimeProfile* m_realTimeProfile;

    
    QTimer timer;
    int m_updateTime;
    int m_supportMeshCount;
    bool m_globalSupportForbidden;
    AkConst::EGlobalSupportTextCode sptTxtCode;

    
    int getAiQualityIdx(float layerHeight);
    int getAiInfillDensityIdx(float density);
    int getGlobalSupportState(bool enable);
    int getAdhesionTypeState(QString type);

#ifdef TOOL_SAVE_AS_PARAM_INI
public:
    QString __ToolSaveAs_1();
    QString __ToolSaveAs_2(QString);
#endif
};

#endif // FDMRIGHTPARAMETERSERVICE_H
