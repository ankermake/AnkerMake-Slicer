#include "akslicepanel.h"
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QDebug>
#include "../common/controlInterface/messageDialog.h"

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
    sliceBtn->setFocusPolicy(Qt::NoFocus);
    connect(sliceBtn, &QPushButton::clicked,this, &AkSlicePanel::slice);

    previewBtn = new QPushButton(this);
    previewBtn->setFont(font);
    previewBtn->setFocusPolicy(Qt::NoFocus);
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
    exportBtn->setFocusPolicy(Qt::NoFocus);
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

int AkSlicePanel::getButtonState()
{
    return ButtonState;
}

void AkSlicePanel::sliceEnable(bool flag)
{
    sliceBtn->setEnabled(flag);
//    sliceBtn->show();
//    previewBtn->hide();
//    quitBtn->hide();
    //    exportBtn->hide();
}


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

        
        case MODEL_IS_SUSPEND_STATUS:
        {
            ButtonState = ButtonState | ESliceBtnStat::MODEL_IS_SUSPEND_STATUS;
            break;
        }
        
        case MODEL_NOT_SUSPEND_STATUS:
        {
            ButtonState = ButtonState & ESliceBtnStat::MODEL_NOT_SUSPEND_STATUS;
            break;
        }
        
        case GENERATE_SUPPORT_RESULT_T:
        {
            ButtonState = ButtonState | ESliceBtnStat::GENERATE_SUPPORT_RESULT_T;
            break;
        }
        
        case GENERATE_SUPPORT_RESULT_N:
        {
            ButtonState = ButtonState & ESliceBtnStat::GENERATE_SUPPORT_RESULT_N;
            break;
        }
    }
}


////
//void AkSlicePanel::doAppendSliceBtnStat(ESliceBtnStat btnStat)
//{

//    if (btnStat != ESliceBtnStat::SLICE_BEING
//            && btnStat != ESliceBtnStat::SLICE_END)
//    {
//        reset();
//    }

//    switch(btnStat)
//    {
//        case MODEL_STATUS_CHANGED:
//        case MODEL_TRANSFROM_CHANGED:

//        case MODEL_SUPPORT_NUMBER_CHANGED:
//        {

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::MODEL_IS_SUSPEND_STATUS)
//            {
//                return;
//            }
//            sliceBtn->setEnabled(true);
//            break;
//        }

//        case PARAMETER_CHANGED:
//        {

//            if (ButtonState == ESliceBtnStat::INIT)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                sliceBtn->setEnabled(false);
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::MODEL_IS_SUSPEND_STATUS)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::GENERATE_SUPPORT_RESULT_T)
//            {
//                return;
//            }
//            sliceBtn->setEnabled(true);
//            break;
//        }


//        case MODEL_IS_SUSPEND_STATUS:
//        {
//            ButtonState = ButtonState | ESliceBtnStat::MODEL_IS_SUSPEND_STATUS;
//            sliceBtn->setEnabled(false);

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                sliceBtn->setEnabled(false);
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                sliceBtn->setEnabled(false);
//                return;
//            }


//            if (ButtonState & ESliceBtnStat::GENERATE_SUPPORT_RESULT_T)
//            {
//                sliceBtn->setEnabled(true);
//                return;
//            }


//            control::MessageDialog messageDialog(tr("Notice"), tr("The model is hovering in the air. Please generate support or switch on [place on bed]."),
//                control::MessageDialog::OK);
//            messageDialog.exec();

//            break;
//        }


//        case MODEL_NOT_SUSPEND_STATUS:
//        {
//            ButtonState = ButtonState & ESliceBtnStat::MODEL_NOT_SUSPEND_STATUS;


//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                return;
//            }

//            sliceBtn->setEnabled(true);
//            break;
//        }


//        case GENERATE_SUPPORT_RESULT_T:
//        {
//            ButtonState = ButtonState | ESliceBtnStat::GENERATE_SUPPORT_RESULT_T;


//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                //sliceBtn->setEnabled(false);
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                sliceBtn->setEnabled(false);
//                return;
//            }

//            sliceBtn->setEnabled(true);
//            break;
//        }


//        case GENERATE_SUPPORT_RESULT_N:
//        {
//            ButtonState = ButtonState & ESliceBtnStat::GENERATE_SUPPORT_RESULT_N;

//            if (ButtonState == ESliceBtnStat::INIT)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                sliceBtn->setEnabled(false);
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {

//                sliceBtn->setEnabled(false);
//                return;
//            }


//            if (ButtonState & ESliceBtnStat::MODEL_IS_SUSPEND_STATUS)
//            {

//                control::MessageDialog messageDialog(tr("Notice"), tr("The model is hovering in the air. Please generate support or switch on [place on bed]."),
//                    control::MessageDialog::OK);
//                messageDialog.exec();

//                sliceBtn->setEnabled(false);
//                return;
//            }
//            sliceBtn->setEnabled(true);
//            break;
//        }


//        case VISIBLE_MODEL_COUNT_ZERO:
//        {
//            sliceBtn->setEnabled(false);
//            ButtonState = ButtonState | ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO;
//            break;
//        }


//        case MODEL_NOT_ALL_IN_BOX:
//        {
//            sliceBtn->setEnabled(false);
//            ButtonState = ButtonState | ESliceBtnStat::MODEL_NOT_ALL_IN_BOX;
//            break;
//        }
//        case SLICE_BEING:
//        case SUPPORT_PROCESSING_START:
//        {

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                return;
//            }
//            sliceBtn->setEnabled(false);
//            break;
//        }
//        case SLICE_END:
//        case SUPPORT_PROCESSING_END:
//        {

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                return;
//            }
//            sliceBtn->setEnabled(true);
//            break;
//        }


//        case MODEL_ALL_IN_BOX:
//        {

//            ButtonState = ButtonState & ESliceBtnStat::MODEL_ALL_IN_BOX;

//            if (ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::MODEL_IS_SUSPEND_STATUS)
//            {
//                return;
//            }
//            sliceBtn->setEnabled(true);
//            break;
//        }


//        case VISIBLE_MODEL_COUNT_NOT_ZERO:
//        {

//            ButtonState = ButtonState & ESliceBtnStat::VISIBLE_MODEL_COUNT_NOT_ZERO;

//            if (ButtonState & ESliceBtnStat::MODEL_NOT_ALL_IN_BOX)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::MODEL_IS_SUSPEND_STATUS)
//            {
//                return;
//            }

//            if (ButtonState & ESliceBtnStat::GENERATE_SUPPORT_RESULT_N)
//            {
//                return;
//            }
//            sliceBtn->setEnabled(true);
//            break;
//        }
//    }
//}

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


