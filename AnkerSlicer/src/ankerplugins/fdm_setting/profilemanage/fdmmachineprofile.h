#ifndef FDMMACHINEPROFILE_H
#define FDMMACHINEPROFILE_H
#include <QString>
#include <QVariant>

#include "fdmprofilecategory.h"
#include "fdmbaseprofile.h"

class FdmMachineProfile : public FdmBaseProfile
{

public:
    //FdmMachineProfile(){}
    FdmMachineProfile(const QString dateSource,ProfileGroup group):FdmBaseProfile(dateSource, group){}
    FdmMachineProfile(const QString &dateSource, const QList<FdmProfileCategory> &categories, ProfileGroup group):FdmBaseProfile(dateSource,categories, group){}

    
    int getExtruderCount();

    
    void getCategoriesForRefreshTree(int extruderNum, QList<FdmProfileCategory>& categories);

    
    void getComponentCategories(int componentIdx, QList<FdmProfileCategory>& categories);

    
    void setSetting(const QString& categoryName, const FdmSettingItem& item) override;

    
    //int getActiveComponentIdx() const;
    //void setActiveComponentIdx(int idx);

private:
    
    FdmProfileCategory getExtruderCategory(int extruderNum);
    void setExtruderCategory(const FdmProfileCategory& category);

    
    const FdmProfileCategory *getMachineCategory();

private:
    
    //int activeComponentIdx = 0;
};




#endif // FDMMACHINEPROFILE_H
