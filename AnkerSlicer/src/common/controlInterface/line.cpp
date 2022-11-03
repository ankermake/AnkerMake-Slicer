#include "line.h"

Line::Line(QWidget *parent, QString backgroundColor, Shape shape) : QFrame(parent)
{
    this->setFrameShape(shape);
    this->setFrameShadow(QFrame::Sunken);
    if(shape == QFrame::HLine) {
    this->setStyleSheet(QString("border:none; max-height : 1px; background-color: %1").arg(backgroundColor));
    }
    else {
        setStyleSheet(QString("border:none; max-width : 1px; background-color:  %1").arg(backgroundColor));
    }
}
