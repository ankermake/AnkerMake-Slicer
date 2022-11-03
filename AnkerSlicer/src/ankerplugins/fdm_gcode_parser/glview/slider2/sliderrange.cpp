#include "sliderrange.h"

#include <QPainter>
#include <QEvent>
#include "qmath.h"
#include <QDebug>

SliderRange::SliderRange(QWidget *parent, bool horizontalValue) : QWidget(parent),
	minValue(0), maxValue(100), leftValue(40), rightValue(60), belowValue(40), upValue(60),
    borderWidth(1), horizontal(horizontalValue), showText(false),
	usedColor(QColor(24, 189, 155)), freeColor(QColor(70, 70, 70)), 
	textColor(QColor(80, 80, 80)), rangeTextColor(QColor(200, 200, 200)),
	sliderColor(QColor(250, 250, 250)), borderColor(QColor(QColor(255, 107, 107))),
	sliderStyle(SliderStyle_Line), sliderBgPercent(SliderBgPercent_0_2), sliderPercent(SliderPercent_0_3),
	leftPressed(false), rightPressed(false), upPressed(false), belowPressed(false)
{
	setFont(QFont(tr("Arial"), 8));
}

SliderRange::~SliderRange()
{

}

void SliderRange::mousePressEvent(QMouseEvent *e)
{
    if (e->button() & Qt::LeftButton) {
        
        /*
        if (leftSliderRect.contains(e->pos())) {
            leftPressed = true;
            update();
        } else if (rightSliderRect.contains(e->pos())) {
            rightPressed = true;
            update();
        } */
        
        int x = e->pos().x();
        int y = e->pos().y();
        QRect tmpRect = upSliderRect;   
        tmpRect.setRect(upSliderRect.x()- 5, upSliderRect.y() - 5, upSliderRect.width() + 10, upSliderRect.height() + 10); 
        //upSliderRect = tmpRect;
        bool isContains = tmpRect.contains(x, y);
        if (isContains)
        {
            upPressed = true;
           // update();
        }
        tmpRect = belowSliderRect;
        tmpRect.setRect(belowSliderRect.x() - 5, belowSliderRect.y() - 5, belowSliderRect.width() + 10, belowSliderRect.height() + 10);
        //belowSliderRect = tmpRect;
        isContains = tmpRect.contains(x, y);
        if(isContains)
        {
            belowPressed = true;
            //update();
        }
    }
}

void SliderRange::mouseReleaseEvent(QMouseEvent *)
{
    //leftPressed = false;
    //rightPressed = false;
    upPressed = false;
    belowPressed = false;
    update();
}

void SliderRange::mouseMoveEvent(QMouseEvent *e)
{
	   
    if (upPressed) {
        if (e->pos().y() <= rect().height()) {
            int height = this->height();
            double incrememt = (double)height / (maxValue - minValue);
            int value = (height - e->pos().y()) / incrememt;

            if (value >= belowValue && value <= maxValue) {
                upValue = value;
                //emit valueChanged(belowValue, upValue);
                update();
            }
        }
    }
    else if (belowPressed) {
        if (e->pos().y() >= rect().y()) {
            int height = this->height();
            double increment = (double)height / (maxValue - minValue);
            int value = (height - e->pos().y()) / increment;

            if (value >= minValue && value <= upValue) {
                belowValue = value;
                //emit valueChanged(belowValue, upValue);
                update();
            }
        }
    }
    if (leftPressed) {
        
        /*if (e->pos().x() >= rect().x()) {
            int width = this->width();
            double increment = (double)width / (maxValue - minValue);
            int value = e->pos().x() / increment;

            if (value >= minValue && value <= rightValue) {
                leftValue = value;
                emit valueChanged(leftValue, rightValue);
                update();
            }

        }*/

    } else if (rightPressed) {
        
        /*if (e->pos().x() <= rect().width()) {
            int width = this->width();
            double increment = (double)width / (maxValue - minValue);
            int value = e->pos().x() / increment;

            if (value >= leftValue && value <= maxValue) {
                rightValue = value;
                emit valueChanged(leftValue, rightValue);
                update();
            }
        } */

    }
}

void SliderRange::paintEvent(QPaintEvent *)
{
	
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    SliderOrientation curOrientation = SliderOrientation::SliderOrientation_Vertical;
    if(this->horizontal)
    {
        curOrientation = SliderOrientation::SliderOrientation_Horizontal;
    }

    
    //drawSliderBg(&painter);
    drawSliderBg(&painter, curOrientation);

    
    if (sliderStyle == SliderStyle_Line) {
        
        drawSliderLine(&painter, curOrientation);
    } else if (sliderStyle == SliderStyle_Circle) {
        
        drawSliderCircle(&painter,curOrientation);
    }

    
    
    drawValue(&painter, curOrientation);
}

void SliderRange::drawSliderBg(QPainter *painter)
{
    painter->save();

    int width = this->width();
    int height = this->height();
    int penWidth = height * ((double)sliderBgPercent / 10);
    int radius = penWidth / 2;

    
    int offset = 1;

    QPen pen;
    pen.setWidth(penWidth);
    pen.setColor(freeColor);
    pen.setCapStyle(Qt::RoundCap);

    painter->setPen(pen);
    painter->drawLine(radius + offset, height / 2, width - radius - offset, height / 2);

    painter->restore();
}

void SliderRange::drawSliderBg(QPainter* painter, SliderOrientation orientation)
{
    switch (orientation)
    {
    case SliderOrientation::SliderOrientation_Horizontal:
    {
        painter->save();
        int width = this->width();
        int height = this->height();
        int penWidth = height * ((double)sliderBgPercent / 10);
        int radius = penWidth / 2;

        
        int offset = 1;
        QPen pen;
        pen.setWidth(height);
        pen.setColor(freeColor);
        pen.setCapStyle(Qt::RoundCap);

        painter->setPen(pen);
        painter->drawLine(height / 2, radius + offset, height / 2, width - (radius + offset));

        painter->restore();
        break;
    }

    case SliderOrientation::SliderOrientation_Vertical:
    {
        painter->save();
        int width = this->width();
        int height = this->height();
        int penWidth = width * ((double)sliderBgPercent / 10);
        int radius = penWidth / 2;

        
        int offset = 1;
        QPen pen;
        pen.setWidth(penWidth);
        pen.setColor(freeColor);
        pen.setCapStyle(Qt::RoundCap);

        painter->setPen(pen);
        painter->drawLine(width / 2, radius + offset, width / 2, height - (radius + offset));

        painter->restore();
        break;
    }
    default:
        break;
    }
}


void SliderRange::drawSliderLine(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    int width = this->width();
    int height = this->height();
    sliderLen = height * ((double)sliderPercent / 10);

    
    double increment = (double)width / (maxValue - minValue);
    int initY = 0;

    
    leftSliderRect = QRect(leftValue * increment, initY, sliderLen, height);
    rightSliderRect = QRect(rightValue * increment, initY, sliderLen, height);

    
    int penWidth = height * ((double)sliderBgPercent / 10);

    painter->setBrush(usedColor);
    painter->drawRect(leftSliderRect.x(), (height - penWidth) / 2, rightSliderRect.x() - leftSliderRect.x(), penWidth);

    QPen pen = QPen();
    pen.setWidth(penWidth);
    pen.setColor(usedColor);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    
    painter->drawLine(leftSliderRect.x(), sliderLen, leftSliderRect.x(), height - sliderLen);

    
    painter->drawLine(rightSliderRect.x(), sliderLen, rightSliderRect.x(), height - sliderLen);

    painter->restore();
}


void SliderRange::drawSliderLine(QPainter* painter, SliderOrientation orientation)
{
    switch (orientation)
    {
    case SliderRange::SliderOrientation_Horizontal:
    {
        painter->save();
        painter->setPen(Qt::NoPen);

        
        int width = this->width();
        int height = this->height();
        sliderLen = height * ((double)sliderPercent / 10);

        
        double increment = (double)width / (maxValue - minValue);
        int initX = 0;

        
        rightSliderRect = QRect(initX, (maxValue - upValue) * increment, height, sliderLen);
        leftSliderRect = QRect(initX, (maxValue - belowValue) * increment, height, sliderLen);

        
        int penWidth = height * ((double)sliderBgPercent / 10);

        painter->setBrush(usedColor);
        painter->drawRect((width - penWidth) / 2, rightSliderRect.y(),  leftSliderRect.y() - rightSliderRect.y(),penWidth);

        QPen pen = QPen();
        pen.setWidth(penWidth);
        pen.setColor(usedColor);
        pen.setCapStyle(Qt::RoundCap);
        painter->setPen(pen);

        
        painter->drawLine(sliderLen, upSliderRect.y(), width - sliderLen, upSliderRect.y());

        
        painter->drawLine(sliderLen, belowSliderRect.y(), width - sliderLen, belowSliderRect.y());
        painter->restore();
        break;
    }

    case SliderRange::SliderOrientation_Vertical:
    {
        painter->save();
        painter->setPen(Qt::NoPen);

        int width = this->width();
        int height = this->height();
        sliderLen = width * ((double)sliderPercent / 10);

        
        double increment = (double)height / (maxValue - minValue);
        int initX = 0;

        
        upSliderRect = QRect(initX, (maxValue - upValue) * increment, width, sliderLen);
        belowSliderRect = QRect(initX, (maxValue - belowValue) * increment, width, sliderLen);

        
        int penWidth = width * ((double)sliderBgPercent / 10);

        painter->setBrush(usedColor);
        painter->drawRect((width - penWidth) / 2, upSliderRect.y(), penWidth, belowSliderRect.y() - upSliderRect.y());

        QPen pen = QPen();
        pen.setWidth(penWidth);
        pen.setColor(usedColor);
        pen.setCapStyle(Qt::RoundCap);
        painter->setPen(pen);

        
        painter->drawLine(sliderLen, upSliderRect.y(), width - sliderLen, upSliderRect.y());

        
        painter->drawLine(sliderLen, belowSliderRect.y(), width - sliderLen, belowSliderRect.y());
        painter->restore();
        break;
    }

    default:
        break;
    }
}

void SliderRange::drawSliderCircle(QPainter* painter, SliderOrientation orientation)
{
    switch (orientation)
    {
    case SliderRange::SliderOrientation_Horizontal:
    {
        painter->save();
        painter->setPen(Qt::NoPen);

        int width = this->height();
        int height = this->width();
        sliderLen = width * ((double)sliderPercent / 10);

        
        if (sliderLen >= width / 2)
        {
            sliderLen = width / 2 - 1;
        }

        
        double increment = (double)height / (maxValue - minValue);

        
        int initX = (width - sliderLen * 2) / 2;
        int side = sliderLen * 2;
        int borderSide = side + borderWidth * 2;

        
        upSliderRect = QRect(initX, height - (upValue * increment - sliderLen), side, side);
        belowSliderRect = QRect(initX, height - (belowValue * increment - sliderLen), side, side);

        
        int penWidth = width * ((double)sliderBgPercent / 10);
        painter->setBrush(usedColor);
        painter->drawRect((width - penWidth) / 2, upSliderRect.y() + sliderLen, penWidth, belowSliderRect.y() - upSliderRect.y());

        
        painter->setBrush(borderColor);
        painter->drawEllipse(upSliderRect.x() - borderWidth, upSliderRect.y() - borderWidth, borderSide, borderSide);
        painter->setBrush(sliderColor);
        painter->drawEllipse(upSliderRect);

        
        painter->setBrush(borderColor);
        painter->drawEllipse(belowSliderRect.x() - borderWidth, belowSliderRect.y() - borderWidth, borderSide, borderSide);
        painter->setBrush(sliderColor);
        painter->drawEllipse(belowSliderRect);
        painter->restore();
    }
        break;
    case SliderRange::SliderOrientation_Vertical:
    {
        painter->save();
        painter->setPen(Qt::NoPen);

        int width = this->width();
        int height = this->height();
        sliderLen = width * ((double)sliderPercent / 10);

        
        if (sliderLen >= width / 2)
        {
            sliderLen = width / 2 - 1;
        }

        
        double increment = (double)height / (maxValue - minValue);

        
        int initX = (width - sliderLen * 2) / 2;
        int side = sliderLen * 2;
        int borderSide = side + borderWidth * 2;

        
        upSliderRect = QRect(initX, height - (upValue * increment - sliderLen), side, side);
        belowSliderRect = QRect(initX, height - (belowValue * increment - sliderLen), side, side);

        
        int penWidth = width * ((double)sliderBgPercent / 10);
        painter->setBrush(usedColor);
        painter->drawRect((width - penWidth) / 2, upSliderRect.y() + sliderLen, penWidth, belowSliderRect.y() - upSliderRect.y());

        
        painter->setBrush(borderColor);
        painter->drawEllipse(upSliderRect.x() - borderWidth, upSliderRect.y() - borderWidth, borderSide, borderSide);
        painter->setBrush(sliderColor);
        painter->drawEllipse(upSliderRect);

        
        painter->setBrush(borderColor);
        painter->drawEllipse(belowSliderRect.x() - borderWidth, belowSliderRect.y() - borderWidth, borderSide, borderSide);
        painter->setBrush(sliderColor);
        painter->drawEllipse(belowSliderRect);
        painter->restore();
    }
        break;
    default:
        break;
    }
}

void SliderRange::drawSliderCircle(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    int width = this->width();
    int height = this->height();
    sliderLen = height * ((double)sliderPercent / 10);

    
    if (sliderLen >= height / 2) {
        sliderLen = height / 2 - 1;
    }

    
    double increment = (double)width / (maxValue - minValue);

    
    int initY = (height - sliderLen * 2) / 2;
    int side = sliderLen * 2;
    int borderSide = side + borderWidth * 2;

    
    leftSliderRect = QRect(leftValue * increment - sliderLen, initY, side, side);
    rightSliderRect = QRect(rightValue * increment - sliderLen, initY, side, side);

    
    int penWidth = height * ((double)sliderBgPercent / 10);
    painter->setBrush(usedColor);
    painter->drawRect(leftSliderRect.x() + sliderLen, (height - penWidth) / 2, rightSliderRect.x() - leftSliderRect.x(), penWidth);

    
    painter->setBrush(borderColor);
    painter->drawEllipse(leftSliderRect.x() - borderWidth, leftSliderRect.y() - borderWidth, borderSide, borderSide);
    painter->setBrush(sliderColor);
    painter->drawEllipse(leftSliderRect);

    
    painter->setBrush(borderColor);
    painter->drawEllipse(rightSliderRect.x() - borderWidth, rightSliderRect.y() - borderWidth, borderSide, borderSide);
    painter->setBrush(sliderColor);
    painter->drawEllipse(rightSliderRect);

    painter->restore();
}


void SliderRange::drawValue(QPainter *painter)
{
    if (!showText) {
        return;
    }

    painter->save();

    
    QFont font;
    font.setPixelSize(leftSliderRect.width() / 1.6);
    painter->setFont(font);

    
    painter->setPen(textColor);

    
    painter->drawText(leftSliderRect, Qt::AlignCenter, QString::number(leftValue));
    
    painter->drawText(rightSliderRect, Qt::AlignCenter, QString::number(rightValue));

    painter->restore();
}

void SliderRange::drawValue(QPainter* painter, SliderOrientation orientation)
{
    switch (orientation)
    {
    case  SliderOrientation::SliderOrientation_Horizontal:
    {
        if (!showText)
        {
            return;
        }
        painter->save();

        
        QFont font;
        font.setPixelSize(belowSliderRect.height() / 1.6);
        painter->setFont(font);

        
        painter->setPen(textColor);

        
        painter->drawText(upSliderRect, Qt::AlignCenter, QString::number(upValue));
        
        painter->drawText(belowSliderRect, Qt::AlignCenter, QString::number(belowValue));
        painter->restore();
    }
        break;
    case SliderOrientation::SliderOrientation_Vertical:
    {
        if (!showText)
        {
            return;
        }
        painter->save();

        
        QFont font;
        font.setPixelSize(belowSliderRect.height() / 1.6);
        painter->setFont(font);

        
        painter->setPen(textColor);

        
        painter->drawText(upSliderRect, Qt::AlignCenter, QString::number(upValue));
        
        painter->drawText(belowSliderRect, Qt::AlignCenter, QString::number(belowValue));
        painter->restore();
    }
    default:
        break;
    }
}

int SliderRange::getMinValue() const
{
    return this->minValue;
}

int SliderRange::getMaxValue() const
{
    return this->maxValue;
}

int SliderRange::getLeftValue() const
{
    return this->leftValue;
}

int SliderRange::getRightValue() const
{
    return this->rightValue;
}

int SliderRange::getUpValue() const
{
    return this->upValue;
}

int SliderRange::getBelowValue() const
{
    return this->belowValue;
}

int SliderRange::getBorderWidth() const
{
    return this->borderWidth;
}

bool SliderRange::getHorizontal() const
{
    return this->horizontal;
}

bool SliderRange::getShowText() const
{
    return this->showText;
}

QColor SliderRange::getUsedColor() const
{
    return this->usedColor;
}

QColor SliderRange::getFreeColor() const
{
    return this->freeColor;
}

QColor SliderRange::getTextColor() const
{
    return this->textColor;
}

QColor SliderRange::getRangeTextColor() const
{
    return this->rangeTextColor;
}

QColor SliderRange::getSliderColor() const
{
    return this->sliderColor;
}

QColor SliderRange::getBorderColor() const
{
    return this->borderColor;
}

SliderRange::SliderStyle SliderRange::getSliderStyle() const
{
    return this->sliderStyle;
}

SliderRange::SliderBgPercent SliderRange::getSliderBgPercent() const
{
    return this->sliderBgPercent;
}

SliderRange::SliderPercent SliderRange::getSliderPercent() const
{
    return this->sliderPercent;
}

QSize SliderRange::sizeHint() const
{
   // return QSize(300, 50);
    return QSize(10, 600);
}

QSize SliderRange::minimumSizeHint() const
{
    return QSize(10, 10);
}

void SliderRange::setRange(int minValue, int maxValue)
{
    
    if (minValue >= maxValue) {
        return;
    }

    this->minValue = minValue;
    this->maxValue = maxValue;

    
    if (leftValue < minValue) {
        leftValue = minValue;
    }

    if (rightValue > maxValue) {
        rightValue = maxValue;
    }

    if (leftValue < minValue || rightValue > maxValue) {
        setCurrentRange(leftValue, rightValue);
    }

    update();
}

void SliderRange::setRange(int minValue, int maxValue, SliderOrientation orientation)
{
    switch (orientation)
    {
    case SliderRange::SliderOrientation_Horizontal:
        break;
    case SliderRange::SliderOrientation_Vertical:
    {
        
        if (minValue >= maxValue) {
            return;
        }
        this->minValue = minValue;
        this->maxValue = maxValue;

        
        if (belowValue < minValue)
        {
            belowValue = minValue;
        }
        if (upValue > maxValue)
        {
            upValue = maxValue;
        }

        if (belowValue < minValue || upValue > maxValue)
        {
            setCurrentRange(belowValue, upValue, SliderOrientation::SliderOrientation_Vertical);
        }
        update();
    }
        break;
    default:
        break;
    }
}

void SliderRange::setMinValue(int minValue)
{
    setRange(minValue, maxValue);
}

void SliderRange::setMaxValue(int maxValue)
{
    setRange(minValue, maxValue);
}

void SliderRange::setCurrentRange(int leftValue, int rightValue)
{
    
    if (leftValue > rightValue) {
        return;
    }

    
    if (leftValue < minValue || rightValue > maxValue) {
        return;
    }

    this->leftValue = leftValue;
    this->rightValue = rightValue;
    emit valueChanged(leftValue, rightValue);

    update();
}

void SliderRange::setCurrentRange(int belowValue, int upValue, SliderOrientation orientation)
{
    switch (orientation)
    {
    case SliderRange::SliderOrientation_Horizontal:
        break;
    case SliderRange::SliderOrientation_Vertical:
    {
        if (belowValue > upValue) {
            return;
        }

        if (belowValue < minValue || upValue > maxValue) {
            return;
        }

        this->belowValue = belowValue;
        this->upValue = upValue;
        emit valueChanged(belowValue, upValue);
    }
        break;
    default:
        break;
    } 
}

void SliderRange::setLeftValue(int leftValue)
{
    setCurrentRange(leftValue, rightValue);
}

void SliderRange::setRightValue(int rightValue)
{
    setCurrentRange(leftValue, rightValue);
}

void SliderRange::setUpValue(int upValue)
{
    setCurrentRange(belowValue, upValue, SliderOrientation::SliderOrientation_Vertical);
}

void SliderRange::setBelowValue(int belowValue)
{
    setCurrentRange(belowValue, upValue, SliderOrientation::SliderOrientation_Vertical);
}

void SliderRange::setBorderWidth(int borderWidth)
{
    if (this->borderWidth != borderWidth) {
        this->borderWidth = borderWidth;
        update();
    }
}

void SliderRange::setHorizontal(bool horizontal)
{
    if (this->horizontal != horizontal) {
        this->horizontal = horizontal;
        update();
    }
}

void SliderRange::setShowText(bool showText)
{
    if (this->showText != showText) {
        this->showText = showText;
        update();
    }
}

void SliderRange::setUsedColor(const QColor &usedColor)
{
    if (this->usedColor != usedColor) {
        this->usedColor = usedColor;
        update();
    }
}

void SliderRange::setFreeColor(const QColor &freeColor)
{
    if (this->freeColor != freeColor) {
        this->freeColor = freeColor;
        update();
    }
}

void SliderRange::setTextColor(const QColor &textColor)
{
    if (this->textColor != textColor) {
        this->textColor = textColor;
        update();
    }
}

void SliderRange::setRangeTextColor(const QColor &rangeTextColor)
{
    if (this->rangeTextColor != rangeTextColor) {
        this->rangeTextColor = rangeTextColor;
        update();
    }
}

void SliderRange::setSliderColor(const QColor &sliderColor)
{
    if (this->sliderColor != sliderColor) {
        this->sliderColor = sliderColor;
        update();
    }
}

void SliderRange::setBorderColor(const QColor &borderColor)
{
    if (this->borderColor != borderColor) {
        this->borderColor = borderColor;
        update();
    }
}

void SliderRange::setSliderStyle(const SliderRange::SliderStyle &sliderStyle)
{
    if (this->sliderStyle != sliderStyle) {
        this->sliderStyle = sliderStyle;
        update();
    }
}

void SliderRange::setSliderBgPercent(const SliderRange::SliderBgPercent &sliderBgPercent)
{
    if (this->sliderBgPercent != sliderBgPercent) {
        this->sliderBgPercent = sliderBgPercent;
        update();
    }
}

void SliderRange::setSliderPercent(const SliderRange::SliderPercent &sliderPercent)
{
    if (this->sliderPercent != sliderPercent) {
        this->sliderPercent = sliderPercent;
        update();
    }
}
