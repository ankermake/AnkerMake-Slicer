#include "CHModelManualSupportParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>
#include "../common/utilities/tlogger.h"


CHModelManualSupportParamsSetUI::CHModelManualSupportParamsSetUI(QWidget* parent)
    : BubbleWidget(parent,Qt::LeftArrow)
{
    AkUtil::TFunction("");
    move(96, 636);
    setFixedSize(192, 194);
    setAutoFillBackground(true);
    setStyleSheet("background-color:white;");

    QVBoxLayout* mainblaout = new QVBoxLayout(this);

    QFont font1;
    font1.setPixelSize(14);
    QLabel* m_supportLabel = new QLabel;
    m_supportLabel->setFont(font1);
    m_supportLabel->setText(tr("Manual Support"));
    //m_supportLabel->setStyleSheet("QLabel{ \n\tfont: roboto;\n\twidth: 100px; \n\theight: 16px; \n\ttop: 648px; \n\tleft: 109px; \n\tcolor: #333333; \n }");
    QHBoxLayout* hblayout1 = new QHBoxLayout;
    hblayout1->addWidget(m_supportLabel, Qt::AlignLeft);
    mainblaout->addLayout(hblayout1);

    Line* line = new Line;
    line->setStyleSheet(QString::fromUtf8("QFrame{\n"
        "   width: 168px;\n"
        "   height: 1px;\n"
        "   top: 676px;\n"
        "   left: 109px;\n"
        "   color: #E7E7E9;\n"
        "}"));
    QHBoxLayout* hblaout2 = new QHBoxLayout;
    hblaout2->addWidget(line);
    mainblaout->addLayout(hblaout2);

    QLabel* xLogo = new QLabel;
    QPixmap xLogoPixmap(":/images/fdm_manual_x_icon_u.png");
    xLogo->setPixmap(xLogoPixmap);
    xLogo->setScaledContents(true);
    xLogo->setStyleSheet(QString::fromUtf8("QLabel{\n"
        "   width: 30px;\n"
        "   height: 30px;\n"
        "   top: 689px;\n"
        "   left: 104px;\n"
        "   font-size: 14px;\n"
        "}"));
    QLabel* xlabel = new QLabel(QString("X"));
    xlabel->setStyleSheet("QLabel{\n\twidth: 6px;\n\theight: 16px;\n\ttop: 696px;\n\tleft: 138px;\n\tcolor: #E32525;\n}");

    m_lenthEdit = new QDoubleSpinBox;
    m_lenthEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_lenthEdit->setAutoFillBackground(true);
    m_lenthEdit->setSuffix("mm");
    m_lenthEdit->setStyleSheet(QString::fromUtf8("QDoubleSpinBox{\n"
        "width: 120px;\n"
        "height: 30px;\n"
        "top: 689px;\n"
        "left: 156px;\n"
        "border: 1px solid #E7E7E9;\n"
        "border-radius:4px;\n"
        "font-size: 12px;\n"
        "\n"
        "}"));

    QHBoxLayout* xhlayout = new QHBoxLayout;
    xhlayout->addWidget(xLogo);
    xhlayout->addWidget(xlabel);
    xhlayout->addWidget(m_lenthEdit);
    xhlayout->setStretch(0, 15);
    xhlayout->setStretch(1, 6);
    xhlayout->setStretch(2, 120);
    mainblaout->addLayout(xhlayout);


    QLabel* yLogo = new QLabel;
    QPixmap yLogoPixmap(":/images/fdm_manual_y_icon_u.png");
    yLogo->setPixmap(yLogoPixmap);
    yLogo->setScaledContents(true);
    yLogo->setStyleSheet(QString::fromUtf8("QLabel{\n"
        "   width: 30px;\n"
        "   height: 30px;\n"
        "   top: 731px;\n"
        "   left: 104px;\n"
        "   font-size: 14px;\n"
        "}"));
    QLabel* ylabel = new QLabel(QString("Y"));
    ylabel->setStyleSheet("QLabel{\n\twidth: 6px;\n\theight: 16px;\n\ttop: 738px;\n\tleft: 138px;\n\tcolor: #62D361;\n}");

    m_wideEdit = new QDoubleSpinBox;
    m_wideEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_wideEdit->setAutoFillBackground(true);
    m_wideEdit->setSuffix("mm");
    m_wideEdit->setStyleSheet(QString::fromUtf8("QDoubleSpinBox{\n"
        "width: 120px;\n"
        "height: 30px;\n"
        "top: 731px;\n"
        "left: 156px;\n"
        "border: 1px solid #E7E7E9;\n"
        "border-radius:4px;\n"
        "font-size: 12px;\n"
        "\n"
        "}"));

    QHBoxLayout* yhlayout = new QHBoxLayout;
    yhlayout->addWidget(yLogo);
    yhlayout->addWidget(ylabel);
    yhlayout->addWidget(m_wideEdit);
    yhlayout->setStretch(0, 15);
    yhlayout->setStretch(1, 6);
    yhlayout->setStretch(2, 120);
    mainblaout->addLayout(yhlayout);

    Line* line2 = new Line;
    line2->setStyleSheet(QString::fromUtf8("QFrame{\n"
        "   width: 169px;\n"
        "   height: 1px;\n"
        "   top: 773px;\n"
        "   left: 108px;\n"
        "   color: #E7E7E9;\n"
        "}"));
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
        "   background-color: rgb(237,237,237);\n"
        "   border-radius:4px;\n"
        "   font-size: 12px;\n"
        "}"));

    doneButton = new QToolButton;
    doneButton->setText(tr("Done"));
    doneButton->setStyleSheet(QString("QToolButton{\n"
        "   width: 80px;\n"
        "   height: 32px;\n"
        "   top: 786px;\n"
        "   left: 196px;\n"
        //"   border: 1px solid #EDEDED;\n"
        "   background-color: rgb(237,237,237);\n"
        "   border-radius:4px;\n"
        "   font-size: 12px;\n"
        "}"));
    QHBoxLayout* hblayout2 = new QHBoxLayout;
    hblayout2->addWidget(clearButton);
    hblayout2->addWidget(doneButton);
    mainblaout->addLayout(hblayout2);

    mainblaout->setStretch(0, 16);
    mainblaout->setStretch(1, 1);
    mainblaout->setStretch(2, 30);
    mainblaout->setStretch(3, 30);
    mainblaout->setStretch(4, 1);
    mainblaout->setStretch(5, 32);

    m_lenthEdit->setDecimals(2);
    m_wideEdit->setDecimals(2);
    m_lenthEdit->setMaximum(100);
    m_lenthEdit->setMinimum(1);
    m_wideEdit->setMaximum(100);
    m_wideEdit->setMinimum(1);

    connect(m_lenthEdit, SIGNAL(valueChanged(double)), this, SLOT(submit(double)));
    connect(m_wideEdit, SIGNAL(valueChanged(double)), this, SLOT(submit(double)));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearBtnClicked()));
    connect(doneButton, SIGNAL(clicked()), this, SLOT(doneBtnClickde()));

    submit(1.0);
}

CHModelManualSupportParamsSetUI::~CHModelManualSupportParamsSetUI()
{

}

void CHModelManualSupportParamsSetUI::submit()
{
    emit profileSizeChanged(m_lenthEdit->value(), m_wideEdit->value());
}
void CHModelManualSupportParamsSetUI::submit(double)
{
    emit profileSizeChanged(m_lenthEdit->value(), m_wideEdit->value());
}

void CHModelManualSupportParamsSetUI::clearBtnClicked()
{
    m_clearSupport = !m_clearSupport;
    submit(0.0);
}

void CHModelManualSupportParamsSetUI::doneBtnClickde()
{
}







