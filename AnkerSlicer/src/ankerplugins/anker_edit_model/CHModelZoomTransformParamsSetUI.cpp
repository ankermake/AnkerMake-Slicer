#include "CHModelZoomTransformParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>
#include "edit_meshtransform_factory.h"
#include "../common/utilities/tlogger.h"

#define MAXNUM 99999
#define MINNUM -99999
#define ZERO 0.0001

CHModelZoomTransformParamsSetUI::CHModelZoomTransformParamsSetUI(QWidget* parent)
    : BubbleWidget(parent,Qt::LeftArrow)
{
    AkUtil::TFunction("");
    qRegisterMetaType<ZoomChangedType>("ZoomChangedType");

    move(97, 422);
    setFixedSize(248, 262);

    setAutoFillBackground(true);
    //setStyleSheet("background-color:white;");
    setBackgroundColor(QColor(41,42,45));
    setStyleSheet("background:transparent");
    m_ratios.resize(3);

    QVBoxLayout* mainblaout = new QVBoxLayout(this);
    QFont font1;
    font1.setPixelSize(14);
    font1.setFamily(QString("Arial"));
    m_scaleLabel = new QLabel;
    m_scaleLabel->setFont(font1);
    m_scaleLabel->setText(tr("Scale"));

    m_resetButton = new QToolButton;
    m_resetButton->setFocusPolicy(Qt::NoFocus);
    m_resetButton->setObjectName("resetLabel");
    m_resetButton->setIcon(QIcon(":/images/fdm_remakes_small_icon_n.png"));
    m_resetButton->setMaximumWidth(30);
    m_resetButton->setMaximumHeight(30);
    m_resetButton->setMinimumWidth(30);
    m_resetButton->setMinimumHeight(30);
    connect(m_resetButton, &QToolButton::clicked, this, &CHModelZoomTransformParamsSetUI::resetSignal);
    QHBoxLayout* hblaout1 = new QHBoxLayout;
    hblaout1->addWidget(m_scaleLabel);
    hblaout1->addSpacing(172);
    hblaout1->addWidget(m_resetButton);
    hblaout1->setStretch(0, 35);
    hblaout1->setStretch(1, 172);
    hblaout1->setStretch(2, 13);
    mainblaout->addLayout(hblaout1);


    Line* line = new Line;

    QHBoxLayout* hblayout2 = new QHBoxLayout;
    hblayout2->addWidget(line);
    mainblaout->addLayout(hblayout2);

    QFont font2;
    font2.setPixelSize(12);
    m_uScaleLabel = new QLabel;
    m_uScaleLabel->setObjectName("uniformScaleLabel");
    m_uScaleLabel->setText(tr("Uniform Scaling"));
    m_uScaleLabel->setFont(font2);

    m_lockCheckBox = new SwitchButton;
    m_lockCheckBox->setFixedWidth(26);
    m_lockCheckBox->setFixedHeight(14);
    connect(m_lockCheckBox, &SwitchButton::stateChanged, this, &CHModelZoomTransformParamsSetUI::setLock);
    m_lockCheckBox->setCheckState(m_lockScaleRatio);
    QHBoxLayout* hblayout3 = new QHBoxLayout;
    hblayout3->addWidget(m_uScaleLabel);
    hblayout3->addWidget(m_lockCheckBox, Qt::AlignRight);
    hblayout3->setStretch(0, 86);
    hblayout3->setStretch(1, 26);
    mainblaout->addLayout(hblayout3);


    QLabel* xLogo = new QLabel;
    QPixmap xLogoPixmap(":/images/fdm_scale_x_icon_u.png");
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
    xlabel->setMidLineWidth(10);
    xlabel->setMinimumHeight(16);

    m_xSizeBox = new SizeDoubleSpinBox(this, ZoomAxisType_X);
    m_xSizeBox->setMaximumWidth(70);
    m_xSizeBox->setMaximumHeight(30);
    m_xSizeBox->setMinimumWidth(70);
    m_xSizeBox->setMinimumHeight(30);
    m_xSizeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_xSizeBox->setAutoFillBackground(true);
    m_xSizeBox->setSuffix(" mm");
    m_xSizeBox->setDecimals(2);
    m_xSizeBox->setMinimum(MINNUM);
    m_xSizeBox->setMaximum(MAXNUM);
    //m_xBox->setReadOnly(true);

    QLabel* xLogo2 = new QLabel;
    QPixmap xLogo2Pixmap(":/images/fm_scale_round_icon_u.png");
    xLogo2->setPixmap(xLogo2Pixmap);
    xLogo2->setScaledContents(true);
    xLogo2->setMaximumWidth(16);
    xLogo2->setMaximumHeight(16);
    xLogo2->setMinimumWidth(16);
    xLogo2->setMinimumHeight(16);

    m_xScaleBox = new ScaleDoubleSpinBox(this, ZoomAxisType_X);
    m_xScaleBox->setMaximumWidth(68);
    m_xScaleBox->setMaximumHeight(30);
    m_xScaleBox->setMinimumWidth(68);
    m_xScaleBox->setMinimumHeight(30);
    m_xScaleBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_xScaleBox->setAutoFillBackground(true);
    m_xScaleBox->setValue(100.0);
    m_xScaleBox->setMinimum(MINNUM);
    m_xScaleBox->setMaximum(MAXNUM);
    m_xScaleBox->setSuffix("%");

    QHBoxLayout* hblayout4 = new QHBoxLayout;
    hblayout4->addWidget(xLogo);
    hblayout4->addWidget(xlabel);
    hblayout4->addWidget(m_xSizeBox);
    hblayout4->addWidget(xLogo2);
    hblayout4->addWidget(m_xScaleBox);
    hblayout4->setStretch(0, 30);
    hblayout4->setStretch(1, 10);
    hblayout4->setStretch(2, 70);
    hblayout4->setStretch(3, 16);
    hblayout4->setStretch(4, 68);
    mainblaout->addLayout(hblayout4);

    QLabel* yLogo = new QLabel;
    QPixmap yLogoPixmap(":/images/fdm_scale_y_icon_u.png");
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
    ylabel->setMidLineWidth(10);
    ylabel->setMinimumHeight(16);

    m_ySizeBox = new SizeDoubleSpinBox(this, ZoomAxisType_Y);
    m_ySizeBox->setMaximumWidth(70);
    m_ySizeBox->setMaximumHeight(30);
    m_ySizeBox->setMinimumWidth(70);
    m_ySizeBox->setMinimumHeight(30);
    m_ySizeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_ySizeBox->setAutoFillBackground(true);
    m_ySizeBox->setSuffix(" mm");
    m_ySizeBox->setDecimals(2);
    m_ySizeBox->setMinimum(MINNUM);
    m_ySizeBox->setMaximum(MAXNUM);

    //m_yBox->setReadOnly(true);
    QLabel* yLogo2 = new QLabel;
    QPixmap yLogo2Pixmap(":/images/fm_scale_round_icon_u.png");
    yLogo2->setPixmap(yLogo2Pixmap);
    yLogo2->setScaledContents(true);
    yLogo2->setMaximumWidth(16);
    yLogo2->setMaximumHeight(16);
    yLogo2->setMinimumWidth(16);
    yLogo2->setMinimumHeight(16);

    m_yScaleBox = new ScaleDoubleSpinBox(this, ZoomAxisType_Y);
    m_yScaleBox->setMaximumWidth(68);
    m_yScaleBox->setMaximumHeight(30);
    m_yScaleBox->setMinimumWidth(68);
    m_yScaleBox->setMinimumHeight(30);
    m_yScaleBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_yScaleBox->setAutoFillBackground(true);
    m_yScaleBox->setValue(100.0);
    m_yScaleBox->setMinimum(0.001);
    m_yScaleBox->setMaximum(10000);
    m_yScaleBox->setSuffix("%");

    QHBoxLayout* hblayout5 = new QHBoxLayout;
    hblayout5->addWidget(yLogo);
    hblayout5->addWidget(ylabel);
    hblayout5->addWidget(m_ySizeBox);
    hblayout5->addWidget(yLogo2);
    hblayout5->addWidget(m_yScaleBox);
    hblayout5->setStretch(0, 30);
    hblayout5->setStretch(1, 10);
    hblayout5->setStretch(2, 70);
    hblayout5->setStretch(3, 16);
    hblayout5->setStretch(4, 68);
    mainblaout->addLayout(hblayout5);


    QLabel* zLogo = new QLabel;
    QPixmap zLogoPixmap(":/images/fdm_scale_z_icon_u.png");
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
    zlabel->setMidLineWidth(10);
    zlabel->setMinimumHeight(16);
    m_zSizeBox = new SizeDoubleSpinBox(this, ZoomAxisType_Z);
    m_zSizeBox->setMaximumWidth(70);
    m_zSizeBox->setMaximumHeight(30);
    m_zSizeBox->setMinimumWidth(70);
    m_zSizeBox->setMinimumHeight(30);
    m_zSizeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_zSizeBox->setAutoFillBackground(true);
    m_zSizeBox->setDecimals(2);
    m_zSizeBox->setMinimum(MINNUM);
    m_zSizeBox->setMaximum(MAXNUM);
    m_zSizeBox->setSuffix(" mm");

    QLabel* zLogo2 = new QLabel;
    QPixmap zLogo2Pixmap(":/images/fm_scale_round_icon_u.png");
    zLogo2->setPixmap(zLogo2Pixmap);
    zLogo2->setScaledContents(true);
    zLogo2->setMaximumWidth(16);
    zLogo2->setMaximumHeight(16);
    zLogo2->setMinimumWidth(16);
    zLogo2->setMinimumHeight(16);

    m_zScaleBox = new ScaleDoubleSpinBox(this, ZoomAxisType_Z);
    m_zScaleBox->setMaximumWidth(68);
    m_zScaleBox->setMaximumHeight(30);
    m_zScaleBox->setMinimumWidth(68);
    m_zScaleBox->setMinimumHeight(30);
    m_zScaleBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_zScaleBox->setAutoFillBackground(true);
    m_zScaleBox->setValue(100.0);
    m_zScaleBox->setMinimum(0.001);
    m_zScaleBox->setMaximum(10000);
    m_zScaleBox->setSuffix("%");
    QHBoxLayout* hblayout6 = new QHBoxLayout;
    hblayout6->addWidget(zLogo);
    hblayout6->addWidget(zlabel);
    hblayout6->addWidget(m_zSizeBox);
    hblayout6->addWidget(zLogo2);
    hblayout6->addWidget(m_zScaleBox);
    hblayout6->setStretch(0, 30);
    hblayout6->setStretch(1, 10);
    hblayout6->setStretch(2, 70);
    hblayout6->setStretch(3, 16);
    hblayout6->setStretch(4, 68);
    mainblaout->addLayout(hblayout6);

    Line* line2 = new Line;
    QHBoxLayout* hblayout8 = new QHBoxLayout;
    hblayout8->addWidget(line2);
    mainblaout->addLayout(hblayout8);

    m_scaleToFitButton = new QToolButton;
    m_scaleToFitButton->setObjectName("scaleToFitButton");
    m_scaleToFitButton->setText(tr("Scale To Fit"));
    m_scaleToFitButton->setMaximumWidth(200);
    m_scaleToFitButton->setMaximumHeight(32);
    m_scaleToFitButton->setMinimumWidth(200);
    m_scaleToFitButton->setMinimumHeight(32);
    m_scaleToFitButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "   background-color: #3A3B3F;\n"
        "   border-radius:4px;\n"
        "   color:#FFFFFF;\n"
        "   font-size:14px;\n"
        "}\n"));
    QHBoxLayout* hblayout7 = new QHBoxLayout;
   // hblayout7->addSpacing(80);
    hblayout7->setContentsMargins(0,0,0,0);
    hblayout7->setSpacing(0);
    hblayout7->addStretch();
    hblayout7->addWidget(m_scaleToFitButton, Qt::AlignRight);
    mainblaout->addLayout(hblayout7);
    connect(m_scaleToFitButton, &QToolButton::clicked, this, &CHModelZoomTransformParamsSetUI::scaleToFitSignal);

    m_xScaleBox->setDecimals(2);
    m_xScaleBox->setMaximum(MAXNUM);
    m_xScaleBox->setMinimum(MINNUM);
    connect(m_xScaleBox, &ScaleDoubleSpinBox::viewValueChanged, this, &CHModelZoomTransformParamsSetUI::viewValueChanged);
    connect(m_xScaleBox, &ScaleDoubleSpinBox::ratioScaleCheck, this, &CHModelZoomTransformParamsSetUI::scaleRatioChanged);

    m_yScaleBox->setDecimals(2);
    m_yScaleBox->setMaximum(MAXNUM);
    m_yScaleBox->setMinimum(MINNUM);
    connect(m_yScaleBox, &ScaleDoubleSpinBox::viewValueChanged, this, &CHModelZoomTransformParamsSetUI::viewValueChanged);
    connect(m_yScaleBox, &ScaleDoubleSpinBox::ratioScaleCheck, this, &CHModelZoomTransformParamsSetUI::scaleRatioChanged);


    m_zScaleBox->setDecimals(2);
    m_zScaleBox->setMaximum(MAXNUM);
    m_zScaleBox->setMinimum(MINNUM);
    connect(m_zScaleBox, &ScaleDoubleSpinBox::viewValueChanged, this, &CHModelZoomTransformParamsSetUI::viewValueChanged);
    connect(m_zScaleBox, &ScaleDoubleSpinBox::ratioScaleCheck, this, &CHModelZoomTransformParamsSetUI::scaleRatioChanged);


    connect(m_xSizeBox, &SizeDoubleSpinBox::scaleValueChanged, m_xScaleBox, &ScaleDoubleSpinBox::scaleValueChanged);
    connect(m_ySizeBox, &SizeDoubleSpinBox::scaleValueChanged, m_yScaleBox, &ScaleDoubleSpinBox::scaleValueChanged);
    connect(m_zSizeBox, &SizeDoubleSpinBox::scaleValueChanged, m_zScaleBox, &ScaleDoubleSpinBox::scaleValueChanged);

    connect(m_xSizeBox, &SizeDoubleSpinBox::sizeRatioCheck, this, &CHModelZoomTransformParamsSetUI::sizeRatioChanged);
    connect(m_ySizeBox, &SizeDoubleSpinBox::sizeRatioCheck, this, &CHModelZoomTransformParamsSetUI::sizeRatioChanged);
    connect(m_zSizeBox, &SizeDoubleSpinBox::sizeRatioCheck, this, &CHModelZoomTransformParamsSetUI::sizeRatioChanged);//setScale100Value

    connect(m_xSizeBox, &SizeDoubleSpinBox::setScale100Value, m_xScaleBox, &ScaleDoubleSpinBox::setOnlyValue);
    connect(m_ySizeBox, &SizeDoubleSpinBox::setScale100Value, m_yScaleBox, &ScaleDoubleSpinBox::setOnlyValue);
    connect(m_zSizeBox, &SizeDoubleSpinBox::setScale100Value, m_zScaleBox, &ScaleDoubleSpinBox::setOnlyValue);

}

CHModelZoomTransformParamsSetUI::~CHModelZoomTransformParamsSetUI()
{

}

void CHModelZoomTransformParamsSetUI::initBoxSize(double x, double y, double z)
{
    m_xSizeBox->setInitValue(x);
    m_ySizeBox->setInitValue(y);
    m_zSizeBox->setInitValue(z);
    m_initRadious = sqrt(x*x + y*y + z*z) / 2.0;
}

void CHModelZoomTransformParamsSetUI::initScale(double sx, double sy, double sz)
{
    m_xScaleBox->setInitValue(sx);
    m_yScaleBox->setInitValue(sy);
    m_zScaleBox->setInitValue(sz);
}

void CHModelZoomTransformParamsSetUI::setOnlyScaleValue(double sx, double sy, double sz)
{
    m_xScaleBox->setOnlyValue(sx);
    m_yScaleBox->setOnlyValue(sy);
    m_zScaleBox->setOnlyValue(sz);
}

void CHModelZoomTransformParamsSetUI::setOnlySizeValue(double x, double y, double z)
{
    m_xSizeBox->setOnlyValue(x);
    m_ySizeBox->setOnlyValue(y);
    m_zSizeBox->setOnlyValue(z);
}

void CHModelZoomTransformParamsSetUI::reset()
{
    m_xScaleBox->setValue(100.0);
    m_yScaleBox->setValue(100.0);
    m_zScaleBox->setValue(100.0);
}

void CHModelZoomTransformParamsSetUI::setLock(bool checked)
{
    m_lockScaleRatio = checked;
}

void CHModelZoomTransformParamsSetUI::viewValueChanged(double value, ZoomChangedType type, ZoomAxisType axisType)
{
    std::vector<double> params(3);
    params[0] = ((fabs(m_xScaleBox->value()) < ZERO ? 0.01 : m_xScaleBox->value()) / 100.0);
    params[1] = ((fabs(m_yScaleBox->value()) < ZERO ? 0.01 : m_yScaleBox->value()) / 100.0);
    params[2] = ((fabs(m_zScaleBox->value()) < ZERO ? 0.01 : m_zScaleBox->value()) / 100.0);
    emit viewValuesChanged(params, type, axisType);
}

void CHModelZoomTransformParamsSetUI::boxSizeValuesChanged(std::vector<double> values, ZoomChangedType type)
{
    if(values.size() != 3)
    {
        return;
    }
    if(ZoomChangedType_Init == type)
    {
        m_xSizeBox->setOnlyValue(values[0]);
        m_ySizeBox->setOnlyValue(values[1]);
        m_zSizeBox->setOnlyValue(values[2]);
        m_xScaleBox->setOnlyValue(values[0] / m_xSizeBox->initValue * 100.0);
        m_yScaleBox->setOnlyValue(values[1] / m_ySizeBox->initValue * 100.0);
        m_zScaleBox->setOnlyValue(values[2] / m_zSizeBox->initValue * 100.0);
        viewValueChanged(values[0] / m_xSizeBox->initValue, type);
    }
    else if(ZoomChangedType_ScaleToFit == type)
    {
        m_xSizeBox->setOnlyValue(values[0]);
        m_ySizeBox->setOnlyValue(values[1]);
        m_zSizeBox->setOnlyValue(values[2]);

//        m_xScaleBox->setOnlyValue(values[0] / m_xSizeBox->initValue * 100.0);
//        m_yScaleBox->setOnlyValue(values[1] / m_ySizeBox->initValue * 100.0);
//        m_zScaleBox->setOnlyValue(values[2] / m_zSizeBox->initValue * 100.0);
    }
    else if(ZoomChangedType_Reset == type)
    {
        int dir[3] = {0};
        dir[0] = values[0] > 0 ? 1: -1;
        dir[1] = values[1] > 0 ? 1: -1;
        dir[2] = values[2] > 0 ? 1: -1;
        m_xSizeBox->setOnlyValue(m_xSizeBox->initValue * dir[0]);
        m_ySizeBox->setOnlyValue(m_ySizeBox->initValue * dir[1]);
        m_zSizeBox->setOnlyValue(m_zSizeBox->initValue * dir[2]);
        m_xScaleBox->setOnlyValue(100.0 * dir[0]);
        m_yScaleBox->setOnlyValue(100.0 * dir[1]);
        m_zScaleBox->setOnlyValue(100.0 * dir[2]);
        viewValueChanged(1.0, type);
    }
    else
    {
        m_xSizeBox->sizeValueChanged((fabs(values[0]) < ZERO ? m_xSizeBox->initValue * 0.01 : values[0]), type);
        m_ySizeBox->sizeValueChanged((fabs(values[1]) < ZERO ? m_ySizeBox->initValue * 0.01 : values[1]), type);
        m_zSizeBox->sizeValueChanged((fabs(values[2]) < ZERO ? m_zSizeBox->initValue * 0.01 : values[2]), type);
    }

}

void CHModelZoomTransformParamsSetUI::scaleValuesChanged(std::vector<double> values, ZoomChangedType type)
{
    if(values.size() != 3)
    {
        return;
    }

    if(type == ZoomChangedType_ScaleToFit)
    {
        qDebug() << "ZoomChangedType_Ratio: " << values[0] << ", " << values[1] << ", " << values[2];
        m_xScaleBox->setOnlyValue(values[0] * 100.0);
        m_yScaleBox->setOnlyValue(values[1] * 100.0);
        m_zScaleBox->setOnlyValue(values[2] * 100.0);
        //viewValueChanged(values[0], type);
    }
    else
    {
        m_xScaleBox->scaleValueChanged((fabs(values[0]) < ZERO ? 0.01 : values[0]) * 100.0, type);
        m_yScaleBox->scaleValueChanged((fabs(values[1]) < ZERO ? 0.01 : values[1]) * 100.0, type);
        m_zScaleBox->scaleValueChanged((fabs(values[2]) < ZERO ? 0.01 : values[2]) * 100.0, type);
    }
}

void CHModelZoomTransformParamsSetUI::sizeRatioChanged(double value, ZoomChangedType type, ZoomAxisType axisType )
{
    if(m_lockScaleRatio)
    {
        int dir[3] = {1};
        value = fabs(value);
        value = value < ZERO ? 0.01 : value;
        dir[0] = (m_xSizeBox->value() > 0 ? 1 : -1);
        dir[1] = (m_ySizeBox->value() > 0 ? 1 : -1);
        dir[2] = (m_zSizeBox->value() > 0 ? 1 : -1);
        m_xSizeBox->setOnlyValue(value * m_xSizeBox->getInitValue() * dir[0]);
        m_ySizeBox->setOnlyValue(value * m_ySizeBox->getInitValue() * dir[1]);
        m_zSizeBox->setOnlyValue(value * m_zSizeBox->getInitValue() * dir[2]);

        if(axisType == ZoomAxisType_X)
        {
            m_yScaleBox->setOnlyValue(value * 100 * dir[1]);
            m_zScaleBox->setOnlyValue(value * 100 * dir[2]);
            //m_ySizeBox->sizeValueChanged(value * m_ySizeBox->getInitValue(), ZoomChangedType_Ratio);
            //m_zSizeBox->sizeValueChanged(value * m_zSizeBox->getInitValue(), ZoomChangedType_Ratio);
        }
        else if(axisType == ZoomAxisType_Y)
        {
            m_xScaleBox->setOnlyValue(value * 100 * dir[0]);
            m_zScaleBox->setOnlyValue(value * 100 * dir[2]);
            //m_xSizeBox->sizeValueChanged(value * m_xSizeBox->getInitValue(), ZoomChangedType_Ratio);
            //m_zSizeBox->sizeValueChanged(value * m_zSizeBox->getInitValue(), ZoomChangedType_Ratio);
        }
        else if(axisType == ZoomAxisType_Z)
        {
            m_xScaleBox->setOnlyValue(value * 100 * dir[0]);
            m_yScaleBox->setOnlyValue(value * 100 * dir[1]);
            //m_xSizeBox->sizeValueChanged(value * m_xSizeBox->getInitValue(), ZoomChangedType_Ratio);
            //m_ySizeBox->sizeValueChanged(value * m_ySizeBox->getInitValue(), ZoomChangedType_Ratio);
        }
    }
}

void CHModelZoomTransformParamsSetUI::scaleRatioChanged(double value, ZoomChangedType type, ZoomAxisType axisType)
{
    if(m_lockScaleRatio)
    {
        value = fabs(value) < ZERO ? 0.01 : value;
        m_xScaleBox->setOnlyValue(value);
        m_yScaleBox->setOnlyValue(value);
        m_zScaleBox->setOnlyValue(value);
    }
}



SizeDoubleSpinBox::SizeDoubleSpinBox(QWidget *parent, ZoomAxisType axisType) : QDoubleSpinBox{parent}, axis(axisType)
{
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(sizeValueChanged(double)));
}

void SizeDoubleSpinBox::setInitValue(double value)
{
    initValue = value;
}

void SizeDoubleSpinBox::setOnlyValue(double value)
{
    
    disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(sizeValueChanged(double)));
    setValue(value);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(sizeValueChanged(double)));
}

double SizeDoubleSpinBox::getInitValue() const
{
    return initValue;
}

ZoomAxisType SizeDoubleSpinBox::getAxis() const
{
    return axis;
}

void SizeDoubleSpinBox::sizeValueChanged(double value, ZoomChangedType type)
{
    if(type == ZoomChangedType_Size)
    {
        if(m_lockScaleRatio)
        {
            emit sizeRatioCheck(value / initValue, ZoomChangedType_Ratio, axis); 
        }
    }
    else
    {
        setOnlyValue(value);

        if(type == ZoomChangedType_Scale)
        {
            return;
        }

    }
    double scaleValue = value / initValue * 100.0;
    emit scaleValueChanged(scaleValue, type);
}


ScaleDoubleSpinBox::ScaleDoubleSpinBox(QWidget *parent, ZoomAxisType axisType) : QDoubleSpinBox{parent}, axis(axisType)
{
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(scaleValueChanged(double)));
}

void ScaleDoubleSpinBox::setInitValue(double value)
{
    initValue = value;
}

ZoomAxisType ScaleDoubleSpinBox::getAxis() const
{
    return axis;
}

void ScaleDoubleSpinBox::scaleValueChanged(double value, ZoomChangedType type)
{
    if(type == ZoomChangedType_Scale)
    {
        if(m_lockScaleRatio)
        {
            emit ratioScaleCheck(value, ZoomChangedType_Ratio, axis);
        }
    }
    else
    {
        setOnlyValue(value);
        if(type == ZoomChangedType_View)
        {
            return;
        }
    }
    emit viewValueChanged(value / 100.0, type, axis);
}

void ScaleDoubleSpinBox::setOnlyValue(double value)
{
    disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(scaleValueChanged(double)));
    setValue(value);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(scaleValueChanged(double)));
}

void CHModelZoomTransformParamsSetUI::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (m_scaleLabel != nullptr) {
            m_scaleLabel->setText(tr("Scale"));
        }
        if (m_uScaleLabel != nullptr) {
            m_uScaleLabel->setText(tr("Uniform Scaling"));
        }
        if (m_scaleToFitButton != nullptr) {
            m_scaleToFitButton->setText(tr("Scale To Fit"));
        }
    }
}
