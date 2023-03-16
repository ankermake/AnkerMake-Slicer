#include "machinewidget.h"

#include "filemanage/fdmqmlengine.h"
#include "service/fdmmachineprofileservice.h"

namespace control {
MachineWidget::MachineWidget(PageWidget *parent)
    : PageWidget(parent),
      m_service(nullptr),
      m_oldName(QString()),
      m_messageDialog(nullptr)
{
    //set page name
    m_displayName = QString("    ") + tr("Machine");

    //load MachineView.qml
    m_quickView = new QQuickView(FdmQmlEngine::instance(), nullptr);
    QWidget *widget = QWidget::createWindowContainer(m_quickView,this);
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_service = FdmMachineProfileService::instance();
    if(m_service) {
        m_quickView->rootContext()->setContextProperty("machineParameter", m_service);
    }

    QStringList keyList;
    keyList << "machine_width"  << "machine_depth" << "machine_height" <<
               "machine_shape" << "machine_center_is_zero" << "machine_heated_bed"<<
               "machine_heated_build_volume" << "machine_gcode_flavor" << "machine_extruder_count" <<
               "machine_use_extruder_offset_to_offset_coords" << "machine_start_gcode" <<"machine_end_gcode" <<
               "machine_nozzle_size" << "machine_nozzle_offset_x" << "machine_nozzle_offset_y" <<
               "machine_extruder_cooling_fan_number" << "machine_extruder_start_code" << "machine_extruder_end_code";

    FdmParamNode *node =FdmQmlSourceTree::instance().qmlSourceTree_Preference();
    if(node) {
        QVariantMap list2 = findParameters(node,"machine_settings",keyList);

     qDebug() << " value ====" << list2.find("machine_extruder_count")->toList();
        if(!list2.isEmpty()) {
            m_quickView->rootContext()->setContextProperty("machineList", QVariant::fromValue(list2));
        }
    }
    m_quickView->setSource(QUrl::fromLocalFile(":/qml/PreferenceSettings/MachineView.qml"));

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentMachineChanged(QString)),m_service,SLOT(currentMachineChanged(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomMachine(QString)),m_service,SLOT(deleteCustomMachine(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateMachine(QString)),m_service,SLOT(createMachine(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentTabChanged(int)),m_service,SLOT(onTabSwitched(int)));

    //rename
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomMachine(QString)),this,SLOT(renameCustomMachine(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(machineRename(QString,QString)),m_service,SLOT(renameCustomMachine(QString,QString)), Qt::QueuedConnection);

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(widget);
}

MachineWidget::~MachineWidget()
{
    if(m_messageDialog) {
        delete  m_messageDialog;
        m_messageDialog = nullptr;
    }
}

QVariantMap MachineWidget::findParameters(FdmParamNode *root, QString keyNodeName, QStringList list)
{
    if(!root) {
        return QVariantMap();
    }
    QVariantMap map;
    QList<FdmQml_Param *> listNode;
    FdmParamNode * materialnode = root->findChildNode(keyNodeName);
    for(const auto &name : list) {
        auto node = materialnode->findChildNode(name);
        if(!node) {
            node = root->findChildNode(name);
        }
        if(node) {
            map.insert(name,QVariant::fromValue( node->getQmlObj()));
            if(name == "machine_extruder_count") {
            node->dumpParam();
            }
            listNode.append(node->getQmlObj());
        }
    }
    return map;
}

void MachineWidget::getDefaultNewName(const QStringList &list, const QString &name)
{
    m_newName = QString("#").append(name);
    if(list.contains(m_newName)) {
        getDefaultNewName(list,m_newName);
    }
}

void MachineWidget::renameCustomMachine(const QString &oldName)
{
    m_oldName = oldName;
    if(!m_messageDialog) {
        m_messageDialog = new MessageDialog(tr("Name These Parameters"),tr("Input a name before you save."),MessageDialog::CANCEL|MessageDialog::SAVE,this);
        connect(m_messageDialog,&MessageDialog::buttonClick,this,&MachineWidget::textValid);
       // QRegExp rx("^[^?\  * | "" < > : /]{1,128}$");
          QRegExp rx("[^\\\\/:*?\"<>|\\s]{1,128}$");
        QRegExpValidator *reg = new QRegExpValidator(rx,this);
        m_messageDialog->setValidator(reg);
         m_messageDialog->setAutoClosed(false);
        // QValidator validator();
    }
    getDefaultNewName(m_service->getAllMachineList(),oldName);
    m_messageDialog->setEditText(m_newName);
    m_messageDialog->setEditMode(true);
    m_messageDialog->exec();
}

void MachineWidget::textValid(int flag)
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
   // qDebug() << " save new name"  << newname << " old name =" << m_oldName;

    QStringList list = m_service->getAllMachineList();
    if(list.contains(newname)) {
        m_messageDialog->setWarning(tr("Name already exists. Try another."));
        return;
    }
    emit machineRename(m_oldName,newname);
    delete  m_messageDialog;
    m_messageDialog = nullptr;
}

void MachineWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        m_displayName = QString("    ") + tr("Machine");
        disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentMachineChanged(QString)),m_service,SLOT(currentMachineChanged(QString)));
        disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomMachine(QString)),m_service,SLOT(deleteCustomMachine(QString)));
        disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateMachine(QString)),m_service,SLOT(createMachine(QString)));
        disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentTabChanged(int)),m_service,SLOT(onTabSwitched(int)));

        //rename
        disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomMachine(QString)),this,SLOT(renameCustomMachine(QString)));
        disconnect(this,SIGNAL(machineRename(QString,QString)),m_service,SLOT(renameCustomMachine(QString,QString)));
        m_quickView->source().clear();
        m_quickView->setSource(QUrl::fromLocalFile(":/qml/PreferenceSettings/MachineView.qml"));
        connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentMachineChanged(QString)),m_service,SLOT(currentMachineChanged(QString)), Qt::QueuedConnection);
        connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomMachine(QString)),m_service,SLOT(deleteCustomMachine(QString)), Qt::QueuedConnection);
        connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateMachine(QString)),m_service,SLOT(createMachine(QString)), Qt::QueuedConnection);
        connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentTabChanged(int)),m_service,SLOT(onTabSwitched(int)));

        //rename
        connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomMachine(QString)),this,SLOT(renameCustomMachine(QString)),Qt::QueuedConnection);
        connect(this,SIGNAL(machineRename(QString,QString)),m_service,SLOT(renameCustomMachine(QString,QString)), Qt::QueuedConnection);
    }
    QWidget::changeEvent(e);
}

}
