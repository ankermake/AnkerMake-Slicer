#include "basetabwidget.h"
#include <QHBoxLayout>
#include <QDebug>
#include "common/utilities/tlogger.h"
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

int BaseTabWidget::addTab(QWidget *page, const QIcon &iconSelect, const QIcon &iconNotSelect,const QString &label, int index)
{
    QPushButton *listItemButton = new QPushButton(this);
    //QGridLayout *listItemButtonLayout = new QGridLayout(listItemButton);
    listItemButton->setObjectName("listItemButton");
    if(index == 5)
    {
        listItemButton->setFixedSize(QSize(280,40));
    }
    else
    {
        listItemButton->setFixedSize(QSize(200,40));//200,40
    }
    listItemButton->setCheckable(true);
    listItemButton->setAutoExclusive(true);
    listItemButton->setFocusPolicy(Qt::NoFocus);
    QWidget *Widget = new QWidget(listItemButton);

    QHBoxLayout *mainLayout = new QHBoxLayout(Widget);
    //mainLayout->setSpacing(7);
   // mainLayout->setContentsMargins(0,0,0,0);
    QLabel *iconLabel = new QLabel(Widget);
    iconLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    QPixmap pix;
    if(m_buttonGroup->buttons().size() == 0)
    {
        pix = iconSelect.pixmap(iconLabel->size());
    }
    else
    {
        pix = iconNotSelect.pixmap(iconLabel->size());
    }
    iconLabel->setScaledContents(true);//true
    iconLabel->setPixmap(pix);
    mainLayout->addWidget(iconLabel);

    QLabel *itemTextLabel = new QLabel();
    itemTextLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    if(index == 5)
    {
        itemTextLabel->setFixedWidth(280);
        itemTextLabel->setOpenExternalLinks(true);
    }
    else
    {
        itemTextLabel->setFixedWidth(140);
    }
    int insertIdx = 0;
    if (index == -1) {
       if (m_topHboxLayout->isEmpty()) {
           m_titleLabels.insert(0, itemTextLabel);
       } else {
           m_titleLabels.insert(m_topHboxLayout->count() - 1, itemTextLabel);
           insertIdx = m_topHboxLayout->count() - 1;
       }
    } else {
       insertIdx = index;
       if (!m_titleLabels.contains(index)) {
           m_titleLabels.insert(index, itemTextLabel);
       } else {
           for (int i = m_titleLabels.size() - 1; i >= index; i--) {
               m_titleLabels.insert(i+1, m_titleLabels[i]);
           }
           m_titleLabels.insert(index, itemTextLabel);
       }
    }
    //add icon
    m_iconMaps[insertIdx]=QVector<QIcon>();
    m_iconMaps[insertIdx].push_back(iconSelect);
    m_iconMaps[insertIdx].push_back(iconNotSelect);
    itemTextLabel->setStyleSheet("font-size:14px");
    itemTextLabel->setText(label);
    if(index == 5)
    {
        mainLayout->addWidget(itemTextLabel, 1);
    }
    else
    {
        itemTextLabel->setObjectName("itemTextLabel");
        mainLayout->addWidget(itemTextLabel);
    }

    QFontMetrics fm(font());
    int strLength =  fm.horizontalAdvance(label);
    int merginLeft = (listItemButton->size().width() - 69 - strLength)/2;
    //mainLayout->setContentsMargins(merginLeft,10,merginLeft,0);
    //iconLabel->setStyleSheet(QString("padding-left:%1px").arg(merginLeft));

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
    } else {
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

    for (auto button: buttonList) {
    int i = m_topHboxLayout->indexOf(button);
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

void BaseTabWidget::updateTabMergein()
{
    return;
    QList<QAbstractButton*> list = m_buttonGroup->buttons();
    for(auto button : list) {
        QWidget *widget = button->findChild<QWidget *>();
        QHBoxLayout *mainLayout = widget->findChild<QHBoxLayout *>();
        QLabel *theLabel = dynamic_cast<QLabel*>(mainLayout->itemAt(1)->widget());
        int strLength = 6;
        if (nullptr != theLabel)
        {
            QFontMetrics fm(font());
            strLength =  fm.horizontalAdvance( theLabel->text());
        }

        int merginLeft = (button->size().width() - 69 - strLength)/2;
        //mainLayout->setContentsMargins(merginLeft,10,merginLeft,0);
        //theLabel->setStyleSheet(QString("padding-left:%1px").arg(merginLeft));
    }

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
        //get subobject
        QWidget *widget = button->findChild<QWidget *>();
        QHBoxLayout *mainLayout = widget->findChild<QHBoxLayout *>();
        if(id == m_currentIndex) {
            button->setChecked(true);
            button->setStyleSheet(QString::fromUtf8("background-color: #354138"));
            //iconLabel.change icon
            //mainLayout->itemAt(0)->widget()->setStyleSheet(QString::fromUtf8("color: #61D37D"));
            //QLabel *iconLabel = new QLabel(Widget);
            if (!m_iconMaps.contains(id))
            {
                continue;
            }

            QLabel *iconLabel = dynamic_cast<QLabel*>(mainLayout->itemAt(0)->widget());
            if (nullptr == iconLabel)
            {
                AkUtil::TDebug("dynamic_cast<QLabel*>(mainLayout->itemAt(0)->widget()) failed. please check.");
                continue;
            }
            //QIcon icon(":/images/icon/fdm-slice-tab-s.png");

            iconLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
            //iconLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

            QPixmap pix = m_iconMaps[id][0].pixmap(iconLabel->size());
            iconLabel->setScaledContents(false);
            iconLabel->setPixmap(pix);

            //textLable
            mainLayout->itemAt(1)->widget()->setStyleSheet(QString::fromUtf8("color: #61D37D"));
        }
        else
        {
            button->setStyleSheet(QString::fromUtf8("background-color: #343539"));
            //iconLabel
            //mainLayout->itemAt(0)->widget()->setStyleSheet(QString::fromUtf8("color: #FFFFFF"));
            QLabel *iconLabel = dynamic_cast<QLabel*>(mainLayout->itemAt(0)->widget());
            if (nullptr == iconLabel)
            {
                AkUtil::TDebug("dynamic_cast<QLabel*>(mainLayout->itemAt(0)->widget()) failed. please check.");
                continue;
            }
            //diffrent
            //QIcon icon(":/images/icon/fdm-slice-tab-n.png");
            iconLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
            //iconLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            QPixmap pix =  m_iconMaps[id][1].pixmap(iconLabel->size());
            iconLabel->setScaledContents(false);
            iconLabel->setPixmap(pix);

            //textLable
            mainLayout->itemAt(1)->widget()->setStyleSheet(QString::fromUtf8("color: #FFFFFF"));
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




