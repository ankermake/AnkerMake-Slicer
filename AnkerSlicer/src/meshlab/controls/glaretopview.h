#ifndef GLARETOPVIEW_H
#define GLARETOPVIEW_H

#include <QObject>
//#include "glarea.h"
#include "modeltransformwidget.h"
#include "splitterwidget.h"
#include "viewwidget.h"
#include "meshinfowidget.h"
#include "toolBar.h"

namespace  control{
class GlareaTopView : public QObject
{
    Q_OBJECT
public:
    explicit GlareaTopView(QWidget *glarea, QObject *parent = nullptr);
    ~GlareaTopView();

    void addActionToModelTranform(QAction *action);

    void addWidgetToModelTransForm(QWidget *widget, int actionIndex);
    void addSeparatorToModelTransForm();
    void repaceWidget(int index,QWidget *widget);

   // void addWidgetToViewSwitch(QWidget *widget);
    void addActionToViewTransform(QAction *action);

    void addMeshInfoWidget();

    void setMainwindow(QWidget *widget);
    void addProgressBar(QWidget *widget) ;

public slots:
    void progressDelete();

private:
    void initModelTranformWidget(QAction *action);

private:
    QWidget *m_glarea;
    QHBoxLayout *m_mainLayout;
    ModelTransformWidget *m_tranformWidget;
    ViewSwitchWidget *m_viewSwitch;
    MeshInfoWidget *m_meshInfo;
    ToolBar *m_viewTransform;

    QWidget *m_mainwindow;
    SplitterWidget *m_rightWidget;
    QWidget *m_progress;
};

}
#endif // GLARETOPVIEW_H
