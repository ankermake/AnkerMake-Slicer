
#include "ankerguidepage.h"

ankerGuidePage::ankerGuidePage(QMainWindow *parent,shared_ptr<AnkerMainWindow> p)
    : QMainWindow{parent}
{
    m_window = p;
    initUi();
#ifdef Q_OS_WIN
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
#endif
}

void ankerGuidePage::initUi()
{

    // create layouts
    QWidget* center = new QWidget(this);
    center->setStyleSheet(QString::fromUtf8("border-image:url(:/images/icon/guidePageBackground.png)"));
    QHBoxLayout* mainLayout = new QHBoxLayout(center);
    QVBoxLayout* leftLayout = new QVBoxLayout();
    QVBoxLayout* middleLayout = new QVBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();

    //creat button
    QPushButton *button_slicer = new QPushButton("");
    QPushButton *button_preview = new QPushButton("");
    QPushButton *button_device = new QPushButton("");
    button_slicer->setStyleSheet(QString::fromUtf8("QPushButton{border-image:url(:/images/icon/slice-n.png); background-color:transparent;}\n"
                                                   "QPushButton:hover {\n"
                                                   "    background-color: #343539;\n"
                                                   "}\n"
                                                   "QPushButton:pressed{border-image:url(:/images/icon/slice-s.png);}"));
    button_preview->setStyleSheet(QString::fromUtf8("QPushButton{border-image:url(:/images/icon/Preview-n.png); background-color:transparent;}\n"
                                                   "QPushButton:hover {\n"
                                                   "    background-color: #343539;\n"
                                                   "}\n"
                                                   "QPushButton:pressed{border-image:url(:/images/icon/Preview-s.png);}"));
    button_device->setStyleSheet(QString::fromUtf8("QPushButton{border-image:url(:/images/icon/Device-n.png); background-color:transparent;}\n"
                                                   "QPushButton:hover {\n"
                                                   "    background-color: #343539;\n"
                                                   "}\n"
                                                   "QPushButton:pressed{border-image:url(:/images/icon/Device-s.png);}"));

    QList<QPushButton*> buttonlist;
    buttonlist.push_back(button_slicer);
    buttonlist.push_back(button_preview);
    buttonlist.push_back(button_device);

    //set button style
    for(auto pushButton_it:buttonlist){
         pushButton_it->setMinimumSize(QSize(168,168));
         pushButton_it->setMaximumSize(QSize(168,168));
    }


    //creat lable
    QLabel *label_slicer = new QLabel(tr("Slicer"));
    QLabel *label_preview = new QLabel(tr("Preview & AI"));
    QLabel *label_device = new QLabel(tr("DevicePortal"));
    QList<QLabel*> LabelList;
    LabelList.push_back(label_slicer);
    LabelList.push_back(label_preview);
    LabelList.push_back(label_device);
    for(auto lable_it:LabelList){
        lable_it->setMaximumHeight(32);
        lable_it->setMinimumHeight(32);//28
        lable_it->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QFont font = lable_it->font();
        font.setPointSize(20);//in chinese point size 24 > 28
        font.setWeight(63);
        lable_it->setFont(font);

    }

    //creat lable
    QString slicerStr = tr("Beginner friendly,  which you can make some simple modifications for your models.");
    QString previewStr = tr("A tool to preview Gcode files, and you can enable AI feature here.");
    QString deviceStr = tr("Live view and remote control you printer, start you print anytime.");
    QLabel *label_slicer_des = new QLabel(slicerStr);
    QLabel *label_preview_des = new QLabel(previewStr);
    QLabel *label_device_des = new QLabel(deviceStr);
    QList<QLabel*> LabelDesList;
    LabelDesList.push_back(label_slicer_des);
    LabelDesList.push_back(label_preview_des);
    LabelDesList.push_back(label_device_des);
    for(auto lable_it:LabelDesList){
        lable_it->setMinimumWidth(240);//230
        lable_it->setMaximumWidth(240);
        lable_it->setMaximumHeight(64);
        lable_it->setMinimumHeight(64);
        lable_it->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        lable_it->setWordWrap(true);
        lable_it->setAlignment(Qt::AlignHCenter);
        lable_it->setStyleSheet(QString::fromUtf8("color: rgb(171,171,171);background-color:transparent;"));
    }

    // set size policy for the labels
    for (auto layout : { leftLayout, middleLayout, rightLayout }) {
        layout->addItem(new QSpacerItem(0, 178, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
        layout->addWidget(buttonlist.front(), 168, Qt::AlignHCenter);
        buttonlist.pop_front();
        layout->addItem(new QSpacerItem(0, 12, QSizePolicy::Fixed, QSizePolicy::Maximum));
        layout->addWidget(LabelList.front(), 28, Qt::AlignHCenter);
        LabelList.pop_front();
        layout->addItem(new QSpacerItem(0, 8, QSizePolicy::Fixed, QSizePolicy::Maximum));
        layout->addWidget(LabelDesList.front(), 64,Qt::AlignHCenter);
        LabelDesList.pop_front();
        layout->addItem(new QSpacerItem(0, 208, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
        //layout->setAlignment(Qt::AlignHCenter);

    }

    // set layout ratios
    mainLayout->addItem(new QSpacerItem(181, 0, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    mainLayout->addLayout(leftLayout, 230);
    mainLayout->addItem(new QSpacerItem(81, 0, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    mainLayout->addLayout(middleLayout, 230);
    mainLayout->addItem(new QSpacerItem(81, 0, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    mainLayout->addLayout(rightLayout, 230);
    mainLayout->addItem(new QSpacerItem(181, 0, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    connect(button_slicer, SIGNAL(clicked()), this, SLOT(openSlicer()));
    connect(button_preview, SIGNAL(clicked()), this, SLOT(openPreview()));
    connect(button_device, SIGNAL(clicked()), this, SLOT(openDevice()));
#ifdef Q_OS_WIN
    auto mainMenuBar = new QMenuBar();
    mainMenuBar->setFixedHeight(28);
    mainMenuBar->setObjectName("mainMenuBar");
    m_mainTitleBar = new customTitle(this, mainMenuBar,true);
    m_mainTitleBar->setObjectName("m_mainTitleBar");
    m_mainTitleBar->setFixedHeight(40);

    this->setMenuWidget(m_mainTitleBar);
    m_mainTitleBar->setMaxScreen(false);

    connect(m_mainTitleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_mainTitleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()), Qt::DirectConnection);
    connect(m_mainTitleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()), Qt::DirectConnection);
    connect(m_mainTitleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
#endif

    this->setCentralWidget(center);

}

void ankerGuidePage::openSlicer()
{
        m_window->showMaximized();
        FdmMainWidget* mainwidgt = m_window->mainwidget;
        if (mainwidgt)
        {
            mainwidgt->onEntryMainWindow();
        }
        m_window->mainwidget->currentPageChanged(FDMWIDGET);
        this->close();
}

void ankerGuidePage::openPreview()
{
    m_window->showMaximized();
    FdmMainWidget* mainwidgt = m_window->mainwidget;
    if (mainwidgt)
    {
        mainwidgt->onEntryMainWindow();
    }
    m_window->mainwidget->currentPageChanged(GCODEWIDGET);
    this->close();

}

void ankerGuidePage::openDevice()
{
    m_window->showMaximized();
    FdmMainWidget* mainwidgt = m_window->mainwidget;
    if (mainwidgt)
    {
        mainwidgt->onEntryMainWindow();
    }
    m_window->mainwidget->currentPageChanged(NETWORKWIDGET);
    this->close();
}

void ankerGuidePage::onButtonMinClicked()
{
    showMinimized();
}

void ankerGuidePage::onButtonRestoreClicked()
{
    TFunction("");
    showNormalWin();
}

void ankerGuidePage::onButtonMaxClicked()
{
    TFunction("");
    showMaximizedWin();
}

void ankerGuidePage::onButtonCloseClicked()
{
    close();
}

void ankerGuidePage::openFileFromAppRaram(const QString &fileName)
{
    this->close();
    m_window->showMaximized();
    m_window->openFileFromAppRaram(fileName);
    //Object::connect(&app, &MeshLabApplication::openFileSignal, window.get(), &AnkerMainWindow::openFileFromAppRaram, Qt::QueuedConnection);

}


void ankerGuidePage::showNormalWin()
{
    QScreen* screen =  this->screen();
    if(screen)
    {
        QSize minSZ= this->minimumSize();
        QRect availableGeometry = screen->availableGeometry();
        // set win size to 70% of screen size
        int w = availableGeometry.width()*0.7 < minSZ.width() ? availableGeometry.width()*0.7 : max(minSZ.width(),1244);
        int h = availableGeometry.height()*0.7 < minSZ.height() ? availableGeometry.height()*0.7 : max(minSZ.height(),720);
        int x = availableGeometry.x() + (availableGeometry.width() - w)*0.5;
        int y = availableGeometry.y() + (availableGeometry.height() -h)*0.5;
        showNormal();
        this->setGeometry(x,y,w,h);
    }
}

void ankerGuidePage::openFileList(QStringList argfileList)
{
    if(!argfileList.isEmpty()) {
       this->close();
       m_window->showMaximized();
    }
    m_window->openFileList(argfileList);
    return;
}

void ankerGuidePage::showMaximizedWin()
{
    QScreen* screen = this->screen();
    if(screen)
    {
        QRect availableGeometry = screen->availableGeometry();
        this->setGeometry(availableGeometry.x(),availableGeometry.y(),availableGeometry.width(), availableGeometry.height());
    }
    return;
}

//void ankerGuidePage::paintEvent(QPaintEvent *e)
//{
//    if (m_framelessHelper == 0)
//        return;
//    m_framelessHelper->paintEvent(e);
//}


