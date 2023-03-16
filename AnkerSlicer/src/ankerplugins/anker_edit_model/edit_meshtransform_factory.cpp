#include "edit_meshtransform_factory.h"
#include "edit_meshrotationtransform.h"
#include "common/plugins/interfaces/meshlab_plugin.h"
#include "edit_meshmovetransform.h"
#include "edit_meshzoomtransform.h"
#include "edit_meshmirrortransform.h"
#include "vcg/space/box3.h"
//#include "edit_manualtreesupport.h"


bool m_lockToPrintPlatform = false;
bool m_lockScaleRatio = true;
bool m_clearSupport = false;
int operateType = 0;

ControlInterface* EditMeshTransformFactory::m_conInt = nullptr;


EditMeshTransformFactory::EditMeshTransformFactory() {
    __initActionEditTools();
}
EditMeshTransformFactory::~EditMeshTransformFactory() {
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
        QIcon icon_n(":/images/fdm_move_icon_n.png");
        QIcon icon_s(":/images/fdm_move_icon_s.png");
//        QIcon *pMoveIcon = new QIcon(":/images/fdm_move_icon_n.png");//Active
////        pMoveIcon->addPixmap(QPixmap(":/images/fdm_move_icon_n.png"), QIcon::Disabled, QIcon::On);
////        pMoveIcon->addPixmap(QPixmap(":/images/fdm_move_icon_s.png"), QIcon::Disabled, QIcon::Off);
//        pMoveIcon->addPixmap(QPixmap(":/images/fdm_move_icon_s.png"), QIcon::Selected, QIcon::On);
//        pMoveIcon->addPixmap(QPixmap(":/images/fdm_move_icon_s.png"), QIcon::Active, QIcon::On);
//        pMoveIcon->addPixmap(QPixmap(":/images/fdm_move_icon_n.png"), QIcon::Normal, QIcon::On);
// //       pMoveIcon->addPixmap(QPixmap(":/images/fdm_move_icon_n.png"), QIcon::Normal, QIcon::Off);

        //QIcon icon_e = QIcon(":/images/fdm_move_icon_e.png"); // is hovered
        m_moveMeshTransform = new ActionEditTool(icon_n, tr("Move"), this);
        //m_moveMeshTransform = new ActionEditTool(*pMoveIcon, tr("Move"), this);
        m_moveMeshTransform->setObjectName("Move");
        auto ankerEditTool  = new EditMeshMoveTransformTool();
        m_moveMeshTransform->setDescription("Move Mesh Model");//QActionEvent

        ankerEditTool->setParent(m_moveMeshTransform);
        m_moveMeshTransform->ankerEditTool   = ankerEditTool;
        m_moveMeshTransform->ankerEditPlugin = this;
        QObject::connect(m_moveMeshTransform, &QAction::toggled, [this, icon_s, icon_n](bool check){
            QIcon tmpIcon = m_moveMeshTransform->icon();
            m_moveMeshTransform->setIcon(check ? icon_s : icon_n);
        });

        /*
        QEvent *event= nullptr;
        m_moveMeshTransform->eventFilter(this, event);
        event->type()
*/
//        QObject::connect(m_moveMeshTransform, &QAction::changed, [this, icon_s, icon_n]{
//            QEvent *event1= nullptr;
//            //m_moveMeshTransform->eventFilter(this, event);
//            this->event(event1);

//            if(event1)
//            {
//                qDebug() << "Changed: " << event1->type();
//            }

//        });

        actionList.push_back(m_moveMeshTransform);
    }

    {
        QIcon icon_s = QIcon(":/images/fdm_scale_icon_s.png"); // is strat
        QIcon icon_n = QIcon(":/images/fdm_scale_icon_n.png"); // is end
        m_scaleMeshTransform = new ActionEditTool(icon_n, tr("Zoom"), this);
        m_scaleMeshTransform->setObjectName("Zoom");
        auto ankerEditTool = new EditMeshZoomTransformTool();
        ankerEditTool->setParent(m_scaleMeshTransform);
        m_scaleMeshTransform->setDescription(tr("Zoom Mesh Model"));
        m_scaleMeshTransform->ankerEditTool   = ankerEditTool;
        m_scaleMeshTransform->ankerEditPlugin = this;
        QObject::connect(m_scaleMeshTransform, &QAction::toggled, [this, icon_s, icon_n](bool check){
            m_scaleMeshTransform->setIcon(check ? icon_s : icon_n);
        });
        actionList.push_back(m_scaleMeshTransform);
    }

    {
        QIcon icon_s = QIcon(":/images/fdm_rotate_icon_s.png"); // is strat
        QIcon icon_n = QIcon(":/images/fdm_rotate_icon_n.png"); // is end
        m_rotateMeshTransform = new ActionEditTool(icon_n, tr("Rotate"), this);
        m_rotateMeshTransform->setObjectName("Rotate");
        auto ankerEditTool = new EditMeshRotationTransformTool();
        ankerEditTool->setParent(m_rotateMeshTransform);
        m_rotateMeshTransform->setDescription("Rotate Mesh Model");
        m_rotateMeshTransform->ankerEditTool   = ankerEditTool;
        m_rotateMeshTransform->ankerEditPlugin = this;
        QObject::connect(m_rotateMeshTransform, &QAction::toggled, [this, icon_s, icon_n](bool check){
            m_rotateMeshTransform->setIcon(check ? icon_s : icon_n);
        });
        actionList.push_back(m_rotateMeshTransform);
    }

    {
        QIcon icon_s = QIcon(":/images/fdm_mirror_icon_s.png"); // is strat
        QIcon icon_n = QIcon(":/images/fdm_mirror_icon_n.png"); // is end
        m_mirrorMeshTransform = new ActionEditTool(icon_n, tr("Mirror"), this);
        m_mirrorMeshTransform->setObjectName("Mirror");
        auto ankerEditTool = new EditMeshMirrorTransformTool();
        ankerEditTool->setParent(m_mirrorMeshTransform);
        m_mirrorMeshTransform->setDescription("Mirror Mesh Model");
        m_mirrorMeshTransform->ankerEditTool   = ankerEditTool;
        m_mirrorMeshTransform->ankerEditPlugin = this;
        QObject::connect(m_mirrorMeshTransform, &QAction::toggled, [this, icon_s, icon_n](bool check){
            m_mirrorMeshTransform->setIcon(check ? icon_s : icon_n);
        });
        actionList.push_back(m_mirrorMeshTransform);
    }


//    {
//        QIcon icon_s = QIcon(":/images/fdm_manual_icon_s.png"); // is strat
//        QIcon icon_n = QIcon(":/images/fdm_manual_icon_n.png"); // is end
//        m_manualTreeSupport = new ActionEditTool(icon_n, tr("Support"), this);
//        m_manualTreeSupport->setObjectName("Support");
//        auto ankerEditTool = new ManualTreeSupportTool();

//        ankerEditTool->setParent(m_manualTreeSupport);
//        m_manualTreeSupport->setDescription(tr("Tree Support"));
//        m_manualTreeSupport->ankerEditTool = ankerEditTool;
//        m_manualTreeSupport->ankerEditPlugin = this;
//        QObject::connect(m_manualTreeSupport, &QAction::toggled, [this, icon_s, icon_n](bool check) {
//            m_manualTreeSupport->setIcon(check ? icon_s : icon_n);
//            });
//        actionList.push_back(m_manualTreeSupport);
//    }

//    {
//        QIcon icon_s = QIcon(":/images/fdm_manual_icon_s.png"); // is strat
//        QIcon icon_n = QIcon(":/images/fdm_manual_icon_n.png"); // is end
//        m_manualTreeSupport = new ActionEditTool(icon_n, tr("Support"), this);
//        m_manualTreeSupport->setObjectName("Support");
//        auto ankerEditTool = new ManualTreeSupportTool();

//        ankerEditTool->setParent(m_manualTreeSupport);
//        m_manualTreeSupport->setDescription(tr("Tree Support"));
//        m_manualTreeSupport->ankerEditTool = ankerEditTool;
//        m_manualTreeSupport->ankerEditPlugin = this;
//        QObject::connect(m_manualTreeSupport, &QAction::toggled, [this, icon_s, icon_n](bool check) {
//            m_manualTreeSupport->setIcon(check ? icon_s : icon_n);
//            });
//        actionList.push_back(m_manualTreeSupport);
//    }

    //{
    //    QIcon icon_s = QIcon(":/images/fdm_manual_icon_s.png"); // is strat
    //    QIcon icon_e = QIcon(":/images/fdm_manual_icon_e.png"); // is end
    //    m_manualSupport = new ActionEditTool(icon_e, tr("Support"), this);
    //    auto ankerEditTool = new ManualSupportTool();
    //    ankerEditTool->setParent(m_manualSupport);
    //    m_manualSupport->setDescription(tr("Manual Support"));
    //    m_manualSupport->ankerEditTool   = ankerEditTool;
    //    m_manualSupport->ankerEditPlugin = this;
    //    QObject::connect(m_manualSupport, &QAction::toggled, [this, icon_s, icon_e](bool check){
    //        m_manualSupport->setIcon(check ? icon_s : icon_e);
    //    });
    //    actionList.push_back(m_manualSupport);
    //}


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

