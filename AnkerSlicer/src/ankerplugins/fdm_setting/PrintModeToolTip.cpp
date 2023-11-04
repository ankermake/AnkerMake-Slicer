#include "PrintModeToolTip.h"
void PrintModeToolTip::init()
{
    int titleWidth = 88;
    this->setBackgroundColor(QColor(57,58,61));
    this->setArrowType(Qt::RightArrow);
    this->setTriangleStartX(10);
    this->setTriangleHeight(10);
    this->setTriangleWidth(20);
    this->setRadius(8);

    QPixmap greenPixmap(":/images/images/fmd_green_light.png");
    QPixmap grayPixMap(":/images/images/fmd_gray_light.png");

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setContentsMargins(12,12,12,12);

    auto drawDetailPage = [&](QString title, QString description, int greenCount1,int greenCount2,int greenCount3)->void {
        QLabel* modeLabel = new QLabel(this);
        modeLabel->setWordWrap(true);
        modeLabel->setFixedWidth(235);
        modeLabel->adjustSize();
        modeLabel->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum));
        modeLabel->setText(title);
        modeLabel->setStyleSheet("QLabel{ font: normal normal 16px \"Microsoft YaHei\"; color:#FFFFFF;}");

        QLabel* despctionLable = new QLabel(this);
        despctionLable->setWordWrap(true);
        despctionLable->setFixedWidth(235);
        despctionLable->adjustSize();
        despctionLable->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        //despctionLable->setObjectName("m_fastDespction");
        despctionLable->setStyleSheet("QLabel{ font: normal normal 14px \"Microsoft YaHei\"; color:#999999;}");
        despctionLable->setText(description);
        mainlayout->addWidget(modeLabel);

        mainlayout->addWidget(despctionLable);
        QGridLayout *ParamlayoutMode = new QGridLayout(this);
        ParamlayoutMode->setVerticalSpacing(8);
        ParamlayoutMode->setHorizontalSpacing(4);
        ParamlayoutMode->setContentsMargins(0,0,0,0);
        //ParamlayoutFastMode->setColumnMinimumWidth(0, 100);
        ParamlayoutMode->setGeometry(QRect(0,0,235,0));

        QLabel* speedLabel = new QLabel(this);
        speedLabel->setWordWrap(true);
        speedLabel->setFixedWidth(titleWidth);
        speedLabel->adjustSize();
        speedLabel->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum));
        speedLabel->setText(tr("Speed"));
        speedLabel->setStyleSheet("QLabel{ font: normal normal 14px \"Microsoft YaHei\"; color:#FFFFFF;}");
        ParamlayoutMode->addWidget(speedLabel, 1,1,1,1,Qt::AlignLeft);
        for(int i = 0;i<greenCount1;i++)
        {
            QLabel *greenPic = new QLabel(this);
            greenPic->setPixmap(greenPixmap);
            greenPic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            greenPic->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
            greenPic->setFixedSize(15,6);
            ParamlayoutMode->addWidget(greenPic, 1,2+i,1,1,Qt::AlignLeft);
        }
        for(int i = greenCount1;i<5;i++)
        {
            QLabel *grayPic = new QLabel(this);
            grayPic->setPixmap(grayPixMap);
            grayPic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            grayPic->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
            grayPic->setFixedSize(15,6);
            ParamlayoutMode->addWidget(grayPic, 1,2+i,1,1,Qt::AlignLeft);
        }

        QLabel* detailLabel = new QLabel(this);
        detailLabel->setWordWrap(true);
        detailLabel->setFixedWidth(titleWidth);
        detailLabel->adjustSize();
        detailLabel->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum));
        detailLabel->setText(tr("Detail"));
        detailLabel->setStyleSheet("QLabel{ font: normal normal 14px \"Microsoft YaHei\"; color:#FFFFFF;}");
        ParamlayoutMode->addWidget(detailLabel, 2,1,1,1,Qt::AlignLeft);
        for(int i = 0;i<greenCount2;i++)
        {
            QLabel *greenPic = new QLabel(this);
            greenPic->setPixmap(greenPixmap);
            greenPic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            greenPic->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
            greenPic->setFixedSize(15,6);
            ParamlayoutMode->addWidget(greenPic, 2,2+i,1,1,Qt::AlignLeft);
        }
        for(int i = greenCount2;i<5;i++)
        {
            QLabel *grayPic = new QLabel(this);
            grayPic->setPixmap(grayPixMap);
            grayPic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            grayPic->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
            grayPic->setFixedSize(15,6);
            ParamlayoutMode->addWidget(grayPic, 2,2+i,1,1,Qt::AlignLeft);
        }


        QLabel* reliabilityLabel = new QLabel(this);
        reliabilityLabel->setWordWrap(true);
        reliabilityLabel->setFixedWidth(titleWidth);
        reliabilityLabel->adjustSize();
        reliabilityLabel->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum));
        reliabilityLabel->setText(tr("Reliability"));
        reliabilityLabel->setStyleSheet("QLabel{ font: normal normal 14px \"Microsoft YaHei\"; color:#FFFFFF;}");
        ParamlayoutMode->addWidget(reliabilityLabel, 3,1,1,1,Qt::AlignLeft);
        for(int i = 0;i<greenCount3;i++)
        {
            QLabel *greenPic = new QLabel(this);
            greenPic->setPixmap(greenPixmap);
            greenPic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            greenPic->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
            greenPic->setFixedSize(15,6);
            ParamlayoutMode->addWidget(greenPic, 3,2+i,1,1,Qt::AlignLeft);
        }
        for(int i = greenCount3;i<5;i++)
        {
            QLabel *grayPic = new QLabel(this);
            grayPic->setPixmap(grayPixMap);
            grayPic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            grayPic->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
            grayPic->setFixedSize(15,6);
            ParamlayoutMode->addWidget(grayPic, 3,2+i,1,1,Qt::AlignLeft);
        }

        mainlayout->addLayout(ParamlayoutMode);
    };
    drawDetailPage(tr("Speedy"),tr("Prioritizes speed over precision, utilizing higher printing speeds and accelerations within the safe operating range of the printer's performance."),5,3,4);
    Line *line1 = new Line(this,QString("#565656"),QFrame::HLine);
    mainlayout->addWidget(line1);
    drawDetailPage(tr("Normal"),tr("Maintain a balance between printing speed and precision, thereby achieving superior printing quality at high printing speeds."),4,4,4);
    Line *line2 = new Line(this,QString("#565656"),QFrame::HLine);
    mainlayout->addWidget(line2);
    drawDetailPage(tr("Smooth"),tr("Prioritizes the smoothness of the model surface, with more uniform layer heights between adjacent layers."),4,5,4);


    //m_affect->setStyleSheet("QLabel{ color:#99FF00 }");

    this->setLayout(mainlayout);

}
