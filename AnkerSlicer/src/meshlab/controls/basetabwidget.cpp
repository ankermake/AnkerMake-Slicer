#include "basetabwidget.h"

BaseTabWidget::BaseTabWidget(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(new QGridLayout(this)),
      m_stackLayout(new QStackedLayout),
      m_topHboxLayout(new QHBoxLayout()),
      m_buttonGroup(new QButtonGroup()),
      m_count(-1),
      m_currentIndex(-1)
{

    m_mainLayout->addLayout(m_topHboxLayout,0,0);
    m_mainLayout->addLayout(m_stackLayout,1,0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0,0,0,0);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_btnGroupClicked(int)));
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

int BaseTabWidget::addTab(QWidget *page, const QIcon &icon, const QString &label, int index)
{
    QPushButton *listItemButton = new QPushButton(this);
    listItemButton->setObjectName("listItemButton");
    listItemButton->setFixedSize(QSize(136,40));
    listItemButton->setCheckable(true);
    listItemButton->setAutoExclusive(true);

    QWidget *Widget = new QWidget(listItemButton);
    QGridLayout *mainLayout = new QGridLayout(Widget);
    //mainLayout->setSpacing(7);
   // mainLayout->setContentsMargins(0,0,0,0);
    QLabel *iconLabel = new QLabel(Widget);
    iconLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //iconLabel->setFixedSize(24,24);
    QPixmap pix = icon.pixmap(iconLabel->size());;
    //pix.scaled(iconLabel->size(),Qt::KeepAspectRatio);
    iconLabel->setScaledContents(true);
    iconLabel->setPixmap(pix);
    mainLayout->addWidget(iconLabel,0,0,Qt::AlignRight);
    QLabel *itemTextLabel = new QLabel(Widget);
    if (index == -1) {
           if (m_topHboxLayout->isEmpty()) {
               m_titleLabels.insert(0, itemTextLabel);
           } else {
               m_titleLabels.insert(m_topHboxLayout->count() - 1, itemTextLabel);
           }
       } else {
           if (!m_titleLabels.contains(index)) {
               m_titleLabels.insert(index, itemTextLabel);
           } else {
               for (int i = index; i < m_titleLabels.size(); i++) {
                   m_titleLabels.insert(i+1, m_titleLabels[i]);
               }
               m_titleLabels.insert(index, itemTextLabel);
           }
       }
    itemTextLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    itemTextLabel->setText(label);
    itemTextLabel->setObjectName("itemTextLabel");
    mainLayout->addWidget(itemTextLabel,0,1,Qt::AlignLeft);
    mainLayout->setColumnStretch(0,1);
    mainLayout->setColumnStretch(1,1);
    Widget->setLayout(mainLayout);
    int addIndex = -1;
    if(index != -1) {
          if(m_topHboxLayout->isEmpty()) {
              m_topHboxLayout->addWidget(listItemButton);
              m_topHboxLayout->addStretch();
              //m_buttonGroup->addButton(listItemButton,addIndex);
          }
          else {
              m_topHboxLayout->insertWidget((index >= m_topHboxLayout->count()) ? (index - 1 ): index ,listItemButton);
              //qDebug() << " lay count = " << m_topHboxLayout->count()  << " index ==" << index;
          }
          addIndex = m_stackLayout->insertWidget(index,page);
    }else {
        // m_baseListWidget->setItemWidget(item,Widget);
        if(m_topHboxLayout->isEmpty()) {
            m_topHboxLayout->addWidget(listItemButton);
            m_topHboxLayout->addStretch();
            m_buttonGroup->addButton(listItemButton,addIndex);
        }
        else {
            m_topHboxLayout->insertWidget(m_topHboxLayout->count() - 1 ,listItemButton);
        }
        addIndex = m_stackLayout->addWidget(page);
    }

    //qDebug() << " m_currentIndex ==" << addIndex;
    m_buttonGroup->addButton(listItemButton,addIndex);
    //qDebug() << "m_buttonGroup list =" << m_buttonGroup->buttons();
    QList <QAbstractButton* > buttonList = m_buttonGroup->buttons();

    for(auto button: buttonList) {
    int i =    m_topHboxLayout->indexOf(button);
    m_buttonGroup->setId(button,i);
    }
    if(m_currentIndex < 0) {
        setCurrentIndex(0);
    }

    m_count = m_buttonGroup->buttons().size();
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
    m_currentIndex = newCurrentIndex;
    QList<QAbstractButton*> list = m_buttonGroup->buttons();

    //list.at(m_currentIndex)->setChecked(true);
    for(auto button : list) {
        int id = m_buttonGroup->id(button);
        if(id == m_currentIndex) {
            button->setChecked(true);
        }
    }
    m_stackLayout->setCurrentIndex(m_currentIndex);
    emit currentChanged(m_currentIndex);
}

void BaseTabWidget::slot_btnGroupClicked(int index)
{
    qDebug() << "BaseTabWidget slot_btnGroupClicked index= " << index;
    if( index < 0) {
        return;
    }
    setCurrentIndex(index);
}

void BaseTabWidget::setButtonId()
{
    m_buttonGroup->buttons();
}

void BaseTabWidget::setCurrentWidget(QWidget *widget)
{

    if(!widget) {
        return;
    }
    int preIndex = m_stackLayout->indexOf(widget);
   // qDebug () <<" preIndex ==" << preIndex << " m_baseListWidget count =" << m_baseListWidget->count();
    if( preIndex < 0) {
        return;
    }
    setCurrentIndex(preIndex);
}

//void BaseTabWidget::setCurrentPage(QListWidgetItem *item)
//{
//    if(!item) {
//        return;
//    }
//   int row =  m_baseListWidget->row(item);
//   if(row < 0) {
//       return;
//   }
//   setCurrentIndex(row);
//}




