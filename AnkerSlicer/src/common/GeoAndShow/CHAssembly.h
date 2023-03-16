

#ifndef CHAssembly_H
#define CHAssembly_H

#include "CHShowObj.h"


DEF_PTR(CHAssembly)
class COMMONLIB_EXPORT CHAssembly :public CHShowObj
{
public:
	CHAssembly();
	virtual ~CHAssembly();

public:

	virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);
	virtual void updateToScene();
	virtual CHAABB3D calBaseAABB();
	virtual CHAABB3D calRealAABB();

	virtual void setColor(QColor color);
	virtual void setTransform(QMatrix4x4 tran);
	virtual void setVisuable(bool show);
	virtual void setDelete(bool del);
	virtual void setDirty(bool dirty);
	virtual void setLightTest(bool value);
	virtual void setCalLight(bool value);
	virtual void setStatus(ObjStatus status);

	//void injectPropertiesToChildren();

public:
	
		
		
	std::vector<CHShowObjPtr> m_baseShowObjs;

};


#endif
