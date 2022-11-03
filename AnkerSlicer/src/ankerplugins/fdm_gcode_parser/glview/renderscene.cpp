#include "renderscene.h"

RenderMesh::RenderMesh()
{
}

RenderMesh::~RenderMesh()
{

}

RenderPointSphere::RenderPointSphere()
{
}

RenderPointSphere::~RenderPointSphere()
{
}

RenderCurve::RenderCurve()
{
}

RenderCurve::~RenderCurve()
{
}

RenderData::RenderData() : m_shaderProgramPtr(nullptr), m_printMachineBoxPtr(new CHPrintMachineBox)
{
}

RenderData::~RenderData()
{
}

void RenderData::setShaderProgram(QOpenGLShaderProgram* glsp)
{
	m_shaderProgramPtr = glsp;
}

CHPrintMachineBoxPtr& RenderData::getPrintMachineBox()
{
	return m_printMachineBoxPtr;
}

void RenderData::bindPrintMachineBoxRenderData()
{
    qDebug() << "Bind Base Show Size: " << m_printMachineBoxPtr->m_baseShowObjs.size();
	for (int i = 0; i < m_printMachineBoxPtr->m_baseShowObjs.size(); i++)
	{
        if(std::dynamic_pointer_cast<CoordinateAxis>(m_printMachineBoxPtr->m_baseShowObjs[i]) != nullptr)
        {
           std::vector<CHShowObjPtr> showObjs = std::dynamic_pointer_cast<CoordinateAxis>(m_printMachineBoxPtr->m_baseShowObjs[i])->m_baseShowObjs;
           for(int j = 0; j < showObjs.size(); j++)
           {
               if(std::dynamic_pointer_cast<CHMeshShowObj>(showObjs[j]) != nullptr)
               {
                   CHMeshShowObjPtr meshShowObjPtr = std::dynamic_pointer_cast<CHMeshShowObj>(showObjs[j]);
                   RenderMeshPtr renderMeshPtr = RenderMeshPtr(new RenderMesh);
                   m_allmeshes.insert(std::pair<CHMeshShowObjPtr, RenderMeshPtr>(meshShowObjPtr, renderMeshPtr));
               }
               else if(std::dynamic_pointer_cast<CHCurveShowObj>(showObjs[j]) != nullptr)
               {
                   CHCurveShowObjPtr curveShowObjPtr = std::dynamic_pointer_cast<CHCurveShowObj>(showObjs[j]);
                   RenderCurvePtr renderCurvePtr = RenderCurvePtr(new RenderCurve);
                   m_allcurves.insert(std::pair<CHCurveShowObjPtr, RenderCurvePtr>(curveShowObjPtr, renderCurvePtr));
               }
           }

        }
		if (std::dynamic_pointer_cast<CHMeshShowObj>(m_printMachineBoxPtr->m_baseShowObjs[i]) != nullptr)
		{		 //CHMeshShowObjPtr		  std::dynamic_pointer_cast<CH3DPrintModel>(mesh)
			CHMeshShowObjPtr meshShowObjPtr = std::dynamic_pointer_cast<CHMeshShowObj>(m_printMachineBoxPtr->m_baseShowObjs[i]);
			RenderMeshPtr renderMeshPtr = RenderMeshPtr(new RenderMesh);
			m_allmeshes.insert(std::pair<CHMeshShowObjPtr, RenderMeshPtr>(meshShowObjPtr, renderMeshPtr));
		}
		else if (std::dynamic_pointer_cast<CHCurveShowObj>(m_printMachineBoxPtr->m_baseShowObjs[i]) != nullptr)
		{
			CHCurveShowObjPtr curveShowObjPtr = std::dynamic_pointer_cast<CHCurveShowObj>(m_printMachineBoxPtr->m_baseShowObjs[i]);
			RenderCurvePtr renderCurvePtr = RenderCurvePtr(new RenderCurve);
			m_allcurves.insert(std::pair<CHCurveShowObjPtr, RenderCurvePtr>(curveShowObjPtr, renderCurvePtr));
		}
	}

}


