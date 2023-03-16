#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include "line.h"
#include "bubbleWidget.h"

class COMMONLIB_EXPORT TitleWidget : public control::BubbleWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = nullptr);
    void setTitle(const QString &title);
    void setCloseButtonIcon(QIcon icon);

signals:
    void closeCurrentWidget();

private slots:
    void closeButtonClicked();

private:
    QLabel *m_title;
    QToolButton *titleCloseButton;

};

#endif // TITLEWIDGET_H
