#include "sliderlabel.h"

#include <QPainter>
#include <QEvent>
#include <QDebug>


SliderLabel::SliderLabel(QWidget *parent) : QLabel(parent)
{
    borderRadius = 5;
    background = QColor(100, 184, 255);
    foreground = QColor(255, 255, 255);

    showArrow = true;
    arrowSize = 5;
    arrowPosition = ArrowPosition_Right;

    showTriangle = false;
    triangleLen = 5;
    trianglePosition = TrianglePosition_Left;
    triangleColor = QColor(255, 255, 255);

    bgRect = this->geometry();
    setFont(QFont("Microsoft Yahei", 10));
}

void SliderLabel::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

void SliderLabel::paintEvent(QPaintEvent *)
{
    
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    
    drawBg(&painter);
    
    drawText(&painter);
    
    drawTriangle(&painter);
}

void SliderLabel::drawBg(QPainter *painter)
{
    int width = this->width();
    int height = this->height();
    int endX = width - arrowSize;
    int endY = height - arrowSize;

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(background);

    QPolygon pts;
    if (arrowPosition == ArrowPosition_Right) {
        bgRect = QRect(0, 0, endX, height);
        pts.setPoints(3, endX, height / 2 - arrowSize, endX, height / 2 + arrowSize, width, height / 2);
    } else if (arrowPosition == ArrowPosition_Left) {
        bgRect = QRect(arrowSize, 0, width - arrowSize, height);
        pts.setPoints(3, arrowSize, height / 2 - arrowSize, arrowSize, height / 2 + arrowSize, 0, height / 2);
    } else if (arrowPosition == ArrowPosition_Bottom) {
        bgRect = QRect(0, 0, width, endY);
        pts.setPoints(3, width / 2 - arrowSize, endY, width / 2 + arrowSize, endY, width / 2, height);
    } else if (arrowPosition == ArrowPosition_Top) {
        bgRect = QRect(0, arrowSize, width, height - arrowSize);
        pts.setPoints(3, width / 2 - arrowSize, arrowSize, width / 2 + arrowSize, arrowSize, width / 2, 0);
    }

    
    if (!showArrow) {
        bgRect = rect();
        painter->drawRoundedRect(bgRect, borderRadius, borderRadius);
    } else {
        painter->drawRoundedRect(bgRect, borderRadius, borderRadius);
        painter->drawPolygon(pts);
    }

    painter->restore();
}

void SliderLabel::drawText(QPainter *painter)
{
    painter->save();
    painter->setPen(foreground);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(bgRect, Qt::AlignCenter, text());
    painter->restore();
}

void SliderLabel::drawTriangle(QPainter *painter)
{
    if (!showTriangle) {
        return;
    }

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(triangleColor);

    
    int width = this->width();
    int height = this->height();
    int midWidth = width / 2;
    int midHeight = height / 2;

    QPolygon pts;
    if (trianglePosition == TrianglePosition_Left) {
        pts.setPoints(3, triangleLen, midHeight, 0, midHeight - triangleLen, 0, midHeight + triangleLen);
    } else if (trianglePosition == TrianglePosition_Right) {
        pts.setPoints(3, width - triangleLen, midHeight, width, midHeight - triangleLen, width, midHeight + triangleLen);
    } else if (trianglePosition == TrianglePosition_Top) {
        pts.setPoints(3, midWidth, triangleLen, midWidth - triangleLen, 0, midWidth + triangleLen, 0);
    } else if (trianglePosition == TrianglePosition_Bottom) {
        pts.setPoints(3, midWidth, height - triangleLen, midWidth - triangleLen, height, midWidth + triangleLen, height);
    }

    painter->drawPolygon(pts);

    painter->restore();
}

int SliderLabel::getBorderRadius() const
{
    return borderRadius;
}

QColor SliderLabel::getBackground() const
{
    return background;
}

QColor SliderLabel::getForeground() const
{
    return foreground;
}

bool SliderLabel::getShowArrow() const
{
    return showArrow;
}

int SliderLabel::getArrowSize() const
{
    return arrowSize;
}

SliderLabel::ArrowPosition SliderLabel::getArrowPosition() const
{
    return arrowPosition;
}

bool SliderLabel::getShowTriangle() const
{
    return showTriangle;
}

int SliderLabel::getTriangleLen() const
{
    return triangleLen;
}

SliderLabel::TrianglePosition SliderLabel::getTrianglePosition() const
{
    return trianglePosition;
}

QColor SliderLabel::getTriangleColor() const
{
    return triangleColor;
}

QSize SliderLabel::sizeHint() const
{
    return QSize(100, 30);
}

QSize SliderLabel::minimumSizeHint() const
{
    return QSize(20, 10);
}

void SliderLabel::setBorderRadius(int borderRadius)
{
    if (borderRadius != borderRadius) {
        borderRadius = borderRadius;
        update();
    }
}

void SliderLabel::setShowArrow(bool showArrow)
{
    if (showArrow != showArrow) {
        showArrow = showArrow;
        update();
    }
}

void SliderLabel::setArrowSize(int arrowSize)
{
    if (arrowSize != arrowSize) {
        arrowSize = arrowSize;
        update();
    }
}

void SliderLabel::setArrowPosition(const SliderLabel::ArrowPosition &arrowPosition)
{
    if (this->arrowPosition != arrowPosition) {
        this->arrowPosition = arrowPosition;
        update();
    }
}

void SliderLabel::setShowTriangle(bool showTriangle)
{
    if (showTriangle != showTriangle) {
        showTriangle = showTriangle;
        update();
    }
}

void SliderLabel::setTriangleLen(int triangleLen)
{
    if (triangleLen != triangleLen) {
        triangleLen = triangleLen;
        update();
    }
}

void SliderLabel::setTrianglePosition(const SliderLabel::TrianglePosition &trianglePosition)
{
    if (this->trianglePosition != trianglePosition) {
        this->trianglePosition = trianglePosition;
        update();
    }
}

void SliderLabel::setTriangleColor(const QColor &triangleColor)
{
    if (this->triangleColor != triangleColor) {
        this->triangleColor = triangleColor;
        update();
    }
}

void SliderLabel::setBackground(const QColor &background)
{
    if (this->background != background) {
        this->background = background;
        update();
    }
}

void SliderLabel::setForeground(const QColor &foreground)
{
    if (this->foreground != foreground) {
        this->foreground = foreground;
        update();
    }
}
