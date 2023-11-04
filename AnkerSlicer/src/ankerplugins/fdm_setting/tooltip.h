#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFontMetrics>
#include "common/controlInterface/line.h"
#include "common/controlInterface/bubbleWidget.h"
#include <QTextBrowser>
#include <QTextEdit>
class ToolTipBase : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit ToolTipBase(QWidget *parent = nullptr);
    void setPoint(QPoint point,int maxY);
    virtual void init(){}
protected:

    virtual void resizeEvent(QResizeEvent *event);
    QPoint m_point;
    int m_maxY;
};

//class ToolTip : public control::BubbleWidget
//{
//    Q_OBJECT
//public:
//    explicit ToolTip(QWidget *parent = nullptr, bool hasLine = true);
//    void setDescription(const QString &title, const QString &text, const QString &affects, const QString &affectedBy);

//    void setPoint(QPoint point,int maxY);
//private:
//    void init(bool hasLine);
//signals:

//protected:
//    virtual void resizeEvent(QResizeEvent *event);

//protected:
//    QLabel *m_title;
//    QLabel *m_despction;

//    QLabel *m_affect;
//    QPoint m_point;
//    int m_maxY;
//};

class ToolTip : public ToolTipBase
{
    Q_OBJECT
public:
    explicit ToolTip(QWidget *parent = nullptr): ToolTipBase(parent){init();}
    void setDescription(const QString &title, const QString &text, const QString &affects, const QString &affectedBy);

protected:
    virtual void init();
    QLabel *m_title=nullptr;
    QLabel *m_despction=nullptr;
    QLabel *m_affect=nullptr;
};

class HtmlToolTip : public ToolTipBase
{
Q_OBJECT
public:
    explicit HtmlToolTip(QWidget *parent = nullptr): ToolTipBase(parent){init();}
    void setHtml(const QString html);

protected:
    virtual void init();
    //QTextBrowser *m_textBrower=nullptr;
     //QTextEdit *m_textBrower=nullptr;
    QLabel *m_textBrower=nullptr;
};

#endif // TOOLTIP_H
