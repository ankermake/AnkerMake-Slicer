#include "SupportAssemblyMeshes.h"

SupportAssemblyMeshes::SupportAssemblyMeshes()
{
    m_priority = Mesh;
}

SupportAssemblyMeshes::~SupportAssemblyMeshes()
{
}

bool SupportAssemblyMeshes::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
    std::vector<PickResult> allRes;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        //m_baseShowObjs[i]->setTransform(getTransform());

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

void SupportAssemblyMeshes::updateToScene()
{
    //injectPropertiesToChildren();

    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->updateToScene();
    }
}

CHAABB3D SupportAssemblyMeshes::calBaseAABB()
{
    m_baseAABB.makeEmpty();
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseAABB.add(m_baseShowObjs[i]->calBaseAABB());
    }
    return m_baseAABB;
}

CHAABB3D SupportAssemblyMeshes::calRealAABB()
{
    m_realAABB.makeEmpty();
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_realAABB.add(m_baseShowObjs[i]->calRealAABB());
    }
    return m_realAABB;
}

void SupportAssemblyMeshes::setColor(QColor color)
{
    m_color = color;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setColor(color);
    }
}

void SupportAssemblyMeshes::setTransform(QMatrix4x4 tran)
{
    m_tran = tran;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        //m_baseShowObjs[i]->setTransform(tran * m_baseShowObjs[i]->m_initTransform);
        m_baseShowObjs[i]->setTransform(tran * m_baseShowObjs[i]->getLocalTransform());
    }
}

void SupportAssemblyMeshes::setVisuable(bool show)
{
    m_visuable = show;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setVisuable(show);
    }
}

void SupportAssemblyMeshes::setDelete(bool del)
{
    m_delete = del;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setDelete(del);
    }
}

void SupportAssemblyMeshes::setDirty(bool dirty)
{
    m_dirty = dirty;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setDirty(dirty);
    }
}

void SupportAssemblyMeshes::setLightTest(bool value)
{
    m_lightTest = value;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setLightTest(value);
    }
}

void SupportAssemblyMeshes::setCalLight(bool value)
{
    m_calLight = value;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setCalLight(value);
    }
}

void SupportAssemblyMeshes::setStatus(ObjStatus status)
{
    m_status = status;
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setStatus(status);
    }
}

void SupportAssemblyMeshes::append(const SupportMeshPtr supportPtr)
{
    m_baseShowObjs.push_back(supportPtr);
    calBaseAABB();
    calRealAABB();
}

void SupportAssemblyMeshes::copy(CHShowObjPtr& obj)
{
    if (!obj)
    {
        obj = SupportAssemblyMeshesPtr(new SupportAssemblyMeshes);
    }

    SupportAssemblyMeshesPtr out = std::dynamic_pointer_cast<SupportAssemblyMeshes>(obj);
    if (!out)
    {
        return;
    }
    out->m_baseShowObjs = m_baseShowObjs;

    out->setParent(m_parent);
    out->setObjectName(m_name);

    out->m_baseAABB = m_baseAABB;
    out->m_realAABB = m_realAABB;
    out->setColor(m_color);
    out->setLightTest(m_lightTest);
    out->setCalLight(m_calLight);
    out->setTransform(m_tran);

    out->m_vertices = m_vertices;
    out->m_nors = m_nors;
    out->m_uvs = m_uvs;
    out->m_trians = m_trians;
    out->m_initTransform = m_initTransform;
    out->m_rotCenter[0] = m_rotCenter[0];
    out->m_rotCenter[1] = m_rotCenter[1];
    out->m_rotCenter[2] = m_rotCenter[2];
    out->m_params = m_params;
}
