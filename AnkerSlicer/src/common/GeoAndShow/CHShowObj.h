/*



*/
#ifndef CHShowObj_H
#define CHShowObj_H

#include "CHBase.h"
#include "QColor"
#include "QMatrix4x4"
#include "CHAABB3D.h"
#include "CHObject.h"


#define canPickedColor QColor(97,211,125)

enum ObjStatus
{
	general = 0,   
	canPicked,   
	selected     
};

enum PickPriority
{
	Point = 0,
	Curve,
	Mesh
};

struct CHPointOnMesh
{
	int m_faceIndex;
	QVector3D m_pt;
	QVector3D m_nor;
};

DEF_PTR(CHShowObj)
struct PickResult  
{
	QVector3D m_ptOnObj;
	double m_uOnObj;
	double m_deep;
	CHPointOnMesh m_pointOnMesh;
	CHShowObjPtr m_pickObj;
};

class CHShowObj :public CHObject, virtual public std::enable_shared_from_this<CHShowObj>
{
public:
	CHShowObj();
	virtual ~CHShowObj();


public:
	static bool pred1(const PickResult& left, const PickResult& right)  
	{
		return left.m_deep < right.m_deep;
	}
	static bool pred2(const PickResult& left, const PickResult& right) 
	{
		return left.m_pickObj->getPickPriority() < right.m_pickObj->getPickPriority();
	}

	virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10) = 0;
    virtual bool pickAABB(int pixelX, int pixelY, int pickTol = 10){return 0;}
	virtual void updateToScene() = 0;

	virtual CHAABB3D calBaseAABB() = 0;
	virtual CHAABB3D calRealAABB() = 0;
	CHAABB3D calSimilarAABB();
	CHAABB3D getBaseAABB();
	CHAABB3D getRealAABB();


	virtual void setColor(QColor color);
	QColor getColor();

	virtual void setTransform(QMatrix4x4 tran);
	QMatrix4x4 getTransform();

	virtual void setVisuable(bool show);
	bool getVisuable();

	virtual void setDelete(bool del);
	bool getDelete();

	virtual void setDirty(bool dirty);
	bool getDirty();

	virtual void setLightTest(bool value);
	bool getLightTest();

	virtual void setCalLight(bool value);
	bool getCalLight();

	virtual void setStatus(ObjStatus status);
	ObjStatus getStatus();

	PickPriority getPickPriority();

public:
	CHAABB3D m_baseAABB;
    CHAABB3D m_realAABB;

protected:
	QColor m_color;
	bool m_visuable;
	bool m_delete;
	bool m_dirty;
	bool m_lightTest;
	bool m_calLight;
	QMatrix4x4/*Matrix44m*/ m_tran;
	ObjStatus m_status;
	PickPriority m_priority;
};


#endif
