#ifndef ANKER_EDIT_PLUGIN_H
#define ANKER_EDIT_PLUGIN_H


#include "anker_plugin.h"
#include "edit_plugin.h"


class AnkerEditTool;
class AnkerEditPlugin;
class ActionEditTool : public QAction{
    Q_OBJECT
public:
    using QAction::QAction;
    inline QString getDescription()const {return whatsThis();}
    inline void    setDescription(const QString & des){return setWhatsThis(des);}
    AnkerEditTool   *ankerEditTool  {nullptr};
    AnkerEditPlugin *ankerEditPlugin{nullptr};
};

class AnkerEditTool : public EditTool
{
public:
    virtual ~AnkerEditTool() {}

    virtual bool startAnkerEdit(ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) { return false;  Q_UNUSED(action);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void   endAnkerEdit(ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(action);Q_UNUSED(arg1);Q_UNUSED(arg2);}

    virtual void mouseDoubleClickEvent(QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void mousePressEvent    (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void mouseMoveEvent     (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void mouseReleaseEvent  (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void keyReleaseEvent    (QKeyEvent    *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void keyPressEvent      (QKeyEvent    *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void wheelEvent         (QWheelEvent  *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}
    virtual void tableEvent         (QTabletEvent *event, void * arg1=nullptr, void *arg2=nullptr) {Q_UNUSED(event);Q_UNUSED(arg1);Q_UNUSED(arg2);}

    inline bool isActivated(){return m_isActivated;}
protected:
    bool m_isActivated{false}; // <==> QAction::isChecked
};


class AnkerEditPlugin : public QObject, public AnkerEditTool, public MeshLabPlugin
{
    Q_OBJECT
public:
    AnkerEditPlugin() {}
    virtual ~AnkerEditPlugin() {}

    virtual void setEditToolsEnable(QVariant){} //  for AnkerEditPlugin

    //gets a list of actions available from this plugin
    virtual QList<QAction *> actions() const {return actionList;};

    //get the edit tool for the given action
    //virtual AnkerEditTool* getEditTool(const QAction *) = 0;
    virtual AnkerEditTool* getEditTool(const QAction *) = 0;

    //get the description for the given action
    virtual QString getEditToolDescription(const QAction *) = 0;

protected:
    QList<QAction*> actionList;
};


#define Anker_EDIT_PLUGIN_IID  "Anker.AnkerEditPlugin/1.0"
Q_DECLARE_INTERFACE(AnkerEditPlugin, Anker_EDIT_PLUGIN_IID)


#endif // ANKER_EDIT_PLUGIN_H
