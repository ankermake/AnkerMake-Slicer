#ifndef FDMPARAMETERPROFILESERVICE_H
#define FDMPARAMETERPROFILESERVICE_H

#include <QObject>
#include <QStringList>
#include "filemanage/fdmqmlsourcetree.h"
#include "profilemanage/fdmparameterprofile.h"


class ParamterItem
{
public:
    QString name;
    
    int status;
};

class FdmParameterProfileService : public QObject
{
    Q_OBJECT

    
    Q_PROPERTY(QStringList defaultParameterList READ getDefaultParameterList  NOTIFY defaultParameterListChanged)
    Q_PROPERTY(QStringList customParameterList READ getCustomParameterList NOTIFY customParameterListChanged)

    Q_PROPERTY(QStringList allParameterList READ getAllParameterList NOTIFY allParameterListChanged)

    Q_PROPERTY(QList<ParamterItem> customItemList READ getCustomItemList NOTIFY customItemListChanged)

    
    Q_PROPERTY(QString currentParameterName READ getCurrentSelectedName NOTIFY currentParameterNameChanged)

    
    //Q_PROPERTY(QString exportProperName READ getExportProperName NOTIFY exportProperNameChanged)

    
    
    Q_PROPERTY(QString machineName READ getMachineName NOTIFY machineNameChanged)
    Q_PROPERTY(QString materialName READ getMaterialName NOTIFY materialNameChanged)
    
    Q_PROPERTY(QStringList machineList READ getMachineList NOTIFY machineListChanged)
    Q_PROPERTY(QStringList materialList READ getMaterialList NOTIFY materialListChanged)

    Q_PROPERTY(int extruderCount READ getExtruderCount NOTIFY extruderCountChanged)

public:
    static FdmParameterProfileService* instance()
    {
        static FdmParameterProfileService manager;
        return &manager;
    }

    void init();

    
    
    QStringList getDefaultParameterList() ;

    QStringList getCustomParameterList() ;
    
    QString getCurrentSelectedName() ;
    QStringList getAllParameterList() ;

    
    
    int getExtruderCount();
    //void setExtruderCount(int count);

    
    QStringList getMachineList() ;
    QStringList getMaterialList() ;
    
    QString getMachineName() ;
    QString getMaterialName() ;

    QList<ParamterItem> getCustomItemList();

    void setCurrentProfile(QString name);
    void setCurrentMachine(QString name);
    void setCurrentMaterial(QString name);

    //QString &getExportProperName() ;

    
    void refreshUI();
    
    void exportIniFromTree(QString profileFullName);

    FdmParameterProfile* getCurrentProifle();
    //
    void selectProfile(QString name);
    void selectMachine(QString name);
    void selectMaterial(QString name);
    //void setCurrentProfileName(QString name);
    void refreshCurrentPage();

    void clearCookie();



public slots:
    
    void deleteCustomParameter(QString parameterName) ;
    void renameCustomParameter(QString oldParameterName, QString newParameterName);
    
    void createParameter(QString parameterName);
    
    void onImportBtnClicked(QString profileFullName);
    
    void onExportBtnClicked(QString profileFullName);
    
    void currentParameterChanged(QString parameterName) ;
    
    void onMachineNameChanged(const QString name);
    void onMaterialNameChanged(const QString name);

    
    void onExtruderSwitched(int extruderIdx);

    //void onSourceTreeApiNodeValueChanged(const QString& categoryName, const FdmSettingItem& item);

    void doCustomMachineRenamed(QString oldName, QString newName);
    void doCustomMachineDeleted(QString name);
    void doCustomMaterialRenamed(QString oldName, QString newName);
    void doCustomMaterialDeleted(QString name);

    void doOperateComplete();
signals:
    void defaultParameterListChanged();
    void customParameterListChanged();

    void currentParameterNameChanged();

    void machineNameChanged();
    void materialNameChanged();
    void machineListChanged();
    void materialListChanged();

    void extruderCountChanged();

    
    void ParameterSelected(const QString & name);

    void allParameterListChanged();

    void exportProperNameChanged();

    void customItemListChanged();


private:
    //void onExtruderSwitched(int extruderIdx, FdmQmlTreeApi & treeApi);
    void refreshTree();

    FdmParameterProfile* m_currentProfile;

    
    QString m_currentProfileName;
    
    QString m_currentMachineName;
    QString m_currentMaterialName;

    
    QMap<QString, int> machineActiveTabIdxMap;
    
    

    
};



#endif // FDMPARAMETERPROFILESERVICE_H
