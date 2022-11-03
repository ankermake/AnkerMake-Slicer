#include "CHModelManualTreeSupportParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>
#include <QComboBox>
#include "support/service/FdmSupportService.h"
#include "../common/utilities/tlogger.h"

CHModelManualTreeSupportParamsSetUI::CHModelManualTreeSupportParamsSetUI(QWidget* parent)
    : BubbleWidget(parent,Qt::LeftArrow)
{
    AkUtil::TFunction("");
    move(96, 636);
    setFixedSize(192, 194);
    setAutoFillBackground(true);
    //setStyleSheet("background-color:white;");
    setBackgroundColor(QColor(41,42,45));
    setStyleSheet("background:transparent");
    QVBoxLayout* mainblaout = new QVBoxLayout(this);

    QFont font1;
    font1.setPixelSize(14);
    QLabel* m_supportLabel = new QLabel;
    m_supportLabel->setFont(font1);
    m_supportLabel->setText(tr("Tree Support"));
    //m_supportLabel->setStyleSheet("QLabel{ \n\tfont: roboto;\n\twidth: 100px; \n\theight: 16px; \n\ttop: 648px; \n\tleft: 109px; \n\tcolor: #333333; \n }");
    QHBoxLayout* hblayout1 = new QHBoxLayout;
    hblayout1->addWidget(m_supportLabel, Qt::AlignLeft);
    mainblaout->addLayout(hblayout1);

    Line* line = new Line;
//    line->setStyleSheet(QString::fromUtf8("QFrame{\n"
//        "   width: 168px;\n"
//        "   height: 1px;\n"
//        "   top: 676px;\n"
//        "   left: 109px;\n"
//        "   color: #E7E7E9;\n"
//        "}"));
    QHBoxLayout* hblaout2 = new QHBoxLayout;
    hblaout2->addWidget(line);
    mainblaout->addLayout(hblaout2);

    
    QHBoxLayout* hbLayoutTopSharp = new QHBoxLayout;

    QLabel* topShapeTypelabel = new QLabel(tr("TopShape"));
    topShapeTypelabel->setStyleSheet("QLabel{\n\twidth: 6px;\n\theight: 16px;\n\ttop: 738px;\n\tleft: 138px;\n\tcolor: #62D361;\n}");

    auto topShapeComboBox = new QComboBox();
    topShapeComboBox->addItem(tr("Trapezoid"));
    topShapeComboBox->addItem(tr("Sphere"));
    topShapeComboBox->addItem(tr("Cylinder"));
    topShapeComboBox->addItem(tr("Bridge"));
    topShapeComboBox->setObjectName(QString::fromUtf8("topSharpcomboBox"));

    hbLayoutTopSharp->addWidget(topShapeTypelabel);
    hbLayoutTopSharp->addWidget(topShapeComboBox);
    hbLayoutTopSharp->setStretch(0, 2);
    hbLayoutTopSharp->setStretch(1, 8);
    mainblaout->addLayout(hbLayoutTopSharp);

    //connect(topShapeComboBox, &QComboBox::currentIndexChanged, this, &CHModelManualTreeSupportParamsSetUI::topShapeChanged);
    //connect(topShapeComboBox, &QComboBox::currentIndexChanged, this, &CHModelManualTreeSupportParamsSetUI::topShapeChanged);
    connect(topShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(topShapeChanged(int)));
    

    
    QLabel* SptStrengthLabel = new QLabel(tr("SptStrength"));
    SptStrengthLabel->setStyleSheet("QLabel{\n\twidth: 6px;\n\theight: 16px;\n\ttop: 738px;\n\tleft: 138px;\n\tcolor: #62D361;\n}");

    m_SptStrengthEdit = new QSpinBox;
    m_SptStrengthEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_SptStrengthEdit->setAutoFillBackground(true);
    m_SptStrengthEdit->setStyleSheet(QString::fromUtf8("QSpinBox{\n"
        "width: 120px;\n"
        "height: 30px;\n"
        "top: 689px;\n"
        "left: 156px;\n"
        "border: 1px solid #51545F;\n"
        "border-radius:4px;\n"
        "font-size: 12px;\n"
        "color:#FFFFFF;\n"
        "\n"
        "}"));

    m_SptStrengthEdit->setMinimum(1);
    m_SptStrengthEdit->setMaximum(2);

    QHBoxLayout* SptStrengthHlayout = new QHBoxLayout;
    SptStrengthHlayout->addWidget(SptStrengthLabel);
    SptStrengthHlayout->addWidget(m_SptStrengthEdit);
    SptStrengthHlayout->setStretch(0, 2);
    SptStrengthHlayout->setStretch(1, 8);
    mainblaout->addLayout(SptStrengthHlayout);
    connect(m_SptStrengthEdit, SIGNAL(valueChanged(int)), this, SIGNAL(sptStrengthChanged(int)));
    m_SptStrengthEdit->setValue(2);

    Line* line2 = new Line;
//    line2->setStyleSheet(QString::fromUtf8("QFrame{\n"
//        "   width: 169px;\n"
//        "   height: 1px;\n"
//        "   top: 773px;\n"
//        "   left: 108px;\n"
//        "   color: #E7E7E9;\n"
//        "}"));
    QHBoxLayout* hblaout3 = new QHBoxLayout;
    hblaout3->addWidget(line2);
    mainblaout->addLayout(hblaout3);

    clearButton = new QToolButton;
    clearButton->setText(tr("Clear"));
    clearButton->setStyleSheet(QString("QToolButton{\n"
        "   width: 80px;\n"
        "   height: 32px;\n"
        "   top: 786px;\n"
        "   left: 108px;\n"
        //"   border: 1px solid #EDEDED;\n"
        "   background-color: #3A3B3F;\n"
        "   border-radius:4px;\n"
        "   font-size: 12px;\n"
        "color: #FFFFFF;\n"
        "}"));

    autoSupportButton = new QToolButton;
    autoSupportButton->setText(tr("AutoSupport"));
    autoSupportButton->setStyleSheet(QString("QToolButton{\n"
        "   width: 80px;\n"
        "   height: 32px;\n"
        "   top: 786px;\n"
        "   left: 196px;\n"
        //"   border: 1px solid #EDEDED;\n"
        "   background-color: #3A3B3F;\n"
        "   border-radius:4px;\n"
        "   font-size: 12px;\n"
        "color: #FFFFFF;\n"
        "}"));
    QHBoxLayout* hblayout2 = new QHBoxLayout;
    hblayout2->addWidget(clearButton);
    hblayout2->addWidget(autoSupportButton);
    mainblaout->addLayout(hblayout2);


    //connect(m_wideEdit, SIGNAL(valueChanged(double)), this, SLOT(submit(double)));
    connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clearBtnClicked()));
    connect(autoSupportButton, SIGNAL(clicked()), this, SIGNAL(autoSupportBtnClickde()));

    mainblaout->setStretch(0, 16);
    mainblaout->setStretch(1, 1);
    mainblaout->setStretch(2, 30);
    mainblaout->setStretch(3, 30);
    mainblaout->setStretch(4, 1);
    mainblaout->setStretch(5, 32);
    //submit(1.0);
}

CHModelManualTreeSupportParamsSetUI::~CHModelManualTreeSupportParamsSetUI()
{

}
//
//void CHModelManualTreeSupportParamsSetUI::submit()
//{
//    emit profileSizeChanged(m_lenthEdit->value(), m_wideEdit->value());
//}
//void CHModelManualTreeSupportParamsSetUI::submit(double)
//{
//    emit profileSizeChanged(m_lenthEdit->value(), m_wideEdit->value());
//}

//void CHModelManualTreeSupportParamsSetUI::clearBtnClicked()
//{
//    //m_clearSupport = !m_clearSupport;
//    //submit(0.0);
//}
//
//void CHModelManualTreeSupportParamsSetUI::autoSupportBtnClickde()
//{
//
//}

//void CHModelManualTreeSupportParamsSetUI::topShapeChanged(int index)
//{
//    FdmSupportService::instance()->m_setting->TouchShape = (TopShape)index;
//}







