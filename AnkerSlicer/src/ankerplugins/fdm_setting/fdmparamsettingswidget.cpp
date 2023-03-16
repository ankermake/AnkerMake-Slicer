#include "fdmparamsettingswidget.h"

#include <QDir>

namespace fdmsettings {

FdmParamSettingsWidget::FdmParamSettingsWidget(QWidget *parent) :
    QWidget(parent),
    model(new ParamListModel),
    treeModel(new FdmSettingsTreeModel),
    m_toolTip(nullptr),
    m_timer(new QTimer(this))
{
    connect(m_timer,&QTimer::timeout,this,&FdmParamSettingsWidget::showBubbleTip);

    m_quickView = new QQuickView(FdmQmlEngine::instance(),nullptr);
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);


    
    //QString anker_expert = ":/curadef/setting_visibility/anker_expert.cfg";
    QString anker_expert = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/back_logic/anker_expert.cfg");

    reload(anker_expert);
    m_quickView->engine()->rootContext() ->setContextProperty("paramModel", model);
    m_quickView->engine()->rootContext() ->setContextProperty("FdmParamSettingsWidget", this);
    auto  parameter = FdmRightParameterService::instance();

    m_quickView->engine()->rootContext() ->setContextProperty("rightParameter", parameter);

    m_quickView->setSource(QUrl::fromLocalFile(":/qml/FdmParameter/SettingView.qml"));

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMachineChanged(QString)),parameter,SLOT(onMachineNameChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMaterialChanged(QString)),parameter,SLOT(onMaterialNameChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentParameterChanged(QString)),parameter,SLOT(onCurrentSelectedNameChanged(QString)), Qt::QueuedConnection);

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenMachinePreference(QString)),this,SLOT(openMachinePreference(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenMaterialPreference(QString)),this,SLOT(openMaterialPreference(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenParameterPreference(QString)),this,SLOT(openParameterPreference(QString)), Qt::QueuedConnection);

    connect(this,SIGNAL(currentIndexClicked(AkConst::EWidgetType,QString)),parameter,SLOT(onSetBtnClicked(AkConst::EWidgetType,QString)));

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlResetParameter()),this,SLOT(resertButtonClicked()));

    connect(this,SIGNAL(resertParameter()),parameter,SLOT(onResetBtnClicked()));

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlAiQualityCurrentIdxChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlLayerHeightCurrentIndexChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlInfillCurrentIndexChanged(int)),parameter,SLOT(onAiInfillDensityIdxChanged(int)));
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlGlobalSupportChanged(int)),parameter,SLOT(onGlobalSupportStateChanged(int)));
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlGenerateAdhesionChanged(int)),parameter,SLOT(onGenerateAdhesionStateChanged(int)));
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlNozzleSizeCurrentIndexChanged(QString)),parameter,SLOT(onNozzleSizeChanged(QString)), Qt::QueuedConnection);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    //layout->setc(0);
    this->setLayout(layout);
    QWidget *widget = QWidget::createWindowContainer(m_quickView,this);
    layout->addWidget(widget);
}

FdmParamSettingsWidget::~FdmParamSettingsWidget()
{
    if(model) {
        delete  model;
        model = nullptr;
    }
    if(m_toolTip) {
        delete   m_toolTip ;
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
   //qDebug() << "description == " << description << "affects ==" << affects << "affectedBy ==" << affectedBy << "point =" << point;
    m_timer->start(1000);
    m_point = point;
    m_titlte =titlte;
    m_description = description;
    m_affects = affects;
    m_affectedBy = m_affectedBy;
}

void FdmParamSettingsWidget::hideToolTip()
{
    m_timer->stop();
    if(m_toolTip) {
        m_toolTip->hide();
        delete m_toolTip;
        m_toolTip = nullptr;
    }
}

void FdmParamSettingsWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (m_quickView) {
            auto parameter = FdmRightParameterService::instance();
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMachineChanged(QString)),parameter,SLOT(onMachineNameChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMaterialChanged(QString)),parameter,SLOT(onMaterialNameChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentParameterChanged(QString)),parameter,SLOT(onCurrentSelectedNameChanged(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenMachinePreference(QString)),this,SLOT(openMachinePreference(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenMaterialPreference(QString)),this,SLOT(openMaterialPreference(QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenParameterPreference(QString)),this,SLOT(openParameterPreference(QString)));
            disconnect(this,SIGNAL(currentIndexClicked(AkConst::EWidgetType,QString)),parameter,SLOT(onSetBtnClicked(AkConst::EWidgetType,QString)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlResetParameter()),this,SLOT(resertButtonClicked()));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlAiQualityCurrentIdxChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlLayerHeightCurrentIndexChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlInfillCurrentIndexChanged(int)),parameter,SLOT(onAiInfillDensityIdxChanged(int)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlGlobalSupportChanged(int)),parameter,SLOT(onGlobalSupportStateChanged(int)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlGenerateAdhesionChanged(int)),parameter,SLOT(onGenerateAdhesionStateChanged(int)));
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlNozzleSizeCurrentIndexChanged(QString)),parameter,SLOT(onNozzleSizeChanged(QString)));
            m_quickView->source().clear();
            m_quickView->setSource(QUrl::fromLocalFile(":/qml/FdmParameter/SettingView.qml"));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMachineChanged(QString)),parameter,SLOT(onMachineNameChanged(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMaterialChanged(QString)),parameter,SLOT(onMaterialNameChanged(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentParameterChanged(QString)),parameter,SLOT(onCurrentSelectedNameChanged(QString)), Qt::QueuedConnection);

            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenMachinePreference(QString)),this,SLOT(openMachinePreference(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenMaterialPreference(QString)),this,SLOT(openMaterialPreference(QString)),Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlOpenParameterPreference(QString)),this,SLOT(openParameterPreference(QString)), Qt::QueuedConnection);
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlResetParameter()),this,SLOT(resertButtonClicked()));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlAiQualityCurrentIdxChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlLayerHeightCurrentIndexChanged(int)),parameter,SLOT(onAiQualityCurrentIdxChanged(int)));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlInfillCurrentIndexChanged(int)),parameter,SLOT(onAiInfillDensityIdxChanged(int)));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlGlobalSupportChanged(int)),parameter,SLOT(onGlobalSupportStateChanged(int)));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlGenerateAdhesionChanged(int)),parameter,SLOT(onGenerateAdhesionStateChanged(int)));
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlNozzleSizeCurrentIndexChanged(QString)),parameter,SLOT(onNozzleSizeChanged(QString)), Qt::QueuedConnection);
        }
    }
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
    if(!m_toolTip) {
        m_toolTip = new ToolTip();
    }
    m_toolTip->setDescription(m_titlte, m_description, m_affects, m_affectedBy);
    QPoint globalP =  mapToGlobal(QPoint( 0,0));
    int maxY = this->parentWidget()->height() + globalP.y();
    m_toolTip->setPoint(m_point,maxY);
    m_toolTip->show();
  // qDebug() << "show  ==== tooltip size = " << m_toolTip->geometry();
}


}
