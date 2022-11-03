#include "toolBar.h"
#include "translator.h"
namespace  control{
ToolBar::ToolBar(QWidget *parent) :
    m_mainLayout(new QHBoxLayout(this)),
    //m_toolMenu(new QToolButton(this)),
    m_mainmenu(nullptr),
    m_menuMap(QMap<QString, QMenu*>()),
    m_toolMap(QMap <QAction *,QToolButton *>())
{
    init();
}


void ToolBar::addActionToToolBar(QAction *action)
{
    QToolButton  *pButton = new QToolButton(this);
    pButton->setIconSize(QSize(28, 28));
    // pButton->setMinimumHeight(50);
    pButton->setAutoRaise(true);
    pButton->setDefaultAction(action);
    pButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_mainLayout->addWidget(pButton);
    m_toolMap.insert(action,pButton);
    QFont font = this->font();
    font.setPixelSize(font.pixelSize() - 2);
    pButton->setFont(font);
    pButton->setContentsMargins(0,0,0,2);
    pButton->setStyleSheet("color:rgb(255,255,255); background:transparent");
    connect(action,&QAction::destroyed,this,&ToolBar::deleteButton);

}

void ToolBar::addAction(QAction *action, Qt::Alignment aligment)
{
    QToolButton  *pButton = new QToolButton(this);
    pButton->setIconSize(QSize(28, 28));
    // pButton->setMinimumHeight(50);
    pButton->setAutoRaise(true);
    pButton->setDefaultAction(action);
    pButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_mainLayout->addWidget(pButton,0,aligment);
    m_toolMap.insert(action,pButton);
    QFont font = this->font();
    font.setPixelSize(font.pixelSize() - 2);
    pButton->setFont(font);
    pButton->setContentsMargins(0,0,0,2);
    pButton->setStyleSheet("color:rgb(255,255,255); background:transparent");
    connect(action,&QAction::destroyed,this,&ToolBar::deleteButton);
}

void ToolBar::init()
{
    m_mainLayout->setAlignment(Qt::AlignLeft);
    m_mainLayout->setSpacing(10);
    m_mainLayout->setContentsMargins(24,4,24,4);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground);
   // QAction *action = new QAction(QIcon(":/images/icon/fdm_tool_home_icon_n.png"),tr("Device"),nullptr);
  //  addActionToToolBar(action);
  //  createMenuButton();
    this->setBackgroundColor(QColor(52, 53, 57));
    //setStyleSheet("background:transparent");
    this->setRadius(0);
}

void ToolBar::createMenuButton()
{
//    m_mainmenu = new QMenu(this);
//    m_toolMenu->setIcon(QIcon(":/images/icon/fdm_tool_menu_icon_n.png"));
//    m_toolMenu->setIconSize(QSize(28,28));
//    m_toolMenu->setAutoRaise(true);
//    m_toolMenu->setPopupMode(QToolButton::InstantPopup);
//    m_toolMenu->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    m_toolMenu->setText(tr("Menu"));
//    m_toolMenu->setMenu(m_mainmenu);
//    m_toolMenu->setArrowType(Qt::NoArrow);
//    QFont font = this->font();
//    font.setPixelSize(font.pixelSize() - 2);
//    m_toolMenu->setFont(font);
//    m_mainLayout->addWidget(m_toolMenu);

//    m_toolMenu->setStyleSheet("color:rgb(255,255,255);background:transparent");
}

QMenu *ToolBar::addMenu(ControlId  id)
{
    return m_mainmenu;
}


bool ToolBar::addActionToMenu(QAction *action, ControlId id)
{
    QAction *menu = m_actionMap.value(id.id());
    if(!menu) {
        return false;
    }
    m_mainmenu->insertMenu(action,m_mainmenu);
    return true;
}

bool ToolBar::addMenuToMenu(QMenu *menu, ControlId id)
{
    QMenu *idMenu = m_menuMap.value(id.id());
    if(!idMenu) {
        return false;
    }
    idMenu->addMenu(menu);
    return true;
}

void ToolBar::addSeparatorToMenu(ControlId id)
{
    QMenu *idMenu = m_menuMap.value(id.id());
    if(!idMenu) {
        return ;
    }
    idMenu->addSeparator();
}

void ToolBar::setAlignment(Qt::Alignment alignment)
{
    m_mainLayout->setAlignment(alignment);
}

void ToolBar::addStruct()
{
    m_mainLayout->addStretch(2);
}

void ToolBar::addMenuToToolBar(QMenu *menu, Qt::Alignment aligment)
{
    QToolButton *button = new QToolButton(this);
    //qDebug() <<" icon ====" << menu->icon();
    button->setIcon(menu->icon());
    button->setText(menu->title());
    button->setIconSize(QSize(24,24));
    button->setAutoRaise(true);
    button->setPopupMode(QToolButton::InstantPopup);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setMenu(menu);
    button->setArrowType(Qt::NoArrow);
    QFont font = this->font();
    font.setPixelSize(font.pixelSize() - 2);
    button->setFont(font);
    m_mainLayout->addWidget(button);

    button->setStyleSheet("color:rgb(255,255,255);background:transparent");
    m_mainLayout->addWidget(button,aligment);
}

void ToolBar::addSeparatorToToolBar()
{
    QLabel *label= new QLabel(this);
    label->setFixedWidth(10);
    QImage Image;
    Image.load(":/images/icon/line.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    label->setPixmap(pixmap);
    m_mainLayout->addWidget(label);
}

void ToolBar::deleteButton()
{
//    QAction *action = qobject_cast<QAction*>( sender());
//    QToolButton *button =  m_toolMap.value(action);
//    m_toolMap.remove(action);
//    delete  button;
    //    button = nullptr;
}


void ToolBar::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        for (auto i = 0; i < m_mainLayout->count(); i++) {
            auto control = dynamic_cast<QToolButton *>(m_mainLayout->itemAt(i)->widget());
            if (control != nullptr) {
                auto menu = control->menu();
                if (menu != nullptr) {
                    control->setText(menu->title());
                }
            }
        }
    }
}


}
