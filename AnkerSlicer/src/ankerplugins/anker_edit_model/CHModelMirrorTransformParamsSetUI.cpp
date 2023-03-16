#include "CHModelMirrorTransformParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>
#include <QRegExp>
#include "../common/utilities/tlogger.h"

const QString X_STR = "<font color=#E32525>X</font> ";
const QString Y_STR = "<font color=#62D361>Y</font> ";
const QString Z_STR = "<font color=#0167FF>Z</font> ";
CHModelMirrorTransformParamsSetUI::CHModelMirrorTransformParamsSetUI(QWidget* parent)
    : BubbleWidget(parent,Qt::LeftArrow)
{
    AkUtil::TFunction("");
    move(97, 566);
    setFixedSize(158, 178);
    // this->setArrowType(Qt::LeftArrow);
    setAutoFillBackground(true);
    //setStyleSheet("background-color:white;");
    setBackgroundColor(QColor(41,42,45));
    setStyleSheet("background:transparent");
    QVBoxLayout* mainblaout = new QVBoxLayout(this);
    mainblaout->setContentsMargins(12,12,12,12);
    mainblaout->setSpacing(12);
    QFont font1;
    font1.setPixelSize(14);
    m_mirrorLabel = new QLabel;
    m_mirrorLabel->setFont(font1);
    m_mirrorLabel->setText(tr("Mirror"));
    //m_mirrorLabel->setStyleSheet("QLabel{ \n\tfont: roboto;\n\twidth: 39px; \n\theight: 16px; \n\ttop: 578px; \n\tleft: 109px; \n\tcolor: #333333; \n }");

    m_resetButton = new QToolButton;
    m_resetButton->setFocusPolicy(Qt::NoFocus);
    m_resetButton->setObjectName("resetLabel");
    m_resetButton->setIcon(QIcon(":/images/fdm_remakes_small_icon_n.png"));
    m_resetButton->setMaximumWidth(20);
    m_resetButton->setMaximumHeight(20);
    m_resetButton->setMinimumWidth(20);
    m_resetButton->setMinimumHeight(20);
    QHBoxLayout* hblaout1 = new QHBoxLayout;
    hblaout1->setContentsMargins(0,0,0,0);
    hblaout1->setSpacing(0);
    hblaout1->addWidget(m_mirrorLabel);
    hblaout1->addSpacing(64);
    hblaout1->addWidget(m_resetButton);
    hblaout1->setStretch(0, 39);
    hblaout1->setStretch(1, 64);
    hblaout1->setStretch(2, 30);
    mainblaout->addLayout(hblaout1);

    Line* line = new Line();
    QHBoxLayout* hblaout2 = new QHBoxLayout;
    hblaout2->setContentsMargins(0,0,0,0);
    hblaout2->setSpacing(0);
    hblaout2->addWidget(line);
    mainblaout->addLayout(hblaout2);

    m_xButton = new QToolButton;
    m_xButton->setObjectName("m_xButton");
    QString xStr = X_STR + tr("Axis");
    m_xStrLabel = new QLabel;
    m_xStrLabel->setObjectName("m_xStrLabel");
    m_xStrLabel->setText(xStr);
    m_xStrLabel->setAlignment(Qt::AlignCenter);
    m_xStrLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    // m_xStrLabel->setMaximumHeight(14);
    m_xStrLabel->setStyleSheet(QString("QLabel{\n"
            "   font-size: 11px;\n"
            "   background-color: transparent;\n"
            "}\n"
        ));
    QHBoxLayout* xhbLayout = new QHBoxLayout;
    xhbLayout->setContentsMargins(0,0,0,0);
    xhbLayout->setSpacing(0);
    xhbLayout->addWidget(m_xStrLabel, Qt::AlignCenter);
    m_xButton->setLayout(xhbLayout);
    m_xButton->setMaximumWidth(120);
    m_xButton->setMaximumHeight(30);
    m_xButton->setMinimumWidth(120);
    m_xButton->setMinimumHeight(30);
    m_xButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"

            "   background-color: #3A3B3F;\n"
            "   border-radius:4px;\n"
            "}\n"));
    mainblaout->addWidget(m_xButton);

    m_yButton = new QToolButton;
    m_yButton->setObjectName("m_yButton");
    QString yStr = Y_STR + tr("Axis");
    m_yStrLabel = new QLabel;
    m_yStrLabel->setAlignment(Qt::AlignCenter);
    m_yStrLabel->setText(yStr);
    m_yStrLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    // m_yStrLabel->setMaximumHeight(14);
    m_yStrLabel->setStyleSheet(QString("QLabel{\n"
        "   font-size: 11px;\n"
        "   background-color: transparent;\n"
        "}\n"
    ));
    QHBoxLayout* yhbLayout = new QHBoxLayout;
    yhbLayout->setContentsMargins(0,0,0,0);
    yhbLayout->setSpacing(0);
    yhbLayout->addWidget(m_yStrLabel, Qt::AlignCenter);
    m_yButton->setLayout(yhbLayout);
    m_yButton->setMaximumWidth(120);
    m_yButton->setMaximumHeight(30);
    m_yButton->setMinimumWidth(120);
    m_yButton->setMinimumHeight(30);
    m_yButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "   background-color: #3A3B3F;\n"
        "   border-radius:4px;\n"
        "}\n"));
    mainblaout->addWidget(m_yButton);

    m_zButton = new QToolButton;
    m_zButton->setObjectName("m_zButton");
    QString zStr = Z_STR + tr("Axis");
    m_zStrLabel = new QLabel;
    m_zStrLabel->setAlignment(Qt::AlignCenter);
    m_zStrLabel->setText(zStr);
    m_zStrLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    // m_zStrLabel->setMaximumHeight(14);
    m_zStrLabel->setStyleSheet(QString("QLabel{\n"
        "   font-size: 11px;\n"
        "   background-color: transparent;\n"
        "}\n"
    ));
    QHBoxLayout* zhbLayout = new QHBoxLayout;
    zhbLayout->setContentsMargins(0,0,0,0);
    zhbLayout->setSpacing(0);
    zhbLayout->addWidget(m_zStrLabel, Qt::AlignCenter);
    m_zButton->setLayout(zhbLayout);
    m_zButton->setMaximumWidth(120);
    m_zButton->setMaximumHeight(30);
    m_zButton->setMinimumWidth(120);
    m_zButton->setMinimumHeight(30);
    m_zButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "   background-color: #3A3B3F;\n"
        "   border-radius:4px;\n"
        "}\n"));
    mainblaout->addWidget(m_zButton);

    connect(m_xButton, SIGNAL(clicked()), this, SLOT(xBtnClicked()));
    connect(m_yButton, SIGNAL(clicked()), this, SLOT(yBtnClicked()));
    connect(m_zButton, SIGNAL(clicked()), this, SLOT(zBtnClicked()));
    //connect(m_resetButton, SIGNAL(clicked()), this, SLOT(reset()));
}

CHModelMirrorTransformParamsSetUI::~CHModelMirrorTransformParamsSetUI()
{

}

void CHModelMirrorTransformParamsSetUI::submit()
{
    if (sender() == m_xButton)
    {
        emit sendWhichButtonClicked(0);
    }
    else if (sender() == m_yButton)
    {
        emit sendWhichButtonClicked(1);
    }
    else if (sender() == m_zButton)
    {
        emit sendWhichButtonClicked(2);
    }
}

void CHModelMirrorTransformParamsSetUI::xBtnClicked()
{
    emit sendWhichButtonClicked(0);
}
void CHModelMirrorTransformParamsSetUI::yBtnClicked()
{
    emit sendWhichButtonClicked(1);
}
void CHModelMirrorTransformParamsSetUI::zBtnClicked()
{
    emit sendWhichButtonClicked(2);
}


void CHModelMirrorTransformParamsSetUI::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (m_mirrorLabel != nullptr) {
            m_mirrorLabel->setText(tr("Mirror"));
        }
        if (m_xStrLabel != nullptr) {
            m_xStrLabel->setText(X_STR + tr("Axis"));
        }
        if (m_yStrLabel != nullptr) {
            m_yStrLabel->setText(Y_STR + tr("Axis"));
        }
        if (m_zStrLabel != nullptr) {
            m_zStrLabel->setText(Z_STR + tr("Axis"));
        }
    }
}





