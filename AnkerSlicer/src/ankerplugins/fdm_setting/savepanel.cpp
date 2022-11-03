#include "savepanel.h"
#include <QCoreApplication>
#include <QHBoxLayout>
#include "common/ak_const.h"
#include "service/fdmparameterprofileservice.h"
#include "profilemanage/fdmparameterprofilemanager.h"

#include <QDebug>

using namespace control;

SavePanel::SavePanel(QWidget *parent): BubbleWidget(parent),m_messageDialog(nullptr)
{
    //auto vlayout = new QVBoxLayout(this);
    this->setFixedHeight(40);
    this->setBackgroundColor(QColor(32, 33, 35));
    auto hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(12);
    hLayout->setContentsMargins(24,0,24,0);
    this->setRadius(0);
    //AkProgressBar->setStyleSheet("background-color:rgb(255, 255, 255);");
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("SavePanel"));

    QFont font = this->font();
    font.setPixelSize(font.pixelSize() + 2);

    saveBtn = new QPushButton(this);
    saveBtn->setFont(font);
    saveBtn->setMinimumHeight(40);
    saveBtn->setObjectName(QString::fromUtf8("Save Parameters"));
    saveBtn->setText(tr("Save"));
    connect(saveBtn, &QPushButton::clicked,this, &SavePanel::save);

    saveAsBtn = new QPushButton(this);
    saveAsBtn->setFont(font);
    saveAsBtn->setObjectName(QString::fromUtf8("Save As Parameters"));
    saveAsBtn->setMinimumHeight(40);
    saveAsBtn->setText(tr("Save As"));
    connect(saveAsBtn, &QPushButton::clicked,this, &SavePanel::doSaveAsClick);

    hLayout->addWidget(saveBtn);
    hLayout->addWidget(saveAsBtn);

    
    saveBtn->show();
    saveAsBtn->show();
}

void SavePanel::init(QString profileName)
{
    currentProfileName = profileName;
    doParameterChanged(profileName);
}


void SavePanel::doParameterChanged(QString name)
{
    currentProfileName = name;
    qDebug() << "doParameterChanged " << name;
    if (currentProfileName  == AkConst::ProfileName::EXPERT_MODE)
    {
        this->show();
        saveBtn->hide();
        saveAsBtn->show();
    }
    else if  (currentProfileName  == AkConst::ProfileName::SIMPLE_MODE)
    {
        saveBtn->hide();
        saveAsBtn->hide();
        this->hide();
    }
    else
    {
        this->show();
        saveBtn->show();
        saveAsBtn->show();
    }
}

void SavePanel::doPanelShow()
{
    if  (currentProfileName  != AkConst::ProfileName::SIMPLE_MODE)
    {
        this->show();
    }
}
void SavePanel::doPanelHide()
{
    this->hide();
}


//simplemode  -> preview;
//expdertmode -> save as + preview
//custom      -> save&save as&preview
void SavePanel::doSliceSuccess()
{
    doParameterChanged(currentProfileName);
}

void SavePanel::doSliceFailed()
{

}

void SavePanel::doSaveAsClick()
{
 qDebug() << " enter doSaveAsClick";
    
    if(!m_messageDialog) {
        m_messageDialog = new MessageDialog(tr("Save As"),tr("Input a name before you save."),MessageDialog::CANCEL|MessageDialog::SAVE,this);
        qDebug() << "m_messageDialog new suc ";
        connect(m_messageDialog,&MessageDialog::buttonClick,this,&SavePanel::textValid);
        QRegExp rx("^[^?v \  * | "" < > : /]{1,128}$");
        QRegExpValidator *reg = new QRegExpValidator(rx,this);
        m_messageDialog->setValidator(reg);
        m_messageDialog->setAutoClosed(false);
        
        auto currentName = FdmParameterProfileManager::Instance().getRightRealTimeProfile()->getProfileName();
        QString recommandName = FdmParameterProfileManager::Instance().getValidName(currentName);
        qDebug() << "recommandName is " << recommandName;
        m_messageDialog->setEditText(recommandName);
    }

    m_messageDialog->setEditMode(true);
    m_messageDialog->exec();
}


void SavePanel::textValid(int flag)
{
     qDebug() << " enter textValid";
    if(!m_messageDialog) {
        return;
    }
    if(flag == MessageDialog::CANCEL) {
        delete  m_messageDialog;
        m_messageDialog = nullptr;
        return;
    }
    QString newname = m_messageDialog->editText() ;
    if(newname.isEmpty()) {
        m_messageDialog->setWarning(tr("Input a name."));
        return;
    }
    QRegExp rx("^[^?v \  * | "" < > : /]{1,128}$");
    auto pos = rx.indexIn(newname);
    if (pos < 0)
    {
        m_messageDialog->setWarning(tr("Name is longer than 128 characters. Create a shorter one."));
        return;
    }

    //qDebug() << " save new name"  << newname << " old name =" << m_oldName;

    QStringList list = FdmParameterProfileService::instance()->getAllParameterList();
    if(list.contains(newname)) {
        m_messageDialog->setWarning(tr("The Name Already Exists. Please Try Another Name."));
        return;
    }
    //emit materialRename(m_oldName,newname);
    qDebug() << " emit save as"  << newname;
    emit saveAs(newname);
    if (nullptr != m_messageDialog)
    {
        delete  m_messageDialog;
        m_messageDialog = nullptr;
    }

}

void SavePanel::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (saveAsBtn != nullptr) {
            saveAsBtn->setText(tr("Save As Parameters"));
        }
    }
}
