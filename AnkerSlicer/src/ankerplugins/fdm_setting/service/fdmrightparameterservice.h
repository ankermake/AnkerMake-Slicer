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

    
    void syncRightTreeToRealtimeprofile();

    
    void selectProfile(QString name);
    void selectMachine(QString name);
    void selectMaterial(QString name);
    void selectNozzle(QString name);

    void applySimpleModeData();
    void setDefaultValueFromExpertMode();

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

private:
    QList<float> getAiQualityList();
    QList<int> getAiInfillDensityList() ;
    void calcGlobalSupportTextCode() ;

    void onExtruderSwitched(int extruderIdx, FdmQmlTreeApi & treeApi);
    
    void refreshTree();

    //void updateRealtimeProfile(FdmParameterProfile* destProfile);


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
};

#endif // FDMRIGHTPARAMETERSERVICE_H
