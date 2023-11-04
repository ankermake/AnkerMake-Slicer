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
#include <QEvent>
#include <QStackedLayout>
#include <QPainter>
#include <QPainterPath>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QGraphicsDropShadowEffect>
#include "common/controlInterface/pagewidget.h"
#include "common/controlInterface/bubbleWidget.h"
#include "common/controlInterface/line.h"
#include <QtMath>
#include <QDebug>
namespace  control{
class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

    void addWidget(PageWidget *widget, int index = -1);
    QWidget* getWidget(const int index);
    void openWidget(int index);
    void openWidget(QWidget *widget);
    void selectIndex(const int index);

signals:
    void swicthPage(int index);

private  slots:
    void save();
    void closeDialog();
    void selectCurrentIndex(const QModelIndex &index);

protected:
   void paintEvent(QPaintEvent *event) override;
   void changeEvent(QEvent * event) override;
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
    QLabel *m_title = nullptr;
    QPushButton *m_closeButton = nullptr;
    QPushButton *m_saveButton = nullptr;

    int SHADOW_WIDTH = 0;
};
}
#endif // PREFERENCESDIALOG_H
