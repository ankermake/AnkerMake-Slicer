#ifndef AKSLICEPANEL_H
#define AKSLICEPANEL_H

#include <QWidget>
#include <QPushButton>
#include "common/controlInterface/bubbleWidget.h"


enum ESliceBtnStat
{
    INIT = 0x0000,
    MODEL_STATUS_CHANGED = 1, //
    MODEL_TRANSFROM_CHANGED = 2,
    MODEL_SUPPORT_NUMBER_CHANGED = 4,
    PARAMETER_CHANGED = 8,
    VISIBLE_MODEL_COUNT_ZERO = 16,             //0000 0000 0000 0000 0000 0000 0001 0000
    MODEL_NOT_ALL_IN_BOX = 32,                 //0000 0000 0000 0000 0000 0000 0010 0000
    SUPPORT_PROCESSING_START = 64,             //0000 0000 0000 0000 0000 0000 0100 0000
    SUPPORT_PROCESSING_END = 128,              //0000 0000 0000 0000 0000 0000 1000 0000
    SLICE_BEING = 256,                         //0000 0000 0000 0000 0000 0001 0000 0000
    MODEL_IS_SUSPEND_STATUS = 512,             //0000 0000 0000 0000 0000 0010 0000 0000
    GENERATE_SUPPORT_RESULT_T = 1024,          //0000 0000 0000 0000 0000 0100 0000 0000
    SLICE_END = 0x7FFFF1FF,                    //0111 1111 1111 1111 1111 1110 1111 1111
    MODEL_ALL_IN_BOX = 0x7FFFFFDF,             
    VISIBLE_MODEL_COUNT_NOT_ZERO = 0x7FFFFFEF, //0111 1111 1111 1111 1111 1111 1110 1111
    MODEL_NOT_SUSPEND_STATUS = 0x7FFFFDFF,     //0111 1111 1111 1111 1111 1101 1111 1111
    GENERATE_SUPPORT_RESULT_N = 0x7FFFFBFF,    //0111 1111 1111 1111 1111 1011 1111 1111

};

class AkSlicePanel : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit AkSlicePanel(QWidget *parent = nullptr);
    void reset();
    int getButtonState();
signals:
    void slice();
    void preview();
    void previewBtnClicked();
    void quit();
    void save();
protected:
    void changeEvent(QEvent *e);
public slots:
    void doSliceSuccess();
    void doSliceFailed();
    void doQuit();
    void sliceEnable(bool flag);
    void doAppendSliceBtnStat(ESliceBtnStat btnStat);

private:
    QPushButton *sliceBtn;
    QPushButton *previewBtn;
    //QPushButton *quitBtn;
    QPushButton *exportBtn;

    
    int ButtonState = ESliceBtnStat::INIT;
};


#endif // AKSLICEPANEL_H
