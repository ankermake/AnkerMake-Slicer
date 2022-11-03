#include "basetabwidget.h"

BaseTabWidget::BaseTabWidget(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(new QGridLayout(this)),
      m_baseListWidget(new QListWidget(this)),
      m_stackLayout(new QStackedLayout),
      m_count(-1),
      m_currentIndex(-1)
{
    m_mainLayout->addWidget(m_baseListWidget,0,0);
    m_mainLayout->addLayout(m_stackLayout,1,0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_baseListWidget->setFixedHeight(40);
    m_baseListWidget->setObjectName("m_baseListWidget");
    m_baseListWidget->setFlow(QListView::LeftToRight);
    //m_baseListWidget->setLayoutDirection(Qt::RightToLeft);
    connect(m_baseListWidget,&QListWidget::itemClicked,this,&BaseTabWidget::setCurrentPage);
}

//int BaseTabWidget::addTab(QWidget *page, const QString &label)
//{
//    m_baseListWidget->addItem(label);
//    int addIndex = m_stackLayout->addWidget(page);
//    if(m_currentIndex < 0) {
//        setCurrentIndex(0);
//    }
//    m_count = m_baseListWidget->count();
//    return addIndex;
//}

int BaseTabWidget::addTab(QWidget *page, const QIcon &icon, const QString &label)
{
    QListWidgetItem *item = new QListWidgetItem(m_baseListWidget);
    QWidget *Widget = new QWidget(m_baseListWidget);
    QGridLayout *mainLayout = new QGridLayout(Widget);
    mainLayout->setSpacing(7);
    mainLayout->setContentsMargins(0,0,0,0);
    QLabel *iconLabel = new QLabel(Widget);
    iconLabel->setFixedSize(24,24);
    QPixmap pix = icon.pixmap(iconLabel->size());;
    //pix.scaled(iconLabel->size(),Qt::KeepAspectRatio);
    iconLabel->setScaledContents(true);
    iconLabel->setPixmap(pix);
    mainLayout->addWidget(iconLabel,0,0,Qt::AlignVCenter|Qt::AlignRight);
    QLabel *itemTextLabel = new QLabel(Widget);
    itemTextLabel->setText(label);
    itemTextLabel->setObjectName("itemTextLabel");
    mainLayout->addWidget(itemTextLabel,0,1,Qt::AlignVCenter|Qt::AlignLeft);
    m_baseListWidget->setItemWidget(item,Widget);
    int addIndex = m_stackLayout->addWidget(page);
    if(m_currentIndex < 0) {
        setCurrentIndex(0);
    }

    m_count = m_baseListWidget->count();
    return addIndex;
}

int BaseTabWidget::count() const
{
    return m_count;
}

//const QSize &BaseTabWidget::iconSize() const
//{
//    return m_iconSize;
//}

//void BaseTabWidget::setIconSize(const QSize &newIconSize)
//{
//    m_iconSize = newIconSize;
//}

//const BaseTabWidget::TabPosition &BaseTabWidget::tabPosition() const
//{
//    return m_tabPosition;
//}

//void BaseTabWidget::setTabPosition(const TabPosition &newTabPosition)
//{
//    m_tabPosition = newTabPosition;
//}

int BaseTabWidget::currentIndex() const
{
    return m_currentIndex;
}

QWidget *BaseTabWidget::currentWidget() const
{
    if(m_currentIndex < 0 || (m_currentIndex >= m_stackLayout->count())) {
        return nullptr;
    }
    return m_stackLayout->widget(m_currentIndex);
}

void BaseTabWidget::setCurrentIndex(int newCurrentIndex)
{
    if (m_currentIndex == newCurrentIndex) {
        return;
    }
    if((m_baseListWidget->count() != m_stackLayout->count()) || (newCurrentIndex >= m_baseListWidget->count()) ) {
        return;
    }
    m_currentIndex = newCurrentIndex;
    m_baseListWidget->setCurrentRow(newCurrentIndex);
    m_stackLayout->setCurrentIndex(newCurrentIndex);
    emit currentChanged(m_currentIndex);
}

void BaseTabWidget::setCurrentWidget(QWidget *widget)
{

    if(!widget) {
        return;
    }
    int preIndex = m_stackLayout->indexOf(widget);
    qDebug () <<" preIndex ==" << preIndex << " m_baseListWidget count =" << m_baseListWidget->count();
    if( preIndex < 0) {
        return;
    }
    setCurrentIndex(preIndex);
}

void BaseTabWidget::setCurrentPage(QListWidgetItem *item)
{
    if(!item) {
        return;
    }
   int row =  m_baseListWidget->row(item);
   if(row < 0) {
       return;
   }
   setCurrentIndex(row);
}




