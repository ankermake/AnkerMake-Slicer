#include "modeltransformwidget.h"

namespace  control{
ModelTransformWidget::ModelTransformWidget(QWidget *parent)
    : QWidget(parent),
      m_mainLayout (new QVBoxLayout(this)),
      m_toolMap(QMap <int ,QToolButton *>()),
      m_currentWidget(nullptr),
      m_currentButton(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint);
    // setAttribute(Qt::WA_NoSystemBackground);
    //setStyleSheet("background-color:transparent");
    m_mainLayout->setAlignment(Qt::AlignTop);
    m_mainLayout->setContentsMargins(4,12,4,12);
    m_mainLayout->setSpacing(0);
    this->setFixedSize(QSize(68,349));//409
}

void ModelTransformWidget::addWidgetToTransForm(QWidget *widget, int actionIndex)
{
    this->update();
    connect(widget,&QWidget::destroyed,this,&ModelTransformWidget::deleteCurrentWidget,Qt::UniqueConnection);
    m_currentButton = m_toolMap.value(actionIndex);
    if(!m_currentButton) {
        return;
    }
    m_currentWidget = widget;
}

void ModelTransformWidget:: addActionToTransform(QAction *action)
{
    int i = -1;
    QToolButton  *pButton = new QToolButton(this);
    //qDebug() <<" objectname =="  <<  action->objectName() <<" text == " << action->text();
    QString text = action->objectName();
    if(text == "Move") {
        i = AkConst::FDMMeshTransForm::Move;
    }
    else if (text == "Zoom") {
        i = AkConst::FDMMeshTransForm::Zoom;
    }
    else if (text == "Rotate") {
        i = AkConst::FDMMeshTransForm::Rotate;
    }
    else if (text == "Mirror") {
        i = AkConst::FDMMeshTransForm::Mirror;
    }
    else if (text == "Support") {
        i = AkConst::FDMMeshTransForm::Support;
    }else {
        i = -1;
    }
    if(i >= 0)
        m_toolMap.insert(i,pButton);
    // qDebug() << "pButton->iconSize() === " << pButton->iconSize();
    pButton->setFixedSize(QSize(60,60));
    pButton->setIconSize(QSize(60, 60));
    pButton->setAutoRaise(true);
    pButton->setDefaultAction(action);
    pButton->setProperty("index",i);
    pButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
   // pButton->setStyleSheet("background-color:transparent");
    m_mainLayout->addWidget(pButton);
}

void ModelTransformWidget::addSeparator()
{
    QLabel *label= new QLabel(this);
    QImage Image;
    Image.load(":/images/icon/horizontalLine.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    label->setPixmap(pixmap);
    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(60,24);
    m_mainLayout->addWidget(label);
}


void ModelTransformWidget::deleteCurrentWidget()
{
    QWidget *widget = qobject_cast<QWidget *>(sender());
    if(m_currentWidget == widget) {
        m_currentWidget = nullptr;
    }
}

void ModelTransformWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor("#292A2D"));
    // qDebug() << " m_background ==" << painter.background();
    painter.drawRoundedRect(0, 0, this->width(), this->height(), 5, 5);

    if(m_currentWidget && m_currentButton) {
        // qDebug() << " paint event ........" << m_currentButton->property("index");
        int height =  this->y() + m_currentButton->y() + m_currentWidget->height() ;
        int y =  height  < this->parentWidget()->height() ? (this->y() + m_currentButton->y()) :  (this->parentWidget()->height() - m_currentWidget->height() - 10) ;

        //qDebug() << " x == " << (this->x()+ this->width() + 10)  << "y = " << y  ;
        m_currentWidget->setGeometry((this->x()+ this->width() + 10),y ,200,200);
    }

    QWidget::paintEvent(event);

}

}
