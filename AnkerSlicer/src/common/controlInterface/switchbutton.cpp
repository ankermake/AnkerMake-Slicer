#include "switchbutton.h"

SwitchButton::SwitchButton(QWidget* parent)
    : QWidget(parent),
    m_isChecked(false),
    m_startX(1),
    m_backgroundColor(QColor(81, 81, 81))
{
    this->setMinimumSize(50, 26);
}

void SwitchButton::drawBackground(QPainter* painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_backgroundColor);
    QRect rect(0, 0, width(), height());
    int side = qMin(width(), height());


    QPainterPath path1;
    path1.addEllipse(rect.x(), rect.y(), side, side);


    QPainterPath path2;
    path2.addEllipse(rect.width() - side, rect.y(), side, side);


    QPainterPath path3;
    path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, height());

    QPainterPath path = path1 + path2 + path3;
    painter->drawPath(path);
    painter->restore();
}

void SwitchButton::drawSlider(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QColor(255, 255, 255));
    int sliderWidth = qMin(width(), height()) - 3;
    if (m_isChecked) {
        m_startX = this->width() - sliderWidth - 1;
    }
    else {
        m_startX = 1;
    }
    QRect rect(m_startX, 2, sliderWidth, sliderWidth);
    painter->drawEllipse(rect);
    painter->restore();

}

void SwitchButton::updateState(bool state)
{
    //    if(m_isChecked == state) {
    //        return;
    //    }
    m_isChecked = state;
    if (m_isChecked) {
        m_backgroundColor = QColor(97, 211, 125);
    }
    else {
        m_backgroundColor = QColor(81, 81, 81);
    }
    // this->repaint();
    update();
}

bool SwitchButton::checkState() const
{
    return m_isChecked;
}

void SwitchButton::setCheckState(bool state)
{
    updateState(state);
}

void SwitchButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    drawBackground(&painter);
    drawSlider(&painter);
    QWidget::paintEvent(event);
}

void SwitchButton::mouseReleaseEvent(QMouseEvent* event)
{
    updateState(!m_isChecked);
    emit stateChanged(m_isChecked);
}

