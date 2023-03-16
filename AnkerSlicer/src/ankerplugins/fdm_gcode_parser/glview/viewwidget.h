/****************************************************************************
Revision 0.1  2021/12/08 12:00:40  Aden Hu
****************************************************************************/

#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H
#include "common/GeoAndShow/SceneParam.h"
#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QToolButton>
#include<QColorDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QTimer>
#include <QSpinBox>
#include <QComboBox>
#include <QStackedLayout>
#include <QDateTime>
#include <QRemoteObjectNode>
#include <QRemoteObjectDynamicReplica>
//#include "slider/flatui.h"
//#include "slider/slider.h"
#include "ui_viewwidget.h"
#include "slider2/sliderrange_vertical.h"
#include "slider2/sliderrange_horizontal.h"
#include "common/controlInterface/switchbutton.h"
#include "common/controlInterface/progressdialog.h"
//#include "common/Socket/QMainSocket2.h"
#include "gcode_label_edit.h"

namespace Ui
{
    class viewWidget;
}


enum featureType : uint8_t {
    innerWall,
    outerWall,
    skin,
    helper,
    fill,
    travel,
    support,
    unknown,
    Count
};

struct SliderLastValue
{
    int h_lastValue1 = -1;
    int h_lastValue2 = -1;
    int v_lastValue1 = -1;
    int v_lastValue2 = -1;
};

class viewWidget : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent* event);
public:
    explicit  viewWidget(QWidget* parent, const SceneParam& param, bool pmode = false, QString hUrl = "local:tmp",bool imode = false);
    ~viewWidget();
    bool setToolPath(Anker::GCodeProcessor::Result&& gcode_result, std::string gcodePath, bool,int);

    void setSceneParams(const SceneParam& param);

    void reSetGcodePath(std::string gcodePath, bool,int);
    bool processing = true;
    void setDefaultScene();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    void changeEvent(QEvent * event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
private:
    Ui::viewWidget* ui;
    SliderLastValue m_sliderLastValue;
public slots:
    void setOriginalStlName(const QString& oStlName);
    void setAimode(bool _isAimode);
    void setLoggingStatus(bool);
    void pauseGcodePlay(int);
private slots:
    void initForm();
    void setSavePicTrue();
    void setExportPic();
    void verticalSliderValueChanged(int, int);
    void horizontalSliderValueChanged(int, int);
    void onFrontView();
    void onBackView();
    void onLeftView();
    void onRightView();
    void onTopView();
    void onBottomView();
    void onJustView();
    void msgFromFdmGcodePaser(QString);
    void palyView();
    void on_pushButton_quit_clicked();
    void innerWallcheckButton();//erPerimeter
    void outerWallcheckButton();//erExternalPerimeter
    void skincheckButton();//erSkirt
    void helpercheckButton();//erInternalInfill
    void fillcheckButton();//erSolidInfill
    void zlapcheckButton();
    void travelcheckButton();//
    void supportcheckButton();
    void unkonwncheckButton();
    void updatePlayViewer();
    void spinBoxUpValueChanged(int);
    
    void setLoadingProgress();
#ifdef USE_EXTRA_UI
    void linetypeComboxChange(int);
#endif
    void spinBoxDownValueChanged(int);
    void colorChanged();
//    void buttonVisible(qint64,int);
//    void displayDeviceListWidgetFeedbackSLOT(qint64, int);
//    void stateChangedSlot(QRemoteObjectDynamicReplica::State state, QRemoteObjectDynamicReplica::State oldState);

    void onMsgFromFdmGcodePaser(const QString &msg);
    void connectedRPC();
    void paramChange(QVariant p);
//    void rpc_setValue(int);
signals:
    void checkoutPreviewEvent(bool);
    void printGcode(const QString&);
    void processFile(const QString&,bool); 
    //void setValue(int);
    //void startOffRender(const QString& savePath,bool isAiMode,bool cancel);
public:
    void changeSlider();
    void getFilamentCount();
    void setFilamentStr();
    void getTimeCount();
    void setTimeStr();
    void getGcodeSize();
    void setGcodeSize();
    void setPerformance(bool);
    void getGcodeTargetTemperature(float&, float&);

    std::shared_ptr<Anker::GCodeProcessor> processor;
    GcodeViewer* getGcodeView(){return this->ui->openGLWidget;}
    QString getTimesDhms(float timeInSecs);
    //QWidget* widget;
    VSliderRange* verticalSlider;
    HSliderRang* horizontalSlider;
    QPushButton* ExitButton;
    QPushButton* ExportButton;
    QSharedPointer<QRemoteObjectDynamicReplica> m_rpc;
    QRemoteObjectNode *m_node = nullptr;
    QString oStlName;
    
    bool printMode = false;
    //inner mode
    bool innerMode = false;

    
    double timer_value = 0.0;
    QTimer  *tp_timer = nullptr;
    double f_size = 0.0;
private:
    //bool offRenderLock = false;
    bool isHighPerformance = true;
    bool innerModeInit = true;
    bool printModeInit = true;
    QString saveTempPath ;
    ProgressDialog *mpDlg;
#ifdef USE_EXTRA_UI
    QComboBox *linetypeCombox;
    QLabel *colorPatch_label_unit;
    QLabel *colorPatch_label_unit_Flow;
#endif
    QStackedLayout* typeLayout =nullptr;

    
    
    QHBoxLayout  *innerWallLayout =nullptr;            
    QPushButton  *innerWallLabelC =nullptr;             
    QLabel       *innerWallLabel =nullptr;               
    QSpacerItem  *horizontalSpacer_innerWall =nullptr;   //
    SwitchButton *innerWallcheckBox =nullptr;            
    QLabel* linetypelabel =nullptr;
    QLabel* ConsumedLabelabel = nullptr;
    QLabel* filamentLabel = nullptr;
    QLabel* timelabel = nullptr;
    
    QHBoxLayout  *outerWallLayout = nullptr;
    QPushButton  *outerWallLabelC = nullptr;
    QLabel       *outerWallLabel = nullptr;
    QSpacerItem  *horizontalSpacer_outerWall = nullptr;
    SwitchButton *outerWallcheckBox = nullptr;

    
    QHBoxLayout  *skinLayout = nullptr;
    QPushButton  *skinLabelC = nullptr;
    QLabel       *skinLabel = nullptr;
    QSpacerItem  *horizontalSpacer_skin = nullptr;
    SwitchButton *skincheckBox = nullptr;

    
    QHBoxLayout  *helperLayout = nullptr;
    QPushButton  *helperLabelC = nullptr;
    QLabel       *helperLabel = nullptr;
    QSpacerItem  *horizontalSpacer_helper = nullptr;
    SwitchButton *helpercheckBox = nullptr;

    
    QHBoxLayout  *fillLayout = nullptr;
    QPushButton  *fillLabelC = nullptr;
    QLabel       *fillLabel = nullptr;
    QSpacerItem  *horizontalSpacer_fill = nullptr;
    SwitchButton *fillcheckBox = nullptr;

    
    QHBoxLayout  *travelLayout = nullptr;
    QPushButton  *travelLabelC = nullptr;
    QLabel       *travelLabel = nullptr;
    QSpacerItem  *horizontalSpacer_travel = nullptr;
    SwitchButton *travelcheckBox = nullptr;

    
    QHBoxLayout  *supportLayout = nullptr;
    QPushButton  *supportLabelC = nullptr;
    QLabel       *supportLabel = nullptr;
    QSpacerItem  *horizontalSpacer_support = nullptr;
    SwitchButton *supportcheckBox = nullptr;

    
    QHBoxLayout  *unkonwnLayout = nullptr;
    QPushButton  *unkonwnLabelC = nullptr;
    QLabel       *unkonwnLabel = nullptr;
    QSpacerItem  *horizontalSpacer_unkonwn = nullptr;
    SwitchButton *unkonwncheckBox = nullptr;

    
    QHBoxLayout  *zlapLayout = nullptr;
    QPushButton  *zlapLabelC = nullptr;
    QLabel       *zlapLabel = nullptr;
    QSpacerItem  *horizontalSpacer_zlap = nullptr;
    SwitchButton *zlapcheckBox = nullptr;

#ifdef USE_EXTRA_UI
    
    QHBoxLayout *innerWallLayout_speed = nullptr;
    QLabel *innerWallLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_innerWall_speed = nullptr;
    SwitchButton *innerWallcheckBox_speed = nullptr;

    QHBoxLayout *outerWallLayout_speed = nullptr;
    QLabel *outerWallLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_outerWall_speed = nullptr;
    SwitchButton *outerWallcheckBox_speed = nullptr;

    QHBoxLayout *skinLayout_speed = nullptr;
    QLabel *skinLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_skin_speed = nullptr;
    SwitchButton *skincheckBox_speed = nullptr;

    QHBoxLayout *helperLayout_speed = nullptr;
    QLabel *helperLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_helper_speed = nullptr;
    SwitchButton *helpercheckBox_speed = nullptr;

    QHBoxLayout *fillLayout_speed = nullptr;
    QLabel *fillLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_fill_speed = nullptr;
    SwitchButton *fillcheckBox_speed = nullptr;

    QHBoxLayout *travelLayout_speed = nullptr;
    QLabel *travelLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_travel_speed = nullptr;
    SwitchButton *travelcheckBox_speed = nullptr;

    QHBoxLayout *supportLayout_speed = nullptr;
    QLabel *supportLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_support_speed = nullptr;
    SwitchButton *supportcheckBox_speed = nullptr;

    QHBoxLayout *unkonwnLayout_speed = nullptr;
    QLabel *unkonwnLabel_speed = nullptr;
    QSpacerItem *horizontalSpacer_unkonwn_speed = nullptr;
    SwitchButton *unkonwncheckBox_speed = nullptr;


    
    QHBoxLayout *innerWallLayout_trapezoid = nullptr;
    QLabel *innerWallLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_innerWall_trapezoid = nullptr;
    SwitchButton *innerWallcheckBox_trapezoid = nullptr;

    QHBoxLayout *outerWallLayout_trapezoid = nullptr;
    QLabel *outerWallLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_outerWall_trapezoid = nullptr;
    SwitchButton *outerWallcheckBox_trapezoid = nullptr;

    QHBoxLayout *skinLayout_trapezoid = nullptr;
    QLabel *skinLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_skin_trapezoid = nullptr;
    SwitchButton *skincheckBox_trapezoid = nullptr;

    QHBoxLayout *helperLayout_trapezoid = nullptr;
    QLabel *helperLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_helper_trapezoid = nullptr;
    SwitchButton *helpercheckBox_trapezoid = nullptr;

    QHBoxLayout *fillLayout_trapezoid = nullptr;
    QLabel *fillLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_fill_trapezoid = nullptr;
    SwitchButton *fillcheckBox_trapezoid = nullptr;

    QHBoxLayout *travelLayout_trapezoid = nullptr;
    QLabel *travelLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_travel_trapezoid = nullptr;
    SwitchButton *travelcheckBox_trapezoid = nullptr;

    QHBoxLayout *supportLayout_trapezoid = nullptr;
    QLabel *supportLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_support_trapezoid = nullptr;
    SwitchButton *supportcheckBox_trapezoid = nullptr;

    QHBoxLayout *unkonwnLayout_trapezoid = nullptr;
    QLabel *unkonwnLabel_trapezoid = nullptr;
    QSpacerItem *horizontalSpacer_unkonwn_trapezoid = nullptr;
    SwitchButton *unkonwncheckBox_trapezoid = nullptr;

    
    QHBoxLayout *innerWallLayout_Flow = nullptr;
    QLabel *innerWallLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_innerWall_Flow = nullptr;
    SwitchButton *innerWallcheckBox_Flow = nullptr;

    QHBoxLayout *outerWallLayout_Flow = nullptr;
    QLabel *outerWallLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_outerWall_Flow = nullptr;
    SwitchButton *outerWallcheckBox_Flow = nullptr;

    QHBoxLayout *skinLayout_Flow = nullptr;
    QLabel *skinLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_skin_Flow = nullptr;
    SwitchButton *skincheckBox_Flow = nullptr;

    QHBoxLayout *helperLayout_Flow = nullptr;
    QLabel *helperLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_helper_Flow = nullptr;
    SwitchButton *helpercheckBox_Flow = nullptr;

    QHBoxLayout *fillLayout_Flow = nullptr;
    QLabel *fillLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_fill_Flow = nullptr;
    SwitchButton *fillcheckBox_Flow = nullptr;

    QHBoxLayout *travelLayout_Flow = nullptr;
    QLabel *travelLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_travel_Flow = nullptr;
    SwitchButton *travelcheckBox_Flow = nullptr;

    QHBoxLayout *supportLayout_Flow = nullptr;
    QLabel *supportLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_support_Flow = nullptr;
    SwitchButton *supportcheckBox_Flow = nullptr;

    QHBoxLayout *unkonwnLayout_Flow = nullptr;
    QLabel *unkonwnLabel_Flow = nullptr;
    QSpacerItem *horizontalSpacer_unkonwn_Flow = nullptr;
    SwitchButton *unkonwncheckBox_Flow = nullptr;

    QLabel* colorPatch_label_start_trapezoid = nullptr;
    QLabel* colorPatch_label_end_trapezoid = nullptr;

    
    QLabel* colorPatch_label_start = nullptr;
    QLabel* colorPatch_label_end = nullptr;
    QLabel* colorPatch_label_start_Flow = nullptr;
    QLabel* colorPatch_label_end_Flow = nullptr;
#endif

    QLabel* filamentValue = nullptr;
    QLabel* timeValue = nullptr;

    QLabel* allTimeLabel = nullptr;
    QLabel* allfilamentLabel = nullptr;
    QLabel* allSizeLabel = nullptr;

    float allTimeValue;
    double allFilamentValue;

    gcodeLabelEdit* spinBox_2 = nullptr;//up
    gcodeLabelEdit* spinBox = nullptr;//down

    QTimer* mPlayTimer = nullptr;
    QPushButton* pushButton_9 = nullptr; //paly view button

    std::array<double, featureType::Count> showLabelCount{ 0 };
    std::array<float, featureType::Count> showTimeLabelCount{ 0 };
    std::array<int, 3> showAllSize{ 0 };
    SceneParam m_sceneParam;
    bool isAiMode = false;
    bool loggingStatus = false;
    int runTimesId;
    const qint64 qs_id = QDateTime::currentSecsSinceEpoch();
    ProgressDialog *m_progessDlg = nullptr;
private:
    QColorDialog* colorDlg;
    const std::map<uint8_t, QString> roleDict = {
    {ExtrusionRole::erNone ,"unkonwnLabelC"},
    {ExtrusionRole::erPerimeter,"innerWallLabelC"},
    {ExtrusionRole::erExternalPerimeter,"outerWallLabelC"},
    {ExtrusionRole::erOverhangPerimeter ,"unkonwnLabelC"},
    {ExtrusionRole::erInternalInfill ,"fillLabelC"},
    {ExtrusionRole::erSolidInfill,"skinLabelC"},
    {ExtrusionRole::erTopSolidInfill,"unkonwnLabelC"},
    {ExtrusionRole::erIroning,"unkonwnLabelC"},
    {ExtrusionRole::erBridgeInfill,"unkonwnLabelC"},
    {ExtrusionRole::erInternalBridgeInfill,"unkonwnLabelC"},
    {ExtrusionRole::erThinWall,"unkonwnLabelC"},
    {ExtrusionRole::erGapFill,"unkonwnLabelC"},
    {ExtrusionRole::erSkirt,"helperLabelC"},
    {ExtrusionRole::erSupportMaterial,"supportLabelC"},
    {ExtrusionRole::erSupportMaterialInterface,"supportLabelC"},
    {ExtrusionRole::erWipeTower,"helperLabelC"},
    {ExtrusionRole::erMilling,"unkonwnLabelC"},
    {ExtrusionRole::erCustom,"unkonwnLabelC"},
    {ExtrusionRole::erMixed,"unkonwnLabelC"}
    };

    const std::map<QString, QColor> colorDict = {
        {"innerWallLabelC" ,{80, 99, 92}},
        {"outerWallLabelC" ,{255, 144, 82}},
        {"skinLabelC" ,{97, 211, 125}},
        {"helperLabelC" ,{71, 143, 255}},
        {"fillLabelC" ,{138, 76, 216}},
        {"travelLabelC" ,{91, 207, 207}},
#ifdef USE_EXTRA_UI
        {"zlapLabelC" ,{91, 207, 207}},
#endif
        {"supportLabelC" ,{249, 211, 86}},
        {"unkonwnLabelC" ,{255, 105, 105}}
    };

    std::map<QString, int> labelIndexDict = {
        {"innerWallLabelC" ,0},
        {"outerWallLabelC" ,1},
        {"skinLabelC"      ,2},
        {"helperLabelC"    ,3},
        {"fillLabelC"      ,4},
        {"travelLabelC"    ,5},
#ifdef USE_EXTRA_UI
        {"zlapLabelC"      ,6},
        {"supportLabelC"   ,7},
        {"unkonwnLabelC"   ,8}
#else
        {"supportLabelC"   ,6},
        {"unkonwnLabelC"   ,7}
#endif

    };
};



#endif // !VIEWWIDGET_H
