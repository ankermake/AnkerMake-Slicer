#include "titlewidget.h"

TitleWidget::TitleWidget(QWidget *parent) : BubbleWidget(parent)
{
    this->setRadius(16);
    QGridLayout *mainlayout = new QGridLayout(this);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setVerticalSpacing(16);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setContentsMargins(16,16,16,0);
    m_title = new QLabel(this);
    QFont font = this->font();
    font.setPixelSize(18);
    m_title->setFont(font);
    m_title->setObjectName("m_title");
    m_title->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_title->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    hbox->addStretch(2);
    hbox->addWidget(m_title);
    hbox->addStretch(2);

    /*QToolButton **/titleCloseButton = new QToolButton(this);
    titleCloseButton->setObjectName("titleCloseButton");
    titleCloseButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed));
    connect(titleCloseButton,&QToolButton::clicked,this,&TitleWidget::closeButtonClicked);
//    QIcon icon(":/img/fdm_nav_del_icon.png");
//    // QIcon icon2(":/img/dummy.png");
//    QImage image(":/img/fdm_nav_del_icon.png");
//    QImage icon2(":/img/dummy.png");
//    qDebug() << " icon ========" << icon.isNull()  << " formats ==" << image.format() << " icon2 =" << icon2.format();
//    titleCloseButton->setIcon(icon);
//    titleCloseButton->setIconSize(QSize(20,20));
    titleCloseButton->setMaximumSize(20,20);
    hbox->addWidget(titleCloseButton);
    titleCloseButton->hide();
    mainlayout->addLayout(hbox,0,0);

    Line *line = new Line(this,"#545863",QFrame::HLine);
    mainlayout->addWidget(line,1,0);
}

void TitleWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

void TitleWidget::setCloseButtonIcon(QIcon icon)
{
   // QIcon icon(":/img/fdm_nav_del_icon.png");
    // QIcon icon2(":/img/dummy.png");
   // QImage image(":/img/fdm_nav_del_icon.png");
   // QImage icon2(":/img/dummy.png");
   // qDebug() << " icon ========" << icon.isNull()  << " formats ==" << image.format() << " icon2 =" << icon2.format();
    titleCloseButton->setIcon(icon);
    titleCloseButton->setIconSize(QSize(20,20));
    titleCloseButton->show();
}

void TitleWidget::closeButtonClicked()
{
    emit closeCurrentWidget();
}
