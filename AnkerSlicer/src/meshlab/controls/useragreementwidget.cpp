#include "useragreementwidget.h"

UserAgreementWidget::UserAgreementWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
}

void UserAgreementWidget::initUi()
{
    setWindowTitle(tr("User Agreement"));

    QFrame *frame = new QFrame(this);
    frame->setStyleSheet("background-color:#151619;border:none;border-radius:0px;");
    frame->setObjectName("frame");
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0,0,0,0);
    hbox->addWidget(frame);
    m_label = new QLabel(tr("Welcome to AnkerMake"),frame);
    QFont font(this->font());
    font.setPixelSize(30);
    m_label->setFont(font);
    m_label->setGeometry(169,62,380,35);
  //  QLabel *privacyLabel = new QLabel(frame);
    m_privacyLabel = new QLabel(tr("<body>Please click to read the <a href=\"https://public-make-moat-us.s3.us-east-2.amazonaws.com/overall/AnkerMake-terms-of-service.en.html\"><font color = #61D37D><text-decoration: none>Terms of Use</a> and <a href=\"https://public-make-moat-us.s3.us-east-2.amazonaws.com/overall/AnkerMake-privacy.en.html\"><font color = #61D37D><>Privacy Policy</a> </body>"),frame);
    m_privacyLabel->setOpenExternalLinks(true);
    //privacyLabel->setText(QString::fromLocal8Bit("Please click to read the <a href= https://ankermake.com/terms-of-use>Terms of Use</a> and <a href=\"https://ankermake.com/privacy-policy\"><font color = #61D37D>Privacy Policy</a>"));
    //privacyLabel->setText("<body>Please click to read the <a href=\"https://ankermake.com/account/login\"><font color = #61D37D><>Terms of Use</a></body>");
    QFont fontPrivacy(this->font());
    fontPrivacy.setPixelSize(16);
    m_privacyLabel->setFont(fontPrivacy);
    m_privacyLabel->setGeometry(134,117,440,19);
//Please click to read the Terms of Use and Privacy Policy
//    <body>No Acount <a href=\"https://ankermake.com/account/login\"><font color = #61D37D><>Create one</a></body>
    m_acceptButton = new QPushButton(tr("Accept"),frame);
    m_acceptButton->setObjectName("m_acceptButton");
    m_acceptButton->setStyleSheet("border: 0px; border-radius:4;background-color:#61D37D;color:#FFFFFF;font-size:16px");
    connect(m_acceptButton,&QPushButton::clicked,this,&UserAgreementWidget::buttonClick);
    m_acceptButton->setGeometry(219,219,219,32);

    m_declineButton = new QPushButton(tr("Decline"),frame);
    m_declineButton->setObjectName("declineButton");
    m_declineButton->setStyleSheet("border: 0px; border-radius:4;background-color:rgba(255, 255, 255, 20%);color:#FFFFFF;font-size:16px");
    connect(m_declineButton,&QPushButton::clicked,this,&UserAgreementWidget::declineButtonClick);
    m_declineButton->setGeometry(219,271,219,32);

   //setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //frame->setStyleSheet("background-color:#151619;border-radius:10px;border:none");
    //this->setStyleSheet("background-color:#151619;border-radius:10px");
}

void UserAgreementWidget::buttonClick()
{
    done(QDialog::Accepted);
}

void UserAgreementWidget::declineButtonClick()
{
    done(QDialog::Rejected);
}

void UserAgreementWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("User Agreement"));
        if (m_label != nullptr) {
            m_label->setText(tr("Welcome to AnkerMake"));
        }
        if (m_privacyLabel != nullptr) {
            m_privacyLabel->setText(tr("<body>Please click to read the <a href=\"https://public-make-moat-us.s3.us-east-2.amazonaws.com/overall/AnkerMake-terms-of-service.en.html\"><font color = #61D37D><text-decoration: none>Terms of Use</a> and <a href=\"https://public-make-moat-us.s3.us-east-2.amazonaws.com/overall/AnkerMake-privacy.en.html\"><font color = #61D37D><>Privacy Policy</a> </body>"));
        }
        if (m_acceptButton != nullptr) {
            m_acceptButton->setText(tr("Accept"));
        }
        if (m_declineButton != nullptr) {
            m_declineButton->setText(tr("Decline"));
        }
    }
}
