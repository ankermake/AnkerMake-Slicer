#include "tooltip.h"

ToolTip::ToolTip(QWidget *parent) : BubbleWidget(parent)
{
//    this->setMinimumSize(266,246);
//    this->setMaximumSize(266,246);
    init();
    this->setFixedWidth(266);
    setAttribute(Qt::WA_ShowWithoutActivating,true);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Tool);
}

void ToolTip::setDescription(const QString &title, const QString &text,  const QString &affects, const QString &affectedBy)
{
    m_title->setText(title);
    m_despction->setText(text);

    QString affect;
    if(!affects.isEmpty()) {
        affect.append(tr("Affects ") +"<br>" + QString("<font color = #999999 >%1</font>").arg(affects));
    }

    if(!affectedBy.isEmpty()) {
        affect.append( QString("<br>") + tr("Affected By") + "<br>"  + QString("<font color = #999999>%1</font>").arg(affectedBy));
    }
   // qDebug() << " affect ==" << affect;
    if(!affect.isEmpty()) {
        m_affect->setText(affect);
    }
}

void ToolTip::setPoint(QPoint point, int maxY)
{
    m_point = point;
    m_maxY = maxY;
    int x = m_point.x()- this->width();
    int y = ( m_point.y() + this->height()) > m_maxY ? (m_maxY - this->height()) : m_point.y();
    if(y  <10) {
        y = 10;
    }
    this->move(x - 30, y - 1);
}

void ToolTip::init()
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(6);
    mainlayout->setContentsMargins(12,12,12,12);
    m_title = new QLabel(this);
    m_title->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum));
    m_title->setMaximumHeight(30);
    Line *line = new Line(this,QString("#565656"),QFrame::HLine);

    m_despction = new QLabel(this);
    m_despction->setObjectName("m_despction");
    m_despction->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding));
    m_despction->setWordWrap(true);
    mainlayout->addWidget(m_title);
    mainlayout->addWidget(line);
    mainlayout->addWidget(m_despction);

    m_affect = new QLabel(this);
    m_affect->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding));
    //m_affect->adjustSize();
    m_affect->setWordWrap(true);

    mainlayout->addWidget(m_affect);
    this->setLayout(mainlayout);
    this->setBackgroundColor(QColor(57,58,61));
    this->setArrowType(Qt::RightArrow);
    this->setTriangleStartX(10);
    this->setTriangleHeight(10);
    this->setTriangleWidth(20);
    this->setRadius(8);
}

void ToolTip::resizeEvent(QResizeEvent *event)
{
    int x = m_point.x()- this->width();
    int y = ( m_point.y() + this->height()) > m_maxY ? (m_maxY - this->height()) : m_point.y();
    if(y  <10) {
        y = 10;
    }
    this->move(x - 30, y - 1);
    //this->setMaximumHeight(700);
  // qDebug() << " resizeEvent == " << this->geometry()  << "m_title->height() =  "<< m_title->height() << "m_despction->height() = " << m_despction->height()  << "m_affect->height() =" <<m_affect->height() ;

}
