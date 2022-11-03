#ifndef CHModelManualTreeSupportParamsSetUI_H
#define CHModelManualTreeSupportParamsSetUI_H


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

class CHModelManualTreeSupportParamsSetUI : public control::BubbleWidget
{
    Q_OBJECT

public:
    CHModelManualTreeSupportParamsSetUI(QWidget* parent = Q_NULLPTR);
    ~CHModelManualTreeSupportParamsSetUI();


    public
Q_SLOTS:
    //void submit();
    //void submit(double);



   

Q_SIGNALS:
    //void profileSizeChanged(float lenth, float wide);
    //void valueChanged(double);
    void topShapeChanged(int index);
    void sptStrengthChanged(int index);

    void clearBtnClicked();
    void autoSupportBtnClickde();

private:
    QGroupBox* createParamsSetUI();


private:
    QSpinBox* m_SptStrengthEdit;
    //QDoubleSpinBox* m_wideEdit;

    QToolButton* clearButton;
    QToolButton* autoSupportButton;
};
#endif
