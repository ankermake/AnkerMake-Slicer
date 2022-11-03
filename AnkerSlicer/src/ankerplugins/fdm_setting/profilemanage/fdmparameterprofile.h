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

    void setMachineName(QString name);
    void setMaterialName(QString name);

    
    //int getActiveExtruderIdx();
    //void setActiveExtruderIdx(int idx);

private:
    int activeExtruderIdx = 0;

    
    //QString machineId;
    //QString materialId;
};

#endif // FDMPARAMETERPROFILE_H
