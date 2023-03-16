#include "splitterwidget.h"

#include <QDebug>
SplitterWidget::SplitterWidget(QWidget *parent, Qt::Orientation orientation) : BubbleWidget(parent)
{
    this->setBackgroundColor(QColor(32, 33, 35));
    this->setRadius(0);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(12);
    m_mainLayout->setContentsMargins(0,0,0,12);
    this->setFixedWidth(500);
    this->setLayout(m_mainLayout);
}

void SplitterWidget::addFdmSettings(QWidget *widget)
{
    m_mainLayout->insertWidget(0,widget);
    m_mainLayout->setStretchFactor(widget,16);
}

void SplitterWidget::addSliceWidget(QWidget *widget)
{
    m_mainLayout->insertWidget(1,widget,1,Qt::AlignBottom);
   // m_mainLayout->setStretchFactor(widget,1);
}

void SplitterWidget::replaceSplitterWidget(int index, QWidget *widget)
{
   // m_mainLayout->replaceWidget();
}

void SplitterWidget::addWidget(AkConst::EFdmRightWidgetIndex index, QWidget *widget)
{
    m_mainLayout->insertWidget(index,widget,1);

     //  qDebug() << " widget parent == " << widget->parentWidget()  << " parent " << widget->parent();
   // m_mainLayout->setStretchFactor(widget,1);
}


