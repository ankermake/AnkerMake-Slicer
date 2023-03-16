#include "parameterswidget.h"

namespace control {
ParametersWidget::ParametersWidget(PageWidget *parent)
    : PageWidget(parent),
      m_service(nullptr),
      m_oldName(QString()),
      m_messageDialog(nullptr),
      m_newName(QString())
{
    m_displayName = QString("    ") + tr("Parameters");
    m_quickView = new QQuickView(FdmQmlEngine::instance(), nullptr);

    QWidget *widget = QWidget::createWindowContainer(m_quickView,this);
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

    m_service = FdmParameterProfileService::instance();
    m_quickView->rootContext()->setContextProperty("parameter",m_service);

    fdmsettings::ParamListModel *preferenceModel = new fdmsettings::ParamListModel();

    
    //QString anker_expert = ":/curadef/setting_visibility/anker_expert.cfg";
    QString anker_expert = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/back_logic/anker_expert.cfg");

    QList<FdmParamNode *> configList = FdmQmlSourceTree::instance().loadParamListForPreference(anker_expert);
    if(!configList.isEmpty()) {
        preferenceModel->setListDatas(configList);
         m_quickView->rootContext()->setContextProperty("preferenceModel",preferenceModel);
    }

    m_quickView->setSource(QUrl::fromLocalFile(":/qml/PreferenceSettings/ParametersView.qml"));
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentParameterChanged(QString)),m_service,SLOT(currentParameterChanged(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomParameter(QString)),m_service,SLOT(deleteCustomParameter(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateParameter(QString)),m_service,SLOT(createParameter(QString)),Qt::QueuedConnection);

    //rename
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomParameter(QString)),this,SLOT(renameCustomParameter(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlImportButtonClicked()),this,SLOT(importButtonClicked()), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlExportButtonClicked(QString)),this,SLOT(exportButtonClicked(QString)), Qt::QueuedConnection);

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlMachineNameChanged(QString)),m_service,SLOT(onMachineNameChanged(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlMaterialNameChanged(QString)),m_service,SLOT(onMaterialNameChanged(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlNozzleSizeChanged(QString)),m_service,SLOT(onNozzleSizeChanged(QString)), Qt::QueuedConnection);

    connect(this,SIGNAL(parameterRename(QString,QString)),m_service,SLOT(renameCustomParameter(QString,QString)));
    connect(this,SIGNAL(importParameter(QString)),m_service,SLOT(onImportBtnClicked(QString)));
    connect(this,SIGNAL(exportParameter(QString)),m_service,SLOT(onExportBtnClicked(QString)));

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(widget);
}

ParametersWidget::~ParametersWidget()
{
    if(m_messageDialog) {
        delete  m_messageDialog;
        m_messageDialog = nullptr;
    }
}

void ParametersWidget::getDefaultNewName(const QStringList &list, const QString &name)
{
    //qDebug() << " getDefaultNewName list = " << list  << " name ==" << name;
    m_newName = QString("#").append(name);
    if(list.contains(m_newName)) {
        getDefaultNewName(list,m_newName);
    }
   // return newName;
}

void ParametersWidget::renameCustomParameter(const QString &oldName)
{
    m_oldName = oldName;
    if(!m_messageDialog) {
        m_messageDialog = new MessageDialog(tr("Name These Parameters"),tr("Input a name before you save."),MessageDialog::CANCEL|MessageDialog::SAVE,this);
        connect(m_messageDialog,&MessageDialog::buttonClick,this,&ParametersWidget::textValid);
        //QRegExp rx("^[^?v \  * | "" < > : /]{1,128}$");
        QRegExp rx("[^\\\\/:*?\"<>|\\s]{1,128}$");
        QRegExpValidator *reg = new QRegExpValidator(rx,this);
        m_messageDialog->setValidator(reg);
        m_messageDialog->setAutoClosed(false);
    }
    getDefaultNewName(m_service->getAllParameterList(),oldName);
    m_messageDialog->setEditText(m_newName);
    m_messageDialog->setEditMode(true);
    m_messageDialog->exec();
}

void ParametersWidget::textValid(int flag)
{
    if(!m_messageDialog || !m_service) {
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
   // qDebug() << " save new name"  << newname << " old name =" << m_oldName;

    QStringList list = m_service->getAllParameterList();
    if(list.contains(newname)) {
        m_messageDialog->setWarning(tr("Name already exists. Try another."));
        return;
    }
    emit parameterRename(m_oldName,newname);
    delete  m_messageDialog;
    m_messageDialog = nullptr;
}

void ParametersWidget::importButtonClicked()
{
    QString lastPath = QApplication::applicationDirPath();
    QString filePath= QFileDialog::getOpenFileName(this, tr("Import"), lastPath, "files (*.ini) ;; files(*)");

    if(filePath.isEmpty()) {
        return;
    }
   // qDebug() << " importButtonClicked  filepath = " << filePath;
    emit importParameter(filePath);
}

void ParametersWidget::exportButtonClicked(QString name)
{
    QString lastPath = QApplication::applicationDirPath() + "/" + name + ".ini";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export"), lastPath, "files (*.ini) ;; files(*)");
    if(fileName.isEmpty()) {
        return;
    }
    //qDebug() << "exportButtonClicked  name ====  " << name  << " filepath = " << fileName;
    emit exportParameter(fileName);
}

void ParametersWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        m_displayName = QString("    ") + tr("Parameters");
        if (m_quickView) {
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentParameterChanged(QString)),m_service,SLOT(currentParameterChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomParameter(QString)),m_service,SLOT(deleteCustomParameter(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateParameter(QString)),m_service,SLOT(createParameter(QString)));

            //rename
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomParameter(QString)),this,SLOT(renameCustomParameter(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlImportButtonClicked()),this,SLOT(importButtonClicked()));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlExportButtonClicked(QString)),this,SLOT(exportButtonClicked(QString)));

            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlMachineNameChanged(QString)),m_service,SLOT(onMachineNameChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlMaterialNameChanged(QString)),m_service,SLOT(onMaterialNameChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlNozzleSizeChanged(QString)),m_service,SLOT(onNozzleSizeChanged(QString)));

            disconnect(this,SIGNAL(parameterRename(QString,QString)),m_service,SLOT(renameCustomParameter(QString,QString)));
            disconnect(this,SIGNAL(importParameter(QString)),m_service,SLOT(onImportBtnClicked(QString)));
            disconnect(this,SIGNAL(exportParameter(QString)),m_service,SLOT(onExportBtnClicked(QString)));
            m_quickView->source().clear();
            m_quickView->setSource(QUrl::fromLocalFile(":/qml/PreferenceSettings/ParametersView.qml"));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentParameterChanged(QString)),m_service,SLOT(currentParameterChanged(QString)), Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomParameter(QString)),m_service,SLOT(deleteCustomParameter(QString)), Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateParameter(QString)),m_service,SLOT(createParameter(QString)),Qt::QueuedConnection);

            //rename
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomParameter(QString)),this,SLOT(renameCustomParameter(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlImportButtonClicked()),this,SLOT(importButtonClicked()), Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlExportButtonClicked(QString)),this,SLOT(exportButtonClicked(QString)), Qt::QueuedConnection);

            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlMachineNameChanged(QString)),m_service,SLOT(onMachineNameChanged(QString)), Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlMaterialNameChanged(QString)),m_service,SLOT(onMaterialNameChanged(QString)), Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlNozzleSizeChanged(QString)),m_service,SLOT(onNozzleSizeChanged(QString)), Qt::QueuedConnection);

            connect(this,SIGNAL(parameterRename(QString,QString)),m_service,SLOT(renameCustomParameter(QString,QString)));
            connect(this,SIGNAL(importParameter(QString)),m_service,SLOT(onImportBtnClicked(QString)));
            connect(this,SIGNAL(exportParameter(QString)),m_service,SLOT(onExportBtnClicked(QString)));

        }
    }
}

}
