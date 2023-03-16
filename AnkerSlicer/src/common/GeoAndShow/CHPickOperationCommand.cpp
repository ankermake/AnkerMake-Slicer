#include "../ml_document/mesh_model.h"
#include "CHPickOperationCommand.h"
#include "CHScene.h"
#include "QMenu"
#include "CHBaseAlg.h"
#include "../common/plugins/plugin_manager.h"
//#include "../common/globals.h"






SceneRightMenu::SceneRightMenu(QObject *scenePick)
{
    
    connect(scenePick, &QObject::destroyed, this, &ThisType::deleteLater);

    {
        actDelete = new QAction(tr("Delete"), this);
        addAction(actDelete);
        connect(actDelete, SIGNAL(triggered(bool)), scenePick, SLOT(deleteSelectedObjs()));
    }

    if(true){
        
        actCopy = new QAction(tr("Copy"), this);
        addAction(actCopy);
        connect(actCopy, SIGNAL(triggered(bool)), scenePick, SLOT(copySelectedAndPasteObjs()));
    }
    else{
        actCopy = new QAction(tr("Copy"), this);
        addAction(actCopy);
        connect(actCopy, SIGNAL(triggered(bool)), scenePick, SLOT(copySelectedObjs()));

        actPaste = new QAction(tr("Paste"), this);
        addAction(actPaste);
        connect(actPaste, SIGNAL(triggered(bool)), scenePick, SLOT(pasteObjs()));
    }

    {
        actHide = new QAction(tr("Hide"), this);
        addAction(actHide);
        connect(actHide, SIGNAL(triggered(bool)), scenePick, SLOT(hideSelectedObjs()));
    }

    if(false){
        actShow = new QAction(tr("Show"), this);
        addAction(actShow);
        connect(actShow, SIGNAL(triggered(bool)), scenePick, SLOT(showSelectedObjs()));
    }

    {
        actReset = new QAction(tr("Reset"), this);
        addAction(actReset);
        connect(actReset, SIGNAL(triggered(bool)), scenePick, SIGNAL(resetSeletedObjsSig()));
    }
}



CHPickOperationCommand::CHPickOperationCommand()
{
    m_tmesh = 0;
    m_keyDown = false;
    rightMenu = new SceneRightMenu(this);
    //connect(this, SIGNAL(resetSeletedObjsSig()), this, SLOT(resetSelectedObjs()));
}

CHPickOperationCommand::~CHPickOperationCommand()
{

}

void CHPickOperationCommand::mousePressEvent(QMouseEvent* event)
{
    
    
    auto singleSelected = [this](){
        for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
        {
            (*it)->setStatus(general);
        }
        bool flag = !(m_selectedObjs.size() == 1 && *m_selectedObjs.begin() == m_tmesh)
                && (m_selectedObjs.size() > 0 || m_tmesh);

        m_selectedObjs.clear();
        if (m_tmesh)
        {
            m_tmesh->setStatus(selected);
            m_selectedObjs.insert(m_tmesh);
        }

        if (flag)
        {
            emit selectedObjsChanged(QVariant::fromValue(m_selectedObjs.size()));
        }
    };

    
    auto multipleSelected = [this](){
        if (m_tmesh)
        {
            std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.find(m_tmesh);

            if (it == m_selectedObjs.end())
            {
                m_tmesh->setStatus(selected);
                m_selectedObjs.insert(m_tmesh);
            }
            else
            {
                m_tmesh->setStatus(general);
                m_selectedObjs.erase(it);
            }
            emit selectedObjsChanged(QVariant::fromValue(m_selectedObjs.size()));
        }
    };


//    auto sceneMenu = [this](){

//        if (m_tmesh && m_tmesh->getStatus() == canPicked)
//        {
//            m_tmesh->setStatus(general);
//            m_tmesh = 0;
//            curScene->refresh();
//        }

//        bool hasSelected = (havePickedPrintObjs() || haveCanPickedObj());//(m_selectedObjs.size() > 0);
//        rightMenu->setEnabledDelete(hasSelected);
//        rightMenu->setEnabledCopy  (hasSelected);
//        rightMenu->setEnabledHide  (hasSelected);
//        rightMenu->setEnabledShow  (hasSelected);
//        rightMenu->setEnabledReset (hasSelected);
//        rightMenu->setEnabled(hasSelected);

//        if (m_copiedObjs.size() == 0){
//            rightMenu->setEnabledPaste (false);
//        }
//        rightMenu->show();
//        rightMenu->popup(QCursor::pos());
//    };

    
    
    if (auto action = inAnkerEditMode()){
        event->ignore();
        action->ankerEditTool->mousePressEvent(event);
        if(event->isAccepted()){
            return;
        }
        event->accept();
    }

    if (event->button() == Qt::LeftButton)
    {
        if (m_keyDown)
        {
            multipleSelected(); 
        }
        else
        {
            singleSelected();   
        }
        curScene->refresh();

        m_tmesh = 0;
    }
    else if (event->button() == Qt::RightButton)
    {
        
    }
}

void CHPickOperationCommand::mouseMoveEvent(QMouseEvent* event)
{
    
    auto pickMeshes = [event]() -> CHMeshShowObjPtr
    {
        QVector3D np, fp;
        curScene->getCurNearFarPoint(event->pos().x(), event->pos().y(), np, fp);

        CHMeshShowObjPtr pickModel = nullptr;
        float minDis = FLT_MAX;
        for (int i = 0; i < getDoc()->m_printObjs.size(); i++)
        {
            
            if (!getDoc()->m_printObjs[i]->getVisuable())
            {
                continue;
            }

            PickResult pr;
            if (getDoc()->m_printObjs[i]->pick(event->pos().x(), event->pos().y(), pr) && pr.m_pointOnMesh.m_pt.distanceToPoint(np) < minDis)
            {
                minDis = pr.m_pointOnMesh.m_pt.distanceToPoint(np);
                pickModel = getDoc()->m_printObjs[i];
            }

            
            for (int j = 0; j < getDoc()->m_printObjs[i]->m_supportMeshes.size(); j++)
            {
                auto supportMesh = getDoc()->m_printObjs[i]->m_supportMeshes[j];
                if (supportMesh->pick(event->pos().x(), event->pos().y(), pr) && pr.m_pointOnMesh.m_pt.distanceToPoint(np) < minDis)
                {
                    minDis = pr.m_pointOnMesh.m_pt.distanceToPoint(np);
                    pickModel = supportMesh;
                }
            }
        }
        return pickModel;
    };

    
    
    if (auto action = inAnkerEditMode()){
        event->ignore();
        action->ankerEditTool->mouseMoveEvent(event);
        if(event->isAccepted()){
            return;
        }
        event->accept();
    }

    CHMeshShowObjPtr pickModel = pickMeshes();
    if (pickModel && pickModel->getStatus() == general)
    {
        pickModel->setStatus(canPicked);
    }

    if (m_tmesh != pickModel)
    {
        if (m_tmesh && m_tmesh->getStatus() == canPicked)
        {
            m_tmesh->setStatus(general);
        }
        m_tmesh = pickModel;

        curScene->refresh();
    }
}

void CHPickOperationCommand::mouseReleaseEvent(QMouseEvent* event)
{
    
    if (auto action = inAnkerEditMode()){
        action->ankerEditTool->mouseReleaseEvent(event);
        if(event->isAccepted()){
            return;
        }
    }

    
    auto sceneMenu = [this](){
        
        if (m_tmesh && m_tmesh->getStatus() == canPicked)
        {
            m_tmesh->setStatus(general);
            m_tmesh = 0;
            curScene->refresh();
        }

        bool hasSelected = (havePickedPrintObjs() || haveCanPickedObj());//(m_selectedObjs.size() > 0);
        rightMenu->setEnabledDelete(hasSelected);
        rightMenu->setEnabledCopy  (hasSelected);
        rightMenu->setEnabledHide  (hasSelected);
        rightMenu->setEnabledShow  (hasSelected);
        rightMenu->setEnabledReset (hasSelected);
        rightMenu->setEnabled(hasSelected);

        if (m_copiedObjs.size() == 0){
            rightMenu->setEnabledPaste (false);
        }
        rightMenu->show();
        rightMenu->popup(QCursor::pos());
    };
    if (event->button() == Qt::RightButton && !mouseRightMoved)
    {
        sceneMenu(); 
    }
}

void CHPickOperationCommand::mouseDoubleClickEvent(QMouseEvent* event)
{
    
    if (auto action = inAnkerEditMode()){
        event->ignore();
        action->ankerEditTool->mouseDoubleClickEvent(event);
        if(event->isAccepted()){
            return;
        }
        event->accept();
    }
}

void CHPickOperationCommand::keyReleaseEvent(QKeyEvent* event)
{
    
    if (auto action = inAnkerEditMode()){
        event->ignore();
        action->ankerEditTool->keyReleaseEvent(event);
        if(event->isAccepted()){
            return;
        }
        event->accept();
    }

    if (event->key() == Qt::Key::Key_Control)
    {
        m_keyDown = false;
    }
}

void CHPickOperationCommand::keyPressEvent(QKeyEvent* event)
{
    
    if (auto action = inAnkerEditMode()){
        event->ignore();
        action->ankerEditTool->keyPressEvent(event);
        if(event->isAccepted()){
            return;
        }
        event->accept();
    }

    if (event->key() == Qt::Key::Key_Control)
    {
        m_keyDown = true;
    }
    else if (event->key() == Qt::Key::Key_Delete)
    {
        if (m_selectedObjs.size() > 0)
        {
            deleteSelectedObjs();
        }
    }
}

void CHPickOperationCommand::doWithFileListPickedChanged()
{
    m_selectedObjs.clear();
    //qDebug() << "doWithFileListPickedChanged.";
    for (int i = 0; i < getDoc()->m_printObjs.size(); i++)
    {
        //qDebug() << "name: " << getDoc()->m_printObjs[i]->getObjectName() << ", status: " << getDoc()->m_printObjs[i]->getStatus();
        if (getDoc()->m_printObjs[i]->getStatus() == selected && getDoc()->m_printObjs[i]->getVisuable())
        {
            m_selectedObjs.insert(getDoc()->m_printObjs[i]);

        }
    }

    
    if(auto action = inAnkerEditMode()){
        action->setChecked(false);
    }
    //if (curEditTool)
    //{
    //    curEditTool->endAnkerEdit(nullptr);
    //    preEditTool = curEditTool;
    //    curEditTool = 0;
    //}

    emit selectedObjsChanged(QVariant::fromValue(m_selectedObjs.size()));
}

void CHPickOperationCommand::selectAllobjs()
{
    m_selectedObjs.clear();
    for (int i = 0; i < getDoc()->m_printObjs.size(); i++)
    {
        getDoc()->m_printObjs[i]->setStatus(selected);
        if(getDoc()->m_printObjs[i]->getVisuable())
            m_selectedObjs.insert(getDoc()->m_printObjs[i]);
    }

    
    if(auto action = inAnkerEditMode()){
        action->setChecked(false);
    }
    //if (curEditTool)
    //{
    //    curEditTool->endAnkerEdit(nullptr);
    //    preEditTool = curEditTool;
    //    curEditTool = 0;
    //}
    curScene->refresh();

    emit selectedObjsChanged(QVariant::fromValue(m_selectedObjs.size()));
}

CHAABB3D CHPickOperationCommand::getPickedObjsAABB()
{
    CHAABB3D aabb;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        aabb.add((*it)->getRealAABB());
    }
    return aabb;
}

void CHPickOperationCommand::currentSelectedPrints(std::vector<CH3DPrintModelPtr> & selectedObjs)
{
    for(auto it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        if(std::dynamic_pointer_cast<CH3DPrintModel>(*it) != nullptr)
        {
            selectedObjs.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*it));
        }
    }
}

bool CHPickOperationCommand::havePickedObjs()
{
    if (m_selectedObjs.size() > 0)
    {
        return true;
    }
    return false;
}

bool CHPickOperationCommand::havePickedPrintObjs()
{
    auto itBegin = m_selectedObjs.begin();
    auto itEnd = m_selectedObjs.end();
    while (itBegin != itEnd)
    {
        if (std::dynamic_pointer_cast<CH3DPrintModel>(*itBegin) != nullptr)
        {
            return true;
        }
        itBegin++;
    }
    return false;
}

bool CHPickOperationCommand::haveEditPluginsActive()
{         
    PluginManager& pm = meshlab::pluginManagerInstance();
    QVector<AnkerEditPlugin*> editPlugins = pm.getAnkerEditPlugins();
    auto it = editPlugins.begin();
    while (it != editPlugins.end())
    {
        if ((*it)->isEnabled())
        {
            return true;
        }
        it++;
    }
    
    return false;
}

bool CHPickOperationCommand::haveCanPickedObj()
{
    if(m_tmesh == nullptr)
    {
        return false;
    }
    ObjStatus status = m_tmesh->getStatus();
    if(canPicked == status)
        return true;
    return false;
}

void CHPickOperationCommand::doWithDocMeshModelsChanged()
{
    int orginNum = m_selectedObjs.size();
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); /*it++*/)
    {
        int index = -1;
        for (int i = 0; i < getDoc()->m_printObjs.size(); i++)
        {
            if (getDoc()->m_printObjs[i] == *it)
            {
                index = i;
                break;
            }
        }

        
        if (!(index != -1 && (*it)->getVisuable()))
        {
            (*it)->setStatus(general);
            it = m_selectedObjs.erase(it);
        }
        else
        {
            it++;
        }
    }

    if (orginNum != m_selectedObjs.size())
    {
        
        if(auto action = inAnkerEditMode()){
            action->setChecked(false);
        }
        //if (curEditTool)
        //{
        //    curEditTool->endAnkerEdit(nullptr);
        //    preEditTool = curEditTool;
        //    curEditTool = 0;
        //}

        emit selectedObjsChanged(QVariant::fromValue(m_selectedObjs.size()));
    }
}

void CHPickOperationCommand::deleteSelectedObjs()
{
    std::vector<CH3DPrintModelPtr> meshes;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*it));
    }

    getDoc()->deleteObjs(meshes);
}

void CHPickOperationCommand::copySelectedAndPasteObjs()
{
    copySelectedObjs();
    pasteObjs();
    m_copiedObjs.clear();
    curScene->refresh();
}

void CHPickOperationCommand::copySelectedObjs()
{
    m_copiedObjs.clear();
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        if(std::dynamic_pointer_cast<CH3DPrintModel>(*it) != nullptr) 
        {
            m_copiedObjs.push_back(*it);
        }
    }
}

void CHPickOperationCommand::pasteObjs()
{
    std::vector<CH3DPrintModelPtr> meshes;
    for (int i = 0; i < m_copiedObjs.size(); i++)
    {
        CHShowObjPtr copymesh;
        m_copiedObjs[i]->copy(copymesh);
        
        CH3DPrintModelPtr ttt = std::dynamic_pointer_cast<CH3DPrintModel>(copymesh);
        QVector3D offset;
        ttt->calRealAABB();
        ttt->calBaseAABB();
        ttt->m_rotCenter = ttt->m_realAABB.getCenterPoint();
        offset[1] += 20;

        ttt->m_realAABB.m_Xmin = offset[0] + ttt->m_realAABB.m_Xmin;
        ttt->m_realAABB.m_Ymin = offset[1] + ttt->m_realAABB.m_Ymin;
        ttt->m_realAABB.m_Zmin = offset[2] + ttt->m_realAABB.m_Zmin;

        ttt->m_realAABB.m_Xmax = offset[0] + ttt->m_realAABB.m_Xmax;
        ttt->m_realAABB.m_Ymax = offset[1] + ttt->m_realAABB.m_Ymax;
        ttt->m_realAABB.m_Zmax = offset[2] + ttt->m_realAABB.m_Zmax;

        ttt->m_params.resize(9);
        ttt->m_params[0] = ttt->m_params[1] = ttt->m_params[2] =  1.0;
        ttt->m_params[3] = ttt->m_params[4] = ttt->m_params[5] =  0.0;
        ttt->m_params[6] = offset[0];
        ttt->m_params[7] = offset[1];
        ttt->m_params[8] = offset[2];

        QMatrix4x4 trans;
        trans.translate(offset[0], offset[1], offset[2]);
        ttt->setTransform(trans * ttt->getTransform());
        qDebug() << "Copy Transform: " << ttt->getTransform();
        ttt->m_initParams = ttt->m_params;
        ttt->m_initTransform = ttt->getTransform();
        ttt->isSceneIn(ttt->getRealAABB(), getDoc()->m_machineBox->getBaseAABB());
        ttt->updateToScene();
        meshes.push_back(ttt);
    }
    if(meshes.size() > 0)
    {
        getDoc()->addObjs(meshes);
    }
}

void CHPickOperationCommand::hideSelectedObjs()
{
    std::vector<CHMeshShowObjPtr>  meshes;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        meshes.push_back(std::dynamic_pointer_cast<CHMeshShowObj>(*it));
    }

    getDoc()->setObjsVisuable(meshes, false);
}

void CHPickOperationCommand::showSelectedObjs()
{
    std::vector<CH3DPrintModelPtr> meshes;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*it));
    }

    getDoc()->setObjsVisuable(meshes, true);
}

void CHPickOperationCommand::resetSelectedObjs()
{
    std::vector<CH3DPrintModelPtr> meshes;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_selectedObjs.begin(); it != m_selectedObjs.end(); it++)
    {
        meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*it));
    }
    
    getDoc()->resetObjs(meshes);
    if (curScene)
    {
        curScene->refresh();
    }
}

void SceneRightMenu::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (actDelete != nullptr) {
            actDelete->setText(tr("Delete"));
        }
        if (actCopy != nullptr) {
            actCopy->setText(tr("Copy"));
        }
        if (actPaste != nullptr) {
            actCopy->setText(tr("Paste"));
        }
        if (actHide != nullptr) {
            actHide->setText(tr("Hide"));
        }
        if (actShow != nullptr) {
            actShow->setText(tr("Show"));
        }
        if (actReset != nullptr) {
            actReset->setText(tr("Reset"));
        }
    }
}












