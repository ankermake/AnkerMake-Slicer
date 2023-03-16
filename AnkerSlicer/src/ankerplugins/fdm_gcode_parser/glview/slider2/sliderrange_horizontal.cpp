#include "sliderrange_horizontal.h"
#include <QPainter>
#include <QEvent>
#include "qmath.h"
#include <QDebug>
#include "../common/utilities/tlogger.h"

HSliderRang::HSliderRang(QWidget* parent) : QWidget(parent), minValue(0), maxValue(100), leftValue(0), rightValue(100), borderWidth(1),
horizontal(true), showText(false), usedColor(QColor(24, 189, 155)), freeColor(QColor(70, 70, 70)), textColor(QColor(80, 80, 80)),
rangeTextColor(QColor(200, 200, 200)), sliderColor(QColor(250, 250, 250)), borderColor(QColor(255, 107, 107)),
sliderStyle(SliderStyle_Circle), sliderBgPercent(SliderBgPercent_0_2), sliderPercent(SliderPercent_0_2), leftPressed(false), rightPressed(false)
{
    //AkUtil::TFunction("");
	setFont(QFont("Arial", 8));
    setFocusPolicy(Qt::ClickFocus);
}

HSliderRang::~HSliderRang()
{

}

void HSliderRang::mousePressEvent(QMouseEvent* e)
{
    //AkUtil::TFunction("");
	if (e->button() & Qt::LeftButton) {

		int x = e->pos().x();
		int y = e->pos().y();

//        QRect tmpRect = leftSliderRect;

//		bool isContains = tmpRect.contains(x, y);
//		if (isContains)
//		{
//			leftPressed = true;
//			update();
//		}
        QRect tmpRect  = rightSliderRect;
		tmpRect.setRect(rightSliderRect.x() - 5, rightSliderRect.y() - 5, rightSliderRect.width() + 10, rightSliderRect.height() + 10);
		//belowSliderRect = tmpRect;
        bool isContains = tmpRect.contains(x, y);
		if (isContains)
		{
			rightPressed = true;
			update();
		}

        pressPos = press_center;
        if(rightSliderRect.contains(x,y))
        {
            pressPos = press_right;
        }
	}
	e->accept();
}

void HSliderRang::mouseReleaseEvent(QMouseEvent* e)
{
    //AkUtil::TFunction("");
	leftPressed = false;
	rightPressed = false;
	e->accept();
	update();
    alf = 0.0;
}

#include<QtMath>
void HSliderRang::wheelEvent(QWheelEvent * event)
{
    //qDebug() <<"event->delta() "<<event->delta() ;
    if(pressPos == press_right)
    {
        if(event->delta() > 0) 
        {
            alf = alf < 0.0001 ? event->delta() / 120.0 :  qRound(qPow(1.2,qPow((event->delta() / 120.0) - 1, 1.3)) * alf);
            rightValue+=alf;
            rightValue = rightValue > maxValue ? maxValue : rightValue;
            rightValue = rightValue < minValue ? minValue : rightValue;
            emit valueChanged(leftValue, rightValue);
        }
        else 
        {
            //qDebug() <<"alf "<<alf ;
            alf = alf > -0.0001 ? event->delta() / 120.0 : qRound(qPow(1.2,qPow((-event->delta() / 120.0) - 1, 1.3)) * alf);
            rightValue+=alf;
            rightValue = rightValue > maxValue ? maxValue : rightValue;
            rightValue = rightValue < minValue ? minValue : rightValue;
            emit valueChanged(leftValue, rightValue);
        }
    }

    event->accept();
}

void HSliderRang::mouseMoveEvent(QMouseEvent* e)
{
	
 if (rightPressed) {
		int mouseX = e->pos().x();
		int rectW = rect().width();
        int width = this->width() - 16;
        double increment = (double)width / (maxValue - minValue);
        int value = (e->pos().x() - 8) / increment + minValue;

        value = value > maxValue ? maxValue : value;
        value = value < minValue ? minValue : value;

        rightValue = value;
        emit valueChanged(leftValue, rightValue);
        update();

	}
	e->accept();
}

void HSliderRang::focusInEvent( QFocusEvent * event )
{
    //qDebug() << "Focus on the  HSliderRang";
    event->accept();
}

void HSliderRang::focusOutEvent( QFocusEvent * event )
{
    //qDebug() << "Focus out  the  HSliderRang";
    pressPos = press_center;
    event->accept();
}

void HSliderRang::paintEvent(QPaintEvent*)
{
	
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	
	drawSliderBg(&painter);

	
	if (sliderStyle == SliderStyle_Line) {
		drawSliderLine(&painter);
	}
	else if (sliderStyle == SliderStyle_Circle) {
		drawSliderCircle(&painter);
	}

	
	drawValue(&painter);
}


void HSliderRang::drawSliderBg(QPainter* painter)
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
    painter->drawLine(radius + offset + 8, height / 2, width - radius - offset, height / 2);

	painter->restore();
}

void HSliderRang::drawSliderLine(QPainter* painter)
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
	pen.setWidth(sliderLen);
	pen.setColor(usedColor);
	pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);

	
	painter->drawLine(leftSliderRect.x(), sliderLen, leftSliderRect.x(), height - sliderLen);

	
	painter->drawLine(rightSliderRect.x(), sliderLen, rightSliderRect.x(), height - sliderLen);

	painter->restore();
}

void HSliderRang::drawSliderCircle(QPainter* painter)
{
	painter->save();
	painter->setPen(Qt::NoPen);

    int width = this->width() - 16;
	int height = this->height();
	sliderLen = height * ((double)sliderPercent / 10);

	
	if (sliderLen >= height / 2) {
		sliderLen = height / 2 - 1;
	}

	
	double increment = (double)width / (maxValue - minValue);

	
	int initY = (height - sliderLen * 2) / 2;
	int side = sliderLen * 2;
    QColor rightColor=QColor(208, 242, 216);
    //QColor DownColor=QColor(208, 242, 216);
    setBorderWidth(2);
    if(pressPos == press_right)
    {
        rightColor = usedColor;
        setBorderWidth(3);
    }

	int borderSide = side + borderWidth * 2;

	
	leftSliderRect = QRect(leftValue * side - sliderLen, initY, side, side);
    rightSliderRect = QRect(rightValue * increment - sliderLen + 8, initY, side, side);

	
	int penWidth = height * ((double)sliderBgPercent / 10);
	painter->setBrush(usedColor);
	painter->drawRect(leftSliderRect.x() + sliderLen, (height - penWidth) / 2, rightSliderRect.x() - leftSliderRect.x(), penWidth);

	
	//painter->setBrush(borderColor);
	//painter->drawEllipse(leftSliderRect.x() - borderWidth, leftSliderRect.y() - borderWidth, borderSide, borderSide);
	//painter->setBrush(sliderColor);
	//painter->drawEllipse(leftSliderRect);

	
    painter->setBrush(rightColor);
	painter->drawEllipse(rightSliderRect.x() - borderWidth, rightSliderRect.y() - borderWidth, borderSide, borderSide);

	painter->setBrush(sliderColor);
	painter->drawEllipse(rightSliderRect);

	painter->restore();
}


void HSliderRang::drawValue(QPainter* painter)
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

int HSliderRang::getMinValue() const
{
	return this->minValue;
}

int HSliderRang::getMaxValue() const
{
	return this->maxValue;
}

int HSliderRang::getLeftValue() const
{
	return this->leftValue;
}

int HSliderRang::getRightValue() const
{
	return this->rightValue;
}

int HSliderRang::getBorderWidth() const
{
	return this->borderWidth;
}

bool HSliderRang::getHorizontal() const
{
	return this->horizontal;
}

bool HSliderRang::getShowText() const
{
	return this->showText;
}

QColor HSliderRang::getUsedColor() const
{
	return this->usedColor;
}

QColor HSliderRang::getFreeColor() const
{
	return this->freeColor;
}

QColor HSliderRang::getTextColor() const
{
	return this->textColor;
}

QColor HSliderRang::getRangeTextColor() const
{
	return this->rangeTextColor;
}

QColor HSliderRang::getSliderColor() const
{
	return this->sliderColor;
}

QColor HSliderRang::getBorderColor() const
{
	return this->borderColor;
}

HSliderRang::SliderStyle HSliderRang::getSliderStyle() const
{
	return this->sliderStyle;
}

HSliderRang::SliderBgPercent HSliderRang::getSliderBgPercent() const
{
	return this->sliderBgPercent;
}

HSliderRang::SliderPercent HSliderRang::getSliderPercent() const
{
	return this->sliderPercent;
}

QSize HSliderRang::sizeHint() const
{
	return QSize(300, 50);
}

QSize HSliderRang::minimumSizeHint() const
{
	return QSize(10, 10);
}

void HSliderRang::setRange(int minValue, int maxValue, bool isEmit)
{
	
	if (minValue >= maxValue) {
		return;
	}

	this->minValue = minValue;
	this->maxValue = maxValue;

	
    setCurrentRange(minValue, maxValue,isEmit);


	update();
}

void HSliderRang::setMinValue(int minValue)
{
    //AkUtil::TFunction("");
	setRange(minValue, maxValue);
}

void HSliderRang::setMaxValue(int maxValue)
{
    //AkUtil::TFunction("");
	setRange(minValue, maxValue);
}

void HSliderRang::setCurrentRange(int leftValue, int rightValue, bool isEmit )
{
    //AkUtil::TFunction("");
	
	if (leftValue > rightValue) {
		return;
	}

	
	if (leftValue < minValue || rightValue > maxValue) {
		return;
	}

	this->leftValue = leftValue;
	this->rightValue = rightValue;
    if(isEmit)
    {
	emit valueChanged(leftValue, rightValue);
    }
	update();
}

void HSliderRang::setLeftValue(int leftValue)
{
    //AkUtil::TFunction("");
	setCurrentRange(leftValue, rightValue);
}

void HSliderRang::setRightValue(int rightValue)
{
    //AkUtil::TFunction("");
	setCurrentRange(leftValue, rightValue);
}

void HSliderRang::setBorderWidth(int borderWidth)
{
    //AkUtil::TFunction("");
	if (this->borderWidth != borderWidth) {
		this->borderWidth = borderWidth;
		update();
	}
}

void HSliderRang::setHorizontal(bool horizontal)
{
    //AkUtil::TFunction("");
	if (this->horizontal != horizontal) {
		this->horizontal = horizontal;
		update();
	}
}

void HSliderRang::setShowText(bool showText)
{
    //AkUtil::TFunction("");
	if (this->showText != showText) {
		this->showText = showText;
		update();
	}
}

void HSliderRang::setUsedColor(const QColor& usedColor)
{
    //AkUtil::TFunction("");
	if (this->usedColor != usedColor) {
		this->usedColor = usedColor;
		update();
	}
}

void HSliderRang::setFreeColor(const QColor& freeColor)
{
    //AkUtil::TFunction("");
	if (this->freeColor != freeColor) {
		this->freeColor = freeColor;
		update();
	}
}

void HSliderRang::setTextColor(const QColor& textColor)
{
    //AkUtil::TFunction("");
	if (this->textColor != textColor) {
		this->textColor = textColor;
		update();
	}
}

void HSliderRang::setRangeTextColor(const QColor& rangeTextColor)
{
    //AkUtil::TFunction("");
	if (this->rangeTextColor != rangeTextColor) {
		this->rangeTextColor = rangeTextColor;
		update();
	}
}

void HSliderRang::setSliderColor(const QColor& sliderColor)
{
    //AkUtil::TFunction("");
	if (this->sliderColor != sliderColor) {
		this->sliderColor = sliderColor;
		update();
	}
}

void HSliderRang::setBorderColor(const QColor& borderColor)
{
    //AkUtil::TFunction("");
	if (this->borderColor != borderColor) {
		this->borderColor = borderColor;
		update();
	}
}

void HSliderRang::setSliderStyle(const HSliderRang::SliderStyle& sliderStyle)
{
    //AkUtil::TFunction("");
	if (this->sliderStyle != sliderStyle) {
		this->sliderStyle = sliderStyle;
		update();
	}
}

void HSliderRang::setSliderBgPercent(const HSliderRang::SliderBgPercent& sliderBgPercent)
{
    //AkUtil::TFunction("");
	if (this->sliderBgPercent != sliderBgPercent) {
		this->sliderBgPercent = sliderBgPercent;
		update();
	}
}

void HSliderRang::setSliderPercent(const HSliderRang::SliderPercent& sliderPercent)
{
    //AkUtil::TFunction("");
	if (this->sliderPercent != sliderPercent) {
		this->sliderPercent = sliderPercent;
		update();
	}
}

