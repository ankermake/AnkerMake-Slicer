#ifndef PRINTMODETOOLTIP_H
#define PRINTMODETOOLTIP_H
#include "tooltip.h"

//qLable can't display html very well. so
class PrintModeToolTip : public ToolTipBase
{
Q_OBJECT
public:
    explicit PrintModeToolTip(QWidget *parent = nullptr): ToolTipBase(parent){init();}
    //void setHtml(const QString html);

protected:
    virtual void init();
    //QTextBrowser *m_textBrower=nullptr;
     //QTextEdit *m_textBrower=nullptr;

//    QLabel *m_fastMode=nullptr;
//    QLabel *m_fastDespction=nullptr;
//    QLabel *m_speed=nullptr;
//    QLabel *m_detail=nullptr;
//    QLabel *m_reliability=nullptr;
//    QLabel *m_normalMode=nullptr;
//    QLabel *m_normalDespction=nullptr;
//    QLabel *m_greenPic=nullptr;
//    QLabel *m_grayPic=nullptr;
};

#endif // PRINTMODETOOLTIP_H
