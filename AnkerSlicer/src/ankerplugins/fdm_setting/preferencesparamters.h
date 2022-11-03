#ifndef PREFERENCESPARAMTERS_H
#define PREFERENCESPARAMTERS_H

#include <QObject>
#include <QDebug>


//class FdmProfileServer : public QObject
//{
//    Q_OBJECT

//    Q_PROPERTY(QStringList defaultMaterialList READ getDefaultMaterialList  NOTIFY defaultMaterialListChanged)
//    Q_PROPERTY(QStringList customMaterialList READ getCustomMaterialList NOTIFY customMaterialListChanged)
//    Q_PROPERTY(QStringList allMaterialList READ getAllMaterialList NOTIFY allMaterialListChanged)
//    Q_PROPERTY(QString currentMaterialName READ getCurrentMaterialName NOTIFY currentMaterialNameChanged)

//    Q_PROPERTY(QStringList defaultMachineList READ getDefaultMachineList  NOTIFY defaultMachineListChanged)
//    Q_PROPERTY(QStringList customMachineList READ getCustomMachineList NOTIFY customMachineListChanged)
//    Q_PROPERTY(QStringList allMachineList READ getAllMachineList NOTIFY allMachineListChanged)
//    Q_PROPERTY(QString currentMachineName READ getCurrentMachineName NOTIFY currentMachineNameChanged)

//    Q_PROPERTY(QStringList defaultParameterList READ getDefaultParameterList  NOTIFY defaultParameterListChanged)
//    Q_PROPERTY(QStringList customParameterList READ getCustomParameterList NOTIFY customParameterListChanged)
//    Q_PROPERTY(QStringList allParameterList READ getAllParameterList NOTIFY allParameterListChanged)
//    Q_PROPERTY(QString currentParameterName READ getCurrentParameterName NOTIFY currentParameterNameChanged)

//public:
//    explicit FdmProfileServer(QObject *parent = nullptr);

//    const QStringList getDefaultMaterialList() const ;
//    const QStringList getCustomMaterialList() const ;
//    const QStringList getAllMaterialList() const;
//    const QString getCurrentMaterialName() const;

//    const QStringList getDefaultMachineList() const;
//    const QStringList getCustomMachineList() const;
//    const QStringList getAllMachineList() const;
//    const QString getCurrentMachineName() const;

//    const QStringList getDefaultParameterList() const;
//    const QStringList getCustomParameterList() const;
//    const QStringList getAllParameterList() const;
//    const QString getCurrentParameterName() const;

//public slots:

//    void deleteCustomMaterial(QString materialName) ;
//    void renameCustomMaterial(QString oldMaterialName, QString newMaterialName);
//    void createMaterial(QString materialName);
//    void currentMaterialChanged(QString materialName);


//    void deleteCustomMachine(QString machineName) ;
//    void renameCustomMachine(QString oldMachineName, QString newMachineName);
//    void createMachine(QString machineName);
//    void currentMachineChanged(QString machineName);

//    void onMachineExtruderSwitched(int extruderIdx);


//    void deleteCustomParameter(QString parameterName) ;
//    void renameCustomParameter(QString oldParameterName, QString newParameterName);
//    void createParameter(QString parameterName);
//    void currentParameterChanged(QString parameterName) ;

//    void onImportBtnClicked(QString profileFullName);
//    void onExportBtnClicked(QString profileFullName);

//    void onMachineComboxItemSelected(QString machineName);

//    void onMaterialComboxItemSelected(QString materialName);

//    void onParameterExtruderSwitched(int extruderIdx);



//    void onPreferenceWindowOpen();

//    void onSaveBtnClicked();

//    void onCloseBtnClicked();


//    void onSaveParameterBtnClicked();
//    void onSaveParameterAsBtnClicked();

//signals:
//    void defaultMaterialListChanged();
//    void customMaterialListChanged();
//    void allMaterialListChanged();

//    void defaultMachineListChanged();
//    void customMachineListChanged();
//    void allMachineListChanged();

//    void defaultParameterListChanged();
//    void customParameterListChanged();
//    void allParameterListChanged();

//    void currentMaterialNameChanged();
//    void currentMachineNameChanged();
//    void currentParameterNameChanged();
//};

//class FdmProfileData : public QObject
//{
//    Q_OBJECT

//    Q_PROPERTY(QStringList defaultMaterialList READ getDefaultMaterialList  NOTIFY defaultMaterialListChanged)
//    Q_PROPERTY(QStringList customMaterialList READ getCustomMaterialList NOTIFY customMaterialListChanged)
//    Q_PROPERTY(QStringList allMaterialList READ getAllMaterialList NOTIFY allMaterialListChanged)
//    Q_PROPERTY(QString currentMaterialName READ getCurrentMaterialName NOTIFY currentMaterialNameChanged)
//public:
//    const QStringList getDefaultMaterialList() const {return QStringList();};
//    const QStringList getCustomMaterialList() const {return QStringList();};
//    const QStringList getAllMaterialList() const {return QStringList();};
//    const QString getCurrentMaterialName() const {return QString();};

//public slots:
//    void deleteCustomMaterial(QString materialName) {};
//    void renameCustomMaterial(QString oldMaterialName, QString newMaterialName){};
//    void createMaterial(QString materialName){};
//    void currentMaterialChanged(QString materialName){};

//signals:
//    void defaultMaterialListChanged();
//    void customMaterialListChanged();
//    void allMaterialListChanged();
//    void currentMaterialNameChanged();
//};
#endif // PREFERENCESPARAMTERS_H
