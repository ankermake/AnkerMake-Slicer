#ifndef USERAGREEMENTWIDGET_H
#define USERAGREEMENTWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>
#include <QEvent>
#include <QLocale>
#include <QDebug>
#include "common/ak_const.h"
class UserAgreementWidget : public QDialog
{
    Q_OBJECT
public:
    explicit UserAgreementWidget(QWidget *parent = nullptr);
protected:
    void changeEvent(QEvent *e) override;

private:
    void initUi();
    bool getCurrentRegion();
signals:

private slots:
    void buttonClick();
    void declineButtonClick();
private:
    QLabel *m_label = nullptr;
    QLabel *m_privacyLabel = nullptr;
    QPushButton *m_acceptButton = nullptr;
    QPushButton *m_declineButton = nullptr;

};

#endif // USERAGREEMENTWIDGET_H
