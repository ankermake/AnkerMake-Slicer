#include "copyrightwidget.h"

CopyRightWidget::CopyRightWidget(QWidget *parent)
    : QDialog(parent),
      m_dataList(QList<CopyRightInforMation *>())
{
    initUi();
}

void CopyRightWidget::paintEvent(QPaintEvent *event)
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

void CopyRightWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (m_titleWidget) {
            m_titleWidget->setTitle(tr("Copyright Information"));
        }
    }
    return QWidget::changeEvent(event);
}

void CopyRightWidget::initUi()
{
    this->setFixedSize(400,362);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_titleWidget = new TitleWidget(this);
    m_titleWidget->setCloseButtonIcon(QIcon(":/images/icon/fdm_title_close_icon.png"));
    m_titleWidget->setBackgroundColor("#404249");
    m_titleWidget->setTitle(tr("Copyright Information"));
    connect(m_titleWidget,&TitleWidget::closeCurrentWidget,this,&CopyRightWidget::close);
    mainLayout->addWidget(m_titleWidget,0,0);

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(16,16,16,16);
    layout->setSpacing(16);
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setFocusPolicy(Qt::NoFocus);
    m_tableWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_tableWidget->setContentsMargins(16,0,6,0);
    //QHeaderView  *headView = m_tableWidget->horizontalHeader();
    // headView->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->verticalHeader()->hide();
    m_tableWidget->setColumnCount(3);
    m_tableWidget->setColumnWidth(0,120);
    m_tableWidget->setColumnWidth(1,120);
    m_tableWidget->setColumnWidth(2,120);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("License") << tr("Modifications"));
    QHeaderView  *headView = m_tableWidget->horizontalHeader();
    headView->setDefaultAlignment(Qt::AlignLeft);
    layout->addWidget(m_tableWidget,0,0);

   // mainLayout->addWidget(m_tableWidget,1,0);
    QLabel *labelCopyrightDescription = new QLabel(tr("Copyright?2021 Anker Innovations Limited"),this);
    labelCopyrightDescription->setAlignment(Qt::AlignHCenter);
    layout->addWidget(labelCopyrightDescription,1,0,Qt::AlignHCenter|Qt::AlignBottom);

    mainLayout->addLayout(layout,1,0);
    initTableWidget(initData());
}

void CopyRightWidget::initTableWidget(QList<CopyRightInforMation *> dataList)
{

    if(dataList.isEmpty()) {
        return;
    }
    m_tableWidget->setRowCount(dataList.size());
    for(auto i = 0 ; i < dataList.size(); i++) {

        QLabel *labelName = new QLabel(m_tableWidget);
        labelName->setOpenExternalLinks(true);
        QString nameText = QString("<a style='color: #61D37D; text-decoration: none' href = %1>%2").arg(dataList[i]->nameLink).arg(dataList[i]->name);
       //qDebug() << " name text = " << nameText;
        labelName->setText(nameText);
        labelName->setAlignment(Qt::AlignLeft);

        m_tableWidget->setCellWidget(i,0,labelName);

        QLabel *labelLicense = new QLabel(m_tableWidget);
        labelLicense->setOpenExternalLinks(true);
        QString licenseText = QString("<a style='color: #61D37D; text-decoration: none' href = %1>%2").arg(dataList[i]->licenseLink).arg(dataList[i]->licenseType);
        //qDebug() << " licenseText text = " << licenseText << " link ==" << dataList[i]->licenseLink;
        labelLicense->setText(licenseText);
        labelLicense->setAlignment(Qt::AlignLeft);
        m_tableWidget->setCellWidget(i,1,labelLicense);

        m_tableWidget->setItem(i,2,new QTableWidgetItem(dataList.at(i)->isModified ? tr("Yes") : tr("No")));

    }

}

QList<CopyRightInforMation *> CopyRightWidget::initData()
{
    QList<CopyRightInforMation *> list;
    CopyRightInforMation *qtInformation = new CopyRightInforMation();
    qtInformation->name = QString("QT");
    qtInformation->nameLink = QString("https://www.qt.io");
    qtInformation->isModified = false;
    qtInformation->licenseType = QString("LGPL");
    qtInformation->licenseLink = QString("https://doc.qt.io/qt-5/lgpl.html");
    list.append(qtInformation);

    CopyRightInforMation *curaEngineInformation = new CopyRightInforMation();
    curaEngineInformation->name = QString("CuraEngine");
    curaEngineInformation->nameLink = QString("https://github.com/Ultimaker/CuraEngine");
    curaEngineInformation->isModified = true;
    curaEngineInformation->licenseType = QString("AGPL-3.0");
    curaEngineInformation->licenseLink = QString("https://github.com/Ultimaker/CuraEngine/blob/main/LICENSE");
    list.append(curaEngineInformation);

    CopyRightInforMation *angusClipperInformation = new CopyRightInforMation();
    angusClipperInformation->name = QString("Angus Clipper");
    angusClipperInformation->nameLink = QString("http://www.angusj.com/delphi/clipper.php");
    angusClipperInformation->isModified = false;
    angusClipperInformation->licenseType = QString("MIT");
    angusClipperInformation->licenseLink = QString("https://github.com/mapnik/angus-clipper/blob/master/LICENSE");
    list.append(angusClipperInformation);

    CopyRightInforMation *meshlabInformation = new CopyRightInforMation();
    meshlabInformation->name = QString("Meshlab");
    meshlabInformation->nameLink = QString("https://www.meshlab.net");
    meshlabInformation->isModified = true;
    meshlabInformation->licenseType = QString("GPL-3.0");
    meshlabInformation->licenseLink = QString("https://github.com/cnr-isti-vclab/meshlab/blob/main/LICENSE.txt");
    list.append(meshlabInformation);


    CopyRightInforMation *slir3rInformation = new CopyRightInforMation();
    slir3rInformation->name = QString("Slic3r");
    slir3rInformation->nameLink = QString("https://github.com/slic3r/Slic3r");
    slir3rInformation->isModified = true;
    slir3rInformation->licenseType = QString("AGPL-3.0");
    slir3rInformation->licenseLink = QString("https://github.com/slic3r/Slic3r/blob/master/LICENSE");
    list.append(slir3rInformation);

    CopyRightInforMation *vCGlibInformation = new CopyRightInforMation();
    vCGlibInformation->name = QString("VCGlib");
    vCGlibInformation->nameLink = QString("http://vcg.isti.cnr.it/vcglib");
    vCGlibInformation->isModified = false;
    vCGlibInformation->licenseType = QString("GPL-3.0");
    vCGlibInformation->licenseLink = QString("https://github.com/cnr-isti-vclab/vcglib/blob/main/LICENSE.txt");
    list.append(vCGlibInformation);

    CopyRightInforMation *opensslInformation = new CopyRightInforMation();
    opensslInformation->name = QString("OpenSSL");
    opensslInformation->nameLink = QString("https://github.com/openssl/openssl");
    opensslInformation->isModified = false;
    opensslInformation->licenseType = QString("GPL-3.0");
    opensslInformation->licenseLink = QString("https://github.com/openssl/openssl/blob/master/LICENSE.txt");
    list.append(opensslInformation);

    return list;

}

