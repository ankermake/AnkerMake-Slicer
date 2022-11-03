#include "CHCustomLineEdit.h"
#include "QBoxLayout"
#include "QPainter"
#include "QPoint"
#include <QFormLayout>
#include <QDebug>
#include "../common/utilities/tlogger.h"


CHCustomLineEdit::CHCustomLineEdit(QString text, QWidget* parent, double radius, QColor color) : QWidget(parent)
{
    AkUtil::TFunction("");
    m_radius = radius;
    m_color = color;

    m_dSpinBox = new QDoubleSpinBox;
    m_dSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    //QString styleStr = QString("QDoubleSpinBox {\n    border: 1px solid rgb(%1, %2, %3);\t\n\tborder-radius: %4px;\n}").
    //    arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).arg(m_radius);
    QString styleStr = "QDoubleSpinBox {\n\twidth: 120px;\n\theight: 30px;\n\ttop: 403px;\n\tleft: 157px;\n    border: 1px solid rgb(87, 81, 116);\t\n\tborder-radius: 4px;\n}";
    m_dSpinBox->setStyleSheet(styleStr);
    //"QDoubleSpinBox {\n\twidth: 120px;\n\theight: 30px;\n\ttop: 403px;\n\tleft: 157px;\n    border: 1px solid rgb(87, 81, 116);\t\n\tborder-radius: 4px;\n}"
    //m_dSpinBox->setStyleSheet(("border:none"));
    connect(m_dSpinBox, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    m_unitLabel = new QLabel();
    m_unitLabel->setText(text);

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_dSpinBox);
    hlayout->addWidget(m_unitLabel);
}

CHCustomLineEdit::~CHCustomLineEdit()
{

}

QString CHCustomLineEdit::text() const
{
    return m_dSpinBox->text();
}

void CHCustomLineEdit::setText(QString string)
{
    m_dSpinBox->setSpecialValueText(string);
}

void CHCustomLineEdit::setValidator(const QValidator* vd)
{

    //m_lineEdit->setValidator(vd);
}

void CHCustomLineEdit::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    /*QPainter painter(this);
    QPen pen;
    pen.setColor(m_color);
    painter.setPen(pen);


    QRect rect = this->rect();

    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);

    painter.drawRoundedRect(rect, m_radius, m_radius);  */
}















