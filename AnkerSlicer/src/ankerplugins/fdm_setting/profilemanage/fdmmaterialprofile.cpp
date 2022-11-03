#include "fdmmaterialprofile.h"
#include "common/ak_const.h"

void FdmMaterialProfile::getCategoriesForRefreshTree(QList<FdmProfileCategory> &categories)
{
     auto material = getCategory(AkConst::Category::MATERIAL);
     if (material == NULL)
     {
         return;
     }
     categories.append(*material);
}
