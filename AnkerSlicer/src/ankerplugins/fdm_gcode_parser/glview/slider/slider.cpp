#include "slider.h"
#include "sliderlabel.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

Slider::Slider(QWidget *parent) : QSlider(parent)
{
    borderRadius = 5;
    arrowSize = 5;
    arrowStyle = ArrowStyle_Bottom;

    background = QColor(100, 184, 255);
    foreground = QColor(255, 255, 255);

    labTipWidth = 50;
    labTipHeight = 30;
    labTipFont = font();

    showTime = false;
    clickEnable = true;
    unit = "";

    labTip = new SliderLabel;
    labTip->setBorderRadius(borderRadius);
    labTip->setArrowSize(arrowSize);
    labTip->setArrowPosition((SliderLabel::ArrowPosition)arrowStyle);
    labTip->setBackground(background);
    labTip->setForeground(foreground);
    labTip->resize(labTipWidth, labTipHeight);
    labTip->setFont(labTipFont);

    labTip->setFocusPolicy(Qt::NoFocus);
    labTip->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    labTip->setAttribute(Qt::WA_TranslucentBackground, true);
    setOrientation(Qt::Horizontal);
}

Slider::~Slider()
{
    labTip->deleteLater();
}

void Slider::mousePressEvent(QMouseEvent *e)
{
    
    if (e->button() != Qt::LeftButton) {
        return;
    }

    if (clickEnable) {
        
        double pos, value;
        if (orientation() == Qt::Horizontal) {
            pos = e->pos().x() / (double)width();
            value = pos * (maximum() - minimum()) + minimum();
        } else {
            pos = e->pos().y() / (double)height();
            value = maximum() - pos * (maximum() - minimum()) + minimum();
        }

        setValue(value + 0.5);

        
        emit clicked();
    }

    mouseMoveEvent(e);
    labTip->setVisible(true);
    QSlider::mousePressEvent(e);
}

void Slider::mouseReleaseEvent(QMouseEvent *e)
{
    labTip->setVisible(false);
    QSlider::mouseReleaseEvent(e);
}

void Slider::mouseMoveEvent(QMouseEvent *e)
{
    
    if (e->button() & Qt::RightButton) {
        return;
    }

    
    QPoint pos = e->pos();
    int x = pos.x();
    int y = pos.y();

    if (orientation() == Qt::Horizontal) {
        if (x < 0 || x > width()) {
            return;
        }

        x = QCursor::pos().x();
        y = mapToGlobal(this->pos()).y();
        x = x - labTipWidth / 2 + this->x();
        y = y - (arrowStyle == ArrowStyle_Top ? -height() : labTipHeight);
    } else {
        if (y < 0 || y > height()) {
            return;
        }

        x = mapToGlobal(this->pos()).x();
        y = QCursor::pos().y();
        x = x - (arrowStyle == ArrowStyle_Left ? -width() : labTipWidth);
        y = y - labTipHeight / 2 + this->y();
    }

    QString str = QString("%1").arg(value());

    if (showTime) {
        uint length = value() / 1000;
        str = (QString("%1:%2").arg(length / 60, 2, 10, QChar('0')).arg(length % 60, 2, 10, QChar('0')));
    }

    labTip->setText(QString("%1%2").arg(str).arg(unit));
    labTip->move(mapFromParent(QPoint(x, y)));
    QSlider::mouseMoveEvent(e);
}

int Slider::getBorderRadius() const
{
    return borderRadius;
}

int Slider::getArrowSize() const
{
    return arrowSize;
}

Slider::ArrowStyle Slider::getArrowStyle() const
{
    return arrowStyle;
}

QColor Slider::getBackground() const
{
    return background;
}

QColor Slider::getForeground() const
{
    return foreground;
}

int Slider::getLabTipWidth() const
{
    return labTipWidth;
}

int Slider::getLabTipHeight() const
{
    return labTipHeight;
}

QFont Slider::getLabTipFont() const
{
    return labTipFont;
}

bool Slider::getShowTime() const
{
    return showTime;
}

bool Slider::getClickEnable() const
{
    return clickEnable;
}

QString Slider::getUnit() const
{
    return unit;
}

void Slider::setBorderRadius(int borderRadius)
{
    if (borderRadius != borderRadius) {
        borderRadius = borderRadius;
        labTip->setBorderRadius(borderRadius);
    }
}

void Slider::setArrowSize(int arrowSize)
{
    if (arrowSize != arrowSize) {
        arrowSize = arrowSize;
        labTip->setArrowSize(arrowSize);
    }
}

void Slider::setArrowStyle(const Slider::ArrowStyle &arrowStyle)
{
    if (this->arrowStyle != arrowStyle) {
        this->arrowStyle = arrowStyle;
        labTip->setArrowPosition((SliderLabel::ArrowPosition)arrowStyle);
    }
}

void Slider::setBackground(const QColor &background)
{
    if (this->background != background) {
        this->background = background;
        labTip->setBackground(background);
    }
}

void Slider::setForeground(const QColor &foreground)
{
    if (this->foreground != foreground) {
        this->foreground = foreground;
        labTip->setForeground(foreground);
    }
}

void Slider::setLabTipWidth(int labTipWidth)
{
    if (this->labTipWidth != labTipWidth) {
        this->labTipWidth = labTipWidth;
        labTip->resize(labTipWidth, labTipHeight);
    }
}

void Slider::setLabTipHeight(int labTipHeight)
{
    if (this->labTipHeight != labTipHeight) {
        this->labTipHeight = labTipHeight;
        labTip->resize(labTipWidth, labTipHeight);
    }
}

void Slider::setLabTipFont(const QFont &labTipFont)
{
    if (this->labTipFont != labTipFont) {
        this->labTipFont = labTipFont;
        this->labTip->setFont(labTipFont);
    }
}

void Slider::setShowTime(bool showTime)
{
    if (this->showTime != showTime) {
        this->showTime = showTime;
    }
}

void Slider::setClickEnable(bool clickEnable)
{
    if (this->clickEnable != clickEnable) {
        this->clickEnable = clickEnable;
    }
}

void Slider::setUnit(const QString &unit)
{
    if (this->unit != unit) {
		this->unit = unit;
    }
}

