#include "radiusdiaolg.h"

RadiusDialog::RadiusDialog(QWidget *parent)
    : QDialog(parent),
      m_radius(16),
      m_color("#404249")
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
}

void RadiusDialog::setRadius(int radius)
{
    m_radius = radius;
}

void RadiusDialog::setBackgroundColor(QColor color)
{
    m_color = color;
}

void RadiusDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(m_color);
    QPainterPath path;
    path.addRoundedRect(0,0,this->width() , this->height() ,m_radius,m_radius);
    painter.drawPath(path);

    QWidget::paintEvent(event);
}
