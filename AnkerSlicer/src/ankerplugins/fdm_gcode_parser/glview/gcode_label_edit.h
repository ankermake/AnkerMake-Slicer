#ifndef GCODELABELEDIT_H
#define GCODELABELEDIT_H
#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QIntValidator>
#include "common/controlInterface/bubbleWidget.h"
class gcodeLabelEdit : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit gcodeLabelEdit(QWidget *parent);
    QSpinBox  *lineEdit;
    QLabel *label;
    //QIntValidator *validator;

signals:
    void valueChanged(int);
private:
    QGridLayout *horizontalLayout;


private slots:
    void editValueChanged(int);


};

#endif // GCODELABELEDIT_H
