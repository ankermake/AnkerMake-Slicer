#include "fdmparamsettingswidget.h"

#include <QDir>

namespace fdmsettings {

FdmParamSettingsWidget::FdmParamSettingsWidget(QWidget *parent) :
    QWidget(parent),
    model(new ParamListModel),
    treeModel(new FdmSettingsTreeModel),
    m_toolTip(new ToolTip()),
    m_timer(new QTimer(this))
{
    connect(m_timer,&QTimer::timeout,this,&FdmParamSettingsWidget::showBubbleTip);
    //QQuickView  *quickView = new QQuickView(FdmQmlEngine::instance(), nullptr);

    QQuickWidget  *quickView = new QQuickWidget(FdmQmlEngine::instance(),this);
   // QWidget *widget = QWidget::createWindowContainer(quickView,this);
    quickView->setResizeMode(QQuickWidget::SizeRootObjectToView);

    
    //QString anker_expert = ":/curadef/setting_visibility/anker_expert.cfg";
    QString anker_expert = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/back_logic/anker_expert.cfg");

    reload(anker_expert);
    quickView->engine()->rootContext() ->setContextProperty("paramModel", model);
    quickView->engine()->rootContext() ->setContextProperty("FdmParamSettingsWidget", this);
    auto  parameter = FdmRightParameterService::instance();

    quickView->engine()->rootContext() ->setContextProperty("rightParameter", parameter);

    quickView->setSource(QUrl::fromLocalFile(":/qml/FdmParameter/SettingView.qml"));

    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlSettingCurrentMachineChanged(QString)),parameter,SLOT(onMachineNameChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlSettingCurrentMaterialChanged(QString)),parameter,SLOT(onMaterialNameChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlSettingCurrentParameterChanged(QString)),parameter,SLOT(onCurrentSelectedNameChanged(QString)), Qt::QueuedConnection);

    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlOpenMachinePreference(QString)),this,SLOT(openMachinePreference(QString)),Qt::QueuedConnection);
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlOpenMaterialPreference(QString)),this,SLOT(openMaterialPreference(QString)),Qt::QueuedConnection);
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlOpenParameterPreference(QString)),this,SLOT(openParameterPreference(QString)), Qt::QueuedConnection);

    connect(this,SIGNAL(currentIndexClicked(AkConst::EWidgetType,QString)),parameter,SLOT(onSetBtnClicked(AkConst::EWidgetType,QString)));

    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlResetParameter()),this,SLOT(resertButtonClicked()));

     connect(this,SIGNAL(resertParameter()),parameter,SLOT(onResetBtnClicked()));

    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlAiQualityCurrentIdxChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlLayerHeightCurrentIndexChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlInfillCurrentIndexChanged(int)),parameter,SLOT(onAiInfillDensityIdxChanged(int)));
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlGlobalSupportChanged(int)),parameter,SLOT(onGlobalSupportStateChanged(int)));
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlGenerateAdhesionChanged(int)),parameter,SLOT(onGenerateAdhesionStateChanged(int)));
    connect((QObject *)(quickView->rootObject()),SIGNAL(qmlNozzleSizeCurrentIndexChanged(QString)),parameter,SLOT(onNozzleSizeChanged(QString)), Qt::QueuedConnection);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    //layout->setc(0);
    this->setLayout(layout);
    layout->addWidget(quickView);
}

FdmParamSettingsWidget::~FdmParamSettingsWidget()
{
    if(model) {
        delete  model;
        model = nullptr;
    }
}

//":/curadef/setting_visibility/anker_expert.cfg"
void FdmParamSettingsWidget::reload(const QString &fileName)
{
    QList<FdmParamNode *> configList = FdmQmlSourceTree::instance().loadParamList(fileName);
    if(configList.isEmpty()) {
        return;
    }
    model->setListDatas(configList);
}

void FdmParamSettingsWidget::showToolTip(const QString &titlte, const QString &description, const QString &affects, const QString &affectedBy, QPoint point)
{
   // qDebug() << "description == " << description << "affects ==" << affects << "affectedBy ==" << affectedBy << "point =" << point;
    m_timer->start(1000);
    m_point = point;
    m_toolTip->setDescription(titlte, description, affects, affectedBy);
}

void FdmParamSettingsWidget::hideToolTip()
{
    m_timer->stop();
    m_toolTip->hide();
}


void FdmParamSettingsWidget::openMachinePreference(const QString &name)
{
    if(!name.isEmpty()) {
        emit  currentIndexClicked(AkConst::EWidgetType::Machine,name);
    }
    emit openPreferenceIndex(AkConst::EWidgetType::Machine);
}

void FdmParamSettingsWidget::openMaterialPreference(const QString &name)
{
    if(!name.isEmpty()) {
        emit  currentIndexClicked(AkConst::EWidgetType::Material,name);
    }
    emit openPreferenceIndex(AkConst::EWidgetType::Material);
}

void FdmParamSettingsWidget::openParameterPreference(const QString &name)
{
    if(!name.isEmpty()) {
        emit  currentIndexClicked(AkConst::EWidgetType::Parameter,name);
    }
    emit openPreferenceIndex(AkConst::EWidgetType::Parameter);
}

void FdmParamSettingsWidget::resertButtonClicked()
{
    MessageDialog dialog(tr("Reset Parameters"),tr("Parameters will be reset. Continue?"),MessageDialog::CANCEL|MessageDialog::YES,this);
    int i = dialog.exec();
    if(i == MessageDialog::YES) {
        emit resertParameter();
    }
}

void FdmParamSettingsWidget::showBubbleTip()
{
    if(!m_toolTip->isHidden()) {
        return;
    }
    QPoint globalP =  mapToGlobal(QPoint( 0,0));
    int maxY = this->parentWidget()->height() + globalP.y();
    m_toolTip->setPoint(m_point,maxY);
    m_toolTip->show();
  // qDebug() << "show  ==== tooltip size = " << m_toolTip->geometry();
}


}
