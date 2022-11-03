#include "sliderrange_vertical.h"
#include <QPainter>
#include <QEvent>
#include "qmath.h"
#include <QDebug>
#include "../common/utilities/tlogger.h"

VSliderRange::VSliderRange(QWidget* parent) : QWidget(parent), minValue(0), maxValue(1), belowValue(0), upValue(100),
borderWidth(1), horizontal(true), showText(false), usedColor(QColor(24, 189, 155)), freeColor(QColor(70, 70, 70)),
textColor(QColor(80, 80, 80)), rangeTextColor(QColor(200, 200, 200)), sliderColor(QColor(250, 250, 250)), borderColor(QColor(QColor(255, 107, 107))),
sliderStyle(SliderStyle_Circle), sliderBgPercent(SliderBgPercent_0_2), sliderPercent(SliderPercent_0_2), upPressed(false), belowPressed(false)
{
    //AkUtil::TFunction("");
	setFont(QFont(tr("Arial"), 8));
    setFocusPolicy(Qt::ClickFocus);
}

VSliderRange::~VSliderRange()
{

}

void VSliderRange::mousePressEvent(QMouseEvent* e)
{
    //AkUtil::TFunction("");
	if (e->button() & Qt::LeftButton)
    {   setFocus();
		int x = e->pos().x();
		int y = e->pos().y();
		QRect tmpRect = upSliderRect;
		tmpRect.setRect(upSliderRect.x() - 5, upSliderRect.y() - 5, upSliderRect.width() + 10, upSliderRect.height() + 10); 
		bool isContains = tmpRect.contains(x, y);
		if (isContains)
		{
			upPressed = true;
		}

		tmpRect = belowSliderRect;
		tmpRect.setRect(belowSliderRect.x() - 5, belowSliderRect.y() - 5, belowSliderRect.width() + 10, belowSliderRect.height() + 10);
		isContains = tmpRect.contains(x, y);
		if (isContains)
		{
			belowPressed = true;
		}

        pressPos = press_center;
        if(upSliderRect.contains(x,y))
        {
            pressPos = press_up;
        }

        if(belowSliderRect.contains(x,y))
        {
            pressPos = press_down;
        }

        qDebug() << "pressPos" << pressPos;
	}
    update();
	e->accept();
}

void VSliderRange::focusInEvent( QFocusEvent * event )
{
    //qDebug() << "Focus on the  VSliderRange";
    event->accept();
}

void VSliderRange::focusOutEvent( QFocusEvent * event )
{
    //qDebug() << "Focus out  the  VSliderRange";
    pressPos = press_center;
    event->accept();
}

void VSliderRange::wheelEvent(QWheelEvent * event)
{
    //qDebug() <<"event->delta() "<<event->delta() ;
    if(pressPos == press_up)
    {
        if(event->delta() > 0) 
        { 
            double alf_d = qPow(1.2,qPow((event->delta() / 120.0) - 1, 1.3)) ;
            alf = alf_d > 1.0 ? alf : 1.0;
            alf = alf < 0.0001 ? event->delta() / 120.0 :  qRound(alf_d * alf);

            upValue+=alf;
            upValue = upValue > maxValue ? maxValue : upValue;
            upValue = upValue < minValue ? minValue : upValue;
            belowValue = belowValue > upValue ? upValue : belowValue;
            emit valueChanged(belowValue, upValue);
        }
        else 
        {
            //alf = alf > -0.0001 ? event->delta() / 120.0 : qRound(qPow(1.2,qPow((-event->delta() / 120.0) - 1, 1.3)) * alf);
            double alf_d = qPow(1.2,qPow((-event->delta() / 120.0) - 1, 1.3)) ;
            alf = alf_d > 1.0 ? alf : 1.0;
            alf = alf > -0.0001 ? event->delta() / 120.0 :  qRound(alf_d * alf);
            upValue+=alf;
            upValue = upValue > maxValue ? maxValue : upValue;
            upValue = upValue < minValue ? minValue : upValue;
            belowValue = belowValue > upValue ? upValue : belowValue;
            emit valueChanged(belowValue, upValue);
        }
    }
    if(pressPos == press_down)
    {
        if(event->delta() > 0) 
        {
            belowValue++;
            belowValue = belowValue > maxValue ? maxValue : belowValue;
            belowValue = belowValue < minValue ? minValue : belowValue;
            upValue = upValue < belowValue ? belowValue : upValue;
            emit valueChanged(belowValue, upValue);
        }
        else 
        {
            belowValue--;
            belowValue = belowValue > maxValue ? maxValue : belowValue;
            belowValue = belowValue < minValue ? minValue : belowValue;
            upValue = upValue < belowValue ? belowValue : upValue;
            emit valueChanged(belowValue, upValue);
        }
    }
    event->accept();
}

void  VSliderRange::mouseReleaseEvent(QMouseEvent* e)
{
    //AkUtil::TFunction("");
	upPressed = false;
	belowPressed = false;
	e->accept();
	update();
    alf = 0.0;
}

void VSliderRange::mouseMoveEvent(QMouseEvent* e)
{

    if (upPressed) {
            int height = this->height() - 22 - 12;
			double incrememt = (double)height / (maxValue - minValue);

            int value = maxValue - ( 1.0 / incrememt) * ( e->pos().y() - 8);   //(height - e->pos().y()) / incrememt;//8 + sliderLen / 2

            value = value > maxValue ? maxValue : value;
            value = value < minValue ? minValue : value;

            upValue = value;
            belowValue = belowValue > upValue ? upValue : belowValue;
            emit valueChanged(belowValue, upValue);
            update();

	}
	else if (belowPressed) {
            int height = this->height() - 22 - 12;
            double increment = (double)(height) / (maxValue - minValue); //24
            int value = minValue + (this->height() - 14 - e->pos().y()) / increment ;//(height - e->pos().y()) / increment + 10 + 16;
            value = value > maxValue ? maxValue : value;
            value = value < minValue ? minValue : value;
             belowValue = value;
             upValue = upValue < belowValue ? belowValue : upValue;
             emit valueChanged(belowValue, upValue);
             update();
	}
	e->accept();
}

void VSliderRange::paintEvent(QPaintEvent*)
{
	
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	
	drawSliderBg(&painter);

	
	if (sliderStyle == SliderStyle_Line)
	{
		drawSliderLine(&painter);
	}
	else if (sliderStyle == SliderStyle_Circle)
	{
		drawSliderCircle(&painter);
	}

	
	drawValue(&painter);
}

void  VSliderRange::drawSliderBg(QPainter* painter)
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
    painter->drawLine(width / 2, radius + offset+8, width / 2, height - (radius + offset));

	painter->restore();
}

void  VSliderRange::drawSliderLine(QPainter* painter)
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
}

void  VSliderRange::drawSliderCircle(QPainter* painter)
{
	painter->save();
	painter->setPen(Qt::NoPen);

	int width = this->width();
    int height = this->height() - 22 - 12; 
	sliderLen = width * ((double)sliderPercent / 10);

	
	if (sliderLen >= width / 2)
	{
		sliderLen = width / 2 - 1;
	}

	
	double increment = (double)height / (maxValue - minValue);

	
	int initX = (width - sliderLen * 2) / 2;
	int side = sliderLen * 2;
    QColor upColor=QColor(208, 242, 216);
    QColor DownColor=QColor(208, 242, 216);
    setBorderWidth(2);
    if(pressPos == press_up)
    {
        upColor = usedColor;
        setBorderWidth(3);
    }
    if(pressPos == press_down)
    {
        DownColor = usedColor;
        setBorderWidth(3);
    }
	int borderSide = side + borderWidth * 2;

	
	//upSliderRect = QRect(initX, height - (upValue * increment - sliderLen), side, side);
	//belowSliderRect = QRect(initX, height - (belowValue * increment - sliderLen), side, side);
    upSliderRect = QRect(initX, (maxValue - upValue) * increment + 8 , side, side);
    belowSliderRect = QRect(initX, this->height() - (belowValue - minValue) * increment - 7 - 7 , side, side);

	
	int penWidth = width * ((double)sliderBgPercent / 10);
	painter->setBrush(usedColor);
	painter->drawRect((width - penWidth) / 2, upSliderRect.y() + sliderLen, penWidth, belowSliderRect.y() - upSliderRect.y());

	
    painter->setBrush(upColor);
    painter->drawEllipse(upSliderRect.x() - borderWidth, upSliderRect.y() - borderWidth, borderSide, borderSide);
    painter->setBrush(sliderColor);
    painter->drawEllipse(upSliderRect);

	
    painter->setBrush(DownColor);
	painter->drawEllipse(belowSliderRect.x() - borderWidth, belowSliderRect.y() - borderWidth, borderSide, borderSide);
	painter->setBrush(sliderColor);
	painter->drawEllipse(belowSliderRect);
	painter->restore();
}

void VSliderRange::drawValue(QPainter* painter)
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

int VSliderRange::getMinValue() const
{
	return this->minValue;
}

int VSliderRange::getMaxValue() const
{
	return this->maxValue;
}


int VSliderRange::getUpValue() const
{
	return this->upValue;
}

int VSliderRange::getBelowValue() const
{
	return this->belowValue;
}

int VSliderRange::getBorderWidth() const
{
	return this->borderWidth;
}

bool VSliderRange::getHorizontal() const
{
	return this->horizontal;
}

bool VSliderRange::getShowText() const
{
	return this->showText;
}

QColor VSliderRange::getUsedColor() const
{
	return this->usedColor;
}

QColor VSliderRange::getFreeColor() const
{
	return this->freeColor;
}

QColor VSliderRange::getTextColor() const
{
	return this->textColor;
}

QColor VSliderRange::getRangeTextColor() const
{
	return this->rangeTextColor;
}

QColor VSliderRange::getSliderColor() const
{
	return this->sliderColor;
}

QColor VSliderRange::getBorderColor() const
{
	return this->borderColor;
}

VSliderRange::SliderStyle VSliderRange::getSliderStyle() const
{
	return this->sliderStyle;
}

VSliderRange::SliderBgPercent VSliderRange::getSliderBgPercent() const
{
	return this->sliderBgPercent;
}

VSliderRange::SliderPercent VSliderRange::getSliderPercent() const
{
	return this->sliderPercent;
}

QSize VSliderRange::sizeHint() const
{
	// return QSize(300, 50);
	return QSize(10, 600);
}

QSize VSliderRange::minimumSizeHint() const
{
	return QSize(10, 10);
}

void VSliderRange::setRange(int minValue, int maxValue)
{
	AkUtil::TFunction("");
	
	if (minValue >= maxValue) {
		return;
	}
	this->minValue = minValue;
	this->maxValue = maxValue;

	
	setCurrentRange(minValue, maxValue);
	update();
}

void VSliderRange::setMinValue(int minValue)
{
    //AkUtil::TFunction("");
	setRange(minValue, maxValue);
}

void VSliderRange::setMaxValue(int maxValue)
{
    //AkUtil::TFunction("");
	setRange(minValue, maxValue);
}

void VSliderRange::setCurrentRange(int belowValue, int upValue)
{
    //AkUtil::TFunction("");
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

void VSliderRange::setUpValue(int upValue)
{
    //AkUtil::TFunction("");
	setCurrentRange(belowValue, upValue);
}

void VSliderRange::setBelowValue(int belowValue)
{
    //AkUtil::TFunction("");
	setCurrentRange(belowValue, upValue);
}

void VSliderRange::setBorderWidth(int borderWidth)
{
    //AkUtil::TFunction("");
	if (this->borderWidth != borderWidth) {
		this->borderWidth = borderWidth;
		update();
	}
}

void VSliderRange::setHorizontal(bool horizontal)
{
    //AkUtil::TFunction("");
	if (this->horizontal != horizontal) {
		this->horizontal = horizontal;
		update();
	}
}

void VSliderRange::setShowText(bool showText)
{
    //AkUtil::TFunction("");
	if (this->showText != showText) {
		this->showText = showText;
		update();
	}
}

void VSliderRange::setUsedColor(const QColor& usedColor)
{
    //AkUtil::TFunction("");
	if (this->usedColor != usedColor) {
		this->usedColor = usedColor;
		update();
	}
}

void VSliderRange::setFreeColor(const QColor& freeColor)
{
    //AkUtil::TFunction("");
	if (this->freeColor != freeColor) {
		this->freeColor = freeColor;
		update();
	}
}

void VSliderRange::setTextColor(const QColor& textColor)
{
    //AkUtil::TFunction("");
	if (this->textColor != textColor) {
		this->textColor = textColor;
		update();
	}
}

void VSliderRange::setRangeTextColor(const QColor& rangeTextColor)
{
    //AkUtil::TFunction("");
	if (this->rangeTextColor != rangeTextColor) {
		this->rangeTextColor = rangeTextColor;
		update();
	}
}

void VSliderRange::setSliderColor(const QColor& sliderColor)
{
    //AkUtil::TFunction("");
	if (this->sliderColor != sliderColor) {
		this->sliderColor = sliderColor;
		update();
	}
}

void VSliderRange::setBorderColor(const QColor& borderColor)
{
    //AkUtil::TFunction("");
	if (this->borderColor != borderColor) {
		this->borderColor = borderColor;
		update();
	}
}

void VSliderRange::setSliderStyle(const VSliderRange::SliderStyle& sliderStyle)
{
    //AkUtil::TFunction("");
	if (this->sliderStyle != sliderStyle) {
		this->sliderStyle = sliderStyle;
		update();
	}
}

void VSliderRange::setSliderBgPercent(const VSliderRange::SliderBgPercent& sliderBgPercent)
{
    //AkUtil::TFunction("");
	if (this->sliderBgPercent != sliderBgPercent) {
		this->sliderBgPercent = sliderBgPercent;
		update();
	}
}

void VSliderRange::setSliderPercent(const VSliderRange::SliderPercent& sliderPercent)
{
    //AkUtil::TFunction("");
	if (this->sliderPercent != sliderPercent) {
		this->sliderPercent = sliderPercent;
		update();
	}
}

void VSliderRange::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        setFont(QFont(tr("Arial"), 8));
    }
}
