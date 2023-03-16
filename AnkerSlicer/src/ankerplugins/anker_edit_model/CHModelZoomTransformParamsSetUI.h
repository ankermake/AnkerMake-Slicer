/*



*/


#ifndef CHModelZoomTransformParamsSetUI_H
#define CHModelZoomTransformParamsSetUI_H


#include <QDialog>
#include <QGroupBox>
#include <QScrollArea>
#include "QLabel"
#include "QLineEdit"
#include <vector>
#include "QPushButton"
#include "QCheckBox"
#include "CHCustomLineEdit.h"
#include "../../common/controlInterface/BubbleWidget.h"
#include "../../common/controlInterface/line.h"
#include "../../common/controlInterface/switchbutton.h"
#include <QDoubleSpinBox>
#include <QToolButton>

using namespace std;

enum ZoomChangedType
{
    ZoomChangedType_Unknown = -1,
    ZoomChangedType_Size, 
    ZoomChangedType_Scale,  
    ZoomChangedType_View,   
    ZoomChangedType_Ratio,  
    ZoomChangedType_Reset, 
    ZoomChangedType_ScaleToFit, 
    ZoomChangedType_Init
};

enum ZoomAxisType
{
    ZoomAxisType_Unknown = -1,
    ZoomAxisType_X,
    ZoomAxisType_Y,
    ZoomAxisType_Z
};

/*
*





*/

class SizeDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    SizeDoubleSpinBox(QWidget *parent = nullptr, ZoomAxisType axisType = ZoomAxisType_Unknown);
    void setInitValue(double value);
    void setOnlyValue(double value); 
    double getInitValue() const;
    ZoomAxisType getAxis() const;
signals:
    void scaleValueChanged(double value, ZoomChangedType type);
    void sizeRatioCheck(double value, ZoomChangedType type = ZoomChangedType_Ratio, ZoomAxisType axisType = ZoomAxisType_Unknown); 
    void setScale100Value(double value = 100); 

public slots:
    void sizeValueChanged(double value, ZoomChangedType type = ZoomChangedType_Size);
public:
    double initValue = 1.0;
private:

    ZoomAxisType axis;
};

class ScaleDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    ScaleDoubleSpinBox(QWidget *parent = nullptr, ZoomAxisType axisType = ZoomAxisType_X);
    void setInitValue(double value);
    ZoomAxisType getAxis() const;

signals:
    void viewValueChanged(double value, ZoomChangedType type, ZoomAxisType axisType = ZoomAxisType_X);

    void ratioScaleCheck(double value, ZoomChangedType type = ZoomChangedType_Ratio, ZoomAxisType axisType = ZoomAxisType_X); 

public slots:
    void scaleValueChanged(double value, ZoomChangedType type = ZoomChangedType_Scale);
    void setOnlyValue(double value); 

private:
    double initValue = 1.0;
    ZoomAxisType axis;
};

class CHModelZoomTransformParamsSetUI : public control::BubbleWidget
{
    Q_OBJECT

public:
    CHModelZoomTransformParamsSetUI(QWidget* parent = Q_NULLPTR);
    ~CHModelZoomTransformParamsSetUI();
    void initBoxSize(double x, double y, double z);
    void initScale(double sx, double sy, double sz);


public Q_SLOTS:
    void reset();
    void setLock(bool);

    void viewValueChanged(double value, ZoomChangedType type, ZoomAxisType axisType = ZoomAxisType_X);
    void boxSizeValuesChanged(std::vector<double> values, ZoomChangedType type);
    void scaleValuesChanged(std::vector<double> values, ZoomChangedType type);
    void scaleRatioChanged(double value, ZoomChangedType type, ZoomAxisType axisType = ZoomAxisType_X);
    void sizeRatioChanged(double value, ZoomChangedType type, ZoomAxisType axisType = ZoomAxisType_X);
    void setOnlyScaleValue(double sx, double sy, double sz);
    void setOnlySizeValue(double x, double y, double z);

Q_SIGNALS:
    void stateChanged(bool checked);

    void viewValuesChanged(std::vector<double> values, ZoomChangedType type, ZoomAxisType axisType = ZoomAxisType_X);
    void resetSignal();
    void scaleToFitSignal();
protected:
    void changeEvent(QEvent *e);
private:
    SwitchButton* m_lockCheckBox;
    QToolButton* m_scaleToFitButton;
    QToolButton* m_resetButton;

    SizeDoubleSpinBox* m_xSizeBox;
    SizeDoubleSpinBox* m_ySizeBox;
    SizeDoubleSpinBox* m_zSizeBox;

    ScaleDoubleSpinBox* m_xScaleBox;
    ScaleDoubleSpinBox* m_yScaleBox;
    ScaleDoubleSpinBox* m_zScaleBox;

    std::vector<float> m_ratios;
    float m_initRadious; 
    QLabel *m_scaleLabel = nullptr;
    QLabel *m_uScaleLabel = nullptr;
};
#endif
