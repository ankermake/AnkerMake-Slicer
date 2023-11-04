#include "progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent)
    : QDialog(parent),
      m_mainLayout(new QVBoxLayout(this)),
      m_progressbar(new ProgressBar(this)),
      m_label( new QLabel(this)),
      m_autoClosed(true),
      m_alreadyClosed(false)
{
    init();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ProgressDialog::autoCloseDialog);
    m_timer->start(30 * 1000);
}

void ProgressDialog::init()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setFixedSize(480,330);
    m_mainLayout->setContentsMargins(0,32,0,16);
    m_mainLayout->setSpacing(25);
    QFont font = this->font();
    font.setPixelSize(font.pixelSize() + 2);
    m_label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_label->setFont(font);
    m_label->setVisible(false);
    m_label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_label->setWordWrap(true);
    setAttribute(Qt::WA_TranslucentBackground);
    m_cancelButton = new QPushButton(tr("Cancel"));
    m_cancelButton->setFont(font);
    connect(m_cancelButton,&QPushButton::clicked,this,&ProgressDialog::cancelButtonClick);
    m_cancelButton->setObjectName("m_cancelButton");
    m_cancelButton->setMinimumSize(100,32);
    connect(m_progressbar,&ProgressBar::closeProgress,this,&ProgressDialog::cancelButtonClick);
    m_progressbar->setObjectName("progressbar");
    m_mainLayout->addWidget(m_progressbar,5,Qt::AlignHCenter | Qt::AlignVCenter);
    m_mainLayout->addWidget(m_label,1,Qt::AlignHCenter);
//    Line *line = new Line(this);
//    m_mainLayout->addWidget(line);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setContentsMargins(16,0,16,0);
    vbox->addWidget(m_cancelButton,1,Qt::AlignHCenter);
    m_mainLayout->addLayout(vbox);
    this->setLayout(m_mainLayout);

}
#include <QApplication>
void ProgressDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (m_cancelButton != nullptr) {
            m_cancelButton->setText(tr("Cancel"));
        }
    }
    QDialog::changeEvent(e);
}
void ProgressDialog::setValue(int value)
{
    //qDebug() << "set value ";
    m_progressbar->setValue(value);
    m_currentValue = value;
//    QCoreApplication::processEvents();
    update();
}

void ProgressDialog::autoCloseDialog()
{
   if(m_lastValue == m_currentValue){
       //close();
       emit closeSignal(false);
   }
   m_lastValue = m_currentValue;
}

void ProgressDialog::setText(const QString &text)
{
    m_label->setVisible(true);
    m_label->setText(text);
}

void ProgressDialog::setAutoClosed(bool closed)
{
    m_autoClosed = closed;
    m_progressbar->setAutoClosed( closed);
}

void ProgressDialog::setCancelVisible(bool interrupt)
{
    m_cancelButton->setVisible(interrupt);
}

void ProgressDialog::cancelButtonClick()
{
    qDebug()<< "enter cancel button";

    if(m_alreadyClosed) {
        return;
    }
    QMutex mutex;
    mutex.lock();
    m_alreadyClosed = true;
    mutex.unlock();
    QObject *object = sender();
    if (object->objectName() == "m_cancelButton") {
        emit cancel();
        this->close();
    }
    else {
        if(m_autoClosed) {
            this->setAttribute(Qt::WA_DeleteOnClose);
            QTimer::singleShot(1000,[this](){
                this->close();
            });
        }
        else {
            this->close();
        }
        emit progressClosed();
    }
}

void ProgressDialog::paintEvent(QPaintEvent *event)
{
//    qDebug() << "updata ProgressDialog";
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor("#404249"));
    QPainterPath path;
    path.addRoundedRect(0,0,this->width() , this->height() ,16,16);
    painter.drawPath(path);

    QWidget::paintEvent(event);
}

void ProgressDialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape: /*qDebug() << "Qt::Key_Escape." ;*/ break;
    default: break;
    }


}
