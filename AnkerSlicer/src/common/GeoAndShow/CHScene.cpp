#include "CHScene.h"

ActionEditTool * currActionEdit = nullptr;
CHScene* curScene = nullptr;
CHScene::CHScene()
{
    m_pickCommand = CHPickOperationCommandPtr(new CHPickOperationCommand);
    m_doc = CHDocPtr(new CHDoc);
}

CHScene::~CHScene()
{

}

void CHScene::updateShowObj(CHShowObjPtr showobj)
{
    if (std::dynamic_pointer_cast<CHPointShowObj>(showobj))
    {
        updatePoint(std::dynamic_pointer_cast<CHPointShowObj>(showobj));
    }
    else if (std::dynamic_pointer_cast<CHCurveShowObj>(showobj))
    {
        updateCurve(std::dynamic_pointer_cast<CHCurveShowObj>(showobj));
    }
    else if (std::dynamic_pointer_cast<CHMeshShowObj>(showobj))
    {
        updateMesh(std::dynamic_pointer_cast<CHMeshShowObj>(showobj));
    }
    refresh();
}

void CHScene::updateDoc()
{
    auto bIt = m_doc->m_printObjs.begin();
    auto eIt = m_doc->m_printObjs.end();

    while (bIt != eIt)
    {
//        (*bIt)->cmeshToMyModel();
        (*bIt)->cmeshoToMeshShowObj(*(*bIt)->m_oldMesh);
        (*bIt)->calBaseAABB();
        (*bIt)->calRealAABB();
        (*bIt)->updateToScene();
        (*bIt)->updateSupportMeshes();

        bIt++;
    }
    m_doc->modelCheckSceneIn();
}

CHPickOperationCommandPtr getGlobalPick()
{
    if (curScene)
    {
        return curScene->m_pickCommand;
    }
    return 0;
}

CHDocPtr getDoc()
{
    if (curScene)
    {
        return curScene->m_doc;
    }
    return 0;
}
