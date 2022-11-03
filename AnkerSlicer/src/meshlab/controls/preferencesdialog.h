#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSet>
#include <QListView>
#include <QGridLayout>
#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPainter>
#include <QPainterPath>
#include <QStringListModel>
#include <QAbstractItemView>
#include "common/controlInterface/pagewidget.h"
#include "common/controlInterface/bubbleWidget.h"
#include "common/controlInterface/line.h"
#include <QDebug>
namespace  control{
class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

    void addWidget(PageWidget *widget, int index = -1);
    void openWidget(int index);
    void openWidget(QWidget *widget);

signals:
    void swicthPage(int index);

private  slots:
    void save();
    void closeDialog();
    void selectCurrentIndex(const QModelIndex &index);

protected:
   void paintEvent(QPaintEvent *event) override;

private:
    QSet<PageWidget> pages;
    QGridLayout *m_mainlayout;
    QListView *m_listView;
    QStringListModel *m_listmodel;
    QWidget *m_widget = new QWidget(this);
    QTabWidget *m_tabWidget;
    QMap<int,PageWidget> m_map;
    QStringList m_displayNameList;
    QStackedLayout *m_stackLayout;
};
}
#endif // PREFERENCESDIALOG_H
