/*
*???????????????
*?????
*???2021?12?7?
*/


#ifndef CHModelMirrorTransformParamsSetUI_H
#define CHModelMirrorTransformParamsSetUI_H


#include <QDialog>
#include <QGroupBox>
#include <QScrollArea>
#include <QEvent>
#include "QLabel"
#include "QLineEdit"
#include <vector>
#include "QPushButton"
#include "QCheckBox"
#include "../../common/controlInterface/BubbleWidget.h"
#include "../../common/controlInterface/line.h"
#include <QToolButton>


using namespace std;

class CHModelMirrorTransformParamsSetUI : public control::BubbleWidget
{
    Q_OBJECT

public:
    CHModelMirrorTransformParamsSetUI(QWidget* parent = Q_NULLPTR);
    ~CHModelMirrorTransformParamsSetUI();
protected:
    void changeEvent(QEvent * event);

    public
Q_SLOTS:
    void submit();
    void xBtnClicked();
    void yBtnClicked();
    void zBtnClicked();

Q_SIGNALS:
    void sendWhichButtonClicked(int buttonIndex);



public:
    QToolButton* m_resetButton;

private:
    QLabel *m_mirrorLabel = nullptr;
    QLabel *m_xStrLabel = nullptr;
    QLabel *m_yStrLabel = nullptr;
    QLabel *m_zStrLabel = nullptr;
    QToolButton* m_xButton;
    QToolButton* m_yButton;
    QToolButton* m_zButton;
};
#endif
