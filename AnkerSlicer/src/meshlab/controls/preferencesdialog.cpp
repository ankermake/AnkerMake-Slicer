#include "preferencesdialog.h"

namespace  control{
PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint),
      m_listView(new QListView(this)),
      m_listmodel(new QStringListModel(this)),
      m_stackLayout(new QStackedLayout())
{
    this->setMinimumSize(QSize(900,700));
    this->setMaximumSize(QSize(900,700));
   // this->setWindowFlags(Qt::FramelessWindowHint );
   // setAttribute(Qt::WA_TranslucentBackground,true);

    QFont font = this->font();
    font.setPixelSize(font.pixelSize() + 2);
    m_listView->setModel(m_listmodel);
    m_listView->setMaximumWidth(150);
    m_listView->setFrameShape(QFrame::NoFrame);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->setObjectName(QString::fromUtf8("preferenceListView"));
    m_listView->setFont(font);
    m_listView->setFocusPolicy(Qt::NoFocus);
    connect(m_listView,&QListView::clicked,this,&PreferencesDialog::selectCurrentIndex);
    //=================================
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    this->setLayout(layout);
    m_title = new QLabel(tr("Preferences"));
    m_title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_title->setMinimumHeight(50);
    QFont font2 = this->font();
    font2.setPixelSize(font2.pixelSize() + 4);
    font2.setWeight(QFont::Bold);
    m_title->setFont(font2);
    layout->addWidget(m_title,0,Qt::AlignHCenter | Qt::AlignVCenter);
    Line *lineTop = new Line(this,QString("#484A51"),QFrame::HLine);
    layout->addWidget(lineTop);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(0);
    hbox->addWidget(m_listView);
    Line *line = new Line(this,QString("#484A51"),QFrame::VLine);
    hbox->addWidget(line);
    hbox->addLayout(m_stackLayout);
    layout->addLayout(hbox);
    Line *linebottom = new Line(this, QString("#484A51"),QFrame::HLine);
    layout->addWidget(linebottom);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(24,16,24,16);
    hlayout->setSpacing(12);
    layout->addLayout(hlayout);

    m_closeButton = new QPushButton(tr("Close"),this);
    m_closeButton->setFont(font);
    m_closeButton->setMinimumSize(100,32);
    connect(m_closeButton,&QPushButton::clicked,this,&PreferencesDialog::closeDialog);
    hlayout->addStretch();
    hlayout->addWidget(m_closeButton);
    m_saveButton = new  QPushButton(tr("Save"),this);
    m_saveButton->setObjectName("hightButton");
    m_saveButton->setFont(font);
    m_saveButton->setMinimumSize(100,32);
    connect(m_saveButton,&QPushButton::clicked,this,&PreferencesDialog::save);
    hlayout->addWidget(m_saveButton);
}

void PreferencesDialog::addWidget(PageWidget *widget, int index)
{
    if(!widget) {
        return;
    }
    if(index  < m_displayNameList.size() && index != -1) {
        m_displayNameList.insert(index,widget->displayName());
        m_stackLayout->insertWidget(index,widget);
    }else {
        m_displayNameList.append(widget->displayName());
        m_stackLayout->addWidget(widget);
    }
    m_listmodel->setStringList(m_displayNameList);
}

QWidget* PreferencesDialog::getWidget(const int index)
{
    if (!m_stackLayout)
    {
        return nullptr;
    }
    return m_stackLayout -> widget(index);
}

void PreferencesDialog::openWidget(int index)
{
    if(index > m_stackLayout->count()) {
        return;
    }
    QModelIndex modelIndex = m_listmodel->index(index);
    if(!modelIndex.isValid()) {
        return;
    }
    m_listView->setCurrentIndex(modelIndex);
    m_stackLayout->setCurrentIndex(index);
}

void PreferencesDialog::openWidget(QWidget *widget)
{
    if(!widget) {
        return;
    }
    int cout =  m_stackLayout->count();
    for(auto i = 0 ; i < cout ; ++i) {
        QWidget *temWidget = m_stackLayout->widget(i);
        if(temWidget == widget) {
            m_stackLayout->setCurrentWidget(widget);
            int index = m_stackLayout->indexOf(widget);
            QModelIndex modelIndex =   m_listmodel->index(index);
            if(!modelIndex.isValid()) {
                return;
            }
            m_listView->setCurrentIndex(modelIndex);
            break;
        }
    }
}

void PreferencesDialog::save()
{
    int cout =  m_stackLayout->count();
    for(auto i = 0 ; i < cout ; ++i) {
        QWidget *widget = m_stackLayout->widget(i);
        if(!widget) {
            continue;
        }
        PageWidget *pageWidget = static_cast<PageWidget*>(widget);
        if(!pageWidget) {
            continue;
        }
        pageWidget->save();
    }
    this->close();
}

void PreferencesDialog::closeDialog()
{
    int cout =  m_stackLayout->count();
    for(auto i = 0 ; i < cout ; ++i) {
        QWidget *widget = m_stackLayout->widget(i);
        if(!widget) {
            continue;
        }
        PageWidget *pageWidget = static_cast<PageWidget*>(widget);
        if(!pageWidget) {
            continue;
        }
        pageWidget->closeWidget();
    }
    this->close();
}

void PreferencesDialog::selectCurrentIndex(const QModelIndex &index)
{
    m_stackLayout->setCurrentIndex(index.row());
    emit swicthPage(index.row());
}

void PreferencesDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    // painter.setCompositionMode( QPainter::CompositionMode_Clear );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(41,42,45));
   // QPainterPath path;
   // QPolygon polygon;
   // path.addRoundedRect(0,0,this->width() , this->height() ,40,15);
    painter.drawRoundedRect(0,0,this->width() , this->height() ,0,0);

  //  QStyleOption opt;
   // opt.init(this);
//    QPainter painter(this);
   // style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);
}

void PreferencesDialog::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (m_title) {
            m_title->setText(tr("Preferences"));
        }
        if (m_listView) {
            m_displayNameList.clear();
            m_displayNameList << QString("    ") + tr("General") << QString("    ") + tr("Machine") << QString("    ") + tr("Material") << QString("    ") +  tr("Parameters");
            m_listmodel->setStringList(m_displayNameList);
            m_listView->setModel(m_listmodel);
        }
        if (m_closeButton) {
            m_closeButton->setText(tr("Close"));
        }
        if (m_saveButton) {
            m_saveButton->setText(tr("Save"));
        }
    }
}
}
