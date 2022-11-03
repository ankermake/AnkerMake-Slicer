
#ifndef CHModelMoveTransformParamsSetUI_H
#define CHModelMoveTransformParamsSetUI_H


#include <QDialog>
#include "QLabel"
#include <vector>
#include "../../common/controlInterface/BubbleWidget.h"
#include "../../common/controlInterface/line.h"
#include "../../common/controlInterface/switchbutton.h"
#include <QDoubleSpinBox>
#include <QToolButton>


using namespace std;
enum MoveChangedType
{
    MoveChangedType_Unknown = -1,
    MoveChangedType_BoxMove, 
    MoveChangedType_ViewMove, 
    MoveChangedType_ResetMove, 
    MoveChangedType_LockToPrintPlatform 
};

class MoveDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    MoveDoubleSpinBox(QWidget *parent = nullptr);
    void setInitValue(double value);
    void setOnlyValue(double value); 
signals:
    void moveValueChangedSignal(double value, MoveChangedType type);

public slots:
    void moveValueChangedSlot(double value, MoveChangedType type = MoveChangedType_BoxMove);

private:
    double initValue = 0.0;
};

class CHModelMoveTransformParamsSetUI : public control::BubbleWidget
{
    Q_OBJECT

public:
    CHModelMoveTransformParamsSetUI(QWidget* parent = Q_NULLPTR);
    ~CHModelMoveTransformParamsSetUI();
    void initValues(double x, double y, double z);
    void setOnlyMoveDoubleSpinBox(double x, double y, double z);

public Q_SLOTS:
    void moveValuesChangedSlot(double value, MoveChangedType type = MoveChangedType_BoxMove);
    void receiveParams(std::vector<double> params, MoveChangedType type);
    void setLock(bool);

Q_SIGNALS:
    void sendParams(std::vector<double> params, MoveChangedType type);
    void valueChanged(double);
    void stateChanged(bool checked);
    void reset();
    void lockToPrintPlatformStatusChanged();

protected:
    void changeEvent(QEvent * event);
private:
    MoveDoubleSpinBox* m_xMoveBox;
    MoveDoubleSpinBox* m_yMoveBox;
    MoveDoubleSpinBox* m_zMoveBox;

    SwitchButton* m_lockCheck;
    QToolButton* m_resetButton;
    QLabel *m_moveLabel = nullptr;
    QLabel *m_keepLabel = nullptr;
};


#endif
