#include "akprogressbar.h"
//#include "ui_akprogressbar.h"
#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QCoreApplication>

AkProgressBar::AkProgressBar(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::AkProgressBar)
{
    //setStyleSheet("background-color:rgb(255, 255, 255);border-radius: 25px;");
    setStyleSheet("background-color:rgb(255, 255, 255);");
    //setupUi(this);
    auto setupUi = [=](QWidget *AkProgressBar)-> void {
        auto layout = new QHBoxLayout(AkProgressBar);
        //AkProgressBar->setStyleSheet("background-color:rgb(255, 255, 255);");
        if (AkProgressBar->objectName().isEmpty())
            AkProgressBar->setObjectName(QString::fromUtf8("AkProgressBar"));
        //AkProgressBar->resize(400, 60);
        progressBar = new QProgressBar(AkProgressBar);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        //progressBar->setGeometry(QRect(20, 20, 261, 23));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignLeft |Qt::AlignVCenter);

        pushButton = new QPushButton(AkProgressBar);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        //pushButton->setGeometry(QRect(310, 20, 75, 23));
        //cancel slice
        connect(pushButton, &QPushButton::clicked,this, &AkProgressBar::cancel);

        layout->addWidget(progressBar);
        layout->addWidget(pushButton);

        //retranslateUi(AkProgressBar);
        AkProgressBar->setWindowTitle(QCoreApplication::translate("AkProgressBar", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("AkProgressBar", "Cancel", nullptr));

        //QMetaObject::connectSlotsByName(AkProgressBar);

    };
    setupUi(this);
}

void AkProgressBar::paintEvent(QPaintEvent *event)
{
    QStyleOption option;
    option.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget,&option,&painter,this);
    QWidget::paintEvent(event);
    //setBackgroundRole(QPalette::Background);
}

AkProgressBar::~AkProgressBar()
{
    //delete ui;
}

void AkProgressBar::setValue(int value)
{
    progressBar->setValue(value);
}

void AkProgressBar::setFormat(const QString &format)
{
    progressBar->setFormat(format);
}
