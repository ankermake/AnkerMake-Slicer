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
private:
    //bool offRenderLock = false;
    bool isHighPerformance = true;
    QString saveTempPath ;
    ProgressDialog *mpDlg;
#ifdef USE_EXTRA_UI
    QComboBox *linetypeCombox;
#endif
    QStackedLayout* typeLayout;

    
    
    QHBoxLayout  *innerWallLayout;              
    QPushButton  *innerWallLabelC;              
    QLabel       *innerWallLabel;               
    QSpacerItem  *horizontalSpacer_innerWall;   //
    SwitchButton *innerWallcheckBox;            

    
    QHBoxLayout  *outerWallLayout;
    QPushButton  *outerWallLabelC;
    QLabel       *outerWallLabel;
    QSpacerItem  *horizontalSpacer_outerWall;
    SwitchButton *outerWallcheckBox;

    
    QHBoxLayout  *skinLayout;
    QPushButton  *skinLabelC;
    QLabel       *skinLabel;
    QSpacerItem  *horizontalSpacer_skin;
    SwitchButton *skincheckBox;

    
    QHBoxLayout  *helperLayout;
    QPushButton  *helperLabelC;
    QLabel       *helperLabel;
    QSpacerItem  *horizontalSpacer_helper;
    SwitchButton *helpercheckBox;

    
    QHBoxLayout  *fillLayout;
    QPushButton  *fillLabelC;
    QLabel       *fillLabel;
    QSpacerItem  *horizontalSpacer_fill;
    SwitchButton *fillcheckBox;

    
    QHBoxLayout  *travelLayout;
    QPushButton  *travelLabelC;
    QLabel       *travelLabel;
    QSpacerItem  *horizontalSpacer_travel;
    SwitchButton *travelcheckBox;

    
    QHBoxLayout  *supportLayout;
    QPushButton  *supportLabelC;
    QLabel       *supportLabel;
    QSpacerItem  *horizontalSpacer_support;
    SwitchButton *supportcheckBox;

    
    QHBoxLayout  *unkonwnLayout;
    QPushButton  *unkonwnLabelC;
    QLabel       *unkonwnLabel;
    QSpacerItem  *horizontalSpacer_unkonwn;
    SwitchButton *unkonwncheckBox;

    
    QHBoxLayout  *zlapLayout;
    QPushButton  *zlapLabelC;
    QLabel       *zlapLabel;
    QSpacerItem  *horizontalSpacer_zlap;
    SwitchButton *zlapcheckBox;

#ifdef USE_EXTRA_UI
    
    QHBoxLayout *innerWallLayout_speed;
    QLabel *innerWallLabel_speed;
    QSpacerItem *horizontalSpacer_innerWall_speed;
    SwitchButton *innerWallcheckBox_speed;

    QHBoxLayout *outerWallLayout_speed;
    QLabel *outerWallLabel_speed;
    QSpacerItem *horizontalSpacer_outerWall_speed;
    SwitchButton *outerWallcheckBox_speed;

    QHBoxLayout *skinLayout_speed;
    QLabel *skinLabel_speed;
    QSpacerItem *horizontalSpacer_skin_speed;
    SwitchButton *skincheckBox_speed;

    QHBoxLayout *helperLayout_speed;
    QLabel *helperLabel_speed;
    QSpacerItem *horizontalSpacer_helper_speed;
    SwitchButton *helpercheckBox_speed;

    QHBoxLayout *fillLayout_speed;
    QLabel *fillLabel_speed;
    QSpacerItem *horizontalSpacer_fill_speed;
    SwitchButton *fillcheckBox_speed;

    QHBoxLayout *travelLayout_speed;
    QLabel *travelLabel_speed;
    QSpacerItem *horizontalSpacer_travel_speed;
    SwitchButton *travelcheckBox_speed;

    QHBoxLayout *supportLayout_speed;
    QLabel *supportLabel_speed;
    QSpacerItem *horizontalSpacer_support_speed;
    SwitchButton *supportcheckBox_speed;

    QHBoxLayout *unkonwnLayout_speed;
    QLabel *unkonwnLabel_speed;
    QSpacerItem *horizontalSpacer_unkonwn_speed;
    SwitchButton *unkonwncheckBox_speed;


    
    QHBoxLayout *innerWallLayout_trapezoid;
    QLabel *innerWallLabel_trapezoid;
    QSpacerItem *horizontalSpacer_innerWall_trapezoid;
    SwitchButton *innerWallcheckBox_trapezoid;

    QHBoxLayout *outerWallLayout_trapezoid;
    QLabel *outerWallLabel_trapezoid;
    QSpacerItem *horizontalSpacer_outerWall_trapezoid;
    SwitchButton *outerWallcheckBox_trapezoid;

    QHBoxLayout *skinLayout_trapezoid;
    QLabel *skinLabel_trapezoid;
    QSpacerItem *horizontalSpacer_skin_trapezoid;
    SwitchButton *skincheckBox_trapezoid;

    QHBoxLayout *helperLayout_trapezoid;
    QLabel *helperLabel_trapezoid;
    QSpacerItem *horizontalSpacer_helper_trapezoid;
    SwitchButton *helpercheckBox_trapezoid;

    QHBoxLayout *fillLayout_trapezoid;
    QLabel *fillLabel_trapezoid;
    QSpacerItem *horizontalSpacer_fill_trapezoid;
    SwitchButton *fillcheckBox_trapezoid;

    QHBoxLayout *travelLayout_trapezoid;
    QLabel *travelLabel_trapezoid;
    QSpacerItem *horizontalSpacer_travel_trapezoid;
    SwitchButton *travelcheckBox_trapezoid;

    QHBoxLayout *supportLayout_trapezoid;
    QLabel *supportLabel_trapezoid;
    QSpacerItem *horizontalSpacer_support_trapezoid;
    SwitchButton *supportcheckBox_trapezoid;

    QHBoxLayout *unkonwnLayout_trapezoid;
    QLabel *unkonwnLabel_trapezoid;
    QSpacerItem *horizontalSpacer_unkonwn_trapezoid;
    SwitchButton *unkonwncheckBox_trapezoid;

    
    QHBoxLayout *innerWallLayout_Flow;
    QLabel *innerWallLabel_Flow;
    QSpacerItem *horizontalSpacer_innerWall_Flow;
    SwitchButton *innerWallcheckBox_Flow;

    QHBoxLayout *outerWallLayout_Flow;
    QLabel *outerWallLabel_Flow;
    QSpacerItem *horizontalSpacer_outerWall_Flow;
    SwitchButton *outerWallcheckBox_Flow;

    QHBoxLayout *skinLayout_Flow;
    QLabel *skinLabel_Flow;
    QSpacerItem *horizontalSpacer_skin_Flow;
    SwitchButton *skincheckBox_Flow;

    QHBoxLayout *helperLayout_Flow;
    QLabel *helperLabel_Flow;
    QSpacerItem *horizontalSpacer_helper_Flow;
    SwitchButton *helpercheckBox_Flow;

    QHBoxLayout *fillLayout_Flow;
    QLabel *fillLabel_Flow;
    QSpacerItem *horizontalSpacer_fill_Flow;
    SwitchButton *fillcheckBox_Flow;

    QHBoxLayout *travelLayout_Flow;
    QLabel *travelLabel_Flow;
    QSpacerItem *horizontalSpacer_travel_Flow;
    SwitchButton *travelcheckBox_Flow;

    QHBoxLayout *supportLayout_Flow;
    QLabel *supportLabel_Flow;
    QSpacerItem *horizontalSpacer_support_Flow;
    SwitchButton *supportcheckBox_Flow;

    QHBoxLayout *unkonwnLayout_Flow;
    QLabel *unkonwnLabel_Flow;
    QSpacerItem *horizontalSpacer_unkonwn_Flow;
    SwitchButton *unkonwncheckBox_Flow;

    QLabel* colorPatch_label_start_trapezoid;
    QLabel* colorPatch_label_end_trapezoid;

    
    QLabel* colorPatch_label_start;
    QLabel* colorPatch_label_end;
    QLabel* colorPatch_label_start_Flow;
    QLabel* colorPatch_label_end_Flow;
#endif

    QLabel* filamentValue;
    QLabel* timeValue;

    QLabel* allTimeLabel;
    QLabel* allfilamentLabel;
    QLabel* allSizeLabel;

    gcodeLabelEdit* spinBox_2;//up
    gcodeLabelEdit* spinBox;//down

    QTimer* mPlayTimer = nullptr;
    QPushButton* pushButton_9; //paly view button

    std::array<double, featureType::Count> showLabelCount{ 0 };
    std::array<float, featureType::Count> showTimeLabelCount{ 0 };
    std::array<int, 3> showAllSize{ 0 };
    SceneParam m_sceneParam;
    bool isAiMode = false;
    int runTimesId;
    const qint64 qs_id = QDateTime::currentSecsSinceEpoch();
    ProgressDialog *m_progessDlg;
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
