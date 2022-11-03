#include "CHAssembly.h"



CHAssembly::CHAssembly()
{
	m_priority = Mesh;
}

CHAssembly::~CHAssembly()
{
}

bool CHAssembly::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
	std::vector<PickResult> allRes;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setTransform(getTransform());

		PickResult tRes;
		if (m_baseShowObjs[i]->pick(pixelX, pixelY, tRes, pickTol))
		{
			allRes.push_back(tRes);
		}
	}
	
	if (allRes.size() == 0)
	{
		return false;
	}

	if (allRes.size() > 1)
	{
		std::sort(allRes.begin(), allRes.end(), CHShowObj::pred1);
	}

	result = allRes[0];
	result.m_pickObj = shared_from_this();
	return true;
}

void CHAssembly::updateToScene()
{
	//injectPropertiesToChildren();

	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
        m_baseShowObjs[i]->updateToScene();
	}
}

CHAABB3D CHAssembly::calBaseAABB()
{
	m_baseAABB.makeEmpty();
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseAABB.add(m_baseShowObjs[i]->calBaseAABB());
	}
	return m_baseAABB;
}

CHAABB3D CHAssembly::calRealAABB()
{
	m_realAABB.makeEmpty();
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_realAABB.add(m_baseShowObjs[i]->calRealAABB());
	}
	return m_realAABB;
}

void CHAssembly::setColor(QColor color)
{
	m_color = color;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setColor(color);
	}
}

void CHAssembly::setTransform(QMatrix4x4 tran)
{
	m_tran = tran;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setTransform(tran);
	}
}

void CHAssembly::setVisuable(bool show)
{
	m_visuable = show;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setVisuable(show);
	}
}

void CHAssembly::setDelete(bool del)
{
	m_delete = del;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setDelete(del);
	}
}

void CHAssembly::setDirty(bool dirty)
{
	m_dirty = dirty;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setDirty(dirty);
	}
}

void CHAssembly::setLightTest(bool value)
{
	m_lightTest = value;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setLightTest(value);
	}
}

void CHAssembly::setCalLight(bool value)
{
	m_calLight = value;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setCalLight(value);
	}
}

void CHAssembly::setStatus(ObjStatus status)
{
	m_status = status;
	for (int i = 0; i < m_baseShowObjs.size(); i++)
	{
		m_baseShowObjs[i]->setStatus(status);
	}
}

//void CHAssembly::injectPropertiesToChildren()
//{
//	for (int i = 0; i < m_baseShowObjs.size(); i++)
//	{
//		m_baseShowObjs[i]->setColor(m_color);
//		m_baseShowObjs[i]->setVisuable(m_visuable);
//		m_baseShowObjs[i]->setDelete(m_delete);
//		m_baseShowObjs[i]->setLightTest(m_lightTest);
//		m_baseShowObjs[i]->setCalLight(m_calLight);
//		m_baseShowObjs[i]->setDirty(m_dirty);
//		m_baseShowObjs[i]->setStatus(m_status);
//		m_baseShowObjs[i]->setTransform(m_tran);
//	}
//}












