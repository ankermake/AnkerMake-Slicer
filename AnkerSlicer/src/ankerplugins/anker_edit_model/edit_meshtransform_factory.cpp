#include "edit_meshtransform_factory.h"
#include "edit_meshrotationtransform.h"
#include "common/plugins/interfaces/meshlab_plugin.h"
#include "edit_meshmovetransform.h"
#include "edit_meshzoomtransform.h"
#include "edit_meshmirrortransform.h"
#include "vcg/space/box3.h"
//#include "edit_manualtreesupport.h"
#include <common/utilities/tlogger.h>


bool m_lockToPrintPlatform = false;
bool m_lockScaleRatio = true;
bool m_clearSupport = false;
int operateType = 0;

ControlInterface* EditMeshTransformFactory::m_conInt = nullptr;


EditMeshTransformFactory::EditMeshTransformFactory() {
    __initActionEditTools();
}
EditMeshTransformFactory::~EditMeshTransformFactory() {
     AkUtil::TFunction("");
}


///  AnkerPluginBase interface
QString EditMeshTransformFactory::pluginName() const
{
    return AkConst::Plugin::FDM_EDIT_MODEL;
}


void EditMeshTransformFactory::recMsgfromManager(PluginMessageData metaData)
{
    
//    if (metaData.from == AkConst::Plugin::FDM_SETTING
//        && metaData.msg == AkConst::Msg::GET_GLOBAL_SUPPORT_DISABLED_STATUS){
//        for (auto* action : actionList) {
//            if (action->objectName() == "Support")
//            {
//                auto tool = dynamic_cast<ActionEditTool*>(action);
//                if (tool == nullptr)
//                {
//                    return;
//                }
//                auto sptTool = dynamic_cast<ManualTreeSupportTool*>(tool->ankerEditTool);
//                if (sptTool == nullptr)
//                {
//                    return;
//                }

                
//                PluginMessageData data;
//                data.from = AkConst::Plugin::FDM_EDIT_MODEL;
//                data.dest = AkConst::Plugin::FDM_SETTING;
//                data.msg = AkConst::Msg::GLOBAL_SUPPORT_DISABLED;
//                data.map.insert(AkConst::Param::GLOBAL_SUPPORT_DISABLED_STATUS, sptTool->globalSupportForbidden);
//                emit sendMsg2Manager(data);
//                return;
//            }
//        }
//    }

    
//    else if (metaData.from == AkConst::Plugin::FDM_SLICER
//             && metaData.msg == AkConst::Msg::GET_GLOBAL_SUPPORT_DISABLED_STATUS)
//    {
//        for (auto* action : actionList) {
//            if (action->objectName() == "Support")
//            {
//                auto tool = dynamic_cast<ActionEditTool*>(action);
//                if (tool == nullptr)
//                {
//                    return;
//                }
//                auto sptTool = dynamic_cast<ManualTreeSupportTool*>(tool->ankerEditTool);
//                if (sptTool == nullptr)
//                {
//                    return;
//                }

                
//                PluginMessageData data;
//                data.from = AkConst::Plugin::FDM_EDIT_MODEL;
//                data.dest = AkConst::Plugin::FDM_SLICER;
//                data.msg = AkConst::Msg::GLOBAL_SUPPORT_DISABLED;
//                data.map.insert(AkConst::Param::GLOBAL_SUPPORT_DISABLED_STATUS, sptTool->globalSupportForbidden);
//                emit sendMsg2Manager(data);
//                return;
//            }
//        }
//    }
}

//void EditMeshTransformFactory::sendMsg2Manager(PluginMessageData)
//{

//}
void EditMeshTransformFactory::initialize(ControlInterface* controlmanager, RichParameterList* globalParameterList)
{
    m_conInt = controlmanager;
    for (auto* action : actionList) {
        //action->setCheckable(false); //  delete @2022-03-07 by CL
        action->setCheckable(true);
        action->setEnabled(false);
        m_conInt->addActionToModelTranform(action);
    }
    m_currAction = m_moveMeshTransform;
}

///  AnkerEditPlugin interface
AnkerEditTool* EditMeshTransformFactory::getEditTool(const QAction* action)
{
    const ActionEditTool* actionEditTool = qobject_cast<const ActionEditTool*>(action);
    if (actionEditTool) {
        return actionEditTool->ankerEditTool;
    }
    Q_ASSERT(actionEditTool);
    return nullptr;
}

QString EditMeshTransformFactory::getEditToolDescription(const QAction* action)
{
    const ActionEditTool* actionEditTool = qobject_cast<const ActionEditTool*>(action);
    if (actionEditTool) {
        return actionEditTool->getDescription();
    }
    return tr("Rotate Mesh Model");
}


/// AnkerEditTool interface
void EditMeshTransformFactory::setEditToolsEnable(QVariant var)
{
    if(var.canConvert<int>()){
        int count = var.toInt();
        bool enable = count > 0;
        for(auto act : actionList){
            if(false == enable){
                act->setChecked(false); 
            }
            act->setEnabled(enable);
        }

        if(enable && m_currAction){
            if(m_currAction->isChecked()){
                m_currAction->setChecked(false);
            }
            m_currAction->setChecked(true);
        }
    }
}

bool EditMeshTransformFactory::startAnkerEdit(ActionEditTool *action, void *arg1, void *arg2)
{
    bool res = action->ankerEditTool->startAnkerEdit(action, arg1, arg2);
    if(res){
        m_currAction  = action;
        m_isActivated = true  ;
    }
    return res;
}

void EditMeshTransformFactory::endAnkerEdit(ActionEditTool *action, void *arg1, void *arg2)
{
    if(action->ankerEditTool->isActivated()){
        action->ankerEditTool->endAnkerEdit(action, arg1, arg2);
        m_isActivated = false ;
    }
}

void EditMeshTransformFactory::mousePressEvent(QMouseEvent *event, void *arg1, void *arg2)
{

}

void EditMeshTransformFactory::mouseMoveEvent(QMouseEvent *event, void *arg1, void *arg2)
{

}

void EditMeshTransformFactory::mouseReleaseEvent(QMouseEvent *event, void *arg1, void *arg2)
{

}

void EditMeshTransformFactory::keyReleaseEvent(QKeyEvent *event, void *arg1, void *arg2)
{

}

void EditMeshTransformFactory::keyPressEvent(QKeyEvent *event, void *arg1, void *arg2)
{

}

void EditMeshTransformFactory::wheelEvent(QWheelEvent *event, void *arg1, void *arg2)
{

}

void EditMeshTransformFactory::tableEvent(QTabletEvent *event, void *arg1, void *arg2)
{

}


void EditMeshTransformFactory::__initActionEditTools()
{
    {
        QIcon iconMove;
        iconMove.addPixmap(QPixmap(":/images/fdm_move_icon_n.png"),QIcon::Normal, QIcon::Off);
        iconMove.addPixmap(QPixmap(":/images/fdm_move_icon_s.png"),QIcon::Normal, QIcon::On);
        iconMove.addPixmap(QPixmap(":/images/fdm_move_icon_e.png"),QIcon::Disabled);
        m_moveMeshTransform = new ActionEditTool(iconMove, tr("Move"), this);
        m_moveMeshTransform->setObjectName("Move");
        auto ankerEditTool  = new EditMeshMoveTransformTool();
        m_moveMeshTransform->setDescription("Move Mesh Model");//QActionEvent
        ankerEditTool->setParent(m_moveMeshTransform);
        m_moveMeshTransform->ankerEditTool   = ankerEditTool;
        m_moveMeshTransform->ankerEditPlugin = this;
        actionList.push_back(m_moveMeshTransform);
    }

    {
        QIcon iconScale;
        iconScale.addPixmap(QPixmap(":/images/fdm_scale_icon_n.png"),QIcon::Normal, QIcon::Off);
        iconScale.addPixmap(QPixmap(":/images/fdm_scale_icon_s.png"),QIcon::Normal, QIcon::On);
        iconScale.addPixmap(QPixmap(":/images/fdm_scale_icon_e.png"),QIcon::Disabled);
        m_scaleMeshTransform = new ActionEditTool(iconScale, tr("Zoom"), this);
        m_scaleMeshTransform->setObjectName("Zoom");
        auto ankerEditTool = new EditMeshZoomTransformTool();
        ankerEditTool->setParent(m_scaleMeshTransform);
        m_scaleMeshTransform->setDescription(tr("Zoom Mesh Model"));
        m_scaleMeshTransform->ankerEditTool   = ankerEditTool;
        m_scaleMeshTransform->ankerEditPlugin = this;
        actionList.push_back(m_scaleMeshTransform);
    }

    {
        QIcon iconRotate;
        iconRotate.addPixmap(QPixmap(":/images/fdm_rotate_icon_n.png"),QIcon::Normal, QIcon::Off);
        iconRotate.addPixmap(QPixmap(":/images/fdm_rotate_icon_s.png"),QIcon::Normal, QIcon::On);
        iconRotate.addPixmap(QPixmap(":/images/fdm_rotate_icon_e.png"),QIcon::Disabled);
        m_rotateMeshTransform = new ActionEditTool(iconRotate, tr("Rotate"), this);
        m_rotateMeshTransform->setObjectName("Rotate");
        auto ankerEditTool = new EditMeshRotationTransformTool();
        ankerEditTool->setParent(m_rotateMeshTransform);
        m_rotateMeshTransform->setDescription("Rotate Mesh Model");
        m_rotateMeshTransform->ankerEditTool   = ankerEditTool;
        m_rotateMeshTransform->ankerEditPlugin = this;
        actionList.push_back(m_rotateMeshTransform);
    }

    {
        QIcon iconMirror;
        iconMirror.addPixmap(QPixmap(":/images/fdm_mirror_icon_n.png"),QIcon::Normal, QIcon::Off);
        iconMirror.addPixmap(QPixmap(":/images/fdm_mirror_icon_s.png"),QIcon::Normal, QIcon::On);
        iconMirror.addPixmap(QPixmap(":/images/fdm_mirror_icon_e.png"),QIcon::Disabled);
        m_mirrorMeshTransform = new ActionEditTool(iconMirror, tr("Mirror"), this);
        m_mirrorMeshTransform->setObjectName("Mirror");
        auto ankerEditTool = new EditMeshMirrorTransformTool();
        ankerEditTool->setParent(m_mirrorMeshTransform);
        m_mirrorMeshTransform->setDescription("Mirror Mesh Model");
        m_mirrorMeshTransform->ankerEditTool   = ankerEditTool;
        m_mirrorMeshTransform->ankerEditPlugin = this;
        actionList.push_back(m_mirrorMeshTransform);
    }


//    {
//        QIcon iconTreeSupport;
//        iconTreeSupport.addPixmap(QPixmap(":/images/fdm_manual_icon_n.png"),QIcon::Normal, QIcon::Off);
//        iconTreeSupport.addPixmap(QPixmap(":/images/fdm_manual_icon_s.png"),QIcon::Normal, QIcon::On);
//        iconTreeSupport.addPixmap(QPixmap(":/images/fdm_manual_icon_e.png"),QIcon::Disabled);
//        m_manualTreeSupport = new ActionEditTool(iconTreeSupport, tr("Support"), this);
//        m_manualTreeSupport->setObjectName("Support");
//        auto ankerEditTool = new ManualTreeSupportTool();

//        ankerEditTool->setParent(m_manualTreeSupport);
//        m_manualTreeSupport->setDescription(tr("Tree Support"));
//        m_manualTreeSupport->ankerEditTool = ankerEditTool;
//        m_manualTreeSupport->ankerEditPlugin = this;
//        actionList.push_back(m_manualTreeSupport);
//    }

}

void EditMeshTransformFactory::initMeshModelEulerParams(CHMeshShowObjPtr tmm)
{
    //vcg::Box3f aabb = tmm->bbox;
    CHAABB3D aabb = tmm->getBaseAABB();
    if ((int)tmm->m_params.size() == 0)
    {
        //vcg::Point3f pt = aabb.Center();
        QVector3D pt = aabb.getCenterPoint();

        tmm->m_rotCenter[0] = pt[0];
        tmm->m_rotCenter[1] = pt[1];
        tmm->m_rotCenter[2] = pt[2];
        tmm->m_params.resize(9);
        for (int i = 0; i < 3; i++)
        {
            tmm->m_params[i] = 1;
        }
        for (int i = 3; i < 9; i++)
        {
            tmm->m_params[i] = 0;
        }
    }
}


void EditMeshTransformFactory::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (m_moveMeshTransform != nullptr) {
            m_moveMeshTransform->setText(tr("Move"));
            m_moveMeshTransform->setDescription(tr("Move Mesh Model"));
        }
        if (m_scaleMeshTransform != nullptr) {
            m_scaleMeshTransform->setText(tr("Zoom"));
            m_scaleMeshTransform->setDescription(tr("Zoom Mesh Model"));
        }
        if (m_rotateMeshTransform != nullptr) {
            m_rotateMeshTransform->setText(tr("Rotate"));
            m_rotateMeshTransform->setDescription(tr("Rotate Mesh Model"));
        }
        if (m_mirrorMeshTransform != nullptr) {
            m_mirrorMeshTransform->setText(tr("Mirror"));
            m_mirrorMeshTransform->setDescription(tr("Mirror Mesh Model"));
        }
        if (m_manualTreeSupport != nullptr) {
            m_manualTreeSupport->setText(tr("Support"));
            m_manualTreeSupport->setDescription(tr("Tree Support"));
        }
    }
}


MESHLAB_PLUGIN_NAME_EXPORTER(EditMeshTransformFactory)

