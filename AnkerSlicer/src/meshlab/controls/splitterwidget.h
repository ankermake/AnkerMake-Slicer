#ifndef SPLITTERWIDGET_H
#define SPLITTERWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QGridLayout>
#include <QToolButton>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include "common/ak_const.h"
#include "common/controlInterface/bubbleWidget.h"
class SplitterWidget :  public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit SplitterWidget(QWidget *parent = nullptr,Qt::Orientation orientation = Qt::Orientation::Vertical);

    void addFdmSettings(QWidget *widget);
    void addSliceWidget(QWidget *widget);
    void replaceSplitterWidget(int index,QWidget *widget);

    void addWidget(AkConst::EFdmRightWidgetIndex index, QWidget* widget);

signals:

private:
    QVBoxLayout *m_mainLayout;
};

#endif // SPLITTERWIDGET_H
