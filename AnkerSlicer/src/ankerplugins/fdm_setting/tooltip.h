#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFontMetrics>
#include "common/controlInterface/line.h"
#include "common/controlInterface/bubbleWidget.h"

class ToolTip : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit ToolTip(QWidget *parent = nullptr);
    void setDescription(const QString &title, const QString &text, const QString &affects, const QString &affectedBy);

    void setPoint(QPoint point,int maxY);
private:
    void init();
signals:

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    QLabel *m_title;
    QLabel *m_despction;

    QLabel *m_affect;
    QPoint m_point;
    int m_maxY;
};

#endif // TOOLTIP_H
