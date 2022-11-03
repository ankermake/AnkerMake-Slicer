#ifndef COPYRIGHTWIDGET_H
#define COPYRIGHTWIDGET_H

#include <QDialog>
//#include "common/controlInterface/radiusdiaolg.h"
#include "common/controlInterface/titlewidget.h"
#include <QLabel>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>

struct CopyRightInforMation {
    QString name = QString();
    QString nameLink = QString();
    QString licenseType = QString();
    QString licenseLink = QString();
    bool isModified = false;
};

class CopyRightWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CopyRightWidget(QWidget *parent = nullptr);

protected:
   void paintEvent(QPaintEvent *event) override;
signals:

private:
    void initUi();
    void initTableWidget(QList<CopyRightInforMation *> dataList);
    QList<CopyRightInforMation *> initData();

private:
    QList<CopyRightInforMation *> m_dataList;

    QTableWidget *m_tableWidget ;

};

#endif // COPYRIGHTWIDGET_H
