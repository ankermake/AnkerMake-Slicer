#include "fdmparamsettingswidget.h"

#include <QDir>

namespace fdmsettings {

FdmParamSettingsWidget::FdmParamSettingsWidget(QWidget *parent) :
    QWidget(parent),
    model(new ParamListModel),
    treeModel(new FdmSettingsTreeModel),
    m_toolTip(nullptr),
    m_timer(new QTimer(this)),
    m_HtmlToolTip(nullptr),
    m_HtmlTimer(new QTimer(this))
{
    connect(m_timer,&QTimer::timeout,this,&FdmParamSettingsWidget::showBubbleTip);
    connect(m_HtmlTimer,&QTimer::timeout,this,&FdmParamSettingsWidget::showHtmlBubbleTip);

    m_quickView = new QQuickWidget(FdmQmlEngine::instance(),nullptr);
    m_quickView->setResizeMode(QQuickWidget::SizeRootObjectToView);
#ifdef Q_OS_WIN
    QSurfaceFormat format;
    format.setSamples(16);
    m_quickView->setFormat(format);
#endif
    
    QString anker_expert = ":/qml/back_logic/anker_expert.cfg"; //  @2023-02-23 by ChunLian

    {
        QString opt = qApp->applicationDirPath() + "/ProcessOptimizationParameter.ini";
        if(QFileInfo(opt).exists())
            anker_expert = opt;
        qDebug() << __CodeLocation__ << "anker_expert =" << anker_expert;
    }

    reload(anker_expert);
    m_quickView->engine()->rootContext() ->setContextProperty("paramModel", model);
    m_quickView->engine()->rootContext() ->setContextProperty("FdmParamSettingsWidget", this);
    auto  parameter = FdmRightParameterService::instance();

    m_quickView->engine()->rootContext() ->setContextProperty("rightParameter", parameter);

    m_quickView->setSource(QUrl::fromLocalFile(":/qml/FdmParameter/SettingView.qml"));

    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMachineChanged(QString)),parameter,SLOT(onMachineNameChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentMaterialChanged(QString)),parameter,SLOT(onMaterialNameChanged(QString)),Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlSettingCurrentParameterChanged(QString)),parameter,SLOT(onCurrentSelectedNameChanged(QString)), Qt::QueuedConnection);
    //connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlPrintModeChanged(QString)),parameter,SLOT(onPrintModeChanged(QString)), Qt::QueuedConnection);
    connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlPrintModeIdxChanged(int)),parameter,SLOT(onPrintModeIdxChanged(int)), Qt::QueuedConnection);

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
    //QWidget *widget = QWidget::createWindowContainer(m_quickView,this);
    layout->addWidget(m_quickView);
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

void FdmParamSettingsWidget::reload(const QString &fileName)
{
    QList<FdmParamNode *> configList = FdmQmlSourceTree::instance().loadParamList(fileName);
    if(configList.isEmpty()) {
        return;
    }
    model->setListDatas(configList);
}

void FdmParamSettingsWidget::showPrintModeToolTip(QPoint point)
{
    qDebug() << "enter showHtmlToolTip";
    m_HtmlTimer->start(500);//1000
    //m_point = point;
    m_point = QPoint(point.x()+20 ,point.y());
    return;
//    QString html;
//    html.append("<font Weight=400>"+ tr("Fast") +"</font><br>" + tr("Prioritizes speed over precision.Higher print speeds and accesleratioins will be whthin the printer's safe operating range.<br>"))
//    .append(tr("Speed") + ("<img src='qrc:/images/images/fdm_an_icon_n.png' /> <img src='qrc:/images/images/fdm_an_icon_n.png' /> <br>"))
//    .append(tr("Detail") +("<img src='qrc:/images/images/fdm_an_icon_n.png' /> <img src='qrc:/images/images/fdm_an_icon_n.png' /> <br>"))
//    .append(tr("Reliability") +("<img src='qrc:/images/images/fdm_an_icon_n.png' /> <img src='qrc:/images/images/fdm_an_icon_n.png' /> "));

//    html.append("<font Weight=400>"+ tr("Normal") +"</font><br>"  + tr("Maintains a balance between speed and precision.Prints will have a higher quality when finished.<br>"))
//    .append(tr("Speed") + ("<img src='qrc:/images/images/fdm_an_icon_n.png' /> <img src='qrc:/images/images/fdm_an_icon_n.png' /><br> "))
//    .append(tr("Detail") +("<img src='qrc:/images/images/fdm_an_icon_n.png' /> <img src='qrc:/images/images/fdm_an_icon_n.png' /> <br>"))
//    .append(tr("Reliability") +("<img src='qrc:/images/images/fdm_an_icon_n.png' /> <img src='qrc:/images/images/fdm_an_icon_n.png' /> "));

//    QString xxx = R"(
//<div style="font-family: Roboto; font-size: 14px; font-weight: 500; line-height: 16px; letter-spacing: 0px; text-align: left; background: #FFFFFF;">
//   <div style="font-weight: bold;">Fast</div>
//    <div>Prioritizes speed over precision. Higher print speeds and accelerations will be within the printer's safe operating range.</div>
//        <div style="display: inline-flex; align-items: center;">
//            <div style="flex-basis: 20px;">Speed</div>
//            <div style="display: inline-flex; align-items: center;">
//              <img src="image1.png" style="width: 10px; height: 10px;">
//              <img src="image2.png" style="width: 10px; height: 10px;">
//            </div>
//        </div>
//        <div style="display: flex; align-items: center;">
//            <div style="flex: 1;">Detail</div>
//            <div style="display: flex; align-items: center;">
//              <img src="image3.png" style="width: 10px; height: 10px;">
//              <img src="image4.png" style="width: 10px; height: 10px;">
//            </div>
//        </div>
//        <div style="display: flex; align-items: center;">
//            <div style="flex: 1;">Reliability</div>
//            <div style="display: flex; align-items: center;">
//              <img src="image5.png" style="width: 10px; height: 10px;">
//              <img src="image6.png" style="width: 10px; height: 10px;">
//            </div>
//        </div>
//  <div style="height: 1px; background-color: #CCCCCC; margin: 10px 0;"></div>
//  <div style="display: flex; align-items: center;">
//    <div style="flex: 1;">Speed</div>
//    <div style="display: flex; align-items: center;">
//      <img src="image1.png" style="width: 50px; height: 50px;">
//      <img src="image2.png" style="width: 50px; height: 50px;">
//    </div>
//  </div>
//  <div style="display: flex; align-items: center;">
//    <div style="flex: 1;">Detail</div>
//    <div style="display: flex; align-items: center;">
//      <img src="image3.png" style="width: 50px; height: 50px;">
//      <img src="image4.png" style="width: 50px; height: 50px;">
//    </div>
//  </div>
//  <div style="display: flex; align-items: center;">
//    <div style="flex: 1;">Reliability</div>
//    <div style="display: flex; align-items: center;">
//      <img src="image5.png" style="width: 50px; height: 50px;">
//      <img src="image6.png" style="width: 50px; height: 50px;">
//    </div>
//  </div>
//</div>

//)";
//    //QString bbb = R"(<img src="qrc:/images/images/print_mode_tip.png" style="width: 215px; height: 351px;" />)";
//    QString bbb = R"(<img src="qrc:/images/images/print_mode_tip.png"/>)";
//    showHtmlToolTip(xxx,point);
}

void FdmParamSettingsWidget::showHtmlToolTip(QString html, QPoint point)
{
    qDebug() << "enter showHtmlToolTip";
    m_HtmlTimer->start(500);//1000
    m_point = QPoint(point.x(),point.y());
//    if(!m_HtmlToolTip) {
//        m_HtmlToolTip = new HtmlToolTip();
//    }
//    m_HtmlToolTip->setHtml(html);
}

void FdmParamSettingsWidget::hideHtmlToolTip()
{
    m_HtmlTimer->stop();
    if(m_HtmlToolTip) {
        m_HtmlToolTip->hide();
        delete m_HtmlToolTip;
        m_HtmlToolTip = nullptr;
    }
}

void FdmParamSettingsWidget::showToolTip(const QString &titlte, const QString &description, const QString &affects, const QString &affectedBy, QPoint point)
{
   //qDebug() << "description == " << description << "affects ==" << affects << "affectedBy ==" << affectedBy << "point =" << point;
    m_timer->start(500);//1000
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
            disconnect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlPrintModeIdxChanged(int)),parameter,SLOT(onPrintModeIdxChanged(int)));
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
            connect((QObject *)(m_quickView->rootObject()),SIGNAL(qmlPrintModeIdxChanged(int)),parameter,SLOT(onPrintModeIdxChanged(int)), Qt::QueuedConnection);
        }
    }
    QWidget::changeEvent(event);
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


void FdmParamSettingsWidget::showHtmlBubbleTip()
{
    if(!m_HtmlToolTip) {
        m_HtmlToolTip = new PrintModeToolTip();
    }
    //m_printModeToolTip->setHtml(m_titlte, m_description, m_affects, m_affectedBy);
    QPoint globalP =  mapToGlobal(QPoint( 0,0));
    int maxY = this->parentWidget()->height() + globalP.y();
    m_HtmlToolTip->setPoint(m_point,maxY);
    m_HtmlToolTip->show();
     qDebug() << "show  ==== tooltip size = " << m_HtmlToolTip->geometry();
}



}
