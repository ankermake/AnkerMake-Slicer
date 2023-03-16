#ifndef EditMeshTransformFactoryPLUGIN_H
#define EditMeshTransformFactoryPLUGIN_H

#include "common/plugins/interfaces/anker_edit_plugin.h"
#include <common/plugins/interfaces/edit_plugin.h>
#include <common/controlInterface/controlInterface.h>
#include "common/GeoAndShow/CHMeshShowObj.h"

using namespace control;


class EditMeshTransformFactory : public AnkerEditPlugin
{
    Q_OBJECT
    MESHLAB_PLUGIN_IID_EXPORTER(Anker_EDIT_PLUGIN_IID)
    Q_INTERFACES(AnkerEditPlugin)

    
public:
    EditMeshTransformFactory();
    ~EditMeshTransformFactory() override;

    void restartCurrentAction();


    void recMsgfromManager(PluginMessageData metaData) override;
signals:
    void sendMsg2Manager(PluginMessageData) override;

    ///  AnkerPluginBase interface
public:
    QString pluginName() const override;
//    void recMsgfromManager(PluginMessageData) override;
//    void sendMsg2Manager  (PluginMessageData) override;
    void initialize(ControlInterface *controlmanager, RichParameterList * globalParameterList) override;

    ///  AnkerEditPlugin interface
public:
    using AnkerEditPlugin::actions;
    AnkerEditTool* getEditTool(const QAction*) override;
    QString   getEditToolDescription(const QAction*) override;

    void setEditToolsEnable(QVariant) override;
    /// AnkerEditTool interface
protected:
    void changeEvent(QEvent *e);
public:
    bool startAnkerEdit  (ActionEditTool *action, void *arg1, void *arg2) override;
    void endAnkerEdit    (ActionEditTool *action, void *arg1, void *arg2) override;
    void mousePressEvent    (QMouseEvent  *event, void *arg1, void *arg2) override;
    void mouseMoveEvent     (QMouseEvent  *event, void *arg1, void *arg2) override;
    void mouseReleaseEvent  (QMouseEvent  *event, void *arg1, void *arg2) override;
    void keyReleaseEvent    (QKeyEvent    *event, void *arg1, void *arg2) override;
    void keyPressEvent      (QKeyEvent    *event, void *arg1, void *arg2) override;
    void wheelEvent         (QWheelEvent  *event, void *arg1, void *arg2) override;
    void tableEvent         (QTabletEvent *event, void *arg1, void *arg2) override;


    
private:
    void __initActionEditTools();
    
    ActionEditTool* m_moveMeshTransform   {nullptr};
    ActionEditTool* m_rotateMeshTransform {nullptr};
    ActionEditTool* m_scaleMeshTransform  {nullptr};
    ActionEditTool* m_mirrorMeshTransform {nullptr};
    ActionEditTool* m_manualSupport       {nullptr};
    ActionEditTool* m_manualTreeSupport   {nullptr};

    ActionEditTool* m_currAction {nullptr};
    void *m_arg1 {nullptr};
    void *m_arg2 {nullptr};

public:
    static void initMeshModelEulerParams(CHMeshShowObjPtr tmm);
    static ControlInterface* m_conInt;
};

extern bool m_lockToPrintPlatform;
extern bool m_lockScaleRatio;
extern bool m_clearSupport;
extern int operateType;

#endif


