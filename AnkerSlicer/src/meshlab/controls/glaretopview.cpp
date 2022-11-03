#include "glaretopview.h"

namespace  control{
GlareaTopView::GlareaTopView(QWidget*glarea, QObject *parent)
    : QObject(parent),
      m_glarea(glarea),
      m_mainLayout(new QHBoxLayout(glarea)),
      m_tranformWidget(nullptr),
      m_viewSwitch(new ViewSwitchWidget(glarea)),
      m_meshInfo(new MeshInfoWidget(glarea)),
      m_viewTransform(new ToolBar(glarea)),
      m_mainwindow(nullptr),
      m_rightWidget(nullptr),
      m_progress(nullptr)
{
    m_mainLayout->setContentsMargins(17,20,20,76);

    m_mainLayout->addWidget(m_viewSwitch,0,Qt::AlignBottom);
    m_mainLayout->addStretch(2);
    m_mainLayout->addWidget(m_meshInfo,0,Qt::AlignTop);

}

GlareaTopView::~GlareaTopView()
{
    if(m_tranformWidget) {
        delete m_tranformWidget;
        m_tranformWidget = nullptr;
    }
    if(m_rightWidget) {
        delete m_rightWidget;
        m_rightWidget = nullptr;
    }
}

void GlareaTopView::addActionToModelTranform(QAction *action)
{
    initModelTranformWidget(action);
}

void GlareaTopView::addWidgetToModelTransForm(QWidget *widget, int actionIndex)
{
    if(!m_tranformWidget) {
        return;
    }
    widget->setParent(m_glarea);
    m_tranformWidget->addWidgetToTransForm(widget,actionIndex);
    widget->show();
}

void GlareaTopView::addSeparatorToModelTransForm()
{
    m_tranformWidget->addSeparator();
}


void GlareaTopView::repaceWidget(int index, QWidget *widget)
{
    m_rightWidget->replaceSplitterWidget(index,widget);
}

//void GlareaTopView::addWidgetToViewSwitch(QWidget *widget)
//{
//    // m_viewSwitch->addWidget(widget);
//}

void GlareaTopView::addActionToViewTransform(QAction *action)
{
    m_viewSwitch->addActionToTransform(action);
}

void GlareaTopView::addMeshInfoWidget()
{
    m_meshInfo->addMeshModel();
}

void GlareaTopView::setMainwindow(QWidget *widget)
{
//    if(widget) {
//    m_mainwindow = widget;
//    }
}

void GlareaTopView::addProgressBar(QWidget *widget)
{
    if(!widget) {
        return;
    }
    m_mainwindow->setEnabled(false);
    widget->setParent(m_mainwindow);
    widget->setGeometry(100,m_mainwindow->height() - 100,(m_mainwindow->width() - 200),90);
    widget->show();
    m_progress = widget;
    connect(widget,&QWidget::destroyed,this,&GlareaTopView::progressDelete);
}


void GlareaTopView::progressDelete()
{
  QWidget *widget = qobject_cast<QWidget*> (sender());
  if(widget == m_progress) {
      m_progress = nullptr;
      m_mainwindow->setEnabled(true);
  }
}

void GlareaTopView::initModelTranformWidget(QAction *action)
{
    if(!m_glarea) {
        return;
    }
    if(!m_tranformWidget) {
        m_tranformWidget = new ModelTransformWidget(m_glarea);
        m_tranformWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        m_mainLayout->insertWidget(0,m_tranformWidget/*,1,Qt::AlignLeft*/);
        m_mainLayout->insertStretch(1,3);
    }
    m_tranformWidget->addActionToTransform(action);
}

}
