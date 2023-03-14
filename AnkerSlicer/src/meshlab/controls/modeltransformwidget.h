#ifndef MODELTRANSFORMWIDGET_H
#define MODELTRANSFORMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QAction>
#include <QStyleOption>
#include <QLabel>
//#include "glarea.h"
#include "common/controlInterface/BubbleWidget.h"
#include "common/ak_const.h"
#include "common/controlInterface/line.h"
namespace  control{
class ModelTransformWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModelTransformWidget(QWidget *parent = nullptr);
    void addWidgetToTransForm(QWidget *widget,  int actionIndex);
    void addActionToTransform(QAction *action);
    void addSeparator();

private slots:
    void deleteCurrentWidget();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QVBoxLayout *m_mainLayout;
    QMap <int ,QToolButton *> m_toolMap;
    QWidget *m_currentWidget;
    QToolButton *m_currentButton;
};

}
#endif // MODELTRANSFORMWIDGET_H
