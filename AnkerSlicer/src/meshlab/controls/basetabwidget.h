#ifndef BASETABWIDGET_H
#define BASETABWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>

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
    int addTab(QWidget *page, const QIcon &icon, const QString &label);
    int count() const;

//    const QSize &iconSize() const;
//    void setIconSize(const QSize &newIconSize);

//    enum TabPosition { North, South, West, East };
//   // Q_ENUM(TabPosition)
//    const TabPosition &tabPosition() const;
//    void setTabPosition(const TabPosition &newTabPosition);
    int currentIndex() const;
    QWidget *currentWidget() const;

Q_SIGNALS:
    void currentChanged(int index);

public Q_SLOTS:
    void setCurrentWidget(QWidget *widget);
    void setCurrentIndex(int newCurrentIndex);

private:
    void setCurrentPage(QListWidgetItem *item);
private:
    QGridLayout *m_mainLayout;
    QListWidget *m_baseListWidget;
    QStackedLayout *m_stackLayout;

    int m_count;
//    QSize m_iconSize;
//    TabPosition m_tabPosition;
    int m_currentIndex;
};

#endif // BASETABWIDGET_H
