/*
*?????????
*?????
*???2021?12?30?
*/


#ifndef CHRenderData_H
#define CHRenderData_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include "QMatrix4x4"
#include "common/GeoAndShow/CHPointShowObj.h"
#include "common/GeoAndShow/CHMeshShowObj.h"
#include "common/GeoAndShow/CHCurveShowObj.h"



DEF_PTR(CHRenderMesh)
class CHRenderMesh   
{
public:
	CHRenderMesh();
	~CHRenderMesh();


public:
	bool m_beBaseMesh;
	QOpenGLBuffer m_vbo;
	QOpenGLBuffer m_norbo;
	QOpenGLVertexArrayObject m_vao;
	GLuint m_nIBOId;
    int m_fNum;

	std::vector<std::pair<CHRenderMeshPtr, QMatrix4x4>> m_childMeshes;
};

DEF_PTR(CHRenderPointSphere)
class CHRenderPointSphere   
{
public:
	CHRenderPointSphere();
	~CHRenderPointSphere();

public:
	QOpenGLBuffer m_vbo;
	QOpenGLBuffer m_norbo;
	QOpenGLVertexArrayObject m_vao;
	GLuint m_nIBOId;
    int m_fNum;
};

DEF_PTR(CHRenderCurve)
class CHRenderCurve   
{
public:
	CHRenderCurve();
	~CHRenderCurve();

public:
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;
    int m_Num;
};


DEF_PTR(CHRenderData)
class CHRenderData
{
public:
	CHRenderData();
	~CHRenderData();
	

public:
    
	CHMeshShowObjPtr getSphereMesh();



public:
	
	QOpenGLShaderProgram m_shaderProgram;

	std::map<CHMeshShowObjPtr, CHRenderMeshPtr> m_allmeshes;

	std::set<CHPointShowObjPtr> m_allpoints;
    
	CHRenderPointSpherePtr m_baseShowPoint; 

	std::map<CHCurveShowObjPtr, CHRenderCurvePtr> m_allcurves;
    CHRenderCurvePtr m_unitLine;
};
#endif
