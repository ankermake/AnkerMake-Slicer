#include "gcode_label_edit.h"

gcodeLabelEdit::gcodeLabelEdit(QWidget *parent)
    :BubbleWidget(parent)
{
    this->setBackgroundColor(QColor("#292A2D"));
    this->setRadius(4);
    horizontalLayout = new QGridLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    lineEdit = new QSpinBox(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    sizePolicy.setHorizontalStretch(0);
//    sizePolicy.setVerticalStretch(0);
//    sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
    lineEdit->setSizePolicy(sizePolicy);
    lineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;font-size:16px;");
    lineEdit->setFixedSize(55,20);
    lineEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    horizontalLayout->addWidget(lineEdit,0,0,Qt::AlignRight|Qt::AlignVCenter);

    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setSizePolicy(sizePolicy);
    label->setFixedSize(65,20);

    label->setStyleSheet("background-color:transparent;border-width:0;border-style:outset;color:#FFFFFF;font-size:16px;");
    label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    //validator =  new QIntValidator(1, 100, this);
    //lineEdit->setValidator(validator);
    horizontalLayout->addWidget(label,0,1,Qt::AlignLeft|Qt::AlignVCenter);
    connect(lineEdit,SIGNAL(valueChanged(int)),this,SLOT(editValueChanged(int)));
}

void gcodeLabelEdit::editValueChanged(int value)
{
//    int pos = 0;
//    int re = value.toInt();
//    QString vValue = value;
//    if(this->validator->validate(vValue,pos) !=  QValidator::Acceptable)
//    {
//        re = re >= this->validator->top() ? this->validator->top() : this->validator->bottom();
//    }
    emit valueChanged(value);
}
