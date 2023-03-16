#include "materialwidget.h"
#include "filemanage/fdmqmlengine.h"

#include <QFontDatabase>
namespace control {
MaterialWidget::MaterialWidget(PageWidget *m_servicent)
    : PageWidget(m_servicent),
      m_service(nullptr),
      m_oldName(QString()),
      m_messageDialog(nullptr)
{
    m_displayName = QString("    ") + tr("Material");
    m_quickView = new QQuickView(FdmQmlEngine::instance(), nullptr);
    QWidget *widget = QWidget::createWindowContainer(m_quickView,this);
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

    m_service = FdmMaterialProfileService::instance();
    if(m_service) {
        m_quickView->rootContext()->setContextProperty("materialParameter", m_service);
    }
    QStringList keyList;
    keyList << "material_flow"  << "material_print_temperature" <<"material_bed_temperature"
            <<"cool_fan_speed" << "material_flow_layer_0" <<"material_print_temperature_layer_0"
            <<"material_bed_temperature_layer_0";

    FdmParamNode *node =FdmQmlSourceTree::instance().qmlSourceTree_Preference();
    QVariantMap list2 = findParameters(node,"material",keyList);

    if(!list2.isEmpty()) {
        m_quickView->rootContext()->setContextProperty("materialList", QVariant::fromValue(list2));
    }
    m_quickView->setSource(QUrl::fromLocalFile(":/qml/PreferenceSettings/MaterialView.qml"));

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentMaterialChanged(QString)),m_service,SLOT(currentMaterialChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomMaterial(QString)),m_service,SLOT(deleteCustomMaterial(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateMaterial(QString)),m_service,SLOT(createMaterial(QString)), Qt::QueuedConnection);
    //rename
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomMaterial(QString)),this,SLOT(renameCustomMaterial(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(materialRename(QString,QString)),m_service,SLOT(renameCustomMaterial(QString,QString)), Qt::QueuedConnection);
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->addWidget(widget);
}

MaterialWidget::~MaterialWidget()
{
    if(m_messageDialog) {
        delete  m_messageDialog;
        m_messageDialog = nullptr;
    }
}

QVariantMap MaterialWidget::findParameters(FdmParamNode *root,QString keyNodeName, QStringList list)
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
            listNode.append(node->getQmlObj());
        }
    }
    return map;
}

void MaterialWidget::getDefaultNewName(const QStringList &list, const QString &name)
{
    m_newName = QString("#").append(name);
    if(list.contains(m_newName)) {
        getDefaultNewName(list,m_newName);
    }
}

void MaterialWidget::renameCustomMaterial(const QString &oldName)
{
    //^[^?v \  * | "" < > : /]{1,256}$

    // [\\\\/:*?\"<>|]
    m_oldName = oldName;
    if(!m_messageDialog) {
        m_messageDialog = new MessageDialog(tr("Name These Parameters"),tr("Input a name before you save."),MessageDialog::CANCEL|MessageDialog::SAVE,this);
        connect(m_messageDialog,&MessageDialog::buttonClick,this,&MaterialWidget::textValid);
        QRegExp rx("[^\\\\/:*?\"<>|\\s]{1,128}$");
         //QRegExp rx("[^\\s]{1,128}$");
        QRegExpValidator *reg = new QRegExpValidator(rx,this);
        m_messageDialog->setValidator(reg);
        m_messageDialog->setAutoClosed(false);
        // QValidator validator();
    }
    getDefaultNewName(m_service->getAllMaterialList(),oldName);
    m_messageDialog->setEditText(m_newName);
    m_messageDialog->setEditMode(true);
    m_messageDialog->exec();
}

void MaterialWidget::textValid(int flag)
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
    //qDebug() << " save new name"  << newname << " old name =" << m_oldName;

    QStringList list = m_service->getAllMaterialList();
    if(list.contains(newname)) {
        m_messageDialog->setWarning(tr("Name already exists. Try another."));
        return;
    }
    emit materialRename(m_oldName,newname);
    delete  m_messageDialog;
    m_messageDialog = nullptr;
}

void MaterialWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        m_displayName = QString("    ") + tr("Material");
        if (m_quickView) {
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentMaterialChanged(QString)),m_service,SLOT(currentMaterialChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomMaterial(QString)),m_service,SLOT(deleteCustomMaterial(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateMaterial(QString)),m_service,SLOT(createMaterial(QString)));
            //rename
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomMaterial(QString)),this,SLOT(renameCustomMaterial(QString)));
            disconnect(this,SIGNAL(materialRename(QString,QString)),m_service,SLOT(renameCustomMaterial(QString,QString)));
            m_quickView->source().clear();
            m_quickView->setSource(QUrl::fromLocalFile(":/qml/PreferenceSettings/MaterialView.qml"));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCurrentMaterialChanged(QString)),m_service,SLOT(currentMaterialChanged(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlDeleteCustomMaterial(QString)),m_service,SLOT(deleteCustomMaterial(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlCreateMaterial(QString)),m_service,SLOT(createMaterial(QString)), Qt::QueuedConnection);
            //rename
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlRenameCustomMaterial(QString)),this,SLOT(renameCustomMaterial(QString)),Qt::QueuedConnection);
            connect(this,SIGNAL(materialRename(QString,QString)),m_service,SLOT(renameCustomMaterial(QString,QString)), Qt::QueuedConnection);
        }
    }
}



}
