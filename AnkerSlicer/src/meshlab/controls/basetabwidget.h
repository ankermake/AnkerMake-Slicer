#ifndef BASETABWIDGET_H
#define BASETABWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>
class BaseListWidgetItem : public QListWidgetItem
{
public:
    explicit BaseListWidgetItem(QListWidget *listview = nullptr, int type = Type)
    {
         QListWidgetItem(listview,type);
    }

    bool operator<(const QListWidgetItem &other) const
    {
        int a = -1, b =-1;
        a = this->data(Qt::UserRole + 1).toInt();
        b = other.data(Qt::UserRole + 1).toInt();
        return a < b;
    }
};

class BaseTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTabWidget(QWidget *parent = nullptr);
    Q_PROPERTY(int count READ count)
    //Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize);
   // Q_PROPERTY(TabPosition tabPosition READ tabPosition WRITE setTabPosition);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    //int addTab(QWidget *page, const QString &label);
    int addTab(QWidget *page, const QIcon &iconSelect, const QIcon &iconNotSelect, const QString &label, int index = -1);
    int count() const;
    QMap<int, QLabel *> &titleLabels() { return m_titleLabels; };
//    const QSize &iconSize() const;
//    void setIconSize(const QSize &newIconSize);

//    enum TabPosition { North, South, West, East };
//   // Q_ENUM(TabPosition)
//    const TabPosition &tabPosition() const;
//    void setTabPosition(const TabPosition &newTabPosition);
    int currentIndex() const;
    QWidget *currentWidget() const;

    void updateTabMergein();
Q_SIGNALS:
    void currentChanged(int index);

public Q_SLOTS:
    void setCurrentWidget(QWidget *widget);
    void setCurrentIndex(int newCurrentIndex);
    void slot_btnGroupClicked(int index);

private:
    //void setCurrentPage(QListWidgetItem *item);
    void setButtonId();
private:
    QGridLayout *m_mainLayout;
    QStackedLayout *m_stackLayout;

    int m_count;
//    QSize m_iconSize;
//    TabPosition m_tabPosition;
    int m_currentIndex;
    QHBoxLayout *m_topHboxLayout;
    QButtonGroup *m_buttonGroup;
    QMap<int, QLabel *> m_titleLabels;
    QMap<int, QVector<QIcon>> m_iconMaps;
};

#endif // BASETABWIDGET_H
