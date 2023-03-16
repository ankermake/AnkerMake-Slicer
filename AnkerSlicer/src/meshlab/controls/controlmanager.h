#ifndef CONTROLMANAGER_H
#define CONTROLMANAGER_H

#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QDebug>
#include "common/controlInterface/controlInterface.h"
#include "toolBar.h"
#include "modeltransformwidget.h"
#include "splitterwidget.h"
#include "glaretopview.h"
#include "preferencesdialog.h"
#include "common/utilities/tlogger.h"
using namespace AkUtil;
namespace  control{

class ControlManager : public ControlInterface
{
      Q_OBJECT
public:
    ControlManager(QObject *parent = nullptr);
    ~ControlManager();

    //for plugin
    void addSeparatorToToolBar() { m_toolBar->addSeparatorToToolBar();};
    void addActionToToolBar(QAction *action) {m_toolBar->addActionToToolBar(action);};
    void addMenuToToolBar(QMenu *menu, Qt::Alignment aligment = Qt::AlignLeft) {m_toolBar->addMenuToToolBar(menu,aligment);};
    void addAction(QAction *action,Qt::Alignment aligment = Qt::AlignLeft) {m_toolBar->addAction(action,aligment);};
    void addStructToToolBar(){m_toolBar->addStruct();};
    //Create menu according to id, Returns itself if it exists
    QMenu *addMenu(ControlId  id) {return  m_toolBar->addMenu(id);};
    void addActionToMenu(QAction *action,ControlId  id) { m_toolBar->addActionToMenu(action,id); };
    void addMenuToMenu(QMenu *menu,ControlId  id) { m_toolBar->addMenuToMenu(menu,id); };
    void addSeparatorToMenu(ControlId id) { m_toolBar->addSeparatorToMenu(id); };

    void addActionToModelTranform(QAction *action);
    void addSeparatorToModelTranform() { m_glareaTopView->addSeparatorToModelTransForm();};
    void addWidgetToModelTransForm(QWidget *widget,int actionIndex);

    //right widget
   // void addFdmSettings(QWidget *widget);
   // void addSliceWidget(QWidget *widget);
    //void repaceWidget(int isndex,QWidget *widget);

  void addWidgetToRightSettings(AkConst::EFdmRightWidgetIndex index,  QWidget* widget);

    //
   // void addWidgetToView(QWidget *widget);
    void addActionToViewTransform(QAction *action);

    void addMeshInfoWidget();
    void addProgressBar(QWidget *widget) ;

    //for mainwindow
    ToolBar *toolBar() { return m_toolBar;};
    QMenu *toolBarMainMenu() {return m_toolBar->mainMenu();};
    void setTopViewGlarea(QWidget *widget);
    void setRightWidget(QHBoxLayout *splitter) ;
    void setGlareaWidget(QWidget *widget);
    void setMainWindow(QWidget *widget);

    PreferencesDialog * preDialog() {return m_preferDialog; };

    void addPageToPreferences(PageWidget *widget,int index) {m_preferDialog->addWidget(widget,index);};
    void openPreferences(int index );
    QWidget* getPageWidgetFromPreferences(const int index);
    // void openPreferences(QWidget *widget);

private slots:
    void clickedToSwitchPreferencesPage(int index);

private:
    ToolBar *m_toolBar;
    GlareaTopView *m_glareaTopView;
    SplitterWidget *m_rightWidget;
    QHBoxLayout *m_splitter; 
    PreferencesDialog *m_preferDialog;
};

}
//Q_DECLARE_METATYPE(control::ControlManager)
//Q_DECLARE_METATYPE(control::ControlManager*)

#endif // CONTROLMANAGER_H
