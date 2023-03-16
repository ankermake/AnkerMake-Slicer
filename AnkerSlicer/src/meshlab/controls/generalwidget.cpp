#include "generalwidget.h"
#include "../common/mlapplication.h"
#include "common/utilities/tlogger.h"
#include "common/controlInterface/messageDialog.h"

#include <QDateTime>
#include <QStandardPaths>
#include <QProcess>
#include <QDir>
#include <QTimer>
#ifdef __APPLE__
#include <unistd.h>
#endif
namespace  control{
GeneralWidget::GeneralWidget(MessageProcessing *messageProcessing, PageWidget *parent)
    : PageWidget(parent),
      m_filePath(QString())
      //messageProcessing(nullptr)

      //m_translator(nullptr)
{
    setObjectName("GeneralWidget");
    m_displayName = QString("    ") + tr("General");
    m_messageProcessing = messageProcessing;
    connect(m_messageProcessing, &MessageProcessing::sendMsg2Update, this, &GeneralWidget::receiveMsgFromNetwork);
    init();
}
void GeneralWidget::init()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(32);
    mainLayout->setContentsMargins(24,12,24,12);
    this->setLayout(mainLayout);

    QFont font = this->font();
    font.setPixelSize(font.pixelSize()  + 4);
    font.setWeight(QFont::Bold);

    QVBoxLayout *languageLayout = new QVBoxLayout();
    languageLayout->setSpacing(12);
    languageLayout->setContentsMargins(0,0,0,0);
    m_languageLabel = new QLabel(tr("Language"),this);
    m_languageLabel->setFont(font);

    languageLayout->addWidget(m_languageLabel);

    m_languageCombox = new QComboBox(this);
    QListView *languageListview = new QListView();
    languageListview->setObjectName("languageListview");
    languageListview->setAttribute(Qt::WA_TranslucentBackground);
    m_languageCombox->setView(languageListview);
    //connect(m_languageCombox,SIGNAL(activated(int)),this,SLOT(languageComboxActived(int)));
    m_languageCombox->setToolTip(tr("Language will change after software restarts."));
//    languageCombox->setStyleSheet("QComboBox{border:1px solid ; border-color:#E7E7E9 ; border-radius:4px; padding:6px 3px 6px 3px  ;background: #F5F6F7;}"
//                                "QComboBox QAbstractItemView{border: 1px solid ; border-color:#E7E7E9 ; border-radius:4px; padding:0px 6px 0px 3px; selection-background-color: #61D37D; background: #F5F6F7; }"
//                                "QComboBox::down-arrow{width:15px; padding: 0px 0px 0px 0px; image:url(:/images/icon/fdm_an_icon_n.png);}"
//                                "QComboBox::drop-down{border:0px;}"
//                                "QComboBox::hover {border-color:#61D37D}"
//                                "QComboBox QAbstractItemView::item{height: 40px}");

    //connect(languageCombox,&QComboBox::currentIndexChanged,this,&GengralWidget::languageComboxActived);
    m_languageCombox->setObjectName("languageCombox");
    m_languageCombox->setFixedSize(300,45);
    m_languageList  = m_settings.getLanguageList();
    m_languageCombox->addItems(m_languageList);

    int currentIndex= m_settings.getCurrentLanguage();
    if(currentIndex <= 0 || currentIndex >= m_languageList.size()) {
        currentIndex = 0;
        m_settings.setCurrentLanguage(currentIndex);
    }
    m_languageCombox->setCurrentIndex(currentIndex);
    languageLayout->addWidget(m_languageCombox,0,Qt::AlignLeft);
    PluginMessageData lanData;
    lanData.from = AkConst::Plugin::AK_MAIN_WINDOW;
    lanData.dest = AkConst::Plugin::FDM_SEND2ALL;
    lanData.msg = AkConst::Msg::LANGUAGE_CHANGED;
    lanData.map.insert(AkConst::Param::LANGUAGE_STATE, m_settings.getCurrentLanguage());
    m_messageProcessing->sendMsg2Manager(lanData);

    QVBoxLayout *aiLayout = new QVBoxLayout();
    aiLayout->setSpacing(12);
    aiLayout->setContentsMargins(0,0,0,0);
    m_aiLabel = new QLabel(tr("Create AI File"),this);
    m_aiLabel->setFont(font);
    aiLayout->addWidget(m_aiLabel);

    m_aiCheckBox = new QCheckBox(tr("Create AI file when slicing."));
    m_aiCheckBox->setFixedHeight(20);
    m_aiCheckBox->setCheckable(true);

    m_aiCheckBox->setChecked(m_settings.getAiMode());
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_SEND2ALL;
    data.msg = AkConst::Msg::AIMODE_CHANGED;
    data.map.insert(AkConst::Param::AIMODE_STATE, m_settings.getAiMode());
    emit m_messageProcessing->sendMsg2Manager(data);

    aiLayout->addWidget(m_aiCheckBox);

    QVBoxLayout *versionLayout = new QVBoxLayout();
    versionLayout->setSpacing(12);
    versionLayout->setContentsMargins(0,0,0,0);
    m_versionLabel = new QLabel(tr("Version"),this);
    m_versionLabel->setFont(font);

    versionLayout->addWidget(m_versionLabel);
    QHBoxLayout *hlayout = new QHBoxLayout();

    QString versionStr = MeshLabApplication::appVer(false, false);
    QLabel *version = new QLabel(versionStr,this);
    hlayout->addWidget(version);

    m_checkButton = new QPushButton(tr("Check"),this);
    m_checkButton->setObjectName("checkButton");
    m_checkButton->setMinimumSize(QSize(50,22));
    connect(m_checkButton, &QPushButton::clicked, this, &GeneralWidget::checkButtonClicked);
    QFont fontCheckButton = this->font();
    fontCheckButton.setPixelSize( 12);
    m_checkButton->setFont(fontCheckButton);
    hlayout->addWidget(m_checkButton);

    m_checkText = new QLabel(this);
    hlayout->addWidget(m_checkText);
    hlayout->addStretch();

    m_updateCheckBox = new QCheckBox(tr("Auto-Check for Updates"));
    m_updateCheckBox->setFixedHeight(20);
    bool autoUpdateChecked = m_settings.getAutomaticallyUpdate();
    m_updateCheckBox->setChecked(autoUpdateChecked);
    //connect(m_updateCheckBox, &QCheckBox::stateChanged, this, &GeneralWidget::updateCheckBox);
    //m_updateCheckBox->setCheckable(true);
    MessageProcessing *tmpMessageProcessing = m_messageProcessing;
    auto autoUpdateCheck = [autoUpdateChecked, versionStr, tmpMessageProcessing]()
    {
        QTimer::singleShot(5000, [autoUpdateChecked, versionStr, tmpMessageProcessing]() {
            PluginMessageData updateData;
            updateData.from = AkConst::Plugin::AK_MAIN_WINDOW;
            updateData.dest = AkConst::Plugin::FDM_NETWORK;
            updateData.msg = AkConst::Msg::AUTO_UPDATE;
            updateData.map.insert(AkConst::Param::UPDATE_DO, autoUpdateChecked);
            updateData.map.insert(AkConst::Param::ANKERMAKE_VERSION, versionStr);
            emit tmpMessageProcessing->sendMsg2Manager(updateData);
        });
    };


    versionLayout->addLayout(hlayout);
    versionLayout->addWidget(m_updateCheckBox);

    mainLayout->addLayout(languageLayout);
    mainLayout->addLayout(aiLayout);
    mainLayout->addLayout(versionLayout);
    mainLayout->addStretch();

    QString recentVersion = m_settings.getRecentDownloadNetworkVersion();
    QString recentPath = m_settings.getRecentDownloadNetworkPath();
    qDebug() << "recentVersion: " << recentVersion << ", recentPath: " << recentPath << ", versionStr: " << versionStr;
    AkUtil::TDebug("Recent Version: " + recentVersion + ", Recent Path: " + recentPath);
    if(!recentVersion.isEmpty() && !recentPath.isEmpty() && recentVersion != versionStr)
    {
        QFileInfo fileInfo(recentPath);
        if(fileInfo.exists() && fileInfo.isFile() && fileInfo.suffix().toLower() == ".exe")
        {
            m_checkButton->setText(tr("Update"));
            m_checkButton->setProperty("type",Update);
            m_filePath = recentPath;
            m_updateFlag = true;
            QTimer::singleShot(3000, [this]() {
                forcedUpdateApp();
            });
        }
        else
        {
                autoUpdateCheck();
        }
    }
    else if(!recentPath.isEmpty() && recentPath.contains(versionStr, Qt::CaseInsensitive))
    {
        bool result = QFile::remove(recentPath);
        qDebug() << "remove " << recentPath << ", result: " << result;
        if(result)
        {
            AkUtil::TDebug(recentPath + "is removed.");
        }
        autoUpdateCheck();
    }
    else
    {
        autoUpdateCheck();
    }
}

void GeneralWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        m_displayName = QString("    ") + tr("General");
        if (m_languageLabel != nullptr) {
            m_languageLabel->setText(tr("Language"));
        }
        if (m_languageCombox != nullptr) {
             m_languageCombox->setToolTip(tr("Language will change after software restarts."));
        }
        if (m_aiLabel != nullptr) {
            m_aiLabel->setText(tr("Create AI File"));
        }
        if (m_aiCheckBox != nullptr) {
            m_aiCheckBox->setText(tr("Create AI file when slicing."));
        }
        if (m_checkButton != nullptr) {
            if (m_updateFlag) {
                m_checkButton->setText(tr("Update"));
            } else {
                 m_checkButton->setText(tr("Check"));
            }
        }
        if (m_versionLabel != nullptr) {
            m_versionLabel->setText(tr("Version"));
        }
        if (m_updateCheckBox != nullptr) {
            m_updateCheckBox->setText(tr("Auto-Check for Updates"));
        }
        m_languageList = m_settings.getLanguageList();
        for (int i = 0; i < m_languageList.size(); i++) {
            m_languageCombox->setItemText(i, m_languageList.at(i));
        }
    }
}

void GeneralWidget::closeWidget()
{
    int currentIndex= m_settings.getCurrentLanguage();
    m_languageCombox->setCurrentIndex(currentIndex);
    m_aiCheckBox->setChecked(m_settings.getAiMode());
    m_updateCheckBox->setChecked(m_settings.getAutomaticallyUpdate());
    emit closeButtonClicked();
}

void GeneralWidget::save()
{
    if(m_settings.getAiMode() != (m_aiCheckBox->checkState() == Qt::Checked ? true : false)) {

        PluginMessageData data;
        data.from = AkConst::Plugin::AK_MAIN_WINDOW;
        data.dest = AkConst::Plugin::FDM_SEND2ALL;
        data.msg = AkConst::Msg::AIMODE_CHANGED;
        data.map.insert(AkConst::Param::AIMODE_STATE, m_aiCheckBox->checkState() == Qt::Checked ? true : false);
        m_messageProcessing->sendMsg2Manager(data);
    }
    if(m_settings.getCurrentLanguage() != m_languageCombox->currentIndex()) {
        PluginMessageData lanData;
        lanData.from = AkConst::Plugin::AK_MAIN_WINDOW;
        lanData.dest = AkConst::Plugin::FDM_SEND2ALL;
        lanData.msg = AkConst::Msg::LANGUAGE_CHANGED;
        lanData.map.insert(AkConst::Param::LANGUAGE_STATE, m_languageCombox->currentIndex());
        m_messageProcessing->sendMsg2Manager(lanData);
    }
    m_settings.setCurrentLanguage(m_languageCombox->currentIndex());
    m_settings.setAiMode(m_aiCheckBox->checkState() == Qt::Checked ? true : false );
    m_settings.setAutomaticallyUpdate(m_updateCheckBox->checkState() == Qt::Checked ? true : false );
    Translator::instance()->loadLanguage(static_cast<Language>(m_languageCombox->currentIndex()));
    qint64 id = QDateTime::currentSecsSinceEpoch();
    QString versionStr = MeshLabApplication::appVer(false, false);
    int check = (m_updateCheckBox->checkState()  == Qt::Checked ? 1 : 0);
    PluginMessageData updateData;
    updateData.from = AkConst::Plugin::AK_MAIN_WINDOW;
    updateData.dest = AkConst::Plugin::FDM_NETWORK;
    updateData.msg = AkConst::Msg::AUTO_UPDATE;
    updateData.map.insert(AkConst::Param::UPDATE_DO, check);
    updateData.map.insert(AkConst::Param::ANKERMAKE_VERSION, versionStr);
    m_messageProcessing->sendMsg2Manager(updateData);
    emit saveButtonClicked();
    emit networkLanguageChangedSignal(m_languageCombox->currentIndex());
}

void GeneralWidget::manualUpdate()
{
    int autoCheck = (m_updateCheckBox->checkState()  == Qt::Checked ? 1 : 0);
    if (autoCheck)
    {
        return;
    }

    AkUtil::TDebug("start manualUpdate.");
    qint64 id = QDateTime::currentSecsSinceEpoch();
    QString versionStr = MeshLabApplication::appVer(false, false);
    PluginMessageData updateData;
    updateData.from = AkConst::Plugin::AK_MAIN_WINDOW;
    updateData.dest = AkConst::Plugin::FDM_NETWORK;
    updateData.msg = AkConst::Msg::MANUAL_UPDATE;
    updateData.map.insert(AkConst::Param::UPDATE_DO, 0);
    updateData.map.insert(AkConst::Param::ANKERMAKE_VERSION, versionStr);
    emit m_messageProcessing->sendMsg2Manager(updateData);
}

void GeneralWidget::clickCheckButton()
{
    emit m_checkButton->click();
}

void GeneralWidget::checkButtonClicked()
{
    QPushButton *button  = qobject_cast<QPushButton *>(sender());
    int type = button->property("type").toInt();
    qDebug() << "type: " << type;
    AkUtil::TDebug("type: " + QString::number(type));
    if(type == Install) {
        qint64 id = QDateTime::currentSecsSinceEpoch();
        QString versionStr = MeshLabApplication::appVer(false, false);

//        if(m_settings.getAutomaticallyUpdate())
//        {
//            MessageDialog dialog(tr("Notice"), tr("Close Auto-Update"), MessageDialog::BUTTONFLAG::YES);
//            int flag = dialog.exec();
//            if(flag == MessageDialog::BUTTONFLAG::YES)
//            {
//                m_updateCheckBox->setCheckState(Qt::CheckState::Unchecked);
//                m_settings.setAutomaticallyUpdate(m_updateCheckBox->checkState() == Qt::Checked ? true : false );
//                PluginMessageData updateData;
//                updateData.from = AkConst::Plugin::AK_MAIN_WINDOW;
//                updateData.dest = AkConst::Plugin::FDM_NETWORK;
//                updateData.msg = AkConst::Msg::AUTO_UPDATE;
//                updateData.map.insert(AkConst::Param::UPDATE_DO, -1);
//                updateData.map.insert(AkConst::Param::ANKERMAKE_VERSION, versionStr);
//                emit m_messageProcessing->sendMsg2Manager(updateData);
//            }
//            return;
//        }
        PluginMessageData updateData;
        updateData.from = AkConst::Plugin::AK_MAIN_WINDOW;
        updateData.dest = AkConst::Plugin::FDM_NETWORK;
        updateData.msg = AkConst::Msg::MANUAL_UPDATE;
        updateData.map.insert(AkConst::Param::UPDATE_DO, 0);
        updateData.map.insert(AkConst::Param::ANKERMAKE_VERSION, versionStr);
        emit m_messageProcessing->sendMsg2Manager(updateData);
    }
    else {
        AkUtil::TDebug("filePath: " + m_filePath);
        qDebug() << "filePath: " << m_filePath;
        if(m_filePath.isNull()) {
            return;
        }
        int index0 = m_filePath.lastIndexOf("/") + 1;
        QString fileName = m_filePath.mid(index0, m_filePath.length() - index0);
        int index1 = fileName.indexOf("AnkerMake_") + 10;
        QString tmpFile = fileName.mid(index1, fileName.length() - index1);
        int index2 = tmpFile.lastIndexOf(".");
        QString version = tmpFile.mid(0, index2);
        m_settings.setRecentDownloadNetworkVersion(version);
        m_settings.setRecentDownloadNetworkPath(m_filePath);
        qDebug() << "install version: " << version;
        auto messageDialog = new MessageDialog(tr("Check for Update"), QString(tr("AnkerMake %1 is ready, the changes will effective once you restart the app.")).arg(version),
                                               MessageDialog::NO|MessageDialog::YES,this);
        int ret = messageDialog->exec();
        if (ret == MessageDialog::YES) {
            update_app(m_filePath);
        }
    }
}

void GeneralWidget::update_app(const QString& fileName)
{
#ifdef __APPLE__
     //TODO:
    QString cmdStr = QString("open \"") + fileName + QString("\"");
    system(cmdStr.toStdString().c_str());
    
    //emit unloadPluginsSignal();
    emit closeMainWindow();

//    sleep(1);
//    QApplication::exit(0);
#else
#ifdef Q_OS_WIN
      //auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
      // update_app_dir = QDir::toNativeSeparators(writableLocation + "/AnkerMakeNetWork/Setup.exe");
      //QString local_install_dir = QDir::toNativeSeparators(writableLocation + "/../../AnkerMake");
      QString update_dir = QDir::toNativeSeparators(fileName);
      //std::unique_ptr<QProcess> cmd(new QProcess);
      QProcess *cmd = new QProcess();
      QStringList args;
      args = QStringList({"-Command", QString("Start-Process %1 /S -Verb runAs").arg(update_dir)});
      AkUtil::TDebug(update_dir);
      cmd->start("powershell", args);
      //qDebug()<<update_app_dir<<args;
#else
    //TODO :linux
#endif

#endif
}


void GeneralWidget::receiveMsgFromNetwork(PluginMessageData metadata)
{
    AkUtil::TFunction("");
    if(metadata.from == AkConst::Plugin::FDM_NETWORK &&
            metadata.dest == AkConst::Plugin::AK_MAIN_WINDOW &&
            metadata.msg == AkConst::Msg::MANUAL_UPDATE)
    {
        AkUtil::TDebug("Manual Update.");
        QString version = metadata.map.find(AkConst::Param::ANKERMAKE_VERSION).value().toString();
        QString fileName = metadata.map.find(AkConst::Param::ANKERMAKE_UPDATE_EXE).value().toString();
//        int index1 = fileName.indexOf("AnkerMake_") + 10;
//        QString tmpFile = fileName.mid(index1, fileName.length() - index1);
//        int index2 = tmpFile.indexOf("_");
//        QString version = tmpFile.mid(index1, index2 - index1);
        m_settings.setRecentDownloadNetworkVersion(version);
        m_settings.setRecentDownloadNetworkPath(fileName);
        m_checkButton->setText(tr("Update"));
        m_updateFlag = true;
        m_checkButton->setProperty("type",Update);
        m_filePath = fileName;
        AkUtil::TDebug("Local Version: " + MeshLabApplication::appVer(false, false) + ", Current Version: " + version);
        AkUtil::TDebug("FilePath: " + m_filePath);
    }
    else if(metadata.from == AkConst::Plugin::FDM_NETWORK &&
            metadata.dest == AkConst::Plugin::AK_MAIN_WINDOW &&
            metadata.msg == AkConst::Msg::AUTO_UPDATE)
    {
        AkUtil::TDebug("Auto Update.");
        QString version = metadata.map.find(AkConst::Param::ANKERMAKE_VERSION).value().toString();
        QString fileName = metadata.map.find(AkConst::Param::ANKERMAKE_UPDATE_EXE).value().toString();
//        int index1 = fileName.indexOf("AnkerMake_") + 10;
//        QString tmpFile = fileName.mid(index1, fileName.length() - index1);
//        int index2 = tmpFile.indexOf("_");
//        QString version = tmpFile.mid(index1, index2 - index1);
        m_settings.setRecentDownloadNetworkVersion(version);
        m_settings.setRecentDownloadNetworkPath(fileName);
        m_checkButton->setText(tr("Update"));
        m_updateFlag = true;
        m_checkButton->setProperty("type",Update);
        m_filePath = fileName;
        AkUtil::TDebug("Local Version: " + MeshLabApplication::appVer(false, false) + ", Current Version: " + version);
        AkUtil::TDebug("FilePath: " + m_filePath);
    }
    else if(metadata.from == AkConst::Plugin::FDM_NETWORK &&
            metadata.dest == AkConst::Plugin::AK_MAIN_WINDOW &&
            metadata.msg == AkConst::Msg::CHECK_UPDATE_TEXT)
    {
        if(metadata.map.contains(AkConst::Param::CHECK_UPDATE_TEXT))
        {
            QString text = metadata.map.find(AkConst::Param::CHECK_UPDATE_TEXT).value().toString();
            m_checkText->setText(text);
            qDebug() << "CHECK_UPDATE_TEXT: " << text;
;        }
    }

    if (metadata.from == AkConst::Plugin::FDM_NETWORK &&
       metadata.dest == AkConst::Plugin::AK_MAIN_WINDOW &&
       (metadata.msg == AkConst::Msg::MANUAL_UPDATE || metadata.msg == AkConst::Msg::AUTO_UPDATE)) {
        forcedUpdateApp();
    }
}


void GeneralWidget::doPluginUnloaded()
{
#ifdef __APPLE__
    AkUtil::TDebug("plugin unloaded finished. exit application");
    QApplication::exit(0);
#endif
}

void GeneralWidget::forcedUpdateApp()
{
    AkUtil::TDebug("filePath: " + m_filePath);
    qDebug() << "filePath: " << m_filePath;
    if(m_filePath.isNull()) {
        return;
    }

    int index0 = m_filePath.lastIndexOf("/") + 1;
    QString fileName = m_filePath.mid(index0, m_filePath.length() - index0);
    int index1 = fileName.indexOf("AnkerMake_") + 10;
    QString tmpFile = fileName.mid(index1, fileName.length() - index1);
    int index2 = tmpFile.lastIndexOf(".");
    QString version = tmpFile.mid(0, index2);
    tmpFile.lastIndexOf(".");
    m_settings.setRecentDownloadNetworkVersion(version);
    m_settings.setRecentDownloadNetworkPath(m_filePath);
    qDebug() << "install version: " << version;
    QSettings settings;
    qDebug() << "forcedUpdateApp: " << settings.value("is_forced").toBool();
    if (settings.value("is_forced").toBool()) {
        MessageDialog *messageDialog = new MessageDialog(tr("Check for Update"), QString(tr("AnkerMake %1 is ready, the changes will effective once you restart the app.")).arg(version),
                                                         MessageDialog::NO|MessageDialog::YES);
        int ret = messageDialog->exec();
        if (ret == MessageDialog::YES) {
            update_app(m_filePath);
        }
    }
}


}
