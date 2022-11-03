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
    VISIBLE_MODEL_COUNT_ZERO = 16, //00000000000000000000000000010000
    MODEL_NOT_ALL_IN_BOX = 32,     //00000000000000000000000000100000
    SUPPORT_PROCESSING_START = 64, //00000000000000000000000001000000
    SUPPORT_PROCESSING_END = 128,  //00000000000000000000000010000000
    SLICE_BEING = 256,                         //00000000000000000000000100000000
    SLICE_END = 0x7FFFF1FF,                    //01111111111111111111111011111111
    MODEL_ALL_IN_BOX = 0x7FFFFFDF,             
    VISIBLE_MODEL_COUNT_NOT_ZERO = 0x7FFFFFEF, //01111111111111111111111111101111

};

class AkSlicePanel : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit AkSlicePanel(QWidget *parent = nullptr);
    void reset();
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
