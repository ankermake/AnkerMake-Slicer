#include "controlmanager.h"

namespace  control{

ControlManager::ControlManager(QObject *parent)
    :
    m_toolBar(new ToolBar()),
      m_glareaTopView(nullptr),
      m_rightWidget(nullptr),
      m_preferDialog(new PreferencesDialog(qobject_cast<QWidget*>(parent)))
{
    m_toolBar->setObjectName("m_toolBar");
    m_preferDialog->setObjectName("m_preferDialog");
    //m_preferDialog->setStyleSheet("QDialog{border-top-left-radius:15px;border-top-right-radius:5px;}");
    connect(m_preferDialog,&PreferencesDialog::swicthPage,this,&ControlManager::clickedToSwitchPreferencesPage);
    //m_toolBar->setStyleSheet("ToolBar{background-color:black;}");
}

ControlManager::~ControlManager()
{
    if(m_toolBar) {
        delete m_toolBar;
        m_toolBar = nullptr;
    }
    if(m_glareaTopView) {
        delete m_glareaTopView;
        m_glareaTopView = nullptr;
    }
}

void ControlManager::addActionToModelTranform(QAction *action)
{
    if(m_glareaTopView && action) {
        m_glareaTopView->addActionToModelTranform(action);
    }
}

void ControlManager::addWidgetToModelTransForm(QWidget *widget, int actionIndex)
{
    if(m_glareaTopView && widget ) {
        m_glareaTopView->addWidgetToModelTransForm(widget,actionIndex);
    }
}

void ControlManager::setTopViewGlarea(QWidget *widget)
{
    if(widget)
    {
        m_glareaTopView = new GlareaTopView(widget/*glarea*/);
    }
}

void ControlManager::setRightWidget(QHBoxLayout *splitter)
{
    m_splitter = splitter;
}

void ControlManager::setGlareaWidget(QWidget *widget)
{
    m_glareaTopView->setMainwindow(widget);
}

void ControlManager::setMainWindow(QWidget *widget)
{
    m_preferDialog->setParent(widget);
   // qDebug() << " this  parent widget==" << widget;
    m_preferDialog->hide();
}

void ControlManager::openPreferences(int index)
{
    TDebug(QString("openPreferences page is %1").arg(index));
    emit openPreferencesDialog(index);
    m_preferDialog->openWidget(index);

  //  qDebug() << " this  parent widget==" << m_preferDialog->parentWidget();
    m_preferDialog->exec();
}

QWidget* ControlManager::getPageWidgetFromPreferences(const int index)
{
    if (!m_preferDialog)
    {
        return nullptr;
    }
    return m_preferDialog -> getWidget(index);
}

void ControlManager::selectIndex(const int index)
{
    m_preferDialog ->selectIndex(index);
}

void ControlManager::clickedToSwitchPreferencesPage(int index)
{
    emit switchPreferencesPage(index);
}

//void ControlManager::openPreferences(QWidget *widget)
//{
//     m_preferDialog->openWidget(widget);
//}

//void ControlManager::addFdmSettings(QWidget *widget)
//{
//    if(!m_splitter || !widget) {
//        return;
//    }
//    if(!m_rightWidget) {
//        m_rightWidget = new SplitterWidget();
//        m_splitter->addWidget(m_rightWidget);
//       // qDebug() << " cout ===" << m_splitter->count();
//       QSplitterHandle *handle = m_splitter->handle(1);
//       handle->setDisabled(true);
//       handle->hide();
//    }
//    m_rightWidget->addFdmSettings(widget);
//}

//void ControlManager::addSliceWidget(QWidget *widget)
//{
//    if(!m_splitter || !widget) {
//        return;
//    }
//    if(!m_rightWidget) {
//        m_rightWidget = new SplitterWidget();
//        m_splitter->addWidget(m_rightWidget);
//        QSplitterHandle *handle = m_splitter->handle(1);
//        handle->setDisabled(true);
//        handle->hide();
//    }
//    m_rightWidget->addSliceWidget(widget);
//}

//void ControlManager::repaceWidget(int index, QWidget *widget)
//{
//    if(m_rightWidget && widget) {
//        m_rightWidget->replaceSplitterWidget(index,widget);
//    }
//}

void ControlManager::addWidgetToRightSettings(AkConst::EFdmRightWidgetIndex index, QWidget *widget)
{
    if(!m_splitter || !widget) {
        return;
    }
    if(!m_rightWidget) {
        m_rightWidget = new SplitterWidget();
        m_rightWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
        m_splitter->addWidget(m_rightWidget);

      //  qDebug() << " parent =====" << m_rightWidget->parentWidget();
        //qDebug() << " parent =====" << m_rightWidget->parentWidget();
    }
    m_rightWidget->addWidget(index,widget);
}


void ControlManager::addActionToViewTransform(QAction *action)
{
    if(m_glareaTopView) {
        m_glareaTopView->addActionToViewTransform(action);
    }
}

void ControlManager::addMeshInfoWidget()
{
    m_glareaTopView->addMeshInfoWidget();
}

void ControlManager::addProgressBar(QWidget *widget)
{
    if(m_glareaTopView ) {
        m_glareaTopView->addProgressBar(widget);
    }
}


}
