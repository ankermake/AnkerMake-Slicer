#include "viewwidget.h"

#include <QDebug>
namespace  control{
ViewSwitchWidget::ViewSwitchWidget(QWidget *parent, Qt::Orientation orientation, QSize size)
    : BubbleWidget(parent),
      m_buttonGroup(new QButtonGroup)
     // m_mainLayout(new QHBoxLayout(this))
{
    if(orientation == Qt::Horizontal) {
        m_mainlayout = new QHBoxLayout(this);
    }
    else {
        m_mainlayout = new QVBoxLayout(this);
    }
    m_mainlayout->setContentsMargins(0,0,0,0);
    m_mainlayout->setSpacing(24);
    m_orientation = orientation;
    m_iconSize = size;
    m_buttonGroup->setExclusive(true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setBackgroundColor(QColor(26, 26, 28,20));
    this->setMaximumHeight(38);
    //setStyleSheet("background-color:transparent");
     //this->setMaximumWidth(210);
}

void ViewSwitchWidget::addActionToTransform(QAction *action, Qt::ToolButtonStyle style)
{
    QToolButton  *pButton = new QToolButton(this);
    pButton->setIconSize(m_iconSize);
    pButton->setFixedSize(m_iconSize);
    pButton->setAutoRaise(true);
    pButton->setDefaultAction(action);
    pButton->setToolButtonStyle(style);
    pButton->setStyleSheet("background-color:transparent");
    m_buttonGroup->addButton(pButton);
    m_mainlayout->addWidget(pButton);
   // qDebug() << " size ====" <<  action->icon() -> size()<< "icon size = " << pButton->sizeHint();
}

void ViewSwitchWidget::addStretch(int stretch)
{
    if(m_orientation == Qt::Horizontal) {
        QHBoxLayout *layout = qobject_cast<QHBoxLayout *> (m_mainlayout);
        if(layout) {
            layout->addStretch(stretch);
        }
    }
    else {
        QVBoxLayout *layout = qobject_cast<QVBoxLayout *> (m_mainlayout);
        if(layout) {
            layout->addStretch(stretch);
        }
    }
}

void ViewSwitchWidget::setToolButtonStyle(Qt::ToolButtonStyle style)
{
    m_toolButtonStyle = style;
}






}
