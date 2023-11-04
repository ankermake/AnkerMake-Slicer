#ifndef CHModelRotationTransformParamsSetUI_H
#define CHModelRotationTransformParamsSetUI_H


#include <QDialog>
#include <QLabel>
#include <QEvent>
#include <vector>
#include "CHCustomLineEdit.h"
#include "../../common/controlInterface/BubbleWidget.h"
#include "../../common/controlInterface/line.h"
#include "../../common/controlInterface/switchbutton.h"
#include <QDoubleSpinBox>
#include <QToolButton>

using namespace std;

enum RotateChangedType
{
    RotateChangedType_Unknown = -1,
    RotateChangedType_BoxRotate, 
    RotateChangedType_ViewRotate, 
    RotateChangedType_ResetRotate, 
    RotateChangedType_initRotate
};

enum RotateAxisType
{
    RotateAxisType_Unknown = -1,
    RotateAxisType_X, 
    RotateAxisType_Y, 
    RotateAxisType_Z, 
};

class RotateDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    RotateDoubleSpinBox(QWidget* parent = nullptr);
    void setInitValue(double value);
    void setOnlyValue(double value);
    bool eventFilter(QObject* obj, QEvent* event) override;

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void rotateValueChangedSignal(double value, RotateChangedType type);

public slots:
    void rotateValueChangedSlot(double value, RotateChangedType type = RotateChangedType_BoxRotate);

private:
    double initValue = 0.0;
};

class CHModelRotationTransformParamsSetUI : public control::BubbleWidget
{
    Q_OBJECT

public:
    CHModelRotationTransformParamsSetUI(QWidget* parent = Q_NULLPTR);
    ~CHModelRotationTransformParamsSetUI();
    void initValues(double x, double y, double z);
    void setOnlyRotateDoubleSpinBox(double x, double y, double z);
    void getBoxEditValue(double &x, double &y, double &z);

public Q_SLOTS:
    void rotateValuesChangedSlot(double value, RotateChangedType type = RotateChangedType_BoxRotate);
    void receiveParam(double angle, RotateAxisType refAxis);
    void receiveParams(std::vector<double> params, RotateChangedType type = RotateChangedType_ViewRotate);
Q_SIGNALS:
    void sendParams(std::vector<double> params, RotateChangedType type);
    void valueChanged(double);
    void reset();
private:
    void adjustSingleAngle(float& angle);
    void changeEvent(QEvent *e);
    bool equalTo180(float value);
    bool equalTo360(float value);
private:

    RotateDoubleSpinBox* m_xRotBox;
    RotateDoubleSpinBox* m_yRotBox;
    RotateDoubleSpinBox* m_zRotBox;
    QToolButton* m_resetButton;
    QLabel *m_rotateLabel = nullptr;
};
#endif
