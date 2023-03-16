#include "customtitle.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <qDebug>
#include <QElapsedTimer>
#define BUTTON_HEIGHT 30		
#define BUTTON_WIDTH 30			
#define TITLE_HEIGHT 30			

customTitle::customTitle(QWidget *parent, QMenuBar *menuBar)
    : QWidget(parent)
    , m_colorR(52)
    , m_colorG(53)
    , m_colorB(57)
    , m_isPressed(false)
    , m_buttonType(MIN_MAX_BUTTON)
    , m_windowBorderWidth(0)
    , m_isTransparent(false)
    , m_menuBar(menuBar)
{
    
    initControl();
    initConnections();
    m_desktopRect = QApplication::desktop()->availableGeometry();
    m_titleH = this->height();
    setAttribute(Qt::WA_StyledBackground);
}

customTitle::~customTitle()
{

}


void customTitle::initControl()
{
    m_pIcon = new QLabel();
    QIcon icon = QIcon(":/images/icon/logo_icon.png");
    QPixmap pic = icon.pixmap(icon.actualSize(QSize(24,24)));
    m_pIcon->setPixmap(pic);
    m_pTitleContent = new QLabel;
    m_pTitleContent->setText("AnkerMake");
    m_pTitleContent->setObjectName("m_pTitleContent");
    m_pButtonMin = new QPushButton;
    m_pButtonMin->setFocusPolicy(Qt::NoFocus);
    m_pButtonMin->setIcon(QIcon(":/images/icon/fdm_nav_min_icon.png"));
    m_pButtonRestoreMax = new QPushButton;
    m_pButtonRestoreMax->setFocusPolicy(Qt::NoFocus);
    m_pButtonRestoreMax->setIcon(QIcon(":/images/icon/fdm_nav_amp_icon.png"));
    m_pButtonClose = new QPushButton;
    m_pButtonClose->setFocusPolicy(Qt::NoFocus);
    m_pButtonClose->setIcon(QIcon(":/images/icon/fdm_nav_del_icon.png"));

    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonRestoreMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleContent->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestoreMax->setObjectName("ButtonRestore");
    m_pButtonClose->setObjectName("ButtonClose");
    QHBoxLayout* mylayout = new QHBoxLayout(this);
    mylayout->setContentsMargins(11, 0, 11, 0);
    mylayout->addWidget(m_pIcon);
    mylayout->addWidget(m_menuBar);
    mylayout->addStretch();
    mylayout->addWidget(m_pTitleContent);
    mylayout->addStretch();
    mylayout->addWidget(m_pButtonMin);
    mylayout->addWidget(m_pButtonRestoreMax);
    mylayout->addWidget(m_pButtonClose);
    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
}


void customTitle::initConnections()
{
    connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
    connect(m_pButtonRestoreMax, SIGNAL(clicked()), this, SLOT(onButtonRestoreMaxClicked()));

   // connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}



void customTitle::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}


void customTitle::getRestoreInfo(QPoint& point, QSize& size)
{
    point = m_restorePos;
    size = m_restoreSize;
}


void customTitle::mouseDoubleClickEvent(QMouseEvent *event)
{
    
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        
        
        if (m_maxScreen == false)
        {
            onButtonMaxClicked();
        }
        else
        {
            onButtonRestoreClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}


void customTitle::mousePressEvent(QMouseEvent *event)
{
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        
//        if (m_pButtonMax->isVisible())
//        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
//        }
    }
    else
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void customTitle::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed && this->parentWidget()->cursor().shape() == Qt::ArrowCursor)
    {
        if (m_maxScreen) {
            QRect desktopRect = QApplication::desktop()->availableGeometry();
            onButtonRestoreClicked();
            QElapsedTimer t;
            t.start();
            while (t.elapsed() < 10) {
                QApplication::processEvents();
            }
            auto rect = this->parentWidget()->normalGeometry();
            int posX = event->globalPos().x() - (float)rect.width() / (float)desktopRect.width() * event->globalPos().x();
            this->parentWidget()->setGeometry(posX, 0, rect.width(), rect.height());
        }
        QPoint widgetPos = this->parentWidget()->pos();
        QPoint movePoint = event->globalPos() - m_startMovePos;
        m_startMovePos = event->globalPos();
        int ax = widgetPos.x() + movePoint.x();
        int xy = widgetPos.y() + movePoint.y();
        if (widgetPos.y() < 0) {
            xy = 0;
        }
        if (widgetPos.y() > (m_desktopRect.height() - m_titleH)) {
            xy = m_desktopRect.height() - m_titleH;
        }
        this->parentWidget()->move(ax, xy);
    }
    return QWidget::mouseMoveEvent(event);
}

void customTitle::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}


void customTitle::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}

void customTitle::onButtonRestoreClicked()
{
    m_pButtonRestoreMax->setIcon(QIcon(":/images/icon/fdm_nav_amp1_icon.png"));
    emit signalButtonRestoreClicked();
    m_maxScreen = false;
}

void customTitle::onButtonRestoreMaxClicked()
{
    if (m_maxScreen) {
        onButtonRestoreClicked();
    } else {
        onButtonMaxClicked();
    }
}

void customTitle::onButtonMaxClicked()
{
    m_pButtonRestoreMax->setIcon(QIcon(":/images/icon/fdm_nav_amp_icon.png"));
    emit signalButtonMaxClicked();
    m_maxScreen = true;
}

void customTitle::onButtonCloseClicked()
{
    emit signalButtonCloseClicked();
}
