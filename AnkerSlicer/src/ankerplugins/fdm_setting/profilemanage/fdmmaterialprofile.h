#ifndef FDMMATERIALPROFILE_H
#define FDMMATERIALPROFILE_H

#include "fdmbaseprofile.h"

class FdmMaterialProfile :public FdmBaseProfile
{
public:
    
    //FdmMaterialProfile(){}
    
    FdmMaterialProfile(QString dateSource, ProfileGroup group):FdmBaseProfile(dateSource,group){}
    FdmMaterialProfile(const QString &dateSource, const QList<FdmProfileCategory> &categories, ProfileGroup group):FdmBaseProfile(dateSource,categories, group){}

    
    void getCategoriesForRefreshTree(QList<FdmProfileCategory>& categories);

};

#endif // FDMMATERIALPROFILE_H
