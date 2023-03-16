/*



*/


#ifndef CHPrintMachineBox_H
#define CHPrintMachineBox_H

#include "../ml_document/cmesh.h"
#include "CHAssembly.h"
#include "CHLineSegment3DShowObj.h"
#include "CHMeshShowObj.h"
#include "CHPointShowObj.h"
#include "coordinate.h"


DEF_PTR(CHPrintMachineBox)
class CHPrintMachineBox :public CHAssembly
{
public:
    CHPrintMachineBox();
    virtual ~CHPrintMachineBox();

public:

    virtual void setColor(QColor color);
    void create(QString logoPath, float lenth, float wide, float height);
    void create(const CMeshO& logoMesh, float length, float wide, float height, const QColor& boxColor);
    void updateMechineSize(float length, float width, float height); 

    
    CMeshO getLogoCMeshO() const;
private:
    std::vector<CHLineSegment3DShowObjPtr> m_lines;
    CHMeshShowObjPtr m_platform;

    CMeshO m_logoCMesh;
};


#endif
