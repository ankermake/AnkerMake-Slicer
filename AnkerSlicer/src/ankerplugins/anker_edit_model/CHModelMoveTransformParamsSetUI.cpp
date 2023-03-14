#include "CHModelMoveTransformParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>
#include <QSpacerItem>
#include "edit_meshtransform_factory.h"
#include "../common/utilities/tlogger.h"

#define MAXNUM 999999
#define MINNUM -999999

CHModelMoveTransformParamsSetUI::CHModelMoveTransformParamsSetUI(QWidget* parent)
    : BubbleWidget(parent,Qt::LeftArrow)
{
    AkUtil::TFunction("");
    move(97, 350);
    setFixedSize(192, 205);
    setAutoFillBackground(true);
    //setStyleSheet("background-color:white;");
    setBackgroundColor(QColor(41,42,45));
    setStyleSheet("background:transparent");
    QVBoxLayout* mainblaout = new QVBoxLayout(this);

    QFont font1;
    font1.setPixelSize(14);
    m_moveLabel = new QLabel;
    m_moveLabel->setFont(font1);
    m_moveLabel->setText(tr("Move"));

    m_resetButton = new QToolButton;
    //    m_resetButton->setIcon(QIcon(":/images/fdm_remakes_small_icon_n.png"));
    //    m_resetButton->setGeometry(QRect(281.3, 277.04, 13.4, 281.3));
    m_resetButton->setObjectName("resetLabel");
    m_resetButton->setIcon(QIcon(":/images/fdm_remakes_small_icon_n.png"));
    m_resetButton->setMaximumWidth(30);
    m_resetButton->setMaximumHeight(30);
    m_resetButton->setMinimumWidth(30);
    m_resetButton->setMinimumHeight(30);
    connect(m_resetButton, &QToolButton::clicked, this, &CHModelMoveTransformParamsSetUI::reset);
    //int left, int top, int width, int height
    QHBoxLayout* hblaout1 = new QHBoxLayout;
    hblaout1->addWidget(m_moveLabel);
    hblaout1->addSpacing(125);
    hblaout1->addWidget(m_resetButton);
    hblaout1->setStretch(0, 35);
    hblaout1->setStretch(1, 125);
    hblaout1->setStretch(2, 13.4);
    mainblaout->addLayout(hblaout1);

    Line* line = new Line;
    //line->setGeometry(QRect(130, 303, 168, 1));

    QHBoxLayout* hblaout2 = new QHBoxLayout;
    hblaout2->addWidget(line);
    mainblaout->addLayout(hblaout2);

    QLabel* xLogo = new QLabel;
    QPixmap xLogoPixmap(":/images/fdm_move_x_icon_u.png");
    xLogo->setPixmap(xLogoPixmap);
    xLogo->setScaledContents(true);
    xLogo->setMaximumWidth(30);
    xLogo->setMaximumHeight(30);
    xLogo->setMinimumWidth(30);
    xLogo->setMinimumHeight(30);
    //xLogo->setGeometry(QRect(105, 403, 30, 30));

    QLabel* xlabel = new QLabel(QString("X"));
    xlabel->setObjectName("xLabel");
    xlabel->setMaximumWidth(10);
    xlabel->setMinimumWidth(10);
    xlabel->setMaximumHeight(16);
    xlabel->setMinimumHeight(16);
    m_xMoveBox = new MoveDoubleSpinBox;
    m_xMoveBox->setMaximumWidth(110);
    m_xMoveBox->setMaximumHeight(30);
    m_xMoveBox->setMinimumWidth(110);
    m_xMoveBox->setMinimumHeight(30);
    QHBoxLayout* xhlayout = new QHBoxLayout;
    xhlayout->addWidget(xLogo);
    xhlayout->addWidget(xlabel);
    xhlayout->addWidget(m_xMoveBox);
    xhlayout->setStretch(0, 30);
    xhlayout->setStretch(1, 10);
    xhlayout->setStretch(2, 110);
    mainblaout->addLayout(xhlayout);

    m_xMoveBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_xMoveBox->setAutoFillBackground(true);
    m_xMoveBox->setSuffix(" mm");

    QLabel* yLogo = new QLabel;
    QPixmap yLogoPixmap(":/images/fdm_move_y_icon_u.png");
    yLogo->setPixmap(yLogoPixmap);
    yLogo->setScaledContents(true);
    yLogo->setMaximumWidth(30);
    yLogo->setMaximumHeight(30);
    yLogo->setMinimumWidth(30);
    yLogo->setMinimumHeight(30);

    QLabel* ylabel = new QLabel(QString("Y"));
    ylabel->setObjectName("yLabel");
    ylabel->setMaximumWidth(10);
    ylabel->setMinimumWidth(10);
    ylabel->setMaximumHeight(16);
    ylabel->setMinimumHeight(16);
    m_yMoveBox = new MoveDoubleSpinBox();
    m_yMoveBox->setMaximumWidth(110);
    m_yMoveBox->setMaximumHeight(30);
    m_yMoveBox->setMinimumWidth(110);
    m_yMoveBox->setMinimumHeight(30);
    m_yMoveBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_yMoveBox->setAutoFillBackground(true);
    m_yMoveBox->setSuffix(" mm");

    QHBoxLayout* yhlayout = new QHBoxLayout;
    yhlayout->addWidget(yLogo);
    yhlayout->addWidget(ylabel);
    yhlayout->addWidget(m_yMoveBox);
    yhlayout->setStretch(0, 30);
    yhlayout->setStretch(1, 10);
    yhlayout->setStretch(2, 110);
    mainblaout->addLayout(yhlayout);

    QLabel* zLogo = new QLabel;
    QPixmap zLogoPixmap(":/images/fdm_move_z_icon_u.png");
    zLogo->setPixmap(zLogoPixmap);
    zLogo->setScaledContents(true);
    zLogo->setMaximumWidth(30);
    zLogo->setMaximumHeight(30);
    zLogo->setMinimumWidth(30);
    zLogo->setMinimumHeight(30);

    QLabel* zlabel = new QLabel(QString("Z"));
    zlabel->setObjectName("zLabel");
    zlabel->setMaximumWidth(10);
    zlabel->setMinimumWidth(10);
    zlabel->setMaximumHeight(16);
    zlabel->setMinimumHeight(16);

    m_zMoveBox = new MoveDoubleSpinBox;
    m_zMoveBox->setMaximumWidth(110);
    m_zMoveBox->setMaximumHeight(30);
    m_zMoveBox->setMinimumWidth(110);
    m_zMoveBox->setMinimumHeight(30);
    m_zMoveBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_zMoveBox->setSuffix(" mm");
    m_zMoveBox->setAutoFillBackground(true);

    QHBoxLayout* zhlayout = new QHBoxLayout;
    zhlayout->addWidget(zLogo);
    zhlayout->addWidget(zlabel);
    zhlayout->addWidget(m_zMoveBox);
    zhlayout->setStretch(0, 30);
    zhlayout->setStretch(1, 10);
    zhlayout->setStretch(2, 110);
    mainblaout->addLayout(zhlayout);

    m_keepLabel = new QLabel;
    m_keepLabel->setObjectName("keepLabel");
    m_keepLabel->setText(tr("Place On Bed"));

    m_lockCheck = new SwitchButton;
    m_lockCheck->setCheckState(false);
    m_lockCheck->setFixedWidth(26);
    m_lockCheck->setFixedHeight(14);
    QHBoxLayout* hblaout3 = new QHBoxLayout;
    hblaout3->addWidget(m_keepLabel);
    hblaout3->addWidget(m_lockCheck, Qt::AlignRight);
    hblaout3->setStretch(0, 110);
    hblaout3->setStretch(1, 26);
    mainblaout->addLayout(hblaout3);

    mainblaout->setStretch(0, 20);
    mainblaout->setStretch(1, 1);
    mainblaout->setStretch(2, 30);
    mainblaout->setStretch(3, 30);
    mainblaout->setStretch(4, 30);
    mainblaout->setStretch(5, 14);
    m_xMoveBox->setDecimals(2);
    m_xMoveBox->setMaximum(MAXNUM);
    m_xMoveBox->setMinimum(MINNUM);//moveValuesChangedSlot
    connect(m_xMoveBox, &MoveDoubleSpinBox::moveValueChangedSignal, this, &CHModelMoveTransformParamsSetUI::moveValuesChangedSlot);

    m_yMoveBox->setDecimals(2);
    m_yMoveBox->setMaximum(MAXNUM);
    m_yMoveBox->setMinimum(MINNUM);
    connect(m_yMoveBox, &MoveDoubleSpinBox::moveValueChangedSignal, this, &CHModelMoveTransformParamsSetUI::moveValuesChangedSlot);

    m_zMoveBox->setDecimals(2);
    m_zMoveBox->setMaximum(MAXNUM);
    m_zMoveBox->setMinimum(MINNUM);
    connect(m_zMoveBox, &MoveDoubleSpinBox::moveValueChangedSignal, this, &CHModelMoveTransformParamsSetUI::moveValuesChangedSlot);

    m_lockCheck->setCheckState(m_lockToPrintPlatform);
    connect(m_lockCheck, SIGNAL(stateChanged(bool)), this, SLOT(setLock(bool)));
}

CHModelMoveTransformParamsSetUI::~CHModelMoveTransformParamsSetUI()
{

}

void CHModelMoveTransformParamsSetUI::initValues(double x, double y, double z)
{
    m_xMoveBox->setInitValue(x);
    m_yMoveBox->setInitValue(y);
    m_zMoveBox->setInitValue(z);
}

void CHModelMoveTransformParamsSetUI::setOnlyMoveDoubleSpinBox(double x, double y, double z)
{
    m_xMoveBox->setOnlyValue(x);
    m_yMoveBox->setOnlyValue(y);
    m_zMoveBox->setOnlyValue(z);
}

void CHModelMoveTransformParamsSetUI::moveValuesChangedSlot(double value, MoveChangedType type)
{
    if(type == MoveChangedType_BoxMove)
    {
        std::vector<double> params(3);
        params[0] = m_xMoveBox->value();
        params[1] = m_yMoveBox->value();
        params[2] = m_zMoveBox->value();
        emit sendParams(params, type);
    }
}

void CHModelMoveTransformParamsSetUI::receiveParams(std::vector<double> params, MoveChangedType type)
{
    if(type == MoveChangedType_ViewMove || type == MoveChangedType_LockToPrintPlatform)
    {
         m_xMoveBox->setOnlyValue(params[0]);
         m_yMoveBox->setOnlyValue(params[1]);
         m_zMoveBox->setOnlyValue(params[2]);
    }
    else if(type == MoveChangedType_ResetMove)
    {
        m_xMoveBox->setOnlyValue(params[0]);
        m_yMoveBox->setOnlyValue(params[1]);
        m_zMoveBox->setOnlyValue(params[2]);
        moveValuesChangedSlot(params[0]);
    }
}

void CHModelMoveTransformParamsSetUI::setLock(bool checked)
{
   m_lockToPrintPlatform = checked;
   emit lockToPrintPlatformStatusChanged();
}

MoveDoubleSpinBox::MoveDoubleSpinBox(QWidget *parent) : QDoubleSpinBox{parent}
{
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(moveValueChangedSlot(double)));
}

void MoveDoubleSpinBox::setInitValue(double value)
{
    initValue = value;
}

void MoveDoubleSpinBox::setOnlyValue(double value)
{
    disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(moveValueChangedSlot(double)));
    setValue(value);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(moveValueChangedSlot(double)));
}

void MoveDoubleSpinBox::moveValueChangedSlot(double value, MoveChangedType type)
{
    if(type == MoveChangedType_BoxMove)
    {
        emit moveValueChangedSignal(value, type);
    }
    else
    {
        setOnlyValue(value);
    }
}

void CHModelMoveTransformParamsSetUI::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (m_moveLabel) {
            m_moveLabel->setText(tr("Move"));
        }
        if (m_keepLabel != nullptr) {
            m_keepLabel->setText(tr("Place On Bed"));
        }
    }
}

