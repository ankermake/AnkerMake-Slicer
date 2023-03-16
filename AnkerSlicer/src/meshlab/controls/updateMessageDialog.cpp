#include "updateMessageDialog.h"
#include <QScrollArea>
#include <QScrollBar>

namespace  control{
updateMessageDialog::updateMessageDialog(const QString &title, const QString &description,
                       int buttons, QWidget *parent)
    : QDialog(parent),
      m_isClosed(true)
{
    init();
//    setWindowFlags(Qt::FramelessWindowHint
//                   | Qt::Tool
//                   | Qt::WindowStaysOnTopHint);
    
//    activateWindow();
//    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

    m_titleWidget->setTitle(title);
    m_description->setText(description);
    setButton(buttons);
    m_warning->setVisible(false);
    setOTAReleaseNoteVisible(false);
   // setFixedSize(400,171);
}


void updateMessageDialog::init()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0,0,0,0);

    m_titleWidget = new TitleWidget(this);
    connect(m_titleWidget,&TitleWidget::closeCurrentWidget,this,&updateMessageDialog::close);
    m_titleWidget->setBackgroundColor(QColor("#404249"));
    m_titleWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_mainLayout->addWidget(m_titleWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(24,16,24,16);
    layout->setSpacing(12);
    m_mainLayout->addLayout(layout);

    m_description = new QLabel(this);
    m_description->setWordWrap(true);
    m_description->setAlignment(Qt::AlignLeft);
    m_description->setStyleSheet(" font: bold ;");
    layout->addWidget(m_description);


    
    m_detailTitle = new QLabel(this);
    m_detailTitle->setWordWrap(true);
    m_detailTitle->setAlignment(Qt::AlignLeft);
    m_detailTitle->setStyleSheet("QLabel {color : #AAAAAA; }");
    layout->addWidget(m_detailTitle);



    const QString ScrollBarVertical = " QScrollBar:vertical {border:0px solid grey;background: #24252B;width: 3px;margin: 0;}"
        "QScrollBar::handle:vertical { background:#797B80;min-height: 20px;}"
        "QScrollBar::add-line:vertical {border: 0px solid grey;background: #24252B;height: 0px;subcontrol-position: bottom;subcontrol-origin: margin;}"
        "QScrollBar::sub-line:vertical {border: 0px solid grey;background: #24252B;height:0px;subcontrol-position: top;subcontrol-origin: margin;}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {border:0px solid grey;width: 0px;height: 0px;background: white;}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}";

    const QString ScrollBarHorizontal  = "QScrollBar:horizontal {border: 0px solid grey;background: #24252B;height:3px;margin: 0;}"
         "QScrollBar::handle:horizontal {background: #797B80;min-width: 20px;}"
         "QScrollBar::add-line:horizontal {background: grey;width: 0px;subcontrol-position: right;subcontrol-origin: margin;border:0px solid black;}"
         "QScrollBar::sub-line:horizontal {background: grey;width: 0px;subcontrol-position: top right;subcontrol-origin: margin;border:0px solid black;position: absolute;right: 20px;}"
         "QScrollBar:left-arrow:horizontal, QScrollBar::right-arrow:horizontal {width: 0px;height: 3px;background: grey;}"
          "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {background: none;}";

    m_releaseNote = new QTextEdit(this);
    m_releaseNote->setReadOnly(true);
    m_releaseNote->setMinimumWidth(350);
    m_releaseNote->setMinimumHeight(200);
    m_releaseNote->verticalScrollBar()->setStyleSheet(ScrollBarVertical);
    m_releaseNote->setStyleSheet("QTextEdit {background-color:rgba(100,100,100,0); color:white; border:0px;}");
    layout->addWidget(m_releaseNote);

    m_edit = new QLineEdit(this);
    m_edit->setMaximumHeight(40);
    m_edit->setObjectName("m_edit");
    layout->addWidget(m_edit);

    m_warning = new QLabel(this);
    m_warning->setMinimumHeight(21);
    m_warning->setObjectName("m_warning");
    m_warning->setAlignment(Qt::AlignVCenter);
    QFont fontWarning( this->font());
    fontWarning.setPixelSize(fontWarning.pixelSize() - 2);
    m_warning->setFont(fontWarning);

    layout->addWidget(m_warning);

    m_pushLayout = new QHBoxLayout();
    m_leftButton = new QPushButton(this);
    m_leftButton->setMinimumHeight(32);
    QFont fontButton( this->font());
    fontButton.setPixelSize(fontButton.pixelSize() + 2);
    m_leftButton->setFont(fontButton);
    m_leftButton->setObjectName("m_leftButton");
    m_leftButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_leftButton,&QPushButton::clicked,this,&updateMessageDialog::buttonClicked);

    m_middleButton = new QPushButton(this);
    m_middleButton->setObjectName("m_middleButton");
    m_middleButton->setFont(fontButton);
    m_middleButton->setMinimumHeight(32);
    m_middleButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_middleButton,&QPushButton::clicked,this,&updateMessageDialog::buttonClicked);

    m_rightButton = new QPushButton(this);
    m_rightButton->setObjectName("m_rightButton");
    m_rightButton->setFont(fontButton);
    m_rightButton->setMinimumHeight(32);
    m_rightButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_rightButton,&QPushButton::clicked,this,&updateMessageDialog::buttonClicked);

    m_pushLayout->addWidget(m_leftButton);

    m_pushLayout->addWidget(m_middleButton);
    m_pushLayout->addWidget(m_rightButton);
    layout->addLayout(m_pushLayout);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setEditMode(false);
}


void updateMessageDialog::setButton(int buttons)
{
    switch (buttons) {
    case(updateMessageDialog::OK):
        m_leftButton->setVisible(false);
        m_middleButton->setVisible(false);
        m_rightButton->setText(tr("OK"));
        setButtonValue(m_rightButton,updateMessageDialog::OK);
        break;

//    case(updateMessageDialog::YES | updateMessageDialog::NO):
//        m_leftButton->setText(tr("No"));
//        setButtonValue(m_leftButton,updateMessageDialog::NO);
//        m_rightButton->setVisible(false);
//        m_middleButton->setText(tr("Yes"));
//        setButtonValue(m_middleButton,updateMessageDialog::YES);
//        break;
    case(updateMessageDialog::CANCEL | updateMessageDialog::SAVE):
        m_leftButton->setText(tr("Cancel"));
        setButtonValue(m_leftButton,updateMessageDialog::CANCEL);
        m_rightButton->setText(tr("Save"));
        setButtonValue(m_rightButton,updateMessageDialog::SAVE);
        m_middleButton->setVisible(false);
        break;
    case(updateMessageDialog::CANCEL | updateMessageDialog::SAVE | updateMessageDialog::DoNotSave):
        m_leftButton->setText(tr("Cancel"));
        setButtonValue(m_leftButton,updateMessageDialog::CANCEL);
        m_rightButton->setText(tr("Save"));
        setButtonValue(m_rightButton,updateMessageDialog::SAVE);
        m_middleButton->setText(tr("Don't Save"));
        setButtonValue(m_middleButton,updateMessageDialog::DoNotSave);
        m_pushLayout->insertStretch(1,1);
        m_leftButton->setFixedWidth(100);
        m_rightButton->setFixedWidth(100);
        m_middleButton->setFixedWidth(100);
        this->setMinimumWidth(480);
        break;
    case(updateMessageDialog::SAVE | updateMessageDialog::DoNotSave):
        m_leftButton->setText(tr("Don't Save"));
        setButtonValue(m_leftButton,updateMessageDialog::DoNotSave);
        m_rightButton->setText(tr("Save"));
        setButtonValue(m_rightButton,updateMessageDialog::SAVE);
        m_middleButton->setVisible(false);
        break;
    default:
        m_leftButton->setText(tr("No"));
        setButtonValue(m_leftButton,updateMessageDialog::NO);
        m_middleButton->setVisible(false);
        m_rightButton->setText(tr("Yes"));
        setButtonValue(m_rightButton,updateMessageDialog::YES);
        break;
    }

    m_leftButton->setFocusPolicy(Qt::NoFocus);
    m_middleButton->setFocusPolicy(Qt::NoFocus);
    m_rightButton->setFocusPolicy(Qt::NoFocus);

}


void updateMessageDialog::setButtonValue(QPushButton *button, BUTTONFLAG flage)
{
    button->setProperty("value",flage);  
}



void updateMessageDialog::setBottonText(updateMessageDialog::BUTTONFLAG botton, const QString &string)
{
    switch (botton) {
    case(updateMessageDialog::YES):
        m_rightButton->setText(string);
        break;
    case(updateMessageDialog::NO):
        m_leftButton->setText(string);
        break;
    default:
        break;
    }
}


/*






*/
QString updateMessageDialog::setLineFeed(const QString &text, const QFont &font, int pxWidth)
{
    QString strText = text;
    QFontMetrics fm(font);

    if (!strText.isEmpty())
    {
        int lineStart = 0;
        for (int i = 1; i < strText.size() ; i++)
        {
            int linewith = fm.width(strText.mid(lineStart,i+2-lineStart));
            if (linewith > pxWidth )
            {
                QString line = strText.mid(lineStart,i-lineStart);
                //int linewith1 = fm.width(line);
                //qDebug() << lineStart<<"-"<<i<<"   line="<< line << "    lineWith:"<<linewith1 << "     ,line+2with:"<<linewith<<" pxWidth:"<<pxWidth << "  char(i):"<<strText.at(i) ;

                if(strText.at(i) == ' ' || strText.at(i-1) == ' ' )
                {
                    
                    strText.insert(i, "\n");
                    i += 1;
                }
                else
                {
                    
                    if(fm.width(line+"-") > pxWidth)
                    {
                        strText.insert(i-1, "-\n");
                        i += 1;
                    }
                    else
                    {
                        strText.insert(i, "-\n");
                        i += 2;
                    }
                }

                lineStart = i;
            }
        }
    }
    return strText;
}

void updateMessageDialog::setDetailText(const QString &text)
{
    if (m_releaseNote){
        m_releaseNote->clear();
        m_releaseNote->setText(text);
    }
}

void updateMessageDialog::setDetailTitleText(const QString &text)
{
    if (m_detailTitle){
        m_detailTitle->clear();
        m_detailTitle->setText(text);
    }
}


void updateMessageDialog::setOTAReleaseNoteVisible(bool show)
{
    if(m_releaseNote){
        m_detailTitle->setVisible(show);
        if(show)
        {
           m_releaseNote->setMinimumWidth(350);
           m_releaseNote->setMinimumHeight(200);
           m_releaseNote->setMaximumWidth(350);
           m_releaseNote->setMaximumHeight(200);
        }
        else
        {
            m_releaseNote->setMinimumWidth(0);
            m_releaseNote->setMinimumHeight(0);
            m_releaseNote->setMaximumWidth(0);
            m_releaseNote->setMaximumHeight(0);
        }
        m_releaseNote->setVisible(show);
    }
}

void updateMessageDialog::setEditMode(bool edit)
{
    m_edit->setVisible(edit);
    if(edit) {
        setMinimumSize(400,230);
        m_warning->setVisible(true);
    }
}

void updateMessageDialog::setValidator(const QValidator *validator)
{
    m_edit->setValidator(validator);
}

void updateMessageDialog::setWarning(const QString &warning)
{
    m_warning->setVisible(true);
    m_warning->setText(warning);
}

void updateMessageDialog::setEditText(const QString &string)
{
    m_edit->setText(string);
}

QString updateMessageDialog::editText() const
{
    return m_edit->text();
}

void updateMessageDialog::setAutoLevelText(const QString& leftText,  const QString &rightText)
{
    m_leftButton->setText(leftText);
    m_middleButton->setText(rightText);
}

void updateMessageDialog::setDescriptionText(const QString &text)
{
    m_description->clear();
    m_description->setText(text);
}

void updateMessageDialog::setAutoClosed(bool ok)
{
     m_isClosed = ok;
}

void updateMessageDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor("#404249"));
    QPainterPath path;
    path.addRoundedRect(0,0,this->width() , this->height() ,16,16);
    painter.drawPath(path);

    QWidget::paintEvent(event);
}

void updateMessageDialog::buttonClicked()
{

    QPushButton *button = qobject_cast<QPushButton* > (sender());
    auto flag = button->property("value").toInt();
   // qDebug() <<" debug m_isClosed==" << flag;
    if(m_isClosed) {
        done(flag);
    }
    else {
        emit buttonClick(flag);
    }
}

/*
TimerMessageDialog::TimerMessageDialog(const QString &title, const QString &description, int time, int buttons, QWidget *parent) :
    updateMessageDialog::updateMessageDialog(title, description, buttons, parent), m_totalSecs(time), m_description(description)
{
    m_timer = new QTimer(this);

    QString tempStr = description + " " + QString::number(m_totalSecs) + "s";
    qDebug() << "----" << tempStr;
    setDescriptionText(tempStr);
    connect(m_timer, &QTimer::timeout, this, [=]() mutable{
        if(m_totalSecs >= 0)
        {
            m_totalSecs--;
            QString tempStr = description + " " + QString::number(m_totalSecs) + "s";
            qDebug() << "----" << tempStr;
            setDescriptionText(tempStr);
        }
        else
        {
            m_timer->stop();
        }
    });
    m_timer->start();
}
*/

}
