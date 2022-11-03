#include "bubbleWidget.h"


namespace  control {
BubbleWidget::BubbleWidget(QWidget* parent, Qt::ArrowType arrowType)
    : QWidget(parent,Qt::FramelessWindowHint),
      m_arrowType(arrowType),
      m_radius(5),
      m_backgroundColor(QColor(255, 255, 255)),
      m_triangleStartX(20),
      m_triangleWidth(17),
      m_triangleHeight(10)
{
    //setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setLayoutMargins(m_arrowType);
}

void BubbleWidget::setArrowType(Qt::ArrowType arrowType)
{
    m_arrowType = arrowType;
    setLayoutMargins(m_arrowType);
    update();
}

void BubbleWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(m_backgroundColor);
//   qDebug() << " m_background ==" << m_backgroundColor;
    painter.drawPath(addPathTo(m_arrowType));
    QWidget::paintEvent(event);
}

QPainterPath BubbleWidget::addPathTo(Qt::ArrowType arrowType)
{
    QPainterPath path;
     // path.addRoundedRect(0, 0, this->width(), this->height(), m_radius, m_radius);
    QPolygon polygon;
    switch (arrowType) {
    case Qt::NoArrow:
         path.addRoundedRect(0, 0, this->width(), this->height(), m_radius, m_radius);
        break;
    case Qt::UpArrow:
        path.addRoundedRect(QRect(0, m_triangleHeight , width() ,height() - m_triangleHeight), m_radius, m_radius);
        polygon << QPoint(m_triangleStartX, m_triangleHeight)
                << QPoint(m_triangleStartX + m_triangleWidth / 2, 0)
                << QPoint(m_triangleStartX + m_triangleWidth, m_triangleHeight);

        break;
    case Qt::DownArrow:
        path.addRoundedRect(QRect(0, 0 , width() ,height() - m_triangleHeight), m_radius, m_radius);
        polygon << QPoint(m_triangleStartX, this->height() - m_triangleHeight)
                << QPoint(m_triangleStartX + m_triangleWidth, this->height() - m_triangleHeight)
                << QPoint(m_triangleStartX + m_triangleWidth / 2, this->height());
        break;
    case Qt::LeftArrow:
        path.addRoundedRect(QRect(m_triangleHeight, 0 , width() - m_triangleHeight,height()), m_radius, m_radius);
        polygon << QPoint(m_triangleHeight, m_triangleStartX)
                << QPoint(0, m_triangleWidth / 2 + m_triangleStartX)
                << QPoint(m_triangleHeight, m_triangleWidth + m_triangleStartX);
        break;
    case Qt::RightArrow:
        path.addRoundedRect(QRect(0, 0 , width() - m_triangleHeight,height()), m_radius, m_radius);
        polygon << QPoint(this->width() - m_triangleHeight, m_triangleStartX)
                << QPoint(this->width(), m_triangleStartX + m_triangleHeight / 2)
                << QPoint(this->width() - m_triangleHeight, m_triangleStartX + m_triangleWidth);
        break;

    }
    path.addPolygon(polygon);
    return path;
}

void BubbleWidget::setLayoutMargins(Qt::ArrowType arrowType)
{

    switch (arrowType) {
    case Qt::NoArrow:
        this->setContentsMargins(0,0,0,0);
        break;
    case Qt::UpArrow:
        this->setContentsMargins(0,m_triangleHeight,0,0);
        break;
    case Qt::DownArrow:
        this->setContentsMargins(0,0,0,m_triangleHeight);
        break;
    case Qt::LeftArrow:
        this->setContentsMargins(m_triangleHeight,0,0,0);
        break;
    case Qt::RightArrow:
        this->setContentsMargins(0,0,m_triangleHeight,0);
    }
}


}
