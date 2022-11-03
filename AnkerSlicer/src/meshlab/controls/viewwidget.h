#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include "common/controlInterface/bubbleWidget.h"
namespace  control{
class ViewSwitchWidget : public BubbleWidget
{
    Q_OBJECT
public:
    explicit ViewSwitchWidget(QWidget *parent = nullptr,
                              Qt::Orientation orientation = Qt::Horizontal,
                              QSize size = QSize(32,32));

    void addActionToTransform(QAction *action, Qt::ToolButtonStyle style = Qt::ToolButtonFollowStyle);

    void setIconSize(QSize size) { m_iconSize = size;};
    void addStretch(int stretch = 0) ;
    void setToolButtonStyle(Qt::ToolButtonStyle style) ;
signals:

private:
    QLayout *m_mainlayout;
    QSize m_iconSize;
    Qt::Orientation m_orientation;
    Qt::ToolButtonStyle m_toolButtonStyle;
    QButtonGroup *m_buttonGroup;
};
}
#endif // VIEWWIDGET_H
