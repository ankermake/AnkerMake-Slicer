#ifndef FDMMACHINEPROFILESERVICE_H
#define FDMMACHINEPROFILESERVICE_H

#include <QObject>
#include "../profilemanage/fdmmachineprofile.h"

class FdmMachineProfileService : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList defaultMachineList READ getDefaultMachineList  NOTIFY defaultMachineListChanged)
    Q_PROPERTY(QStringList customMachineList READ getCustomMachineList NOTIFY customMachineListChanged)
    Q_PROPERTY(QStringList allMachineList READ getAllMachineList NOTIFY allMachineListChanged)
    Q_PROPERTY(QString currentMachineName READ getDefaultSelectedName NOTIFY defaultMachineNameChanged)
    //Q_PROPERTY(QString extruderCount READ getExtruderCount NOTIFY defaultMachineNameChanged)

public:
    static FdmMachineProfileService* instance()
    {
        static FdmMachineProfileService manager;
        return &manager;
    }

    //explicit FdmMachineProfileService(QObject *parent = nullptr);
    QStringList getDefaultMachineList() ;
    QStringList getCustomMachineList() ;
    QStringList getAllMachineList() ;
    
	QString getDefaultSelectedName() ;

//    const QString getCurrentSelectedName() const;


//    const QStringList getTabNameList() const;

//    int getExtruderCount() const;

    
    void refreshUI();
    void refreshTree();

    //
    void selectMachine(QString name);
    void refreshCurrentPage();

    FdmMachineProfile* getCurrentProifle();
    void clearCookie();

public slots:
    
    void deleteCustomMachine(QString machineName) ;
    void renameCustomMachine(QString oldMachineName, QString newMachineName);
    void createMachine(QString machineName);
    void currentMachineChanged(QString machineName);
    
    void onTabSwitched(int idx);

    
    //void onExtruderCountChanged(int count);

signals:
    void defaultMachineListChanged();
    void customMachineListChanged();
    void allMachineListChanged();
    void defaultMachineNameChanged();
    
    void machineSelected(const QString &machineName);

    
    void customMachineRenamed(QString oldMachineName, QString newMachineName);
    void customMachineDeleted(QString machineName);

private:
    
    
    QString currentMachineName;
    
    QMap<QString, int> machineActiveTabIdxMap;
};

#endif // FDMMACHINEPROFILESERVICE_H
