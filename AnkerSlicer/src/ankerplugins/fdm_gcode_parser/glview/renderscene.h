/****************************************************************************
Revision 0.1  2022/02/10 15:50:40  Aden Hu
****************************************************************************/
#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "QMatrix4x4"
#include "common/GeoAndShow/CHPointShowObj.h"
#include "common/GeoAndShow/CHMeshShowObj.h"
#include "common/GeoAndShow/CHCurveShowObj.h"
#include "common/GeoAndShow/CHPrintMachineBox.h"


DEF_PTR(RenderMesh)
class RenderMesh   
{
public:
	RenderMesh();
	~RenderMesh();


public:
	bool m_beBaseMesh;
	QOpenGLBuffer m_vbo;
	QOpenGLBuffer m_norbo;
	QOpenGLVertexArrayObject m_vao;
	GLuint m_nIBOId;
	int m_fNum;

	std::vector<std::pair<RenderMeshPtr, QMatrix4x4>> m_childMeshes;
};

DEF_PTR(RenderPointSphere)
class RenderPointSphere   
{
public:
	RenderPointSphere();
	~RenderPointSphere();

public:
	QOpenGLBuffer m_vbo;
	QOpenGLBuffer m_norbo;
	QOpenGLVertexArrayObject m_vao;
	GLuint m_nIBOId;
	int m_fNum;
};

DEF_PTR(RenderCurve)
class RenderCurve   
{
public:
	RenderCurve();
	~RenderCurve();

public:
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;
	int m_Num;
};

DEF_PTR(RenderData)
class RenderData
{
public:
	RenderData();
	~RenderData();
	void setShaderProgram(QOpenGLShaderProgram* glsp);
	CHPrintMachineBoxPtr& getPrintMachineBox();
	void bindPrintMachineBoxRenderData();

public:
	QOpenGLShaderProgram* m_shaderProgramPtr;
	CHPrintMachineBoxPtr m_printMachineBoxPtr;
	std::map<CHMeshShowObjPtr, RenderMeshPtr>  m_allmeshes;
	RenderCurvePtr m_unitLine;
	std::map<CHCurveShowObjPtr, RenderCurvePtr> m_allcurves;
};




#endif // !RENDER_SCENE_H
