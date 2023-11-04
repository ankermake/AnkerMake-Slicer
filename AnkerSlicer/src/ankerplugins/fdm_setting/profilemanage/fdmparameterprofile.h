#ifndef FDMPARAMETERPROFILE_H
#define FDMPARAMETERPROFILE_H

#include "fdmbaseprofile.h"
#include "fdmmachineprofile.h"
#include "fdmmaterialprofile.h"

class FdmParameterProfile : public FdmBaseProfile
{
public:
    
    FdmParameterProfile(const QString & file, ProfileGroup group): FdmBaseProfile(file,group){}
    FdmParameterProfile(const QString &dateSource, const QList<FdmProfileCategory> &categories, ProfileGroup group):FdmBaseProfile(dateSource,categories, group){}

    
    void getCategoriesForRefreshTree(QList<FdmProfileCategory>& categories) override;
    
    void setExtruderCategory(const FdmProfileCategory& category);
    FdmProfileCategory getExtruderCategory();

    
    void setSetting(const QString &categoryName, const FdmSettingItem & item) override;

    
    int getExtruderCount();

    
    void updateMachine(FdmMachineProfile* profile);
    void updateMaterial(FdmMaterialProfile* profile);
    
    

    //void cloneProfile(FdmParameterProfile* profile);

    
    QString getMachineName();
    QString getMaterialName();
    double getNozzleSize();
    QString getPrintMode();

    //  add @2023-04-13 by ChunLian
    virtual void setPrintMode       (QString value);
    virtual void setMachineName     (QString value);
    virtual void setMaterialName    (QString value);
    virtual void setNozzleName      (QString value);
    virtual void setNozzleSize      (double  value);
    virtual void setProfileName     (QString value);

    virtual void setAdhesionType    (QString value);
    virtual void setLayerHeight     (double  value);
    virtual void setInfillDensity   (double  value);
    virtual void setInfillThickness (double  value);
    virtual void setSupportEnable   (bool    value);




    ////当前选中的挤出头idx, 用于界面显示
    //int getActiveExtruderIdx();
    //void setActiveExtruderIdx(int idx);

private:
    int activeExtruderIdx = 0;

    
    //QString machineId;
    //QString materialId;
};

#endif // FDMPARAMETERPROFILE_H
