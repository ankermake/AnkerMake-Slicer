#include "messagetip.h"
#include <QLabel>
#include <QPushButton>

namespace control {

#define DONT_REMIND_AGAIN_TEXT tr("Don't remind again")
#define OK_TEXT tr("OK")

MessageTipWidget::MessageTipWidget(const QString&messageText, QWidget* parent)
    : BubbleWidget(parent, Qt::RightArrow)
{
    setFixedSize(230, 148);
    setAutoFillBackground(true);
    //setStyleSheet("background-color:white;");
    setBackgroundColor(QColor("#393A3D"));
    setStyleSheet("background:transparent");

    QVBoxLayout* mainblaout = new QVBoxLayout(this);

    QFont font;
    font.setPixelSize(12);

    //Label
    m_pLabel = (new QLabel(messageText));
    m_pLabel->setFont(font);
    m_pLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_pLabel->setWordWrap(true);
    mainblaout->addWidget(m_pLabel);

    //Checkout
    QHBoxLayout* hbLayout = new QHBoxLayout();
    m_pCheckBox = new QCheckBox(tr("Don't remind again"));
    m_pCheckBox->setFont(font);
    m_pCheckBox->setFixedHeight(20);
    hbLayout->addWidget(m_pCheckBox, 0, Qt::AlignCenter);
    mainblaout->addLayout(hbLayout);

    //OK
    m_pButton = new QPushButton(tr("OK"));

    QFont btnfont;
    btnfont.setPixelSize(12);
    m_pButton->setFont(btnfont);

    m_pButton->setStyleSheet("background-color:#55CD2B;");
    m_pButton->setFixedHeight(30);
    mainblaout->addWidget(m_pButton);

    connect(m_pButton, &QPushButton::pressed, this, [=]{
        m_isDontRemindAgain = m_pCheckBox->isChecked();
        emit UserPressOkButton(m_isDontRemindAgain);
        this->hide();
        m_hasPressOk = true;
    });
}
MessageTipWidget::~MessageTipWidget(){

}

void MessageTipWidget::changeEvent(QEvent * event){
    if (event->type() == QEvent::LanguageChange) {
        emit LanguageUpdate();
        m_pCheckBox->setText(tr("Don't remind again"));
        m_pButton->setText(tr("OK"));
    }
}

}
