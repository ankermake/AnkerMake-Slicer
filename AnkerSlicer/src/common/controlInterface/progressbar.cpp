#include "progressbar.h"

ProgressBar::ProgressBar(QWidget *parent) : QWidget(parent),
    m_angle(0),
    m_value(0),
    m_maxValue(100),
    m_minValue(0),
    m_autoClosed(true),
    m_backgroundColor("#404249"),
    m_progressCheckedColor(QColor(97, 211, 125)),
    m_progressUncheckedColor("#4F5F53"),
    m_valuePointSize(32)
{
    this->setMinimumSize(150,150);
}

void ProgressBar::setValue(int value)
{
//    qDebug()<< "value" << value;
    if( value < m_maxValue && value >= m_minValue) {
        m_value = value;
        this->repaint();
       // this->update();
    }
    else if(value >= m_maxValue && m_autoClosed) {
        m_value = m_maxValue;
        this->repaint();
        emit closeProgress();
    }
}

void ProgressBar::setAutoClosed(bool closed)
{
    m_autoClosed = closed;
}

void ProgressBar::setBackgroundColor(QColor color)
{
    m_backgroundColor = color;
    this->repaint();
}

void ProgressBar::setProgressCheckedColor(QColor color)
{
    m_progressCheckedColor = color;
    this->repaint();
}

void ProgressBar::setProgressUncheckedColor(QColor color)
{
    m_progressUncheckedColor = color;
    this->repaint();
}

void ProgressBar::setValuePointSize(int pointSize)
{
    m_valuePointSize = pointSize;
    this->repaint();
}

void ProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::Qt4CompatiblePainting);

    m_angle = 360 * m_value / (m_maxValue - m_minValue);

    const int min = qMin(width(), height());
    const QRectF outRect(0, 0, min, min);
    const QRectF inRect(10, 10, min - 20, min - 20);

    painter.setPen(Qt::NoPen);

    painter.setBrush(QBrush(m_backgroundColor));
    painter.drawRect(outRect);

    painter.setBrush(m_progressUncheckedColor);
    painter.drawEllipse(outRect);

    painter.setBrush(m_progressCheckedColor);
    painter.drawPie(outRect, (90 - m_angle) * 16, m_angle * 16);

    painter.setBrush(m_backgroundColor);
    painter.drawEllipse(inRect);

    QFont font = this->font();
    font.setPixelSize(m_valuePointSize);
    painter.setFont(font);
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(inRect, Qt::AlignCenter, QString("%1%").arg(m_value));
    QWidget::paintEvent(event);
}
