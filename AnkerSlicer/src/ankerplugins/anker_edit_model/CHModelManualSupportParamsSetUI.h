/*
*???????????????
*?????
*???2021?12?7?
*/


#ifndef CHModelManualSupportParamsSetUI_H
#define CHModelManualSupportParamsSetUI_H


#include <QDialog>
#include <QGroupBox>
#include <QScrollArea>
#include "QLabel"
#include "QLineEdit"
#include <vector>
#include "QPushButton"
#include "QCheckBox"
#include "../../common/controlInterface/BubbleWidget.h"
#include "../../common/controlInterface/line.h"
#include "CHCustomLineEdit.h"
#include <QDoubleSpinBox>

#include "edit_meshtransform_factory.h"

using namespace std;

class CHModelManualSupportParamsSetUI : public control::BubbleWidget
{
    Q_OBJECT

public:
    CHModelManualSupportParamsSetUI(QWidget* parent = Q_NULLPTR);
    ~CHModelManualSupportParamsSetUI();


    public
Q_SLOTS:
    void submit();
    void submit(double);

    void clearBtnClicked();
    void doneBtnClickde();

Q_SIGNALS:
    void profileSizeChanged(float lenth, float wide);
    void valueChanged(double);

private:
    QGroupBox* createParamsSetUI();


private:
    QDoubleSpinBox* m_lenthEdit;
    QDoubleSpinBox* m_wideEdit;

    QToolButton* clearButton;
    QToolButton* doneButton;
};
#endif
