#ifndef FDMMATERIALPROFILESERVICE_H
#define FDMMATERIALPROFILESERVICE_H
#include <QObject>
#include <QStringList>
#include "profilemanage/fdmmaterialprofile.h"

class FdmMaterialProfileService : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList defaultMaterialList READ getDefaultMaterialList  NOTIFY defaultMaterialListChanged)
    Q_PROPERTY(QStringList customMaterialList READ getCustomMaterialList NOTIFY customMaterialListChanged)
    Q_PROPERTY(QStringList allMaterialList READ getAllMaterialList NOTIFY allMaterialListChanged)
    //Q_PROPERTY(QString currentMaterialName READ getDefaultSelectedName WRITE currentMaterialChanged NOTIFY defaultMaterialNameChanged)
    Q_PROPERTY(QString currentMaterialName READ getCurrentSelectedName NOTIFY defaultMaterialNameChanged)

    //QString m_currentMaterialName;

public:
    //explicit FdmMaterialProfileService(QObject *parent = nullptr);
    static FdmMaterialProfileService* instance()
    {
        static FdmMaterialProfileService manager;
        return &manager;
    }

    QStringList getDefaultMaterialList();
    QStringList getCustomMaterialList();
    QStringList getAllMaterialList();

    
    FdmMaterialProfile* getCurrentProifle();

    
    QString getCurrentSelectedName() ;
    
    void refreshUI();
    void refreshTree();

    void selectMaterial(QString name);
    void refreshCurrentPage();



public slots:
    //material
    void deleteCustomMaterial(QString materialName);
    void renameCustomMaterial(QString oldMaterialName, QString newMaterialName);
    void createMaterial(QString materialName);
    void currentMaterialChanged(QString materialName);

signals:
    void defaultMaterialListChanged();
    void customMaterialListChanged();
    void allMaterialListChanged();
    void defaultMaterialNameChanged();

    
    void materialSelected(const QString & name);
    
    void customMaterialRenamed(QString oldMaterialName, QString newMaterialName);
    void customMaterialDeleted(QString materialName);
private:
    QString currentMaterialName;

};
#endif // FDMMATERIALPROFILESERVICE_H
