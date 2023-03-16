#include "CHModelRotationTransformParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>
#include <QKeyEvent>
#include <cmath>
#include "../common/utilities/tlogger.h"

#define MAXNUM 360
#define MINNUM 0

CHModelRotationTransformParamsSetUI::CHModelRotationTransformParamsSetUI(QWidget* parent)
    : BubbleWidget(parent,Qt::LeftArrow)
{
    AkUtil::TFunction("");
    move(97, 493);
    setFixedSize(185, 179);
    setAutoFillBackground(true);
//    setStyleSheet("background-color:white;");
    setBackgroundColor(QColor(41,42,45));
    setStyleSheet("background:transparent");
    QVBoxLayout* mainblaout = new QVBoxLayout(this);
    mainblaout->setContentsMargins(12,12,12,12);
    mainblaout->setSpacing(12);
    QFont font1;
    font1.setPixelSize(14);
    m_rotateLabel = new QLabel;
    m_rotateLabel->setFont(font1);
    m_rotateLabel->setText(tr("Rotate"));


    m_resetButton = new QToolButton;
    m_resetButton->setFocusPolicy(Qt::NoFocus);
    m_resetButton->setObjectName("resetLabel");
    static QIcon xIcon = QIcon(":/images/fdm_remakes_small_icon_n.png");
    m_resetButton->setIcon(xIcon);
    m_resetButton->setMaximumWidth(20);
    m_resetButton->setMaximumHeight(20);
    m_resetButton->setMinimumWidth(20);
    m_resetButton->setMinimumHeight(20);

    QHBoxLayout* hblaout1 = new QHBoxLayout;
    hblaout1->setContentsMargins(0,0,0,0);
    hblaout1->setSpacing(0);
    hblaout1->addWidget(m_rotateLabel);
    hblaout1->addWidget(m_resetButton);
    mainblaout->addLayout(hblaout1);

    Line* line = new Line;

    mainblaout->addWidget(line);

    QLabel* xLogo = new QLabel;
    static QPixmap xLogoPixmap(":/images/fdm_rotate_x_icon_u.png");
    xLogo->setPixmap(xLogoPixmap);
    xLogo->setScaledContents(true);
    xLogo->setMaximumWidth(30);
    xLogo->setMaximumHeight(30);
    xLogo->setMinimumWidth(30);
    xLogo->setMinimumHeight(30);

    QLabel* xlabel = new QLabel(QString("X"));
    xlabel->setObjectName("xLabel");
    xlabel->setMaximumWidth(10);
    xlabel->setMaximumHeight(16);
    xlabel->setMinimumWidth(10);
    xlabel->setMinimumHeight(16);

    m_xRotBox = new RotateDoubleSpinBox;

    m_xRotBox->setMaximumWidth(90);
    m_xRotBox->setMaximumHeight(30);
    m_xRotBox->setMinimumWidth(90);
    m_xRotBox->setMinimumHeight(30);
    m_xRotBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_xRotBox->setAutoFillBackground(true);
    m_xRotBox->setSuffix(" °");
    m_xRotBox->setDecimals(2);
    m_xRotBox->setMaximum(MAXNUM);
    m_xRotBox->setMinimum(MINNUM);
    m_xRotBox->setWrapping(true);
    m_xRotBox->setSingleStep(1.0);

    QHBoxLayout* xhlayout = new QHBoxLayout;
    xhlayout->setContentsMargins(0,0,0,0);
    xhlayout->setSpacing(6);
    xhlayout->addWidget(xLogo);
    xhlayout->addWidget(xlabel);
    xhlayout->addStretch();
    xhlayout->addWidget(m_xRotBox);
    mainblaout->addLayout(xhlayout);


    QLabel* yLogo = new QLabel;
    static QPixmap yLogoPixmap(":/images/fdm_rotate_y_icon_u.png");
    yLogo->setPixmap(yLogoPixmap);
    yLogo->setScaledContents(true);
    yLogo->setMaximumWidth(30);
    yLogo->setMaximumHeight(30);
    yLogo->setMinimumWidth(30);
    yLogo->setMinimumHeight(30);

    QLabel* ylabel = new QLabel(QString("Y"));
    ylabel->setObjectName("yLabel");
    ylabel->setMaximumWidth(10);
    ylabel->setMaximumHeight(16);
    ylabel->setMinimumWidth(10);
    ylabel->setMinimumHeight(16);
    m_yRotBox = new RotateDoubleSpinBox;

    m_yRotBox->setMaximumWidth(90);
    m_yRotBox->setMaximumHeight(30);
    m_yRotBox->setMinimumWidth(90);
    m_yRotBox->setMinimumHeight(30);
    m_yRotBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_yRotBox->setAutoFillBackground(true);
    m_yRotBox->setSuffix(" °");
    m_yRotBox->setDecimals(2);
    m_yRotBox->setMaximum(MAXNUM);
    m_yRotBox->setMinimum(MINNUM);
    m_yRotBox->setWrapping(true);
    m_yRotBox->setSingleStep(1.0);

    QHBoxLayout* yhlayout = new QHBoxLayout;
    yhlayout->setContentsMargins(0,0,0,0);
    yhlayout->setSpacing(6);
    yhlayout->addWidget(yLogo);
    yhlayout->addWidget(ylabel);
    yhlayout->addStretch();
    yhlayout->addWidget(m_yRotBox);
    mainblaout->addLayout(yhlayout);


    QLabel* zLogo = new QLabel;
    static QPixmap zLogoPixmap(":/images/fdm_rotate_z_icon_u.png");
    zLogo->setPixmap(zLogoPixmap);
    zLogo->setScaledContents(true);
    zLogo->setMaximumWidth(30);
    zLogo->setMaximumHeight(30);
    zLogo->setMinimumWidth(30);
    zLogo->setMinimumHeight(30);
    QLabel* zlabel = new QLabel(QString("Z"));
    zlabel->setObjectName("zLabel");
    zlabel->setMaximumWidth(10);
    zlabel->setMaximumHeight(16);
    zlabel->setMinimumWidth(10);
    zlabel->setMinimumHeight(16);
    m_zRotBox = new RotateDoubleSpinBox;

    m_zRotBox->setMaximumWidth(90);
    m_zRotBox->setMaximumHeight(30);
    m_zRotBox->setMinimumWidth(90);
    m_zRotBox->setMinimumHeight(30);
    m_zRotBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_zRotBox->setAutoFillBackground(true);
    m_zRotBox->setSuffix(" °");
    m_zRotBox->setDecimals(2);
    m_zRotBox->setMaximum(MAXNUM);
    m_zRotBox->setMinimum(MINNUM);
    m_zRotBox->setWrapping(true);
    m_zRotBox->setSingleStep(1.0);

    QHBoxLayout* zhlayout = new QHBoxLayout;
    zhlayout->setContentsMargins(0,0,0,0);
    zhlayout->setSpacing(6);
    zhlayout->addWidget(zLogo);
    zhlayout->addWidget(zlabel);
    zhlayout->addStretch();
    zhlayout->addWidget(m_zRotBox);
    mainblaout->addLayout(zhlayout);

    connect(m_resetButton, &QToolButton::clicked, this, &CHModelRotationTransformParamsSetUI::reset);
    connect(m_xRotBox, &RotateDoubleSpinBox::rotateValueChangedSignal, this, &CHModelRotationTransformParamsSetUI::rotateValuesChangedSlot);
    connect(m_yRotBox, &RotateDoubleSpinBox::rotateValueChangedSignal, this, &CHModelRotationTransformParamsSetUI::rotateValuesChangedSlot);
    connect(m_zRotBox, &RotateDoubleSpinBox::rotateValueChangedSignal, this, &CHModelRotationTransformParamsSetUI::rotateValuesChangedSlot);

}

CHModelRotationTransformParamsSetUI::~CHModelRotationTransformParamsSetUI()
{

}

void CHModelRotationTransformParamsSetUI::initValues(double x, double y, double z)
{
    m_xRotBox->setInitValue(x);
    m_yRotBox->setInitValue(y);
    m_zRotBox->setInitValue(z);
}

void CHModelRotationTransformParamsSetUI::setOnlyRotateDoubleSpinBox(double x, double y, double z)
{
    if(x < 0)
    {
        x = x + 360.0;
    }
    if(y < 0)
    {
        y = y + 360.0;
    }
    if(z < 0)
    {
        z = z + 360.0;
    }
    if(equalTo180(x) && equalTo180(z))
    {
        x = 0.0;
        z = 0.0;
        if(y >= 270 && y <= 360)
        {
            y = 360 - y + 180;
        }
        else if(y >= 0 && y <= 90)
        {
            y = 180 - y;
        }
    }
    if(x >= 360)
    {
        x = x - 360;
    }
    if(y >= 360)
    {
        y = y - 360;
    }
    if(z >= 360)
    {
        z = z - 360;
    }
    if(equalTo180(x) && equalTo180(z))
    {
        x = 0;
        z = 0;
    }
    if(equalTo360(x))
    {
        x = 0;
    }
    if(equalTo360(y))
    {
        y = 0;
    }
    if(equalTo360(z))
    {
        z = 0;
    }
    m_xRotBox->setOnlyValue(x);
    m_yRotBox->setOnlyValue(y);
    m_zRotBox->setOnlyValue(z);
}

void CHModelRotationTransformParamsSetUI::getBoxEditValue(double &x, double &y, double &z)
{
    x = m_xRotBox->value();
    y = m_yRotBox->value();
    z = m_zRotBox->value();
}


void CHModelRotationTransformParamsSetUI::rotateValuesChangedSlot(double value, RotateChangedType type)
{
    if(type == RotateChangedType_BoxRotate)
    {
        std::vector<double> params(3);
        params[0] = m_xRotBox->value();
        params[1] = m_yRotBox->value();
        params[2] = m_zRotBox->value();
        emit sendParams(params, type);
    }
}

void CHModelRotationTransformParamsSetUI::receiveParam(double angle, RotateAxisType refAxis)
{
    if(refAxis == RotateAxisType_X)
    {
        //m_xRotBox->setOnlyValue(angle);
        setOnlyRotateDoubleSpinBox(angle, m_yRotBox->value(), m_zRotBox->value());
    }
    else if(refAxis == RotateAxisType_Y)
    {
        //m_yRotBox->setOnlyValue(angle);
        setOnlyRotateDoubleSpinBox(m_xRotBox->value(), angle, m_zRotBox->value());
    }
    else if(refAxis == RotateAxisType_Z)
    {
        //m_zRotBox->setOnlyValue(angle);
        setOnlyRotateDoubleSpinBox(m_xRotBox->value(), m_yRotBox->value(), angle);
    }
}

void CHModelRotationTransformParamsSetUI::receiveParams(std::vector<double> params, RotateChangedType type)
{
    //qDebug() << "receiveParams: " << params[0] << ", " << params[1] << ", " << params[2] << ", type: " << type;
    if(type == RotateChangedType_ViewRotate)
    {
        setOnlyRotateDoubleSpinBox(params[0], params[1], params[2]);
    }
    else if(type == RotateChangedType_initRotate ||type ==  RotateChangedType_ResetRotate)
    {
        setOnlyRotateDoubleSpinBox(params[0], params[1], params[2]);
        rotateValuesChangedSlot(params[0]);
        if(RotateChangedType_ResetRotate == type)
        {
            setOnlyRotateDoubleSpinBox(0, 0, 0);
        }
    }
}


void CHModelRotationTransformParamsSetUI::adjustSingleAngle(float& angle)
{
    int n = (int)(angle / 360.0);
    if (angle < 0)
    {
        n--;
    }
    angle = angle - 360.0 * n;
}


RotateDoubleSpinBox::RotateDoubleSpinBox(QWidget *parent) : QDoubleSpinBox{parent}
{
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(rotateValueChangedSlot(double)));
}

void RotateDoubleSpinBox::setInitValue(double value)
{
    initValue = value;
}

void RotateDoubleSpinBox::setOnlyValue(double value)
{
    disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(rotateValueChangedSlot(double)));
    setValue(value);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(rotateValueChangedSlot(double)));
}

void RotateDoubleSpinBox::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Plus) || (event->key() == Qt::Key_Minus)){
        event->ignore();
    }
    else
    {
        QDoubleSpinBox::keyPressEvent(event);
    }
}


void RotateDoubleSpinBox::rotateValueChangedSlot(double value, RotateChangedType type)
{
    if(type == RotateChangedType_BoxRotate)
    {
        emit rotateValueChangedSignal(value, type);
    }
    else
    {
        setOnlyValue(value);
    }
}

void CHModelRotationTransformParamsSetUI::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (m_rotateLabel != nullptr) {
            m_rotateLabel->setText(tr("Rotate"));
        }
    }
    QWidget::changeEvent(e);
}

bool CHModelRotationTransformParamsSetUI::equalTo180(float value)
{
    bool result = false;
    if(fabs(value - 180) <= 0.001)
        result = true;
    return result;
}

bool CHModelRotationTransformParamsSetUI::equalTo360(float value)
{
    bool result = false;
    if(fabs(value - 360) <= 0.001)
        result = true;
    return result;
}

