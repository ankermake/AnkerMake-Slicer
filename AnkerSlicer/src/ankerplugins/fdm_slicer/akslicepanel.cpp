#include "akslicepanel.h"
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QDebug>

AkSlicePanel::AkSlicePanel(QWidget *parent) : BubbleWidget(parent)
{
    this->setBackgroundColor(QColor(32, 33, 35));
    this->setFixedHeight(40);//64
    this->setRadius(0);
    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(24,0,24,0);
    vlayout->setSpacing(0);
    auto hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);
    //AkProgressBar->setStyleSheet("background-color:rgb(255, 255, 255);");
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("AkSlicePanel"));

    QFont font = this->font();
    font.setPixelSize(font.pixelSize() + 2);
    sliceBtn = new QPushButton(this);
    sliceBtn->setFont(font);
    sliceBtn->setObjectName("hightButton");
    sliceBtn->setMinimumHeight(40);
    sliceBtn->setText(tr("Slice"));
    connect(sliceBtn, &QPushButton::clicked,this, &AkSlicePanel::slice);

    previewBtn = new QPushButton(this);
    previewBtn->setFont(font);
    previewBtn->setMinimumHeight(40);
    previewBtn->setObjectName(QString::fromUtf8("Preview"));
    previewBtn->setText(tr("Preview"));
    connect(previewBtn, &QPushButton::clicked,this, &AkSlicePanel::previewBtnClicked);

    //quitBtn = new QPushButton(this);
    //quitBtn->setObjectName(QString::fromUtf8("Quit"));
    //quitBtn->setText(tr("Quit"));
    //connect(quitBtn, &QPushButton::clicked,this, &AkSlicePanel::doQuit);

    exportBtn = new QPushButton(this);
    exportBtn->setFont(font);
    exportBtn->setMinimumHeight(40);
    exportBtn->setObjectName(QString::fromUtf8("Export"));
    exportBtn->setText(tr("Export"));
    connect(exportBtn, &QPushButton::clicked,this, &AkSlicePanel::save);


    hLayout->addWidget(previewBtn);
    //hLayout->addWidget(quitBtn);
    vlayout->addLayout(hLayout);


    vlayout->addWidget(sliceBtn);
    vlayout->addWidget(exportBtn);

    
    reset();
}


void AkSlicePanel::doSliceSuccess()
{
    sliceBtn->hide();
    previewBtn->show();
    //quitBtn->show();

    
    //exportBtn->show();
    exportBtn->hide();
    
    emit preview();
}


void AkSlicePanel::doSliceFailed()
{
    sliceBtn->show();
    previewBtn->hide();
    //quitBtn->hide();
    exportBtn->hide();
    qDebug() << "AkSlicePanel::doSliceFailed()";
}

void AkSlicePanel::doQuit()
{
    reset();
}
void AkSlicePanel::reset()
{
    sliceBtn->show();
    
    previewBtn->hide();
    //quitBtn->hide();
    exportBtn->hide();
}

void AkSlicePanel::sliceEnable(bool flag)
{
    sliceBtn->setEnabled(flag);
//    sliceBtn->show();
//    previewBtn->hide();
//    quitBtn->hide();
    //    exportBtn->hide();
}

//
void AkSlicePanel::doAppendSliceBtnStat(ESliceBtnStat btnStat)
{
    
    if (btnStat != ESliceBtnStat::SLICE_BEING
            && btnStat != ESliceBtnStat::SLICE_END)
    {
        reset();
    }

    switch(btnStat)
    {
        case MODEL_STATUS_CHANGED:
        case MODEL_TRANSFROM_CHANGED:
        case MODEL_SUPPORT_NUMBER_CHANGED:
        {
            
            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
            {
                return;
            }
            
            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
            {
                return;
            }
            sliceBtn->setEnabled(true);
            break;
        }
        
        case PARAMETER_CHANGED:
        {
            
            if (ButtonState == ESliceBtnStat::INIT)
            {
                return;
            }
            
            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
            {
                return;
            }
            
            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
            {
                return;
            }
            sliceBtn->setEnabled(true);
            break;
        }
        case VISIBLE_MODEL_COUNT_ZERO:
        {
            sliceBtn->setEnabled(false);
            ButtonState = ButtonState | ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO;
            break;
        }
        case MODEL_NOT_ALL_IN_BOX:
        {
            sliceBtn->setEnabled(false);
            ButtonState = ButtonState | ESliceBtnStat::MODEL_NOT_ALL_IN_BOX;
            break;
        }
        case SLICE_BEING:
        case SUPPORT_PROCESSING_START:
        {
            
            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
            {
                return;
            }
            
            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
            {
                return;
            }
            sliceBtn->setEnabled(false);
            break;
        }
        case SLICE_END:
        case SUPPORT_PROCESSING_END:
        {
            
            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
            {
                return;
            }
            
            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
            {
                return;
            }
            sliceBtn->setEnabled(true);
            break;
        }
        case MODEL_ALL_IN_BOX:
        {
            
            ButtonState = ButtonState & ESliceBtnStat::MODEL_ALL_IN_BOX;
            
            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
            {
                return;
            }
            sliceBtn->setEnabled(true);
            break;
        }
        case VISIBLE_MODEL_COUNT_NOT_ZERO:
        {
            
            ButtonState = ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_NOT_ZERO;
            
            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
            {
                return;
            }
            sliceBtn->setEnabled(true);
            break;
        }
    }
}

void AkSlicePanel::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (sliceBtn != nullptr) {
            sliceBtn->setText(tr("Slice"));
        }
        if (previewBtn != nullptr) {
            previewBtn->setText(tr("Preview"));
        }
        if (exportBtn != nullptr) {
            exportBtn->setText(tr("Export"));
        }
    }
}


