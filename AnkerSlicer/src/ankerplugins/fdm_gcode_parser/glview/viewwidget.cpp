#include "viewwidget.h"
#include <cmath>
#include <QAction>
#include <QFileDialog>
#include <QButtonGroup>
#include <QApplication>
#include<QDesktopWidget>
#include <QStandardPaths>
#include <QFile>
#include <QThreadPool>
#include <QProgressDialog>
#include "../common/utilities/tlogger.h"
#include "../common/controlInterface/messageDialog.h"
#define PI 3.11415926f

viewWidget::viewWidget(QWidget* parent, const SceneParam& param,bool pmode,QString hUrl, bool imode) : QWidget(parent), ui(new Ui::viewWidget), m_sceneParam(param),printMode(pmode),innerMode(imode)
{
    qRegisterMetaTypeStreamOperators<passSceneParam>("passSceneParam");
    m_node = new QRemoteObjectNode(this);
    ui->setupUi(this);
    initForm();
    processor = std::make_shared<Anker::GCodeProcessor>();
    
    if(pmode || imode){

        m_node->connectToNode(QUrl(hUrl));
        m_rpc.reset(m_node->acquireDynamic("fdmRpc"));
        m_rpc->waitForSource();
        

        QVariant t_wind = QVariant::fromValue((WId) this);
//        qDebug() << "t_wind" <<t_wind;
//        qDebug() << "t_wind void *" <<(void *)t_wind.toULongLong();
        m_rpc.data()->setProperty("wid",t_wind);
        setDefaultScene();
        connect(m_rpc.data(),SIGNAL(passParamsChanged(QVariant)),this,SLOT(paramChange(QVariant)));
        connect(m_rpc.data(),SIGNAL(msgFromFdmGcodePaser(QString)),this,SLOT(msgFromFdmGcodePaser(QString)));
        connect(this->ui->openGLWidget,SIGNAL(setValue(int)),m_rpc.data(),SLOT(msgFromGcodepreview(int))); 
        if(imode){
            ui->openGLWidget->innerMode = true;
            connect(this,SIGNAL(checkoutPreviewEvent(bool)),m_rpc.data(),SLOT(checkoutPreviewEvent(bool)));
            connect(m_rpc.data(),SIGNAL(oStlNameChanged(QString)),this,SLOT(setOriginalStlName(QString)));
            connect(m_rpc.data(),SIGNAL(isAiModeChanged(bool)),this,SLOT(setAimode(bool)));
            connect(this,SIGNAL(printGcode(const QString&)),m_rpc.data(),SLOT(sendGCodePrintSignal(const QString&)));
        }
    }else{
        m_node->connectToNode(QUrl("local:fdmRpc"));
        m_rpc.reset(m_node->acquireDynamic("fdmRpc"));
//        m_rpc->waitForSource();
//        QVariant t_wind (this->winId());
//        m_rpc.data()->setProperty("wid",t_wind);
//        setDefaultScene();


    }

    connect(m_rpc.data(),&QRemoteObjectDynamicReplica::initialized,this,&viewWidget::connectedRPC);
//no need check rpc


    if(printMode)
    {
        ui->openGLWidget->printMode = true;
        ui->widget->hide();
    }
    if(!pmode)
    {
    ExitButton->setText(tr("Print"));
    ExitButton->setVisible(true);
    }

    colorDlg = new QColorDialog(this);
    colorDlg->setStyleSheet(QString("background-color:#9a9a9a;"));
    for(auto &t : colorDict){
        int i = labelIndexDict[t.first];
        int iM = i > 7 ? 2*i - 15 : 2*i;
        colorDlg->setCustomColor(iM, t.second);
    }

}



viewWidget::~viewWidget()
{
    QDir dir(saveTempPath);
    if (dir.exists())
    {
        dir.removeRecursively();
    }
    delete ui;
   // AkUtil::TDebug( "-----paser exit sucess----");
}

void viewWidget::connectedRPC()
{
    AkUtil::TDebug( "-----dynamic connected----");
    QVariant var_test = m_rpc->property("passParams");
    this->m_sceneParam.m_eye = var_test.value<passSceneParam>().m_eye;
    this->m_sceneParam.m_front = var_test.value<passSceneParam>().m_front;
    this->m_sceneParam.m_up = var_test.value<passSceneParam>().m_up;
    this->m_sceneParam.m_printMachineBox = var_test.value<passSceneParam>().m_printMachineBox;
    //QObject::connect(this,SIGNAL(closePreviewEvent(int)), m_rpc.data(),SLOT(msgFromGcodepreview(int)));
    ui->openGLWidget->setSceneParams(m_sceneParam);
    this->changeSlider();
    QString str;
    QDebug(&str) << "p.value<passSceneParam>().m_backgroundColor "<<var_test.value<passSceneParam>().m_backgroundColor;
    QDebug(&str) << "p.value<passSceneParam>().eye "<<var_test.value<passSceneParam>().m_eye;
    QDebug(&str) << "p.value<passSceneParam>().m_front "<<var_test.value<passSceneParam>().m_front;
    QDebug(&str) << "p.value<passSceneParam>().m_up "<<var_test.value<passSceneParam>().m_up;
    QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.m_length "<<var_test.value<passSceneParam>().m_printMachineBox.m_length;
    QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.m_width "<<var_test.value<passSceneParam>().m_printMachineBox.m_width;
    QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.m_height "<<var_test.value<passSceneParam>().m_printMachineBox.m_height;
    QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.m_color "<<var_test.value<passSceneParam>().m_printMachineBox.m_color;
    QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.m_lineWidth "<<var_test.value<passSceneParam>().m_printMachineBox.m_lineWidth;
    QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.num "<<(int)var_test.value<passSceneParam>().m_printMachineBox.num;
    QDebug(&str) << "p.value<passSceneParam>().m_backgroundColor "<<var_test.value<passSceneParam>().m_backgroundColor;
    AkUtil::TDebug( str);
    if(var_test.canConvert<passSceneParam>())
    {
         AkUtil::TDebug( "-----test success----");
    }
    //connect(m_rpc.data(),SIGNAL(passParamsChanged(QVariant)),this,SLOT(paramChange(QVariant)));
    //connect(m_rpc.data(),SIGNAL(msgFromFdmGcodePaser(QString)),this,SLOT(onMsgFromFdmGcodePaser(QString)));
    //connect(this,SIGNAL(closePreviewEvent(int)), m_rpc.data(),SLOT(previewCloseSlot(int)),Qt::BlockingQueuedConnection);
}


void viewWidget::paramChange(QVariant p)
{
    AkUtil::TDebug("in param change ");
    //emit setValue(42);
//    QString str;
    //QDebug(&str) << var_test.toFloat();
    //QDebug(&str) << "p.m_backgroundColor "<<p.value<passSceneParam>();
    ui->openGLWidget->setSceneParams(m_sceneParam);
    this->changeSlider();
//    AkUtil::TDebug( str);
}

void viewWidget::onMsgFromFdmGcodePaser(const QString &msg)
{
    AkUtil::TDebug(msg);
    qDebug() << msg;

}

void viewWidget::initForm()
{
    AkUtil::TFunction("");
    
    //setAttribute(Qt::WA_StyledBackground, true);
   // int tmpR, tmpG, tmpB, tmpA;
   // m_sceneParam.m_backgroundColor.getRgb(&tmpR, &tmpG, &tmpB, &tmpA);
    //setStyleSheet("background-color: rgb(234, 234, 234)");
    //setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)").arg(tmpR).arg(tmpG).arg(tmpB).arg(tmpA));
    //ui->openGLWidget->setSceneParams(m_sceneParam);
   // ui->openGLWidget->setAttribute(Qt::WA_StyledBackground, true);
    //ui->openGLWidget->setStyleSheet("background-color: rgb(234, 234, 234)");
    //ui->openGLWidget->setStyleSheet(QString("background-color: rgb(%1, %2, %3, %4)").arg(31).arg(32).arg(34).arg(tmpA));

    ui->widget->setStyleSheet(QString("background-color:#292A2D;"));
    setWindowTitle(tr("AnkerMake G-Code Preview"));
    setWindowIcon(QIcon(":/pic/ui/logo_AnkerMake.ico"));

    //ui opengl widgt layout
    QVBoxLayout* verticalLayout_in;
    QHBoxLayout* horizontalLayout_in;
    QSpacerItem* horizontalSpacer_in;
    QVBoxLayout* verticalLayout_in_2;
    QSpacerItem* verticalSpacer_in;

    QSpacerItem* verticalSpacer_in_2;
    QHBoxLayout* horizontalLayout_in_2;
    QVBoxLayout* verticalLayout_in_3;
    QHBoxLayout* horizontalLayout_in_3;
    QSpacerItem* horizontalSpacer_in_4;

    //QScrollBar *horizontalScrollBar;
    QSpacerItem* horizontalSpacer_in_5;
    QHBoxLayout* horizontalLayout_in_4;
    QSpacerItem* horizontalSpacer_in_2;
    QToolButton* aiButton;
    QToolButton* justButton;
    QToolButton* frontButton;
    QToolButton* backButton;
    QToolButton* leftButton;
    QToolButton* rightButton;
    QToolButton* topButton;
    QToolButton* bottomButton;
    QSpacerItem* horizontalSpacer_in_3;
    QVBoxLayout* verticalLayout_in_5;

    verticalLayout_in = new QVBoxLayout(ui->openGLWidget);
    verticalLayout_in->setObjectName(QString::fromUtf8("verticalLayout_in"));
    horizontalLayout_in = new QHBoxLayout();
    horizontalLayout_in->setObjectName(QString::fromUtf8("horizontalLayout_in"));
    horizontalSpacer_in = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_in->addItem(horizontalSpacer_in);

    verticalLayout_in_2 = new QVBoxLayout();
    verticalLayout_in_2->setSpacing(11);
    verticalLayout_in_2->setObjectName(QString::fromUtf8("verticalLayout_in_2"));
    verticalSpacer_in = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout_in_2->addItem(verticalSpacer_in);

    spinBox_2 = new gcodeLabelEdit(ui->openGLWidget);
    spinBox_2->setObjectName(QString::fromUtf8("spigcodeLabelEditnBox_2"));
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(spinBox_2->sizePolicy().hasHeightForWidth());
    spinBox_2->setSizePolicy(sizePolicy);
    spinBox_2->setMaximumSize(QSize(125, 28));
    spinBox_2->setMinimumSize(QSize(125, 28));
//    QFont font1;
//    font1.setPointSize(8);
//    spinBox_2->setFont(font1);

    verticalLayout_in_2->addWidget(spinBox_2);
    verticalSlider = new VSliderRange(ui->openGLWidget);
    verticalSlider->setObjectName(QString::fromUtf8("verticalScrollBar"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(verticalSlider->sizePolicy().hasHeightForWidth());
    verticalSlider->setSizePolicy(sizePolicy1);
    verticalSlider->setMinimumSize(QSize(30, 350));
    verticalSlider->setMaximumSize(QSize(30, 400));

    //verticalSlider->setOrientation(Qt::Vertical);
    verticalLayout_in_2->addWidget(verticalSlider, 100, Qt::AlignHCenter);

    spinBox = new gcodeLabelEdit(ui->openGLWidget);
    spinBox->setObjectName(QString::fromUtf8("gcodeLabelEdit"));
    sizePolicy.setHeightForWidth(spinBox->sizePolicy().hasHeightForWidth());
    spinBox->setSizePolicy(sizePolicy);
    spinBox->setMinimumSize(QSize(125, 28));
    spinBox->setMaximumSize(QSize(125, 28));
//    QFont font;
//    font.setPointSize(8);
//    spinBox->setFont(font);
    verticalLayout_in_2->addWidget(spinBox);
    verticalSpacer_in_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout_in_2->addItem(verticalSpacer_in_2);

    verticalLayout_in_2->setStretch(0, 70);
    verticalLayout_in_2->setStretch(2, 100);
    verticalLayout_in_2->setStretch(4, 35);
    horizontalLayout_in->addLayout(verticalLayout_in_2);
    horizontalLayout_in->setStretch(0, 26);
    horizontalLayout_in->setStretch(1, 1);

    verticalLayout_in->addLayout(horizontalLayout_in);
    horizontalLayout_in_2 = new QHBoxLayout();
    horizontalLayout_in_2->setSpacing(6);
    horizontalLayout_in_2->setObjectName(QString::fromUtf8("horizontalLayout_in_2"));
    verticalLayout_in_3 = new QVBoxLayout();
    verticalLayout_in_3->setObjectName(QString::fromUtf8("verticalLayout_in_3"));
    horizontalLayout_in_3 = new QHBoxLayout();
    horizontalLayout_in_3->setObjectName(QString::fromUtf8("horizontalLayout_in_3"));
    horizontalSpacer_in_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_in_3->addItem(horizontalSpacer_in_4);

    //play button
    pushButton_9 = new QPushButton(ui->openGLWidget);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
    pushButton_9->setSizePolicy(sizePolicy2);
    pushButton_9->setMinimumSize(QSize(24, 24));
    pushButton_9->setMaximumSize(QSize(24, 24));
    pushButton_9->setCheckable(true);
    pushButton_9->setIconSize(QSize(24, 24));
    pushButton_9->setStyleSheet(QString::fromUtf8("QPushButton{\n"
        "    border: none;\n"
        "    image: url(:pic/ui/fdm_play_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QPushButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QPushButton:checked {\n"
        "    image: url(:pic/ui/fdm_suspended_icon_n.png);\n"
        "}\n"
        "\n"
        "QPushButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    horizontalLayout_in_3->addWidget(pushButton_9);

    horizontalSlider = new HSliderRang(ui->openGLWidget);
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalScrollBar"));
    sizePolicy2.setHeightForWidth(horizontalSlider->sizePolicy().hasHeightForWidth());
    horizontalSlider->setSizePolicy(sizePolicy2);

    horizontalSlider->setMinimumSize(QSize(400, 30));
    horizontalSlider->setMaximumSize(QSize(400, 30));

    //horizontalScrollBar->setOrientation(Qt::Horizontal);

    horizontalLayout_in_3->addWidget(horizontalSlider);

    horizontalSpacer_in_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_in_3->addItem(horizontalSpacer_in_5);


    verticalLayout_in_3->addLayout(horizontalLayout_in_3);

    horizontalLayout_in_4 = new QHBoxLayout();
    horizontalLayout_in_4->setSpacing(30);
    horizontalLayout_in_4->setObjectName(QString::fromUtf8("horizontalLayout_in_4"));
    horizontalSpacer_in_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_in_4->addItem(horizontalSpacer_in_2);

    QButtonGroup* pButtonGroup;
    pButtonGroup = new QButtonGroup(ui->openGLWidget);
    pButtonGroup->setExclusive(true);

    aiButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_ai = new QAction(QIcon(":pic/ui/fdm_ai_icon_n.png"), tr("Camera View"));
    actiona_ai->setCheckable(true);
    actiona_ai->setChecked(false);
    aiButton->setDefaultAction(actiona_ai);
    aiButton->setObjectName(QString::fromUtf8("aiButton"));
    sizePolicy2.setHeightForWidth(aiButton->sizePolicy().hasHeightForWidth());
    aiButton->setSizePolicy(sizePolicy2);
    aiButton->setMinimumSize(QSize(26, 26));
    aiButton->setMaximumSize(QSize(26, 26));
    //aiButton->setFlat(true);
    aiButton->setIconSize(QSize(26, 26));
    aiButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_ai_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_ai_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    //aiButton->setIcon(QPixmap("://pic//ui/fdm_ai_icon_n.png"));
    //aiButton->setCheckable(true);
    pButtonGroup->addButton(aiButton);

    horizontalLayout_in_4->addWidget(aiButton);

    justButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_just = new QAction(QIcon(":pic/ui/fdm_full_icon_n.png"), tr("Camera View"));
    actiona_just->setCheckable(true);
    actiona_just->setChecked(false);
    justButton->setDefaultAction(actiona_just);
    justButton->setObjectName(QString::fromUtf8("justButton"));
    sizePolicy2.setHeightForWidth(justButton->sizePolicy().hasHeightForWidth());
    justButton->setSizePolicy(sizePolicy2);
    justButton->setMinimumSize(QSize(26, 26));
    justButton->setMaximumSize(QSize(26, 26));
    //justButton->setCheckable(true);
    justButton->setIconSize(QSize(26, 26));
    justButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_full_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_full_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(justButton);

    horizontalLayout_in_4->addWidget(justButton);

    frontButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_front = new QAction(QIcon(":pic/ui/fdm_is_icon_n.png"), tr("Camera View"));
    actiona_front->setCheckable(true);
    actiona_front->setChecked(false);
    frontButton->setDefaultAction(actiona_front);
    frontButton->setObjectName(QString::fromUtf8("frontButton"));
    sizePolicy2.setHeightForWidth(frontButton->sizePolicy().hasHeightForWidth());
    frontButton->setSizePolicy(sizePolicy2);
    frontButton->setMinimumSize(QSize(26, 26));
    frontButton->setMaximumSize(QSize(26, 26));
    //frontButton->setCheckable(true);
    frontButton->setIconSize(QSize(26, 26));
    frontButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_is_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_is_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(frontButton);
    horizontalLayout_in_4->addWidget(frontButton);

    backButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_back = new QAction(QIcon(":pic/ui/fdm_back_icon_n.png"), tr("Camera View"));
    actiona_back->setCheckable(true);
    actiona_back->setChecked(false);
    backButton->setDefaultAction(actiona_back);
    backButton->setObjectName(QString::fromUtf8("backButton"));
    sizePolicy2.setHeightForWidth(backButton->sizePolicy().hasHeightForWidth());
    backButton->setSizePolicy(sizePolicy2);
    backButton->setMinimumSize(QSize(26, 26));
    backButton->setMaximumSize(QSize(26, 26));
    //backButton->setCheckable(true);
    backButton->setIconSize(QSize(26, 26));
    backButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_back_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_back_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(backButton);
    horizontalLayout_in_4->addWidget(backButton);

    leftButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_left = new QAction(QIcon(":pic/ui/fdm_left_icon_n.png"), tr("Camera View"));
    actiona_left->setCheckable(true);
    actiona_left->setChecked(false);
    leftButton->setDefaultAction(actiona_left);
    leftButton->setObjectName(QString::fromUtf8("leftButton"));
    sizePolicy2.setHeightForWidth(leftButton->sizePolicy().hasHeightForWidth());
    leftButton->setSizePolicy(sizePolicy2);
    leftButton->setMinimumSize(QSize(26, 26));
    leftButton->setMaximumSize(QSize(26, 26));
    //leftButton->setCheckable(true);
    leftButton->setIconSize(QSize(26, 26));
    leftButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_left_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_left_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(leftButton);
    horizontalLayout_in_4->addWidget(leftButton);

    rightButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_right = new QAction(QIcon(":pic/ui/fdm_right_icon_n.png"), tr("Camera View"));
    actiona_right->setCheckable(true);
    actiona_right->setChecked(false);
    rightButton->setDefaultAction(actiona_right);
    rightButton->setObjectName(QString::fromUtf8("rightButton"));
    sizePolicy2.setHeightForWidth(rightButton->sizePolicy().hasHeightForWidth());
    rightButton->setSizePolicy(sizePolicy2);
    rightButton->setMinimumSize(QSize(26, 26));
    rightButton->setMaximumSize(QSize(26, 26));
    //rightButton->setCheckable(true);
    rightButton->setIconSize(QSize(26, 26));
    rightButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_right_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_right_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(rightButton);
    horizontalLayout_in_4->addWidget(rightButton);

    topButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_top = new QAction(QIcon(":pic/ui/fdm_on_icon_n.png"), tr("Camera View"));
    actiona_top->setCheckable(true);
    actiona_top->setChecked(false);
    topButton->setDefaultAction(actiona_top);
    topButton->setObjectName(QString::fromUtf8("topButton"));
    topButton->setMinimumSize(QSize(26, 26));
    topButton->setMaximumSize(QSize(26, 26));
    //topButton->setCheckable(true);
    topButton->setIconSize(QSize(26, 26));
    topButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_on_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_on_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(topButton);
    horizontalLayout_in_4->addWidget(topButton);

    bottomButton = new QToolButton(ui->openGLWidget);
    QAction* actiona_bottom = new QAction(QIcon(":pic/ui/fdm_under_icon_n.png.png"), tr("Camera View"));
    actiona_bottom->setCheckable(true);
    actiona_bottom->setChecked(false);
    bottomButton->setDefaultAction(actiona_bottom);
    bottomButton->setObjectName(QString::fromUtf8("bottomButton"));
    sizePolicy2.setHeightForWidth(bottomButton->sizePolicy().hasHeightForWidth());
    bottomButton->setSizePolicy(sizePolicy2);
    bottomButton->setMinimumSize(QSize(26, 26));
    bottomButton->setMaximumSize(QSize(26, 26));
    //bottomButton->setCheckable(true);
    bottomButton->setIconSize(QSize(26, 26));
    bottomButton->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "    border: none;\n"
        "    border-radius:5;\n"
        "    image: url(:pic/ui/fdm_under_icon_n.png);\n"
        "    background-color:transparent;\n"
        "\n"
        "}\n"
        "QToolButton:hover {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "}\n"
        "\n"
        "QToolButton:checked {\n"
        "    background-color:rgb(208, 242, 216);\n"
        "    image: url(:pic/ui/fdm_under_icon_s.png);\n"
        "}\n"
        "\n"
        "QToolButton:pressed {\n"
        "    background-color:rgb(97, 211, 125);\n"
        "}"));
    pButtonGroup->addButton(bottomButton);
    horizontalLayout_in_4->addWidget(bottomButton);
    horizontalSpacer_in_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_in_4->addItem(horizontalSpacer_in_3);


    verticalLayout_in_3->addLayout(horizontalLayout_in_4);

    verticalLayout_in_3->setStretch(0, 2);
    verticalLayout_in_3->setStretch(1, 7);

    horizontalLayout_in_2->addLayout(verticalLayout_in_3);

    verticalLayout_in_5 = new QVBoxLayout();
    verticalLayout_in_5->setSpacing(16);
    verticalLayout_in_5->setObjectName(QString::fromUtf8("verticalLayout_in_5"));
    verticalLayout_in_5->setContentsMargins(24, 8, 24, 8);
    allTimeLabel = new QLabel(ui->openGLWidget);
    allTimeLabel->setObjectName(QString::fromUtf8("label"));
    allTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

    verticalLayout_in_5->addWidget(allTimeLabel);

    allfilamentLabel = new QLabel(ui->openGLWidget);
    allfilamentLabel->setObjectName(QString::fromUtf8("label_2"));
    allfilamentLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

    verticalLayout_in_5->addWidget(allfilamentLabel);

    allSizeLabel = new QLabel(ui->openGLWidget);
    allSizeLabel->setObjectName(QString::fromUtf8("label_3"));
    allSizeLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

    verticalLayout_in_5->addWidget(allSizeLabel);


    horizontalLayout_in_2->addLayout(verticalLayout_in_5);

    horizontalLayout_in_2->setStretch(0, 1390);
    horizontalLayout_in_2->setStretch(1, 224);

    verticalLayout_in->addLayout(horizontalLayout_in_2);

    verticalLayout_in->setStretch(0, 9);
    verticalLayout_in->setStretch(1, 1);


    //    QAction* aiAction = new QAction(QIcon(":/pic/ui/fdm_ai_icon_n.png"), tr("ai view"));
    //    aiAction->setCheckable(true);
    //    aiAction->setChecked(false);
    //    connect(aiAction, &QAction::toggled, this,&::viewWidget::setSavePicTrue);
    //    aiButton->setDefaultAction(aiAction);
    //    aiButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    connect(aiButton, SIGNAL(clicked()), this, SLOT(setSavePicTrue()));
    connect(frontButton, SIGNAL(clicked()), this, SLOT(onFrontView()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(onBackView()));
    connect(leftButton, SIGNAL(clicked()), this, SLOT(onLeftView()));
    connect(rightButton, SIGNAL(clicked()), this, SLOT(onRightView()));
    connect(topButton, SIGNAL(clicked()), this, SLOT(onTopView()));
    connect(bottomButton, SIGNAL(clicked()), this, SLOT(onBottomView()));
    connect(justButton, SIGNAL(clicked()), this, SLOT(onJustView()));
    connect(pushButton_9, SIGNAL(clicked()), this, SLOT(palyView()));


    /*replace start *********************************************/
    QFrame* line_2;

    QFrame* line_3;
    QFrame* line_4;
    QFrame* line_5;
    QFrame* line_6;
    QFrame* line_7;
    QFrame* line_8;
    QFrame* line_9;
    QFrame* line_10;
    QFrame* line_11; //zlap line
    QVBoxLayout* verticalLayout;   
    QHBoxLayout* linetypeLabelLayout;

    QHBoxLayout* ConsumedLabelLayout;



    QHBoxLayout* filamentLayout;
    QLabel* filamentPicLabel;

    QSpacerItem* horizontalSpacer_filamentLayout10;

    QHBoxLayout* timeLayout;
    QLabel* timePiclabel;
    QSpacerItem* horizontalSpacer_time11;

    QVBoxLayout* lineTypeLayout;

    verticalLayout = new QVBoxLayout(ui->widget);
    verticalLayout->setSpacing(9);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(16, 16, 16, 24);
#ifdef   USE_EXTRA_UI
    linetypeCombox = new QComboBox(ui->widget);
    linetypeCombox->addItem(tr("Line Type"));
    linetypeCombox->addItem(tr("Speed"));
    linetypeCombox->addItem(tr("Trapezoid"));
    linetypeCombox->addItem(tr("Flow"));
    verticalLayout->addWidget(linetypeCombox);
#else
    linetypelabel = new QLabel(ui->widget);
    linetypelabel->setObjectName(QString::fromUtf8("linetypelabel"));
    linetypelabel->setStyleSheet(" font: normal bold 16px;");
    linetypelabel->setText(tr("Line Type"));
    verticalLayout->addWidget(linetypelabel);
#endif
    typeLayout = new QStackedLayout();
    typeLayout->setSpacing(0);
    typeLayout->setContentsMargins(0,0,0,0);

    QWidget*  lineTypeWidget = new  QWidget(this);
    lineTypeLayout = new QVBoxLayout(lineTypeWidget); //for linetype Layout
    lineTypeLayout->setSpacing(9);
    lineTypeLayout->setObjectName(QString::fromUtf8("lineTypeLayout"));
    lineTypeLayout->setContentsMargins(0, 0, 0, 0);

    innerWallLayout = new QHBoxLayout();
    innerWallLayout->setSpacing(8);
    innerWallLayout->setObjectName(QString::fromUtf8("innerWallLayout"));
    innerWallLayout->setContentsMargins(0, 0, 0, 0);
    innerWallLabelC = new QPushButton(ui->widget);
    innerWallLabelC->setObjectName(QString::fromUtf8("innerWallLabelC"));
    innerWallLabelC->setStyleSheet(QString::fromUtf8("background-color: rgb(80,99,92);\n"
        "border-radius:5"));//QString::fromUtf8("background-color:rgb(51, 51, 51)\n""border-radius:5"));
    QSizePolicy sizePolicy11(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy11.setHorizontalStretch(0);
    sizePolicy11.setVerticalStretch(0);
    sizePolicy11.setHeightForWidth(innerWallLabelC->sizePolicy().hasHeightForWidth());
    innerWallLabelC->setSizePolicy(sizePolicy1);
    innerWallLabelC->setMinimumSize(QSize(20, 20));
    innerWallLabelC->setMaximumSize(QSize(20, 20));
    //innerWallLabelC->setAlignment(Qt::AlignCenter);
    innerWallLayout->addWidget(innerWallLabelC);
    innerWallLabel = new QLabel(ui->widget);
    innerWallLabel->setObjectName(QString::fromUtf8("innerWallLabel"));
    innerWallLabel->setText(tr("Inner Wall"));
    QSizePolicy sizePolicy21(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy21.setHorizontalStretch(0);
    sizePolicy21.setVerticalStretch(0);
    sizePolicy21.setHeightForWidth(innerWallLabel->sizePolicy().hasHeightForWidth());
    innerWallLabel->setSizePolicy(sizePolicy2);
    innerWallLabel->setMinimumSize(QSize(80, 15));
    innerWallLayout->addWidget(innerWallLabel);
    horizontalSpacer_innerWall = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    innerWallLayout->addItem(horizontalSpacer_innerWall);
    innerWallcheckBox = new SwitchButton(ui->widget);
    innerWallcheckBox->setObjectName(QString::fromUtf8("innerWallcheckBox"));
    innerWallcheckBox->setCheckState(true);
    innerWallcheckBox->setMinimumSize(QSize(41, 20));
    innerWallcheckBox->setMaximumSize(QSize(41, 20));
    innerWallLayout->addWidget(innerWallcheckBox);
    lineTypeLayout->addLayout(innerWallLayout);
    //verticalLayout->addLayout(innerWallLayout);

    line_2 = new QFrame(ui->widget);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
    line_2->setSizePolicy(sizePolicy3);
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_2);
    //verticalLayout->addWidget(line_2);

    outerWallLayout = new QHBoxLayout();
    outerWallLayout->setSpacing(8);
    outerWallLayout->setObjectName(QString::fromUtf8("outerWallLayout"));
    outerWallLayout->setContentsMargins(0, 0, 0, 0);
    outerWallLabelC = new QPushButton(ui->widget);
    outerWallLabelC->setObjectName(QString::fromUtf8("outerWallLabelC"));
    outerWallLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 144, 82, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(outerWallLabelC->sizePolicy().hasHeightForWidth());
    outerWallLabelC->setSizePolicy(sizePolicy1);
    outerWallLabelC->setMinimumSize(QSize(20, 20));
    outerWallLabelC->setMaximumSize(QSize(20, 20));
    //outerWallLabelC->setAlignment(Qt::AlignCenter);
    outerWallLayout->addWidget(outerWallLabelC);
    outerWallLabel = new QLabel(ui->widget);
    outerWallLabel->setObjectName(QString::fromUtf8("outerWallLabel"));
    outerWallLabel->setText(tr("Outer Wall"));
    sizePolicy2.setHeightForWidth(outerWallLabel->sizePolicy().hasHeightForWidth());
    outerWallLabel->setSizePolicy(sizePolicy2);
    outerWallLabel->setMinimumSize(QSize(80, 15));
    outerWallLayout->addWidget(outerWallLabel);
    horizontalSpacer_outerWall = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    outerWallLayout->addItem(horizontalSpacer_outerWall);
    outerWallcheckBox = new SwitchButton(ui->widget);
    outerWallcheckBox->setObjectName(QString::fromUtf8("outerWallcheckBox"));
    outerWallcheckBox->setMinimumSize(QSize(41, 20));
    outerWallcheckBox->setMaximumSize(QSize(41, 20));
    outerWallcheckBox->setCheckState(true);
    outerWallLayout->addWidget(outerWallcheckBox);
    lineTypeLayout->addLayout(outerWallLayout);
    //verticalLayout->addLayout(outerWallLayout);

    line_3 = new QFrame(ui->widget);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    sizePolicy3.setHeightForWidth(line_3->sizePolicy().hasHeightForWidth());
    line_3->setSizePolicy(sizePolicy3);
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_3);
    //verticalLayout->addWidget(line_3);

    skinLayout = new QHBoxLayout();
    skinLayout->setSpacing(8);
    skinLayout->setObjectName(QString::fromUtf8("skinLayout"));
    skinLayout->setContentsMargins(0, 0, 0, 0);
    skinLabelC = new QPushButton(ui->widget);
    skinLabelC->setObjectName(QString::fromUtf8("skinLabelC"));
    skinLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(97, 211, 125, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(skinLabelC->sizePolicy().hasHeightForWidth());
    skinLabelC->setSizePolicy(sizePolicy1);
    skinLabelC->setMinimumSize(QSize(20, 20));
    skinLabelC->setMaximumSize(QSize(20, 20));
    //skinLabelC->setAlignment(Qt::AlignCenter);
    skinLayout->addWidget(skinLabelC);
    skinLabel = new QLabel(ui->widget);
    skinLabel->setObjectName(QString::fromUtf8("skinLabel"));
    skinLabel->setText(tr("Skin"));
    sizePolicy2.setHeightForWidth(skinLabel->sizePolicy().hasHeightForWidth());
    skinLabel->setSizePolicy(sizePolicy2);
    skinLabel->setMinimumSize(QSize(80, 15));
    skinLayout->addWidget(skinLabel);
    horizontalSpacer_skin = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    skinLayout->addItem(horizontalSpacer_skin);
    skincheckBox = new SwitchButton(ui->widget);
    skincheckBox->setObjectName(QString::fromUtf8("skincheckBox"));
    skincheckBox->setCheckState(true);
    skincheckBox->setMinimumSize(QSize(41, 20));
    skincheckBox->setMaximumSize(QSize(41, 20));
    skinLayout->addWidget(skincheckBox);
    lineTypeLayout->addLayout(skinLayout);
    //verticalLayout->addLayout(skinLayout);

    line_4 = new QFrame(ui->widget);
    line_4->setObjectName(QString::fromUtf8("line_4"));
    sizePolicy3.setHeightForWidth(line_4->sizePolicy().hasHeightForWidth());
    line_4->setSizePolicy(sizePolicy3);
    line_4->setFrameShape(QFrame::HLine);
    line_4->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_4);
    //verticalLayout->addWidget(line_4);

    helperLayout = new QHBoxLayout();
    helperLayout->setSpacing(8);
    helperLayout->setObjectName(QString::fromUtf8("helperLayout"));
    helperLayout->setContentsMargins(0, 0, 0, 0);
    helperLabelC = new QPushButton(ui->widget);
    helperLabelC->setObjectName(QString::fromUtf8("helperLabelC"));
    helperLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(71, 143, 255, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(helperLabelC->sizePolicy().hasHeightForWidth());
    helperLabelC->setSizePolicy(sizePolicy1);
    helperLabelC->setMinimumSize(QSize(20, 20));
    helperLabelC->setMaximumSize(QSize(20, 20));
    //helperLabelC->setAlignment(Qt::AlignCenter);
    helperLayout->addWidget(helperLabelC);
    helperLabel = new QLabel(ui->widget);
    helperLabel->setObjectName(QString::fromUtf8("helperLabel"));
    helperLabel->setText(tr("Adhesion"));
    sizePolicy2.setHeightForWidth(helperLabel->sizePolicy().hasHeightForWidth());
    helperLabel->setSizePolicy(sizePolicy2);
    helperLabel->setMinimumSize(QSize(80, 15));
    helperLayout->addWidget(helperLabel);
    horizontalSpacer_helper = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    helperLayout->addItem(horizontalSpacer_helper);
    helpercheckBox = new SwitchButton(ui->widget);
    helpercheckBox->setObjectName(QString::fromUtf8("helpercheckBox"));
    helpercheckBox->setMinimumSize(QSize(41, 20));
    helpercheckBox->setMaximumSize(QSize(41, 20));
    helpercheckBox->setCheckState(true);
    helperLayout->addWidget(helpercheckBox);
    lineTypeLayout->addLayout(helperLayout);
    //verticalLayout->addLayout(helperLayout);

    line_5 = new QFrame(ui->widget);
    line_5->setObjectName(QString::fromUtf8("line_5"));
    sizePolicy3.setHeightForWidth(line_5->sizePolicy().hasHeightForWidth());
    line_5->setSizePolicy(sizePolicy3);
    line_5->setFrameShape(QFrame::HLine);
    line_5->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_5);
    //verticalLayout->addWidget(line_5);

    fillLayout = new QHBoxLayout();
    fillLayout->setSpacing(8);
    fillLayout->setObjectName(QString::fromUtf8("fillLayout"));
    fillLayout->setContentsMargins(0, 0, 0, 0);
    fillLabelC = new QPushButton(ui->widget);
    fillLabelC->setObjectName(QString::fromUtf8("fillLabelC"));
    fillLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(138, 76, 216, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(fillLabelC->sizePolicy().hasHeightForWidth());
    fillLabelC->setSizePolicy(sizePolicy1);
    fillLabelC->setMinimumSize(QSize(20, 20));
    fillLabelC->setMaximumSize(QSize(20, 20));
    //fillLabelC->setAlignment(Qt::AlignCenter);
    fillLayout->addWidget(fillLabelC);
    fillLabel = new QLabel(ui->widget);
    fillLabel->setObjectName(QString::fromUtf8("fillLabel"));
    fillLabel->setText(tr("Fill"));
    sizePolicy2.setHeightForWidth(fillLabel->sizePolicy().hasHeightForWidth());
    fillLabel->setSizePolicy(sizePolicy2);
    fillLabel->setMinimumSize(QSize(80, 15));
    fillLayout->addWidget(fillLabel);
    horizontalSpacer_fill = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    fillLayout->addItem(horizontalSpacer_fill);
    fillcheckBox = new SwitchButton(ui->widget);
    fillcheckBox->setObjectName(QString::fromUtf8("fillcheckBox"));
    fillcheckBox->setMinimumSize(QSize(41, 20));
    fillcheckBox->setMaximumSize(QSize(41, 20));
    fillcheckBox->setCheckState(true);
    fillLayout->addWidget(fillcheckBox);
    lineTypeLayout->addLayout(fillLayout);
    //verticalLayout->addLayout(fillLayout);
    line_6 = new QFrame(ui->widget);
    line_6->setObjectName(QString::fromUtf8("line_6"));
    sizePolicy3.setHeightForWidth(line_6->sizePolicy().hasHeightForWidth());
    line_6->setSizePolicy(sizePolicy3);
    line_6->setFrameShape(QFrame::HLine);
    line_6->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_6);
    //verticalLayout->addWidget(line_6);

    travelLayout = new QHBoxLayout();
    travelLayout->setSpacing(8);
    travelLayout->setObjectName(QString::fromUtf8("travelLayout"));
    travelLayout->setContentsMargins(0, 0, 0, 0);
    travelLabelC = new QPushButton(ui->widget);
    travelLabelC->setObjectName(QString::fromUtf8("travelLabelC"));
    travelLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(91, 207, 207, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(travelLabelC->sizePolicy().hasHeightForWidth());
    travelLabelC->setSizePolicy(sizePolicy1);
    travelLabelC->setMinimumSize(QSize(20, 20));
    travelLabelC->setMaximumSize(QSize(20, 20));
    //travelLabelC->setAlignment(Qt::AlignCenter);
    travelLayout->addWidget(travelLabelC);
    travelLabel = new QLabel(ui->widget);
    travelLabel->setObjectName(QString::fromUtf8("travelLabel"));
    travelLabel->setText(tr("Travel"));
    sizePolicy2.setHeightForWidth(travelLabel->sizePolicy().hasHeightForWidth());
    travelLabel->setSizePolicy(sizePolicy2);
    travelLabel->setMinimumSize(QSize(80, 15));
    travelLayout->addWidget(travelLabel);
    horizontalSpacer_travel = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    travelLayout->addItem(horizontalSpacer_travel);
    travelcheckBox = new SwitchButton(ui->widget);
    travelcheckBox->setObjectName(QString::fromUtf8("travecheckBox"));
    travelcheckBox->setMinimumSize(QSize(41, 20));
    travelcheckBox->setMaximumSize(QSize(41, 20));
    travelLayout->addWidget(travelcheckBox);
    lineTypeLayout->addLayout(travelLayout);
    //verticalLayout->addLayout(travelLayout);
#ifdef USE_EXTRA_UI
#endif
    line_11 = new QFrame(ui->widget);
    line_11->setObjectName(QString::fromUtf8("line_11"));
    sizePolicy3.setHeightForWidth(line_11->sizePolicy().hasHeightForWidth());
    line_11->setSizePolicy(sizePolicy3);
    line_11->setFrameShape(QFrame::HLine);
    line_11->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_11);
    //verticalLayout->addWidget(line_7);


    zlapLayout = new QHBoxLayout();
    zlapLayout->setSpacing(8);
    zlapLayout->setObjectName(QString::fromUtf8("zlapLayout"));
    zlapLayout->setContentsMargins(0, 0, 0, 0);
    zlapLabelC = new QPushButton(ui->widget);
    zlapLabelC->setObjectName(QString::fromUtf8("zlapLabelC"));
    zlapLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(zlapLabelC->sizePolicy().hasHeightForWidth());
    zlapLabelC->setSizePolicy(sizePolicy1);
    zlapLabelC->setMinimumSize(QSize(20, 20));
    zlapLabelC->setMaximumSize(QSize(20, 20));
    //zlapLabelC->setAlignment(Qt::AlignCenter);
    zlapLayout->addWidget(zlapLabelC);
    zlapLabel = new QLabel(ui->widget);
    zlapLabel->setObjectName(QString::fromUtf8("zlapLabel"));
    zlapLabel->setText(tr("zlap"));
    sizePolicy2.setHeightForWidth(zlapLabel->sizePolicy().hasHeightForWidth());
    zlapLabel->setSizePolicy(sizePolicy2);
    zlapLabel->setMinimumSize(QSize(80, 15));
    zlapLayout->addWidget(zlapLabel);
    horizontalSpacer_zlap = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    zlapLayout->addItem(horizontalSpacer_zlap);
    zlapcheckBox = new SwitchButton(ui->widget);
    zlapcheckBox->setObjectName(QString::fromUtf8("zlapcheckBox"));
    zlapcheckBox->setMinimumSize(QSize(41, 20));
    zlapcheckBox->setMaximumSize(QSize(41, 20));
    zlapLayout->addWidget(zlapcheckBox);
    lineTypeLayout->addLayout(zlapLayout);


    line_7 = new QFrame(ui->widget);
    line_7->setObjectName(QString::fromUtf8("line_7"));
    sizePolicy3.setHeightForWidth(line_7->sizePolicy().hasHeightForWidth());
    line_7->setSizePolicy(sizePolicy3);
    line_7->setFrameShape(QFrame::HLine);
    line_7->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_7);
    //verticalLayout->addWidget(line_7);

    supportLayout = new QHBoxLayout();
    supportLayout->setSpacing(8);
    supportLayout->setObjectName(QString::fromUtf8("supportLayout"));
    supportLayout->setContentsMargins(0, 0, 0, 0);
    supportLabelC = new QPushButton(ui->widget);
    supportLabelC->setObjectName(QString::fromUtf8("supportLabelC"));
    supportLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(249, 211, 86, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(supportLabelC->sizePolicy().hasHeightForWidth());
    supportLabelC->setSizePolicy(sizePolicy1);
    supportLabelC->setMinimumSize(QSize(20, 20));
    supportLabelC->setMaximumSize(QSize(20, 20));
    //supportLabelC->setAlignment(Qt::AlignCenter);
    supportLayout->addWidget(supportLabelC);
    supportLabel = new QLabel(ui->widget);
    supportLabel->setObjectName(QString::fromUtf8("supportLabel"));
    supportLabel->setText(tr("Support"));
    sizePolicy2.setHeightForWidth(supportLabel->sizePolicy().hasHeightForWidth());
    supportLabel->setSizePolicy(sizePolicy2);
    supportLabel->setMinimumSize(QSize(80, 15));
    supportLayout->addWidget(supportLabel);
    horizontalSpacer_support = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    supportLayout->addItem(horizontalSpacer_support);
    supportcheckBox = new SwitchButton(ui->widget);
    supportcheckBox->setObjectName(QString::fromUtf8("supportcheckBox"));
    supportcheckBox->setMinimumSize(QSize(41, 20));
    supportcheckBox->setMaximumSize(QSize(41, 20));
    supportcheckBox->setCheckState(true);
    supportLayout->addWidget(supportcheckBox);
    lineTypeLayout->addLayout(supportLayout);
    //verticalLayout->addLayout(supportLayout);

    line_8 = new QFrame(ui->widget);
    line_8->setObjectName(QString::fromUtf8("line_8"));
    sizePolicy3.setHeightForWidth(line_8->sizePolicy().hasHeightForWidth());
    line_8->setSizePolicy(sizePolicy3);
    line_8->setFrameShape(QFrame::HLine);
    line_8->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_8);
    //verticalLayout->addWidget(line_8);

    unkonwnLayout = new QHBoxLayout();
    unkonwnLayout->setSpacing(8);
    unkonwnLayout->setObjectName(QString::fromUtf8("unkonwnLayout"));
    unkonwnLayout->setContentsMargins(0, 0, 0, 0);
    unkonwnLabelC = new QPushButton(ui->widget);
    unkonwnLabelC->setObjectName(QString::fromUtf8("unkonwnLabelC"));
    unkonwnLabelC->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 105, 105, 1);\n"
        "border-radius:5"));
    sizePolicy1.setHeightForWidth(unkonwnLabelC->sizePolicy().hasHeightForWidth());
    unkonwnLabelC->setSizePolicy(sizePolicy1);
    unkonwnLabelC->setMinimumSize(QSize(20, 20));
    unkonwnLabelC->setMaximumSize(QSize(20, 20));
    //unkonwnLabelC->setAlignment(Qt::AlignCenter);
    unkonwnLayout->addWidget(unkonwnLabelC);
    unkonwnLabel = new QLabel(ui->widget);
    unkonwnLabel->setObjectName(QString::fromUtf8("unknowLabel"));
    unkonwnLabel->setText(tr("Unknown"));
    sizePolicy2.setHeightForWidth(unkonwnLabel->sizePolicy().hasHeightForWidth());
    unkonwnLabel->setSizePolicy(sizePolicy2);
    unkonwnLabel->setMinimumSize(QSize(80, 15));
    unkonwnLayout->addWidget(unkonwnLabel);
    horizontalSpacer_unkonwn = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    unkonwnLayout->addItem(horizontalSpacer_unkonwn);
    unkonwncheckBox = new SwitchButton(ui->widget);
    unkonwncheckBox->setObjectName(QString::fromUtf8("unknowcheckBox"));
    unkonwncheckBox->setMinimumSize(QSize(41, 20));
    unkonwncheckBox->setMaximumSize(QSize(41, 20));
    unkonwncheckBox->setCheckState(true);
    unkonwnLayout->addWidget(unkonwncheckBox);
    lineTypeLayout->addLayout(unkonwnLayout);
    //verticalLayout->addLayout(unkonwnLayout);

    line_9 = new QFrame(ui->widget);
    line_9->setObjectName(QString::fromUtf8("line_9"));
    sizePolicy3.setHeightForWidth(line_9->sizePolicy().hasHeightForWidth());
    line_9->setSizePolicy(sizePolicy3);
    line_9->setFrameShape(QFrame::HLine);
    line_9->setFrameShadow(QFrame::Sunken);
    lineTypeLayout->addWidget(line_9);
    //verticalLayout->addWidget(line_9);

    typeLayout->addWidget(lineTypeWidget);
    verticalLayout->addLayout(typeLayout);
#ifdef  USE_EXTRA_UI
    /////////// the freedrate (speed) widget   /////////////
    QVBoxLayout* speedLayout;
    QFrame* line_2_speed;
    QFrame* line_3_speed;
    QFrame* line_4_speed;
    QFrame* line_5_speed;
    QFrame* line_6_speed;
    QFrame* line_7_speed;
    QFrame* line_8_speed;
    QFrame* line_9_speed;
    QWidget*  speedWidget = new  QWidget();
    speedLayout = new QVBoxLayout(speedWidget); //for linetype Layout
    speedLayout->setSpacing(9);
    speedLayout->setObjectName(QString::fromUtf8("speedLayout"));
    speedLayout->setContentsMargins(10, 20, 10, 20);

     innerWallLayout_speed = new QHBoxLayout();
     innerWallLayout_speed->setSpacing(10);
     innerWallLayout_speed->setObjectName(QString::fromUtf8("innerWallLayout_speed"));
     innerWallLayout_speed->setContentsMargins(16, -1, 16, -1);

     innerWallLabel_speed = new QLabel(ui->widget);
     innerWallLabel_speed->setObjectName(QString::fromUtf8("innerWallLabel_speed"));
     innerWallLabel_speed->setText(tr("Inner Wall"));

     innerWallLabel_speed->setSizePolicy(sizePolicy2);
     innerWallLabel_speed->setMinimumSize(QSize(80, 15));
     innerWallLayout_speed->addWidget(innerWallLabel_speed);
     horizontalSpacer_innerWall_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     innerWallLayout_speed->addItem(horizontalSpacer_innerWall_speed);
     innerWallcheckBox_speed = new SwitchButton(ui->widget);
     innerWallcheckBox_speed->setObjectName(QString::fromUtf8("innerWallcheckBox_speed"));
     innerWallcheckBox_speed->setCheckState(true);
     innerWallcheckBox_speed->setMinimumSize(QSize(41, 20));
     innerWallcheckBox_speed->setMaximumSize(QSize(41, 20));
     innerWallLayout_speed->addWidget(innerWallcheckBox_speed);
     speedLayout->addLayout(innerWallLayout_speed);

     line_2_speed = new QFrame(ui->widget);
     line_2_speed->setObjectName(QString::fromUtf8("line_2_speed"));
     //QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
     sizePolicy3.setHorizontalStretch(0);
     sizePolicy3.setVerticalStretch(0);
     sizePolicy3.setHeightForWidth(line_2_speed->sizePolicy().hasHeightForWidth());
     line_2_speed->setSizePolicy(sizePolicy3);
     line_2_speed->setFrameShape(QFrame::HLine);
     line_2_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_2_speed);

     outerWallLayout_speed = new QHBoxLayout();
     outerWallLayout_speed->setSpacing(10);
     outerWallLayout_speed->setObjectName(QString::fromUtf8("outerWallLayout_speed"));
     outerWallLayout_speed->setContentsMargins(16, -1, 16, -1);

     outerWallLabel_speed = new QLabel(ui->widget);
     outerWallLabel_speed->setObjectName(QString::fromUtf8("outerWallLabel_speed"));
     outerWallLabel_speed->setText(tr("Outer Wall"));
     sizePolicy2.setHeightForWidth(outerWallLabel_speed->sizePolicy().hasHeightForWidth());
     outerWallLabel_speed->setSizePolicy(sizePolicy2);
     outerWallLabel_speed->setMinimumSize(QSize(80, 15));
     outerWallLayout_speed->addWidget(outerWallLabel_speed);
     horizontalSpacer_outerWall_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     outerWallLayout_speed->addItem(horizontalSpacer_outerWall_speed);
     outerWallcheckBox_speed = new SwitchButton(ui->widget);
     outerWallcheckBox_speed->setObjectName(QString::fromUtf8("outerWallcheckBox_speed"));
     outerWallcheckBox_speed->setMinimumSize(QSize(41, 20));
     outerWallcheckBox_speed->setMaximumSize(QSize(41, 20));
     outerWallcheckBox_speed->setCheckState(true);
     outerWallLayout_speed->addWidget(outerWallcheckBox_speed);
     speedLayout->addLayout(outerWallLayout_speed);

     line_3_speed = new QFrame(ui->widget);
     line_3_speed->setObjectName(QString::fromUtf8("line_3_speed"));
     sizePolicy3.setHeightForWidth(line_3_speed->sizePolicy().hasHeightForWidth());
     line_3_speed->setSizePolicy(sizePolicy3);
     line_3_speed->setFrameShape(QFrame::HLine);
     line_3_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_3_speed);
     //verticalLayout->addWidget(line_3_speed);

     skinLayout_speed = new QHBoxLayout();
     skinLayout_speed->setSpacing(10);
     skinLayout_speed->setObjectName(QString::fromUtf8("skinLayout_speed"));
     skinLayout_speed->setContentsMargins(16, -1, 16, -1);
     skinLabel_speed = new QLabel(ui->widget);
     skinLabel_speed->setObjectName(QString::fromUtf8("skinLabel_speed"));
     skinLabel_speed->setText(tr("Skin"));
     sizePolicy2.setHeightForWidth(skinLabel_speed->sizePolicy().hasHeightForWidth());
     skinLabel_speed->setSizePolicy(sizePolicy2);
     skinLabel_speed->setMinimumSize(QSize(80, 15));
     skinLayout_speed->addWidget(skinLabel_speed);
     horizontalSpacer_skin_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     skinLayout_speed->addItem(horizontalSpacer_skin_speed);
     skincheckBox_speed = new SwitchButton(ui->widget);
     skincheckBox_speed->setObjectName(QString::fromUtf8("skincheckBox_speed"));
     skincheckBox_speed->setCheckState(true);
     skincheckBox_speed->setMinimumSize(QSize(41, 20));
     skincheckBox_speed->setMaximumSize(QSize(41, 20));
     skinLayout_speed->addWidget(skincheckBox_speed);
     speedLayout->addLayout(skinLayout_speed);

     line_4_speed = new QFrame(ui->widget);
     line_4_speed->setObjectName(QString::fromUtf8("line_4_speed"));
     sizePolicy3.setHeightForWidth(line_4_speed->sizePolicy().hasHeightForWidth());
     line_4_speed->setSizePolicy(sizePolicy3);
     line_4_speed->setFrameShape(QFrame::HLine);
     line_4_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_4_speed);
     //verticalLayout->addWidget(line_4_speed);

     helperLayout_speed = new QHBoxLayout();
     helperLayout_speed->setSpacing(10);
     helperLayout_speed->setObjectName(QString::fromUtf8("helperLayout_speed"));
     helperLayout_speed->setContentsMargins(16, -1, 16, -1);
     helperLabel_speed = new QLabel(ui->widget);
     helperLabel_speed->setObjectName(QString::fromUtf8("helperLabel_speed"));
     helperLabel_speed->setText(tr("Adhesion"));
     sizePolicy2.setHeightForWidth(helperLabel_speed->sizePolicy().hasHeightForWidth());
     helperLabel_speed->setSizePolicy(sizePolicy2);
     helperLabel_speed->setMinimumSize(QSize(80, 15));
     helperLayout_speed->addWidget(helperLabel_speed);
     horizontalSpacer_helper_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     helperLayout_speed->addItem(horizontalSpacer_helper_speed);
     helpercheckBox_speed = new SwitchButton(ui->widget);
     helpercheckBox_speed->setObjectName(QString::fromUtf8("helpercheckBox_speed"));
     helpercheckBox_speed->setMinimumSize(QSize(41, 20));
     helpercheckBox_speed->setMaximumSize(QSize(41, 20));
     helpercheckBox_speed->setCheckState(true);
     helperLayout_speed->addWidget(helpercheckBox_speed);
     speedLayout->addLayout(helperLayout_speed);
     //verticalLayout->addLayout(helperLayout_speed);

     line_5_speed = new QFrame(ui->widget);
     line_5_speed->setObjectName(QString::fromUtf8("line_5_speed"));
     sizePolicy3.setHeightForWidth(line_5_speed->sizePolicy().hasHeightForWidth());
     line_5_speed->setSizePolicy(sizePolicy3);
     line_5_speed->setFrameShape(QFrame::HLine);
     line_5_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_5_speed);
     //verticalLayout->addWidget(line_5_speed);

     fillLayout_speed = new QHBoxLayout();
     fillLayout_speed->setSpacing(10);
     fillLayout_speed->setObjectName(QString::fromUtf8("fillLayout_speed"));
     fillLayout_speed->setContentsMargins(16, -1, 16, -1);
     fillLabel_speed = new QLabel(ui->widget);
     fillLabel_speed->setObjectName(QString::fromUtf8("fillLabel_speed"));
     fillLabel_speed->setText(tr("Fill"));
     sizePolicy2.setHeightForWidth(fillLabel_speed->sizePolicy().hasHeightForWidth());
     fillLabel_speed->setSizePolicy(sizePolicy2);
     fillLabel_speed->setMinimumSize(QSize(80, 15));
     fillLayout_speed->addWidget(fillLabel_speed);
     horizontalSpacer_fill_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     fillLayout_speed->addItem(horizontalSpacer_fill_speed);
     fillcheckBox_speed = new SwitchButton(ui->widget);
     fillcheckBox_speed->setObjectName(QString::fromUtf8("fillcheckBox_speed"));
     fillcheckBox_speed->setMinimumSize(QSize(41, 20));
     fillcheckBox_speed->setMaximumSize(QSize(41, 20));
     fillcheckBox_speed->setCheckState(true);
     fillLayout_speed->addWidget(fillcheckBox_speed);
     speedLayout->addLayout(fillLayout_speed);
     //verticalLayout->addLayout(fillLayout_speed);
     line_6_speed = new QFrame(ui->widget);
     line_6_speed->setObjectName(QString::fromUtf8("line_6_speed"));
     sizePolicy3.setHeightForWidth(line_6_speed->sizePolicy().hasHeightForWidth());
     line_6_speed->setSizePolicy(sizePolicy3);
     line_6_speed->setFrameShape(QFrame::HLine);
     line_6_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_6_speed);
     //verticalLayout->addWidget(line_6_speed);


     supportLayout_speed = new QHBoxLayout();
     supportLayout_speed->setSpacing(10);
     supportLayout_speed->setObjectName(QString::fromUtf8("supportLayout_speed"));
     supportLayout_speed->setContentsMargins(16, -1, 16, -1);
     supportLabel_speed = new QLabel(ui->widget);
     supportLabel_speed->setObjectName(QString::fromUtf8("supportLabel_speed"));
     supportLabel_speed->setText(tr("Support"));
     sizePolicy2.setHeightForWidth(supportLabel_speed->sizePolicy().hasHeightForWidth());
     supportLabel_speed->setSizePolicy(sizePolicy2);
     supportLabel_speed->setMinimumSize(QSize(80, 15));
     supportLayout_speed->addWidget(supportLabel_speed);
     horizontalSpacer_support_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     supportLayout_speed->addItem(horizontalSpacer_support_speed);
     supportcheckBox_speed = new SwitchButton(ui->widget);
     supportcheckBox_speed->setObjectName(QString::fromUtf8("supportcheckBox_speed"));
     supportcheckBox_speed->setMinimumSize(QSize(41, 20));
     supportcheckBox_speed->setMaximumSize(QSize(41, 20));
     supportcheckBox_speed->setCheckState(true);
     supportLayout_speed->addWidget(supportcheckBox_speed);
     speedLayout->addLayout(supportLayout_speed);
     //verticalLayout->addLayout(supportLayout_speed);

     line_8_speed = new QFrame(ui->widget);
     line_8_speed->setObjectName(QString::fromUtf8("line_8_speed"));
     sizePolicy3.setHeightForWidth(line_8_speed->sizePolicy().hasHeightForWidth());
     line_8_speed->setSizePolicy(sizePolicy3);
     line_8_speed->setFrameShape(QFrame::HLine);
     line_8_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_8_speed);
     //verticalLayout->addWidget(line_8_speed);

     unkonwnLayout_speed = new QHBoxLayout();
     unkonwnLayout_speed->setSpacing(10);
     unkonwnLayout_speed->setObjectName(QString::fromUtf8("unkonwnLayout_speed"));
     unkonwnLayout_speed->setContentsMargins(16, -1, 16, -1);
     unkonwnLabel_speed = new QLabel(ui->widget);
     unkonwnLabel_speed->setObjectName(QString::fromUtf8("unknowLabel"));
     unkonwnLabel_speed->setText(tr("Unknown"));
     sizePolicy2.setHeightForWidth(unkonwnLabel_speed->sizePolicy().hasHeightForWidth());
     unkonwnLabel_speed->setSizePolicy(sizePolicy2);
     unkonwnLabel_speed->setMinimumSize(QSize(80, 15));
     unkonwnLayout_speed->addWidget(unkonwnLabel_speed);
     horizontalSpacer_unkonwn_speed = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     unkonwnLayout_speed->addItem(horizontalSpacer_unkonwn_speed);
     unkonwncheckBox_speed = new SwitchButton(ui->widget);
     unkonwncheckBox_speed->setObjectName(QString::fromUtf8("unkonwncheckBox_speed"));
     unkonwncheckBox_speed->setMinimumSize(QSize(41, 20));
     unkonwncheckBox_speed->setMaximumSize(QSize(41, 20));
     unkonwncheckBox_speed->setCheckState(true);
     unkonwnLayout_speed->addWidget(unkonwncheckBox_speed);
     speedLayout->addLayout(unkonwnLayout_speed);
     //verticalLayout->addLayout(unkonwnLayout_speed);

     line_9_speed = new QFrame(ui->widget);
     line_9_speed->setObjectName(QString::fromUtf8("line_9_speed"));
     sizePolicy3.setHeightForWidth(line_9_speed->sizePolicy().hasHeightForWidth());
     line_9_speed->setSizePolicy(sizePolicy3);
     line_9_speed->setFrameShape(QFrame::HLine);
     line_9_speed->setFrameShadow(QFrame::Sunken);
     speedLayout->addWidget(line_9_speed);

     QVBoxLayout* colorPatch;
     QHBoxLayout* HLayout_colorPatch_label;
     QHBoxLayout* HLayout_clolorpatch;
     QSpacerItem* horizontalSpacer_cp;
     QSpacerItem* horizontalSpacer_cp_2;
     QLabel* colorPatch_label;
     colorPatch = new QVBoxLayout();
     colorPatch->setObjectName(QString::fromUtf8("colorPatch"));
     colorPatch->setContentsMargins(0, 0, 0, 0);
     HLayout_colorPatch_label = new QHBoxLayout();
     HLayout_colorPatch_label->setObjectName(QString::fromUtf8("HLayout_colorPatch_label"));
     colorPatch_label_start = new QLabel(ui->widget);
     colorPatch_label_start->setObjectName(QString::fromUtf8("colorPatch_label_start"));
     HLayout_colorPatch_label->addWidget(colorPatch_label_start);
     colorPatch_label_unit = new QLabel(ui->widget);
     colorPatch_label_unit->setObjectName(QString::fromUtf8("colorPatch_label_unit"));
     colorPatch_label_unit->setText(tr("mm/s"));

     HLayout_colorPatch_label->addWidget(colorPatch_label_unit);
     colorPatch_label_end = new QLabel(ui->widget);
     colorPatch_label_end->setObjectName(QString::fromUtf8("colorPatch_label_end"));
     HLayout_colorPatch_label->addWidget(colorPatch_label_end);
     colorPatch_label_start->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
     colorPatch_label_unit->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
     colorPatch_label_end->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);
//     colorPatch_label_start->setText("5");
//     colorPatch_label_end->setText("250");
     colorPatch->addLayout(HLayout_colorPatch_label);

     HLayout_clolorpatch = new QHBoxLayout();
     HLayout_clolorpatch->setSpacing(10);
     HLayout_clolorpatch->setObjectName(QString::fromUtf8("HLayout_clolorpatch"));
     horizontalSpacer_cp = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     HLayout_clolorpatch->addItem(horizontalSpacer_cp);
     colorPatch_label = new QLabel(ui->widget);
     colorPatch_label->setObjectName(QString::fromUtf8("colorPatch_label"));
     QSizePolicy sizePolicy232(QSizePolicy::Expanding, QSizePolicy::Preferred);
     sizePolicy232.setHorizontalStretch(0);
     sizePolicy232.setVerticalStretch(0);
     sizePolicy232.setHeightForWidth(colorPatch_label->sizePolicy().hasHeightForWidth());
     colorPatch_label->setSizePolicy(sizePolicy232);
     colorPatch_label->setMinimumSize(QSize(100, 20));
     colorPatch_label->setMaximumSize(QSize(99999, 9999999));
     colorPatch_label->setStyleSheet(QString::fromUtf8("background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(245, 102, 102), stop:0.02777 rgb(245, 114, 102), stop:0.05555 rgb(245, 126, 102), stop:0.083333 rgb(245, 137, 102),stop:0.1111 rgb(245, 149, 102), \n"
"stop:0.1666 rgb(245, 173, 102) , stop:0.22222 rgb(245, 197, 102), stop:0.25 rgb(245, 209, 102), stop:0.27777 rgb(245, 221, 102), stop:0.33333 rgb(245, 245, 102), \n"
"stop:0.3888 rgb(221, 245, 102), stop:0.44444 rgb(197, 245, 102), stop:0.5 rgb(173, 245, 102),\n"
"stop:0.555555556 rgb(149, 245, 102), \n"
"stop:0.611111111 rgb(126, 245, 102), \n"
"stop:0.666666667 rgb(102, 245, 102), \n"
"stop:0.722222222 rgb(102, 245, 126) , \n"
"stop:0.777777778 rgb(102, 245, 149), \n"
"stop:0.27777 rgb(245, 221, 102), \n"
"stop:0.833333333 rgb(102, 245, 173), \n"
"stop:0.888888889 rgb(102, 245, 197), \n"
"stop:0.944444444 rgb(102, 245, 221), \n"
"stop:1 rgb(102, 245, 245));"));
     HLayout_clolorpatch->addWidget(colorPatch_label);
     horizontalSpacer_cp_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     HLayout_clolorpatch->addItem(horizontalSpacer_cp_2);
     HLayout_clolorpatch->setStretch(0, 1);
     HLayout_clolorpatch->setStretch(1, 8);
     HLayout_clolorpatch->setStretch(2, 1);
     colorPatch->addLayout(HLayout_clolorpatch);

    speedLayout->addLayout(colorPatch);

    typeLayout->addWidget(speedWidget);
    //////////// the freedrate (speed) widget  end ////////////

    //////////// the Trapezoid widget  ////////////
    QVBoxLayout* trapezoidLayout;
    QFrame* line_2_trapezoid;
    QFrame* line_3_trapezoid;
    QFrame* line_4_trapezoid;
    QFrame* line_5_trapezoid;
    QFrame* line_6_trapezoid;
    QFrame* line_7_trapezoid;
    QFrame* line_8_trapezoid;
    QFrame* line_9_trapezoid;
    QWidget*  trapezoidWidget = new  QWidget();
    trapezoidLayout = new QVBoxLayout(trapezoidWidget); //for linetype Layout
    trapezoidLayout->setSpacing(9);
    trapezoidLayout->setObjectName(QString::fromUtf8("trapezoidLayout"));
    trapezoidLayout->setContentsMargins(10, 20, 10, 20);

     innerWallLayout_trapezoid = new QHBoxLayout();
     innerWallLayout_trapezoid->setSpacing(10);
     innerWallLayout_trapezoid->setObjectName(QString::fromUtf8("innerWallLayout_trapezoid"));
     innerWallLayout_trapezoid->setContentsMargins(16, -1, 16, -1);

     innerWallLabel_trapezoid = new QLabel(ui->widget);
     innerWallLabel_trapezoid->setObjectName(QString::fromUtf8("innerWallLabel_trapezoid"));
     innerWallLabel_trapezoid->setText(tr("Inner Wall"));

     innerWallLabel_trapezoid->setSizePolicy(sizePolicy2);
     innerWallLabel_trapezoid->setMinimumSize(QSize(80, 15));
     innerWallLayout_trapezoid->addWidget(innerWallLabel_trapezoid);
     horizontalSpacer_innerWall_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     innerWallLayout_trapezoid->addItem(horizontalSpacer_innerWall_trapezoid);
     innerWallcheckBox_trapezoid = new SwitchButton(ui->widget);
     innerWallcheckBox_trapezoid->setObjectName(QString::fromUtf8("innerWallcheckBox_trapezoid"));
     innerWallcheckBox_trapezoid->setCheckState(true);
     innerWallcheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     innerWallcheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     innerWallLayout_trapezoid->addWidget(innerWallcheckBox_trapezoid);
     trapezoidLayout->addLayout(innerWallLayout_trapezoid);

     line_2_trapezoid = new QFrame(ui->widget);
     line_2_trapezoid->setObjectName(QString::fromUtf8("line_2_trapezoid"));
     //QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
     sizePolicy3.setHorizontalStretch(0);
     sizePolicy3.setVerticalStretch(0);
     sizePolicy3.setHeightForWidth(line_2_trapezoid->sizePolicy().hasHeightForWidth());
     line_2_trapezoid->setSizePolicy(sizePolicy3);
     line_2_trapezoid->setFrameShape(QFrame::HLine);
     line_2_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_2_trapezoid);

     outerWallLayout_trapezoid = new QHBoxLayout();
     outerWallLayout_trapezoid->setSpacing(10);
     outerWallLayout_trapezoid->setObjectName(QString::fromUtf8("outerWallLayout_trapezoid"));
     outerWallLayout_trapezoid->setContentsMargins(16, -1, 16, -1);

     outerWallLabel_trapezoid = new QLabel(ui->widget);
     outerWallLabel_trapezoid->setObjectName(QString::fromUtf8("outerWallLabel_trapezoid"));
     outerWallLabel_trapezoid->setText(tr("Outer Wall"));
     sizePolicy2.setHeightForWidth(outerWallLabel_trapezoid->sizePolicy().hasHeightForWidth());
     outerWallLabel_trapezoid->setSizePolicy(sizePolicy2);
     outerWallLabel_trapezoid->setMinimumSize(QSize(80, 15));
     outerWallLayout_trapezoid->addWidget(outerWallLabel_trapezoid);
     horizontalSpacer_outerWall_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     outerWallLayout_trapezoid->addItem(horizontalSpacer_outerWall_trapezoid);
     outerWallcheckBox_trapezoid = new SwitchButton(ui->widget);
     outerWallcheckBox_trapezoid->setObjectName(QString::fromUtf8("outerWallcheckBox_trapezoid"));
     outerWallcheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     outerWallcheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     outerWallcheckBox_trapezoid->setCheckState(true);
     outerWallLayout_trapezoid->addWidget(outerWallcheckBox_trapezoid);
     trapezoidLayout->addLayout(outerWallLayout_trapezoid);

     line_3_trapezoid = new QFrame(ui->widget);
     line_3_trapezoid->setObjectName(QString::fromUtf8("line_3_trapezoid"));
     sizePolicy3.setHeightForWidth(line_3_trapezoid->sizePolicy().hasHeightForWidth());
     line_3_trapezoid->setSizePolicy(sizePolicy3);
     line_3_trapezoid->setFrameShape(QFrame::HLine);
     line_3_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_3_trapezoid);
     //verticalLayout->addWidget(line_3_trapezoid);

     skinLayout_trapezoid = new QHBoxLayout();
     skinLayout_trapezoid->setSpacing(10);
     skinLayout_trapezoid->setObjectName(QString::fromUtf8("skinLayout_trapezoid"));
     skinLayout_trapezoid->setContentsMargins(16, -1, 16, -1);
     skinLabel_trapezoid = new QLabel(ui->widget);
     skinLabel_trapezoid->setObjectName(QString::fromUtf8("skinLabel_trapezoid"));
     skinLabel_trapezoid->setText(tr("Skin"));
     sizePolicy2.setHeightForWidth(skinLabel_trapezoid->sizePolicy().hasHeightForWidth());
     skinLabel_trapezoid->setSizePolicy(sizePolicy2);
     skinLabel_trapezoid->setMinimumSize(QSize(80, 15));
     skinLayout_trapezoid->addWidget(skinLabel_trapezoid);
     horizontalSpacer_skin_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     skinLayout_trapezoid->addItem(horizontalSpacer_skin_trapezoid);
     skincheckBox_trapezoid = new SwitchButton(ui->widget);
     skincheckBox_trapezoid->setObjectName(QString::fromUtf8("skincheckBox_trapezoid"));
     skincheckBox_trapezoid->setCheckState(true);
     skincheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     skincheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     skinLayout_trapezoid->addWidget(skincheckBox_trapezoid);
     trapezoidLayout->addLayout(skinLayout_trapezoid);

     line_4_trapezoid = new QFrame(ui->widget);
     line_4_trapezoid->setObjectName(QString::fromUtf8("line_4_trapezoid"));
     sizePolicy3.setHeightForWidth(line_4_trapezoid->sizePolicy().hasHeightForWidth());
     line_4_trapezoid->setSizePolicy(sizePolicy3);
     line_4_trapezoid->setFrameShape(QFrame::HLine);
     line_4_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_4_trapezoid);
     //verticalLayout->addWidget(line_4_trapezoid);

     helperLayout_trapezoid = new QHBoxLayout();
     helperLayout_trapezoid->setSpacing(10);
     helperLayout_trapezoid->setObjectName(QString::fromUtf8("helperLayout_trapezoid"));
     helperLayout_trapezoid->setContentsMargins(16, -1, 16, -1);
     helperLabel_trapezoid = new QLabel(ui->widget);
     helperLabel_trapezoid->setObjectName(QString::fromUtf8("helperLabel_trapezoid"));
     helperLabel_trapezoid->setText(tr("Adhesion"));
     sizePolicy2.setHeightForWidth(helperLabel_trapezoid->sizePolicy().hasHeightForWidth());
     helperLabel_trapezoid->setSizePolicy(sizePolicy2);
     helperLabel_trapezoid->setMinimumSize(QSize(80, 15));
     helperLayout_trapezoid->addWidget(helperLabel_trapezoid);
     horizontalSpacer_helper_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     helperLayout_trapezoid->addItem(horizontalSpacer_helper_trapezoid);
     helpercheckBox_trapezoid = new SwitchButton(ui->widget);
     helpercheckBox_trapezoid->setObjectName(QString::fromUtf8("helpercheckBox_trapezoid"));
     helpercheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     helpercheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     helpercheckBox_trapezoid->setCheckState(true);
     helperLayout_trapezoid->addWidget(helpercheckBox_trapezoid);
     trapezoidLayout->addLayout(helperLayout_trapezoid);
     //verticalLayout->addLayout(helperLayout_trapezoid);

     line_5_trapezoid = new QFrame(ui->widget);
     line_5_trapezoid->setObjectName(QString::fromUtf8("line_5_trapezoid"));
     sizePolicy3.setHeightForWidth(line_5_trapezoid->sizePolicy().hasHeightForWidth());
     line_5_trapezoid->setSizePolicy(sizePolicy3);
     line_5_trapezoid->setFrameShape(QFrame::HLine);
     line_5_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_5_trapezoid);
     //verticalLayout->addWidget(line_5_trapezoid);

     fillLayout_trapezoid = new QHBoxLayout();
     fillLayout_trapezoid->setSpacing(10);
     fillLayout_trapezoid->setObjectName(QString::fromUtf8("fillLayout_trapezoid"));
     fillLayout_trapezoid->setContentsMargins(16, -1, 16, -1);
     fillLabel_trapezoid = new QLabel(ui->widget);
     fillLabel_trapezoid->setObjectName(QString::fromUtf8("fillLabel_trapezoid"));
     fillLabel_trapezoid->setText(tr("Fill"));
     sizePolicy2.setHeightForWidth(fillLabel_trapezoid->sizePolicy().hasHeightForWidth());
     fillLabel_trapezoid->setSizePolicy(sizePolicy2);
     fillLabel_trapezoid->setMinimumSize(QSize(80, 15));
     fillLayout_trapezoid->addWidget(fillLabel_trapezoid);
     horizontalSpacer_fill_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     fillLayout_trapezoid->addItem(horizontalSpacer_fill_trapezoid);
     fillcheckBox_trapezoid = new SwitchButton(ui->widget);
     fillcheckBox_trapezoid->setObjectName(QString::fromUtf8("fillcheckBox_trapezoid"));
     fillcheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     fillcheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     fillcheckBox_trapezoid->setCheckState(true);
     fillLayout_trapezoid->addWidget(fillcheckBox_trapezoid);
     trapezoidLayout->addLayout(fillLayout_trapezoid);
     //verticalLayout->addLayout(fillLayout_trapezoid);
     line_6_trapezoid = new QFrame(ui->widget);
     line_6_trapezoid->setObjectName(QString::fromUtf8("line_6_trapezoid"));
     sizePolicy3.setHeightForWidth(line_6_trapezoid->sizePolicy().hasHeightForWidth());
     line_6_trapezoid->setSizePolicy(sizePolicy3);
     line_6_trapezoid->setFrameShape(QFrame::HLine);
     line_6_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_6_trapezoid);
     //verticalLayout->addWidget(line_6_trapezoid);


     supportLayout_trapezoid = new QHBoxLayout();
     supportLayout_trapezoid->setSpacing(10);
     supportLayout_trapezoid->setObjectName(QString::fromUtf8("supportLayout_trapezoid"));
     supportLayout_trapezoid->setContentsMargins(16, -1, 16, -1);
     supportLabel_trapezoid = new QLabel(ui->widget);
     supportLabel_trapezoid->setObjectName(QString::fromUtf8("supportLabel_trapezoid"));
     supportLabel_trapezoid->setText(tr("Support"));
     sizePolicy2.setHeightForWidth(supportLabel_trapezoid->sizePolicy().hasHeightForWidth());
     supportLabel_trapezoid->setSizePolicy(sizePolicy2);
     supportLabel_trapezoid->setMinimumSize(QSize(80, 15));
     supportLayout_trapezoid->addWidget(supportLabel_trapezoid);
     horizontalSpacer_support_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     supportLayout_trapezoid->addItem(horizontalSpacer_support_trapezoid);
     supportcheckBox_trapezoid = new SwitchButton(ui->widget);
     supportcheckBox_trapezoid->setObjectName(QString::fromUtf8("supportcheckBox_trapezoid"));
     supportcheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     supportcheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     supportcheckBox_trapezoid->setCheckState(true);
     supportLayout_trapezoid->addWidget(supportcheckBox_trapezoid);
     trapezoidLayout->addLayout(supportLayout_trapezoid);
     //verticalLayout->addLayout(supportLayout_trapezoid);

     line_8_trapezoid = new QFrame(ui->widget);
     line_8_trapezoid->setObjectName(QString::fromUtf8("line_8_trapezoid"));
     sizePolicy3.setHeightForWidth(line_8_trapezoid->sizePolicy().hasHeightForWidth());
     line_8_trapezoid->setSizePolicy(sizePolicy3);
     line_8_trapezoid->setFrameShape(QFrame::HLine);
     line_8_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_8_trapezoid);
     //verticalLayout->addWidget(line_8_trapezoid);

     unkonwnLayout_trapezoid = new QHBoxLayout();
     unkonwnLayout_trapezoid->setSpacing(10);
     unkonwnLayout_trapezoid->setObjectName(QString::fromUtf8("unkonwnLayout_trapezoid"));
     unkonwnLayout_trapezoid->setContentsMargins(16, -1, 16, -1);
     unkonwnLabel_trapezoid = new QLabel(ui->widget);
     unkonwnLabel_trapezoid->setObjectName(QString::fromUtf8("unknowLabel"));
     unkonwnLabel_trapezoid->setText(tr("Unknown"));
     sizePolicy2.setHeightForWidth(unkonwnLabel_trapezoid->sizePolicy().hasHeightForWidth());
     unkonwnLabel_trapezoid->setSizePolicy(sizePolicy2);
     unkonwnLabel_trapezoid->setMinimumSize(QSize(80, 15));
     unkonwnLayout_trapezoid->addWidget(unkonwnLabel_trapezoid);
     horizontalSpacer_unkonwn_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     unkonwnLayout_trapezoid->addItem(horizontalSpacer_unkonwn_trapezoid);
     unkonwncheckBox_trapezoid = new SwitchButton(ui->widget);
     unkonwncheckBox_trapezoid->setObjectName(QString::fromUtf8("unkonwncheckBox_trapezoid"));
     unkonwncheckBox_trapezoid->setMinimumSize(QSize(41, 20));
     unkonwncheckBox_trapezoid->setMaximumSize(QSize(41, 20));
     unkonwncheckBox_trapezoid->setCheckState(true);
     unkonwnLayout_trapezoid->addWidget(unkonwncheckBox_trapezoid);
     trapezoidLayout->addLayout(unkonwnLayout_trapezoid);
     //verticalLayout->addLayout(unkonwnLayout_trapezoid);

     line_9_trapezoid = new QFrame(ui->widget);
     line_9_trapezoid->setObjectName(QString::fromUtf8("line_9_trapezoid"));
     sizePolicy3.setHeightForWidth(line_9_trapezoid->sizePolicy().hasHeightForWidth());
     line_9_trapezoid->setSizePolicy(sizePolicy3);
     line_9_trapezoid->setFrameShape(QFrame::HLine);
     line_9_trapezoid->setFrameShadow(QFrame::Sunken);
     trapezoidLayout->addWidget(line_9_trapezoid);

     QVBoxLayout* colorPatch_trapezoid;
     QHBoxLayout* HLayout_colorPatch_label_trapezoid;
     QLabel* colorPatch_label_unit_trapezoid;
     QHBoxLayout* HLayout_clolorpatch_trapezoid;
     QSpacerItem* horizontalSpacer_cp_trapezoid;
     QSpacerItem* horizontalSpacer_cp_2_trapezoid;
     QLabel* colorPatch_label_trapezoid;
     colorPatch_trapezoid = new QVBoxLayout();
     colorPatch_trapezoid->setObjectName(QString::fromUtf8("colorPatch_trapezoid"));
     colorPatch_trapezoid->setContentsMargins(0, 0, 0, 0);
     HLayout_colorPatch_label_trapezoid = new QHBoxLayout();
     HLayout_colorPatch_label_trapezoid->setObjectName(QString::fromUtf8("HLayout_colorPatch_label_trapezoid"));
     colorPatch_label_start_trapezoid = new QLabel(ui->widget);
     colorPatch_label_start_trapezoid->setObjectName(QString::fromUtf8("colorPatch_label_start_trapezoid"));
     HLayout_colorPatch_label_trapezoid->addWidget(colorPatch_label_start_trapezoid);
     colorPatch_label_unit_trapezoid = new QLabel(ui->widget);
     colorPatch_label_unit_trapezoid->setObjectName(QString::fromUtf8("colorPatch_label_unit_trapezoid"));
     colorPatch_label_unit_trapezoid->setText(tr("mm/s"));

     HLayout_colorPatch_label_trapezoid->addWidget(colorPatch_label_unit_trapezoid);
     colorPatch_label_end_trapezoid = new QLabel(ui->widget);
     colorPatch_label_end_trapezoid->setObjectName(QString::fromUtf8("colorPatch_label_end_trapezoid"));
     HLayout_colorPatch_label_trapezoid->addWidget(colorPatch_label_end_trapezoid);
     colorPatch_label_start_trapezoid->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
     colorPatch_label_unit_trapezoid->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
     colorPatch_label_end_trapezoid->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);
     colorPatch_label_start_trapezoid->setText("5");
     colorPatch_label_end_trapezoid->setText("250");
     colorPatch_trapezoid->addLayout(HLayout_colorPatch_label_trapezoid);

     HLayout_clolorpatch_trapezoid = new QHBoxLayout();
     HLayout_clolorpatch_trapezoid->setSpacing(10);
     HLayout_clolorpatch_trapezoid->setObjectName(QString::fromUtf8("HLayout_clolorpatch_trapezoid"));
     horizontalSpacer_cp_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     HLayout_clolorpatch_trapezoid->addItem(horizontalSpacer_cp_trapezoid);
     colorPatch_label_trapezoid = new QLabel(ui->widget);
     colorPatch_label_trapezoid->setObjectName(QString::fromUtf8("colorPatch_label_trapezoid"));
     QSizePolicy sizePolicy2323(QSizePolicy::Expanding, QSizePolicy::Preferred);
     sizePolicy232.setHorizontalStretch(0);
     sizePolicy232.setVerticalStretch(0);
     sizePolicy232.setHeightForWidth(colorPatch_label_trapezoid->sizePolicy().hasHeightForWidth());
     colorPatch_label_trapezoid->setSizePolicy(sizePolicy2323);
     colorPatch_label_trapezoid->setMinimumSize(QSize(100, 20));
     colorPatch_label_trapezoid->setMaximumSize(QSize(99999, 9999999));
     colorPatch_label_trapezoid->setStyleSheet(QString::fromUtf8("background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(245, 102, 102), stop:0.02777 rgb(245, 114, 102), stop:0.05555 rgb(245, 126, 102), stop:0.083333 rgb(245, 137, 102),stop:0.1111 rgb(245, 149, 102), \n"
"stop:0.1666 rgb(245, 173, 102) , stop:0.22222 rgb(245, 197, 102), stop:0.25 rgb(245, 209, 102), stop:0.27777 rgb(245, 221, 102), stop:0.33333 rgb(245, 245, 102), \n"
"stop:0.3888 rgb(221, 245, 102), stop:0.44444 rgb(197, 245, 102), stop:0.5 rgb(173, 245, 102),\n"
"stop:0.555555556 rgb(149, 245, 102), \n"
"stop:0.611111111 rgb(126, 245, 102), \n"
"stop:0.666666667 rgb(102, 245, 102), \n"
"stop:0.722222222 rgb(102, 245, 126) , \n"
"stop:0.777777778 rgb(102, 245, 149), \n"
"stop:0.27777 rgb(245, 221, 102), \n"
"stop:0.833333333 rgb(102, 245, 173), \n"
"stop:0.888888889 rgb(102, 245, 197), \n"
"stop:0.944444444 rgb(102, 245, 221), \n"
"stop:1 rgb(102, 245, 245));"));
     HLayout_clolorpatch_trapezoid->addWidget(colorPatch_label_trapezoid);
     horizontalSpacer_cp_2_trapezoid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     HLayout_clolorpatch_trapezoid->addItem(horizontalSpacer_cp_2_trapezoid);
     HLayout_clolorpatch_trapezoid->setStretch(0, 1);
     HLayout_clolorpatch_trapezoid->setStretch(1, 8);
     HLayout_clolorpatch_trapezoid->setStretch(2, 1);
     colorPatch_trapezoid->addLayout(HLayout_clolorpatch_trapezoid);

    trapezoidLayout->addLayout(colorPatch_trapezoid);

    typeLayout->addWidget(trapezoidWidget);

    //////////// the Trapezoid widget  end////////////

    //////////// the Flow widget  ////////////
    QVBoxLayout* FlowLayout;
    QFrame* line_2_Flow;
    QFrame* line_3_Flow;
    QFrame* line_4_Flow;
    QFrame* line_5_Flow;
    QFrame* line_6_Flow;
    QFrame* line_7_Flow;
    QFrame* line_8_Flow;
    QFrame* line_9_Flow;
    QWidget*  FlowWidget = new  QWidget();
    FlowLayout = new QVBoxLayout(FlowWidget); //for linetype Layout
    FlowLayout->setSpacing(9);
    FlowLayout->setObjectName(QString::fromUtf8("FlowLayout"));
    FlowLayout->setContentsMargins(10, 20, 10, 20);

     innerWallLayout_Flow = new QHBoxLayout();
     innerWallLayout_Flow->setSpacing(10);
     innerWallLayout_Flow->setObjectName(QString::fromUtf8("innerWallLayout_Flow"));
     innerWallLayout_Flow->setContentsMargins(16, -1, 16, -1);

     innerWallLabel_Flow = new QLabel(ui->widget);
     innerWallLabel_Flow->setObjectName(QString::fromUtf8("innerWallLabel_Flow"));
     innerWallLabel_Flow->setText(tr("Inner Wall"));

     innerWallLabel_Flow->setSizePolicy(sizePolicy2);
     innerWallLabel_Flow->setMinimumSize(QSize(80, 15));
     innerWallLayout_Flow->addWidget(innerWallLabel_Flow);
     horizontalSpacer_innerWall_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     innerWallLayout_Flow->addItem(horizontalSpacer_innerWall_Flow);
     innerWallcheckBox_Flow = new SwitchButton(ui->widget);
     innerWallcheckBox_Flow->setObjectName(QString::fromUtf8("innerWallcheckBox_Flow"));
     innerWallcheckBox_Flow->setCheckState(true);
     innerWallcheckBox_Flow->setMinimumSize(QSize(41, 20));
     innerWallcheckBox_Flow->setMaximumSize(QSize(41, 20));
     innerWallLayout_Flow->addWidget(innerWallcheckBox_Flow);
     FlowLayout->addLayout(innerWallLayout_Flow);

     line_2_Flow = new QFrame(ui->widget);
     line_2_Flow->setObjectName(QString::fromUtf8("line_2_Flow"));
     //QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
     sizePolicy3.setHorizontalStretch(0);
     sizePolicy3.setVerticalStretch(0);
     sizePolicy3.setHeightForWidth(line_2_Flow->sizePolicy().hasHeightForWidth());
     line_2_Flow->setSizePolicy(sizePolicy3);
     line_2_Flow->setFrameShape(QFrame::HLine);
     line_2_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_2_Flow);

     outerWallLayout_Flow = new QHBoxLayout();
     outerWallLayout_Flow->setSpacing(10);
     outerWallLayout_Flow->setObjectName(QString::fromUtf8("outerWallLayout_Flow"));
     outerWallLayout_Flow->setContentsMargins(16, -1, 16, -1);

     outerWallLabel_Flow = new QLabel(ui->widget);
     outerWallLabel_Flow->setObjectName(QString::fromUtf8("outerWallLabel_Flow"));
     outerWallLabel_Flow->setText(tr("Outer Wall"));
     sizePolicy2.setHeightForWidth(outerWallLabel_Flow->sizePolicy().hasHeightForWidth());
     outerWallLabel_Flow->setSizePolicy(sizePolicy2);
     outerWallLabel_Flow->setMinimumSize(QSize(80, 15));
     outerWallLayout_Flow->addWidget(outerWallLabel_Flow);
     horizontalSpacer_outerWall_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     outerWallLayout_Flow->addItem(horizontalSpacer_outerWall_Flow);
     outerWallcheckBox_Flow = new SwitchButton(ui->widget);
     outerWallcheckBox_Flow->setObjectName(QString::fromUtf8("outerWallcheckBox_Flow"));
     outerWallcheckBox_Flow->setMinimumSize(QSize(41, 20));
     outerWallcheckBox_Flow->setMaximumSize(QSize(41, 20));
     outerWallcheckBox_Flow->setCheckState(true);
     outerWallLayout_Flow->addWidget(outerWallcheckBox_Flow);
     FlowLayout->addLayout(outerWallLayout_Flow);

     line_3_Flow = new QFrame(ui->widget);
     line_3_Flow->setObjectName(QString::fromUtf8("line_3_Flow"));
     sizePolicy3.setHeightForWidth(line_3_Flow->sizePolicy().hasHeightForWidth());
     line_3_Flow->setSizePolicy(sizePolicy3);
     line_3_Flow->setFrameShape(QFrame::HLine);
     line_3_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_3_Flow);
     //verticalLayout->addWidget(line_3_Flow);

     skinLayout_Flow = new QHBoxLayout();
     skinLayout_Flow->setSpacing(10);
     skinLayout_Flow->setObjectName(QString::fromUtf8("skinLayout_Flow"));
     skinLayout_Flow->setContentsMargins(16, -1, 16, -1);
     skinLabel_Flow = new QLabel(ui->widget);
     skinLabel_Flow->setObjectName(QString::fromUtf8("skinLabel_Flow"));
     skinLabel_Flow->setText(tr("Skin"));
     sizePolicy2.setHeightForWidth(skinLabel_Flow->sizePolicy().hasHeightForWidth());
     skinLabel_Flow->setSizePolicy(sizePolicy2);
     skinLabel_Flow->setMinimumSize(QSize(80, 15));
     skinLayout_Flow->addWidget(skinLabel_Flow);
     horizontalSpacer_skin_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     skinLayout_Flow->addItem(horizontalSpacer_skin_Flow);
     skincheckBox_Flow = new SwitchButton(ui->widget);
     skincheckBox_Flow->setObjectName(QString::fromUtf8("skincheckBox_Flow"));
     skincheckBox_Flow->setCheckState(true);
     skincheckBox_Flow->setMinimumSize(QSize(41, 20));
     skincheckBox_Flow->setMaximumSize(QSize(41, 20));
     skinLayout_Flow->addWidget(skincheckBox_Flow);
     FlowLayout->addLayout(skinLayout_Flow);

     line_4_Flow = new QFrame(ui->widget);
     line_4_Flow->setObjectName(QString::fromUtf8("line_4_Flow"));
     sizePolicy3.setHeightForWidth(line_4_Flow->sizePolicy().hasHeightForWidth());
     line_4_Flow->setSizePolicy(sizePolicy3);
     line_4_Flow->setFrameShape(QFrame::HLine);
     line_4_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_4_Flow);
     //verticalLayout->addWidget(line_4_Flow);

     helperLayout_Flow = new QHBoxLayout();
     helperLayout_Flow->setSpacing(10);
     helperLayout_Flow->setObjectName(QString::fromUtf8("helperLayout_Flow"));
     helperLayout_Flow->setContentsMargins(16, -1, 16, -1);
     helperLabel_Flow = new QLabel(ui->widget);
     helperLabel_Flow->setObjectName(QString::fromUtf8("helperLabel_Flow"));
     helperLabel_Flow->setText(tr("Adhesion"));
     sizePolicy2.setHeightForWidth(helperLabel_Flow->sizePolicy().hasHeightForWidth());
     helperLabel_Flow->setSizePolicy(sizePolicy2);
     helperLabel_Flow->setMinimumSize(QSize(80, 15));
     helperLayout_Flow->addWidget(helperLabel_Flow);
     horizontalSpacer_helper_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     helperLayout_Flow->addItem(horizontalSpacer_helper_Flow);
     helpercheckBox_Flow = new SwitchButton(ui->widget);
     helpercheckBox_Flow->setObjectName(QString::fromUtf8("helpercheckBox_Flow"));
     helpercheckBox_Flow->setMinimumSize(QSize(41, 20));
     helpercheckBox_Flow->setMaximumSize(QSize(41, 20));
     helpercheckBox_Flow->setCheckState(true);
     helperLayout_Flow->addWidget(helpercheckBox_Flow);
     FlowLayout->addLayout(helperLayout_Flow);
     //verticalLayout->addLayout(helperLayout_Flow);

     line_5_Flow = new QFrame(ui->widget);
     line_5_Flow->setObjectName(QString::fromUtf8("line_5_Flow"));
     sizePolicy3.setHeightForWidth(line_5_Flow->sizePolicy().hasHeightForWidth());
     line_5_Flow->setSizePolicy(sizePolicy3);
     line_5_Flow->setFrameShape(QFrame::HLine);
     line_5_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_5_Flow);
     //verticalLayout->addWidget(line_5_Flow);

     fillLayout_Flow = new QHBoxLayout();
     fillLayout_Flow->setSpacing(10);
     fillLayout_Flow->setObjectName(QString::fromUtf8("fillLayout_Flow"));
     fillLayout_Flow->setContentsMargins(16, -1, 16, -1);
     fillLabel_Flow = new QLabel(ui->widget);
     fillLabel_Flow->setObjectName(QString::fromUtf8("fillLabel_Flow"));
     fillLabel_Flow->setText(tr("Fill"));
     sizePolicy2.setHeightForWidth(fillLabel_Flow->sizePolicy().hasHeightForWidth());
     fillLabel_Flow->setSizePolicy(sizePolicy2);
     fillLabel_Flow->setMinimumSize(QSize(80, 15));
     fillLayout_Flow->addWidget(fillLabel_Flow);
     horizontalSpacer_fill_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     fillLayout_Flow->addItem(horizontalSpacer_fill_Flow);
     fillcheckBox_Flow = new SwitchButton(ui->widget);
     fillcheckBox_Flow->setObjectName(QString::fromUtf8("fillcheckBox_Flow"));
     fillcheckBox_Flow->setMinimumSize(QSize(41, 20));
     fillcheckBox_Flow->setMaximumSize(QSize(41, 20));
     fillcheckBox_Flow->setCheckState(true);
     fillLayout_Flow->addWidget(fillcheckBox_Flow);
     FlowLayout->addLayout(fillLayout_Flow);
     //verticalLayout->addLayout(fillLayout_Flow);
     line_6_Flow = new QFrame(ui->widget);
     line_6_Flow->setObjectName(QString::fromUtf8("line_6_Flow"));
     sizePolicy3.setHeightForWidth(line_6_Flow->sizePolicy().hasHeightForWidth());
     line_6_Flow->setSizePolicy(sizePolicy3);
     line_6_Flow->setFrameShape(QFrame::HLine);
     line_6_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_6_Flow);
     //verticalLayout->addWidget(line_6_Flow);


     supportLayout_Flow = new QHBoxLayout();
     supportLayout_Flow->setSpacing(10);
     supportLayout_Flow->setObjectName(QString::fromUtf8("supportLayout_Flow"));
     supportLayout_Flow->setContentsMargins(16, -1, 16, -1);
     supportLabel_Flow = new QLabel(ui->widget);
     supportLabel_Flow->setObjectName(QString::fromUtf8("supportLabel_Flow"));
     supportLabel_Flow->setText(tr("Support"));
     sizePolicy2.setHeightForWidth(supportLabel_Flow->sizePolicy().hasHeightForWidth());
     supportLabel_Flow->setSizePolicy(sizePolicy2);
     supportLabel_Flow->setMinimumSize(QSize(80, 15));
     supportLayout_Flow->addWidget(supportLabel_Flow);
     horizontalSpacer_support_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     supportLayout_Flow->addItem(horizontalSpacer_support_Flow);
     supportcheckBox_Flow = new SwitchButton(ui->widget);
     supportcheckBox_Flow->setObjectName(QString::fromUtf8("supportcheckBox_Flow"));
     supportcheckBox_Flow->setMinimumSize(QSize(41, 20));
     supportcheckBox_Flow->setMaximumSize(QSize(41, 20));
     supportcheckBox_Flow->setCheckState(true);
     supportLayout_Flow->addWidget(supportcheckBox_Flow);
     FlowLayout->addLayout(supportLayout_Flow);
     //verticalLayout->addLayout(supportLayout_Flow);

     line_8_Flow = new QFrame(ui->widget);
     line_8_Flow->setObjectName(QString::fromUtf8("line_8_Flow"));
     sizePolicy3.setHeightForWidth(line_8_Flow->sizePolicy().hasHeightForWidth());
     line_8_Flow->setSizePolicy(sizePolicy3);
     line_8_Flow->setFrameShape(QFrame::HLine);
     line_8_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_8_Flow);
     //verticalLayout->addWidget(line_8_Flow);

     unkonwnLayout_Flow = new QHBoxLayout();
     unkonwnLayout_Flow->setSpacing(10);
     unkonwnLayout_Flow->setObjectName(QString::fromUtf8("unkonwnLayout_Flow"));
     unkonwnLayout_Flow->setContentsMargins(16, -1, 16, -1);
     unkonwnLabel_Flow = new QLabel(ui->widget);
     unkonwnLabel_Flow->setObjectName(QString::fromUtf8("unknowLabel"));
     unkonwnLabel_Flow->setText(tr("Unknown"));
     sizePolicy2.setHeightForWidth(unkonwnLabel_Flow->sizePolicy().hasHeightForWidth());
     unkonwnLabel_Flow->setSizePolicy(sizePolicy2);
     unkonwnLabel_Flow->setMinimumSize(QSize(80, 15));
     unkonwnLayout_Flow->addWidget(unkonwnLabel_Flow);
     horizontalSpacer_unkonwn_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     unkonwnLayout_Flow->addItem(horizontalSpacer_unkonwn_Flow);
     unkonwncheckBox_Flow = new SwitchButton(ui->widget);
     unkonwncheckBox_Flow->setObjectName(QString::fromUtf8("unkonwncheckBox_Flow"));
     unkonwncheckBox_Flow->setMinimumSize(QSize(41, 20));
     unkonwncheckBox_Flow->setMaximumSize(QSize(41, 20));
     unkonwncheckBox_Flow->setCheckState(true);
     unkonwnLayout_Flow->addWidget(unkonwncheckBox_Flow);
     FlowLayout->addLayout(unkonwnLayout_Flow);
     //verticalLayout->addLayout(unkonwnLayout_Flow);

     line_9_Flow = new QFrame(ui->widget);
     line_9_Flow->setObjectName(QString::fromUtf8("line_9_Flow"));
     sizePolicy3.setHeightForWidth(line_9_Flow->sizePolicy().hasHeightForWidth());
     line_9_Flow->setSizePolicy(sizePolicy3);
     line_9_Flow->setFrameShape(QFrame::HLine);
     line_9_Flow->setFrameShadow(QFrame::Sunken);
     FlowLayout->addWidget(line_9_Flow);

     QVBoxLayout* colorPatch_Flow;
     QHBoxLayout* HLayout_colorPatch_label_Flow;
     QHBoxLayout* HLayout_clolorpatch_Flow;
     QSpacerItem* horizontalSpacer_cp_Flow;
     QSpacerItem* horizontalSpacer_cp_2_Flow;
     QLabel* colorPatch_label_Flow;
     colorPatch_Flow = new QVBoxLayout();
     colorPatch_Flow->setObjectName(QString::fromUtf8("colorPatch_Flow"));
     colorPatch_Flow->setContentsMargins(0, 0, 0, 0);
     HLayout_colorPatch_label_Flow = new QHBoxLayout();
     HLayout_colorPatch_label_Flow->setObjectName(QString::fromUtf8("HLayout_colorPatch_label_Flow"));
     colorPatch_label_start_Flow = new QLabel(ui->widget);
     colorPatch_label_start_Flow->setObjectName(QString::fromUtf8("colorPatch_label_start_Flow"));
     HLayout_colorPatch_label_Flow->addWidget(colorPatch_label_start_Flow);
     colorPatch_label_unit_Flow = new QLabel(ui->widget);
     colorPatch_label_unit_Flow->setObjectName(QString::fromUtf8("colorPatch_label_unit_Flow"));
     colorPatch_label_unit_Flow->setText(tr("mm/s"));

     HLayout_colorPatch_label_Flow->addWidget(colorPatch_label_unit_Flow);
     colorPatch_label_end_Flow = new QLabel(ui->widget);
     colorPatch_label_end_Flow->setObjectName(QString::fromUtf8("colorPatch_label_end_Flow"));
     HLayout_colorPatch_label_Flow->addWidget(colorPatch_label_end_Flow);
     colorPatch_label_start_Flow->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
     colorPatch_label_unit_Flow->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
     colorPatch_label_end_Flow->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);
     colorPatch_label_start_Flow->setText("5");
     colorPatch_label_end_Flow->setText("250");
     colorPatch_Flow->addLayout(HLayout_colorPatch_label_Flow);

     HLayout_clolorpatch_Flow = new QHBoxLayout();
     HLayout_clolorpatch_Flow->setSpacing(10);
     HLayout_clolorpatch_Flow->setObjectName(QString::fromUtf8("HLayout_clolorpatch_Flow"));
     horizontalSpacer_cp_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     HLayout_clolorpatch_Flow->addItem(horizontalSpacer_cp_Flow);
     colorPatch_label_Flow = new QLabel(ui->widget);
     colorPatch_label_Flow->setObjectName(QString::fromUtf8("colorPatch_label_Flow"));
     //QSizePolicy sizePolicy2323(QSizePolicy::Expanding, QSizePolicy::Preferred);
     sizePolicy232.setHorizontalStretch(0);
     sizePolicy232.setVerticalStretch(0);
     sizePolicy232.setHeightForWidth(colorPatch_label_Flow->sizePolicy().hasHeightForWidth());
     colorPatch_label_Flow->setSizePolicy(sizePolicy2323);
     colorPatch_label_Flow->setMinimumSize(QSize(100, 20));
     colorPatch_label_Flow->setMaximumSize(QSize(99999, 9999999));
     colorPatch_label_Flow->setStyleSheet(QString::fromUtf8("background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(54, 73, 242), stop:0.05555 rgb(54, 113, 242), stop:0.083333 rgb(54, 135, 242),stop:0.1111 rgb(54, 154, 242), \n"
     "stop:0.1666 rgb(54, 195, 242) , stop:0.22222 rgb(54, 236, 242), stop:0.25 rgb(54, 242, 226), stop:0.27777 rgb(54, 242, 208), stop:0.33333 rgb(54, 242, 167), \n"
     "stop:0.3888 rgb(54, 242, 126), stop:0.44444 rgb(54, 242, 85), stop:0.5 rgb(63, 242, 54),\n"
     "stop:0.555555556 rgb(104, 242, 54), \n"
     "stop:0.611111111 rgb(145, 242, 54), \n"
     "stop:0.666666667 rgb(186, 242, 54), \n"
     "stop:0.722222222 rgb(226, 242, 54) , \n"
     "stop:0.777777778 rgb(242, 217, 54), \n"
     "stop:0.833333333 rgb(242, 176, 54), \n"
     "stop:0.888888889 rgb(242, 135, 54), \n"
     "stop:0.944444444 rgb(242, 94, 54), \n"
     "stop:1 rgb(242, 54, 54));"));
     HLayout_clolorpatch_Flow->addWidget(colorPatch_label_Flow);
     horizontalSpacer_cp_2_Flow = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
     HLayout_clolorpatch_Flow->addItem(horizontalSpacer_cp_2_Flow);
     HLayout_clolorpatch_Flow->setStretch(0, 1);
     HLayout_clolorpatch_Flow->setStretch(1, 8);
     HLayout_clolorpatch_Flow->setStretch(2, 1);
     colorPatch_Flow->addLayout(HLayout_clolorpatch_Flow);

    FlowLayout->addLayout(colorPatch_Flow);
    typeLayout->addWidget(FlowWidget);

#endif


    ConsumedLabelLayout = new QHBoxLayout();
    ConsumedLabelLayout->setObjectName(QString::fromUtf8("ConsumedLabelLayout"));
    ConsumedLabelabel = new QLabel(ui->widget);
    ConsumedLabelabel->setObjectName(QString::fromUtf8("ConsumedLabelabel"));
    ConsumedLabelabel->setMargin(5);
    ConsumedLabelabel->setText(tr("Consumables"));
    ConsumedLabelabel->setStyleSheet("font: normal bold 16px;font-family:Microsoft YaHei;");
    ConsumedLabelLayout->addWidget(ConsumedLabelabel);
    verticalLayout->addLayout(ConsumedLabelLayout);

    filamentLayout = new QHBoxLayout();
    filamentLayout->setSpacing(8);
    filamentLayout->setObjectName(QString::fromUtf8("filamentLayout"));
    filamentLayout->setContentsMargins(0, 0, 0, 0);
    filamentPicLabel = new QLabel(ui->widget);
    filamentPicLabel->setObjectName(QString::fromUtf8("filamentPicLabel"));
    sizePolicy1.setHeightForWidth(filamentPicLabel->sizePolicy().hasHeightForWidth());
    filamentPicLabel->setSizePolicy(sizePolicy1);
    filamentPicLabel->setMinimumSize(QSize(40, 40));
    filamentPicLabel->setAlignment(Qt::AlignCenter);
    filamentPicLabel->setPixmap(QPixmap("://pic//ui/fdm_filament_icon_u.png"));
    filamentLayout->addWidget(filamentPicLabel);
    filamentLabel = new QLabel(ui->widget);
    filamentLabel->setObjectName(QString::fromUtf8("filamentLabel"));
    filamentLabel->setText(tr("Filament"));
    filamentLabel->setStyleSheet("font: normal normal 16px;font-family:Microsoft YaHei;");
    sizePolicy2.setHeightForWidth(filamentLabel->sizePolicy().hasHeightForWidth());
    filamentLabel->setSizePolicy(sizePolicy2);
    filamentLabel->setMinimumSize(QSize(80, 15));
    filamentLayout->addWidget(filamentLabel);
    horizontalSpacer_filamentLayout10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    filamentLayout->addItem(horizontalSpacer_filamentLayout10);
    filamentValue = new QLabel(ui->widget);
    filamentValue->setObjectName(QString::fromUtf8("filamentValue"));
    filamentValue->setStyleSheet("font: normal normal 16px Microsoft YaHei;color:#999999");
    QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(filamentValue->sizePolicy().hasHeightForWidth());
    filamentValue->setSizePolicy(sizePolicy4);
    filamentValue->setMinimumSize(QSize(150, 15));
    filamentValue->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
    filamentLayout->addWidget(filamentValue);
    verticalLayout->addLayout(filamentLayout);

    line_10 = new QFrame(ui->widget);
    line_10->setObjectName(QString::fromUtf8("line_10"));
    sizePolicy3.setHeightForWidth(line_10->sizePolicy().hasHeightForWidth());
    line_10->setSizePolicy(sizePolicy3);
    line_10->setFrameShape(QFrame::HLine);
    line_10->setFrameShadow(QFrame::Sunken);
    verticalLayout->addWidget(line_10);

    timeLayout = new QHBoxLayout();
    timeLayout->setSpacing(8);
    timeLayout->setObjectName(QString::fromUtf8("timeLayout"));
    timeLayout->setContentsMargins(0, 0, 0, 0);
    timePiclabel = new QLabel(ui->widget);
    timePiclabel->setObjectName(QString::fromUtf8("timePiclabel"));
    timePiclabel->setPixmap(QPixmap("://pic//ui/fdm_time_icon_u.png"));
    sizePolicy1.setHeightForWidth(timePiclabel->sizePolicy().hasHeightForWidth());
    timePiclabel->setSizePolicy(sizePolicy1);
    timePiclabel->setMinimumSize(QSize(40, 40));
    timePiclabel->setAlignment(Qt::AlignCenter);
    timeLayout->addWidget(timePiclabel);
    timelabel = new QLabel(ui->widget);
    timelabel->setObjectName(QString::fromUtf8("timelabel"));
    timelabel->setText(tr("Time"));
    timelabel->setStyleSheet("font: normal bold 16px Microsoft YaHei");
    sizePolicy2.setHeightForWidth(timelabel->sizePolicy().hasHeightForWidth());
    timelabel->setSizePolicy(sizePolicy2);
    timelabel->setMinimumSize(QSize(80, 15));
    timeLayout->addWidget(timelabel);
    horizontalSpacer_time11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    timeLayout->addItem(horizontalSpacer_time11);
    timeValue = new QLabel(ui->widget);
    timeValue->setStyleSheet("font: normal normal 16px Microsoft YaHei;color:#999999");
    timeValue->setObjectName(QString::fromUtf8("timeValue"));
    sizePolicy4.setHeightForWidth(timeValue->sizePolicy().hasHeightForWidth());
    timeValue->setSizePolicy(sizePolicy4);
    timeValue->setMinimumSize(QSize(150, 15));
    timeValue->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
    timeLayout->addWidget(timeValue);
    verticalLayout->addLayout(timeLayout);

    verticalLayout->addStretch();

    QHBoxLayout* ExportLayout;
    QSpacerItem* horizontalSpacer_ExportLayout12;
    //QPushButton* ExportButton;
    QSpacerItem* horizontalSpacer_ExportLayout13;
    QHBoxLayout* ExitLayout;
    QSpacerItem* horizontalSpacer_Exit14;
    //QPushButton* ExitButton;
    QSpacerItem* horizontalSpacer_Exit15;
    ExportLayout = new QHBoxLayout();
    ExportLayout->setSpacing(0);
    ExportLayout->setContentsMargins(8,0,8,0);
    ExportLayout->setObjectName(QString::fromUtf8("ExportLayout"));
//    horizontalSpacer_ExportLayout12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ExportLayout->addItem(horizontalSpacer_ExportLayout12);
    ExportButton = new QPushButton(ui->widget);
    ExportButton->setObjectName(QString::fromUtf8("ExportButton"));
    ExportButton->setText(tr("Export"));
    QSizePolicy bottomsizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ExportButton->setSizePolicy(bottomsizePolicy);
    ExportButton->setFixedHeight(40);
//    ExportButton->setMinimumSize(252,40);
//    ExportButton->setMaximumSize(252,40);
    ExportButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
        "    color:#FFFFFF;\n"
        "    border: 0px;\n"
        "    font-size:16px;\n"
        "    border-radius:5;\n"
        "    background-color:#3A3B3F;\n"
        "\n"
        "}\n"
        "QPushButton:disabled {\n"
        "    color:#696969;\n"
        "    background-color:#3A3B3F;\n"
        "    border: 0px  solid #000000;\n"
        "}\n"
        "\n"
        "QPushButton:hover {\n"
        "    color:#71D78A;;\n"
        "    border: 1px  solid #71D78A;\n"
        "}\n"
        "\n"
        "QPushButton:pressed {\n"
        "    color:#71D78A;\n"
        "     border: 1px  solid #71D78A;\n"
        "}"));
    ExportLayout->addWidget(ExportButton);
    ExportButton->setEnabled(false);
//    horizontalSpacer_ExportLayout13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ExportLayout->addItem(horizontalSpacer_ExportLayout13);
//    ExportLayout->setStretch(0, 1);
//    ExportLayout->setStretch(1, 3);
//    ExportLayout->setStretch(2, 1);


    ExitLayout = new QHBoxLayout();
    ExitLayout->setSpacing(0);
    ExitLayout->setContentsMargins(8,0,8,0);
//    ExitLayout->setObjectName(QString::fromUtf8("ExitLayout"));
//    horizontalSpacer_Exit14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ExitLayout->addItem(horizontalSpacer_Exit14);
    ExitButton = new QPushButton(ui->widget);
    ExitButton->setObjectName(QString::fromUtf8("ExitButton"));
    ExitButton->setText(tr("Print"));
    ExitButton->setFixedHeight(40);
    ExitButton->setSizePolicy(bottomsizePolicy);
//    ExitButton->setMinimumSize(252,40);
//    ExitButton->setMaximumSize(252,40);
    ExitButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
        "    color:#FFFFFF;\n"
        "    border: 0px;\n"
        "    font-size:16px;\n"
        "    border-radius:5;\n"
        "    background-color:#61D37D;\n"
        "\n"
        "}\n"
        "QPushButton:disabled {\n"
        "    color:#696969;\n"
        "    background-color:#3A3B3F;\n"
        "    border: 0px  solid #000000;\n"
        "}\n"
        "\n"
        "QPushButton:hover {\n"
        "    background-color:#71D78A;\n"
        "}\n"
        "\n"
        "QPushButton:pressed {\n"
        "    color:#FFFFFF;\n"
        "    background-color:#61D37D;\n"
        "}"));
    ExitLayout->addWidget(ExitButton);
    ExitButton->setEnabled(false);
//    horizontalSpacer_Exit15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ExitLayout->addItem(horizontalSpacer_Exit15);
//    ExitLayout->setStretch(0, 1);
//    ExitLayout->setStretch(1, 3);
//    ExitLayout->setStretch(2, 1);

    verticalLayout->addLayout(ExportLayout);
    verticalLayout->addLayout(ExitLayout);
    /*replace  end  ********************************************/

    connect(this->verticalSlider, SIGNAL(valueChanged(int, int)), this, SLOT(verticalSliderValueChanged(int, int)));
    connect(this->horizontalSlider, SIGNAL(valueChanged(int, int)), this, SLOT(horizontalSliderValueChanged(int, int)));
    connect(ExportButton, SIGNAL(clicked()), this, SLOT(setExportPic()));
    connect(ExitButton, SIGNAL(clicked()), this, SLOT(on_pushButton_quit_clicked()));
    connect(innerWallLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(innerWallcheckBox, SIGNAL(stateChanged(bool)), this, SLOT(innerWallcheckButton()));

    connect(outerWallLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(outerWallcheckBox, SIGNAL(stateChanged(bool)), this, SLOT(outerWallcheckButton()));

    connect(skinLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(skincheckBox, SIGNAL(stateChanged(bool)), this, SLOT(skincheckButton()));

    connect(helperLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(helpercheckBox, SIGNAL(stateChanged(bool)), this, SLOT(helpercheckButton()));

    connect(fillLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(fillcheckBox, SIGNAL(stateChanged(bool)), this, SLOT(fillcheckButton()));

    connect(travelLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(travelcheckBox, SIGNAL(stateChanged(bool)), this, SLOT(travelcheckButton()));

    //connect(zlapLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(zlapcheckBox, SIGNAL(stateChanged(bool)), this, SLOT(zlapcheckButton()));

    //connect(travelcheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(travelcheckButton()));
    connect(supportLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(supportcheckBox, SIGNAL(stateChanged(bool)), this, SLOT(supportcheckButton()));

    connect(unkonwnLabelC, SIGNAL(clicked()), this, SLOT(colorChanged()));
    connect(unkonwncheckBox, SIGNAL(stateChanged(bool)), this, SLOT(unkonwncheckButton()));

    connect(this->spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(spinBoxUpValueChanged(int)));
    connect(this->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDownValueChanged(int)));

#ifdef USE_EXTRA_UI
     connect(this->linetypeCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeComboxChange(int)));
    connect(innerWallcheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(innerWallcheckButton()));
    connect(outerWallcheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(outerWallcheckButton()));
    connect(skincheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(skincheckButton()));
    connect(helpercheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(helpercheckButton()));
    connect(fillcheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(fillcheckButton()));
    connect(supportcheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(supportcheckButton()));
    connect(unkonwncheckBox_speed, SIGNAL(stateChanged(bool)), this, SLOT(unkonwncheckButton()));
    connect(innerWallcheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(innerWallcheckButton()));
    connect(outerWallcheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(outerWallcheckButton()));
    connect(skincheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(skincheckButton()));
    connect(helpercheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(helpercheckButton()));
    connect(fillcheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(fillcheckButton()));
    connect(supportcheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(supportcheckButton()));
    connect(unkonwncheckBox_trapezoid, SIGNAL(stateChanged(bool)), this, SLOT(unkonwncheckButton()));

    connect(innerWallcheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(innerWallcheckButton()));
    connect(outerWallcheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(outerWallcheckButton()));
    connect(skincheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(skincheckButton()));
    connect(helpercheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(helpercheckButton()));
    connect(fillcheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(fillcheckButton()));
    connect(supportcheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(supportcheckButton()));
    connect(unkonwncheckBox_Flow, SIGNAL(stateChanged(bool)), this, SLOT(unkonwncheckButton()));

#endif


}

void viewWidget::onFrontView()
{
    ui->openGLWidget->setView(ViewType::FRONT_VIEW_TYPE);
}
void viewWidget::onBackView()
{
    ui->openGLWidget->setView(ViewType::BACK_VIEW_TYPE);
}
void viewWidget::onLeftView()
{
    ui->openGLWidget->setView(ViewType::LEFT_VIEW_TYPE);
}
void viewWidget::onRightView()
{
    ui->openGLWidget->setView(ViewType::RIGHT_VIEW_TYPE);
}
void viewWidget::onTopView()
{
    ui->openGLWidget->setView(ViewType::TOP_VIEW_TYPE);
}
void viewWidget::onBottomView()
{
    ui->openGLWidget->setView(ViewType::BOTTOM_VIEW_TYPE);
}

void viewWidget::onJustView()
{
    ui->openGLWidget->setView(ViewType::JUST_VIEW_TYPE);
}


void viewWidget::msgFromFdmGcodePaser(QString cmd)
{
    AkUtil::TFunction("");
//    if(cmd == "quit")
//    {
//        AkUtil::TDebug("quit");
//        qApp->quit();
//    }
    if(this->innerMode){
//        QThreadPool::globalInstance()->start([&](){
//            reSetGcodePath(cmd.toStdString(),isAiMode,0);
//        });
        this->reSetGcodePath(cmd.toStdString(),this->isAiMode,0);
    }else{
        this->reSetGcodePath(cmd.toStdString(),false,0);
    }
}


void viewWidget::updatePlayViewer()
{

    int curValue = this->horizontalSlider->getRightValue();
    //    if(curValue >= this->horizontalSlider->getMaxValue())
    //    {
    //        this->mPlayTimer->stop();
    //    }
    int palyValue = (curValue + 1) % this->horizontalSlider->getMaxValue();
    //qDebug() << "curValue :" << curValue;
    this->horizontalSlider->setCurrentRange(0, palyValue);

}

void viewWidget::palyView()
{
    //ui->openGLWidget->setView(ViewType::JUST_VIEW_TYPE);
    if (this->pushButton_9->isChecked())
    {//play
        if (this->mPlayTimer == nullptr)
        {
            this->mPlayTimer = new QTimer(this->ui->openGLWidget);
            //this->mPlayTimer->setInterval(1);
            connect(mPlayTimer, SIGNAL(timeout()), this, SLOT(updatePlayViewer()));
            this->horizontalSlider->setCurrentRange(0, 0);
        }
        
        this->mPlayTimer->start();
    }
    else {
        this->mPlayTimer->stop();
    }
}

void viewWidget::innerWallcheckButton()
{
    bool curr_status;
    if(this->sender()->objectName() == "innerWallcheckBox")
    {
        curr_status = this->innerWallcheckBox->checkState();

    }
#ifdef USE_EXTRA_UI
    if(this->sender()->objectName() == "innerWallcheckBox_speed")
    {
        curr_status = this->innerWallcheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "innerWallcheckBox_trapezoid")
    {
        curr_status = this->innerWallcheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "innerWallcheckBox_Flow")
    {
        curr_status = this->innerWallcheckBox_Flow->checkState();
    }
    this->innerWallcheckBox_speed->setCheckState(curr_status);
    this->innerWallcheckBox_trapezoid->setCheckState(curr_status);
    this->innerWallcheckBox_Flow->setCheckState(curr_status);
#endif
    this->innerWallcheckBox->setCheckState(curr_status);

    if (this->innerWallcheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erPerimeter, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erPerimeter, false);
    }

    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
    setFilamentStr();
    setTimeStr();
}


void viewWidget::outerWallcheckButton()
{
    bool curr_status;
    if(this->sender()->objectName() == "outerWallcheckBox")
    {
        curr_status = this->outerWallcheckBox->checkState();

    }
#ifdef USE_EXTRA_UI
    if(this->sender()->objectName() == "outerWallcheckBox_speed")
    {
        curr_status = this->outerWallcheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "outerWallcheckBox_trapezoid")
    {
        curr_status = this->outerWallcheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "outerWallcheckBox_Flow")
    {
        curr_status = this->outerWallcheckBox_Flow->checkState();
    }
    this->outerWallcheckBox_speed->setCheckState(curr_status);
    this->outerWallcheckBox_trapezoid->setCheckState(curr_status);
    this->outerWallcheckBox_Flow->setCheckState(curr_status);
#endif
    this->outerWallcheckBox->setCheckState(curr_status);
    if (this->outerWallcheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erExternalPerimeter, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erExternalPerimeter, false);
    }
    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
    setFilamentStr();
    setTimeStr();
}

void viewWidget::skincheckButton()
{

    bool curr_status;
    if(this->sender()->objectName() == "skincheckBox")
    {
        curr_status = this->skincheckBox->checkState();
    }
#ifdef USE_EXTRA_UI
    if(this->sender()->objectName() == "skincheckBox_speed")
    {
        curr_status = this->skincheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "skincheckBox_trapezoid")
    {
        curr_status = this->skincheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "skincheckBox_Flow")
    {
        curr_status = this->skincheckBox_Flow->checkState();
    }
    this->skincheckBox_speed->setCheckState(curr_status);
    this->skincheckBox_trapezoid->setCheckState(curr_status);
    this->skincheckBox_Flow->setCheckState(curr_status);
#endif
    this->skincheckBox->setCheckState(curr_status);
    if (this->skincheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erSolidInfill, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erSolidInfill, false);
    }
    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
    setFilamentStr();
    setTimeStr();
}

void viewWidget::helpercheckButton()
{
    bool curr_status;
    if(this->sender()->objectName() == "helpercheckBox")
    {
        curr_status = this->helpercheckBox->checkState();

    }

#ifdef  USE_EXTRA_UI
    if(this->sender()->objectName() == "helpercheckBox_speed")
    {
        curr_status = this->helpercheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "helpercheckBox_trapezoid")
    {
        curr_status = this->helpercheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "helpercheckBox_Flow")
    {
        curr_status = this->helpercheckBox_Flow->checkState();
    }
    this->helpercheckBox_speed->setCheckState(curr_status);
    this->helpercheckBox_trapezoid->setCheckState(curr_status);
    this->helpercheckBox_Flow->setCheckState(curr_status);
#endif
    this->helpercheckBox->setCheckState(curr_status);
    if (this->helpercheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erSkirt, true);
        ui->openGLWidget->setRoleVisible(erWipeTower, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erSkirt, false);
        ui->openGLWidget->setRoleVisible(erWipeTower, false);
    }
    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
    setFilamentStr();
    setTimeStr();
}


void viewWidget::fillcheckButton()
{
    bool curr_status;
    if(this->sender()->objectName() == "fillcheckBox")
    {
        curr_status = this->fillcheckBox->checkState();
    }
#ifdef  USE_EXTRA_UI
    if(this->sender()->objectName() == "fillcheckBox_speed")
    {
        curr_status = this->fillcheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "fillcheckBox_trapezoid")
    {
        curr_status = this->fillcheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "fillcheckBox_Flow")
    {
        curr_status = this->fillcheckBox_Flow->checkState();
    }
    this->fillcheckBox_speed->setCheckState(curr_status);
    this->fillcheckBox_trapezoid->setCheckState(curr_status);
    this->fillcheckBox_Flow->setCheckState(curr_status);
#endif
    this->fillcheckBox->setCheckState(curr_status);
    if (this->fillcheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erInternalInfill, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erInternalInfill, false);
    }
    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
    setFilamentStr();
    setTimeStr();
}

void viewWidget::zlapcheckButton()
{
    if (this->zlapcheckBox->checkState())
    {
        ui->openGLWidget->set_toolpath_move_type_visible(EMoveType::Zlap, true);
        ui->openGLWidget->refresh_render_paths(false, false);
    }
    else
    {
        ui->openGLWidget->set_toolpath_move_type_visible(EMoveType::Zlap, false);
        ui->openGLWidget->refresh_render_paths(false, false);
    }
}


void viewWidget::travelcheckButton()
{
#ifdef   USE_EXTRA_UI
    if(this->sender()->objectName() == "travelcheckBox")
    {
        this->travelcheckBox_speed->setCheckState(this->travelcheckBox->checkState());
    }

    if(this->sender()->objectName() == "travelcheckBox_speed")
    {
        this->travelcheckBox->setCheckState(this->travelcheckBox_speed->checkState());
    }
#endif
    if (this->travelcheckBox->checkState())
    {
        ui->openGLWidget->set_toolpath_move_type_visible(EMoveType::Travel, true);
        ui->openGLWidget->refresh_render_paths(false, false);
    }
    else
    {
        ui->openGLWidget->set_toolpath_move_type_visible(EMoveType::Travel, false);
        ui->openGLWidget->refresh_render_paths(false, false);
    }
    setFilamentStr();
    setTimeStr();
}

void viewWidget::supportcheckButton()
{
    bool curr_status;
    if(this->sender()->objectName() == "supportcheckBox")
    {
        curr_status = this->supportcheckBox->checkState();

    }
#ifdef   USE_EXTRA_UI
    if(this->sender()->objectName() == "supportcheckBox_speed")
    {
        curr_status = this->supportcheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "supportcheckBox_trapezoid")
    {
        curr_status = this->supportcheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "supportcheckBox_Flow")
    {
        curr_status = this->supportcheckBox_Flow->checkState();
    }
    this->supportcheckBox_speed->setCheckState(curr_status);
    this->supportcheckBox_trapezoid->setCheckState(curr_status);
    this->supportcheckBox_Flow->setCheckState(curr_status);
#endif
    this->supportcheckBox->setCheckState(curr_status);
    if (this->supportcheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erSupportMaterial, true);
        ui->openGLWidget->setRoleVisible(erSupportMaterialInterface, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erSupportMaterial, false);
        ui->openGLWidget->setRoleVisible(erSupportMaterialInterface, false);
    }
    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
    setFilamentStr();
    setTimeStr();
}

void viewWidget::unkonwncheckButton()
{

//    if(this->sender()->objectName() == "unknowcheckBox")
//    {
//        this->unkonwncheckBox_speed->setCheckState(this->unkonwncheckBox->checkState());
//    }

//    if(this->sender()->objectName() == "unkonwncheckBox_speed")
//    {
//        this->unkonwncheckBox->setCheckState(this->unkonwncheckBox_speed->checkState());
//    }

    bool curr_status;
    if(this->sender()->objectName() == "unknowcheckBox")
    {
        curr_status = this->unkonwncheckBox->checkState();

    }
#ifdef   USE_EXTRA_UI
    if(this->sender()->objectName() == "unkonwncheckBox_speed")
    {
        curr_status = this->unkonwncheckBox_speed->checkState();
    }
    if(this->sender()->objectName() == "unkonwncheckBox_trapezoid")
    {
        curr_status = this->unkonwncheckBox_trapezoid->checkState();
    }
    if(this->sender()->objectName() == "unkonwncheckBox_Flow")
    {
        curr_status = this->unkonwncheckBox_Flow->checkState();
    }
    this->unkonwncheckBox_speed->setCheckState(curr_status);
    this->unkonwncheckBox_trapezoid->setCheckState(curr_status);
    this->unkonwncheckBox_Flow->setCheckState(curr_status);
#endif
    this->unkonwncheckBox->setCheckState(curr_status);
    if (this->unkonwncheckBox->checkState())
    {
        ui->openGLWidget->setRoleVisible(erNone, true);
        ui->openGLWidget->setRoleVisible(erOverhangPerimeter, true);
        ui->openGLWidget->setRoleVisible(erTopSolidInfill, true);
        ui->openGLWidget->setRoleVisible(erIroning, true);
        ui->openGLWidget->setRoleVisible(erBridgeInfill, true);
        ui->openGLWidget->setRoleVisible(erInternalBridgeInfill, true);
        ui->openGLWidget->setRoleVisible(erThinWall, true);
        ui->openGLWidget->setRoleVisible(erGapFill, true);
        ui->openGLWidget->setRoleVisible(erMilling, true);
        ui->openGLWidget->setRoleVisible(erCustom, true);
        ui->openGLWidget->setRoleVisible(erMixed, true);
    }
    else
    {
        ui->openGLWidget->setRoleVisible(erNone, false);
        ui->openGLWidget->setRoleVisible(erOverhangPerimeter, false);
        ui->openGLWidget->setRoleVisible(erTopSolidInfill, false);
        ui->openGLWidget->setRoleVisible(erIroning, false);
        ui->openGLWidget->setRoleVisible(erBridgeInfill, false);
        ui->openGLWidget->setRoleVisible(erInternalBridgeInfill, false);
        ui->openGLWidget->setRoleVisible(erThinWall, false);
        ui->openGLWidget->setRoleVisible(erGapFill, false);
        ui->openGLWidget->setRoleVisible(erMilling, false);
        ui->openGLWidget->setRoleVisible(erCustom, false);
        ui->openGLWidget->setRoleVisible(erMixed, false);
    }
    ui->openGLWidget->refresh_render_paths(false, false);
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    this->update();
    setFilamentStr();
    setTimeStr();
}

#ifdef USE_EXTRA_UI
void viewWidget::linetypeComboxChange(int changeValue)
{

//
    typeLayout->setCurrentIndex(changeValue);
    if (!this->ui->openGLWidget->gcode_result.freedrate_range.empty()) {
        if(changeValue == 1 || changeValue == 2)
        {
            colorPatch_label_start->setText(QString::number(this->ui->openGLWidget->gcode_result.freedrate_range[0]));
            colorPatch_label_end->setText(QString::number(this->ui->openGLWidget->gcode_result.freedrate_range[1]));
            colorPatch_label_start_trapezoid->setText(QString::number(this->ui->openGLWidget->gcode_result.freedrate_range[0]));
            colorPatch_label_end_trapezoid->setText(QString::number(this->ui->openGLWidget->gcode_result.freedrate_range[1]));
            //colorPatch_label_start->update();
            //qDebug()<< "freedrate_range :" <<(QString::number(this->ui->openGLWidget->gcode_result.freedrate_range[0])) ;
        }

        if(changeValue == 3)
        {
            colorPatch_label_start_Flow->setText(QString::number(this->ui->openGLWidget->gcode_result.flow_range[0]));
            colorPatch_label_end_Flow->setText(QString::number(this->ui->openGLWidget->gcode_result.flow_range[1]));
        }

    }

    this->ui->openGLWidget->setColorType(GcodeViewer::colorType(changeValue));
    this->ui->openGLWidget->update();
};
#endif

void viewWidget::spinBoxUpValueChanged(int changeValue)
{

    this->verticalSlider->setUpValue(changeValue);
    //this->spinBox->validator->setTop(changeValue);//setMaximum(changeValue);
    //qDebug() << "viewWidget::spinBoxUpValueChanged" ;

}


void viewWidget::colorChanged()
{
    qDebug() << this->sender()->objectName();
    colorDlg->exec();
    QColor color = colorDlg->selectedColor();//QColorDialog::getColor(Qt::white,this);
    if(!color.isValid())
    {
        return ;
    }
    std::array<float, 3> passColor;
    passColor[0] = float(color.red()) / 255.0;
    passColor[1] = float(color.green()) / 255.0;
    passColor[2] = float(color.blue()) / 255.0;
    QString setColor = QString("background-color: rgb(%1,%2,%3);\n").arg(QString::number(color.red()),QString::number(color.green()),QString::number(color.blue()));
     qDebug() << setColor;
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    button->setStyleSheet(setColor);
    if(sender()->objectName() == "travelLabelC")
    {
       this->ui->openGLWidget->setTravelColor(0,passColor);
    }
    for(auto &t : roleDict){
        if(t.second == sender()->objectName())
        {
            this->ui->openGLWidget->setRoleColor(t.first,passColor);
        }
    }
    ui->openGLWidget->refresh_render_paths(false, false);
}

void viewWidget::spinBoxDownValueChanged(int changeValue)
{
    this->verticalSlider->setBelowValue(changeValue);
}


void viewWidget::verticalSliderValueChanged(int value1, int value2)
{

    if (m_sliderLastValue.v_lastValue1 == value1 && m_sliderLastValue.v_lastValue2 == value2)
    {
    return;
    }

    m_sliderLastValue.v_lastValue1 = value1;
    m_sliderLastValue.v_lastValue2 = value2;

    ui->openGLWidget->verticalsliderMove(value1, value2);
    int verticalSliderMax = this->verticalSlider->getMaxValue();
    this->spinBox->lineEdit->setRange(1, value2);// down 1 ~ v2
    this->spinBox_2->lineEdit->setRange(value1, verticalSliderMax);// up v1 ~ max
    this->spinBox->lineEdit->setValue(value1);
    this->spinBox_2->lineEdit->setValue(value2);
    //fix  ui->openGLWidget->m_sequential_view.endpoints.first;
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first + 1;
    unsigned int layerLast = ui->openGLWidget->m_sequential_view.endpoints.last + 1;
    //ui->openGLWidget->update_sequential_view_current(layerFirst, layerLast);
    //qDebug() <<"layerFirst " <<layerFirst;
    //qDebug() <<"layerLast " <<layerLast;
    this->horizontalSlider->setRange(0, layerLast - layerFirst,false);
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
    //    this->horizontalSlider->setRightValue(layerLast);
    this->update();
}

void viewWidget::horizontalSliderValueChanged(int value1, int value2)
{
    if (m_sliderLastValue.h_lastValue1 == value1 && m_sliderLastValue.h_lastValue2 == value2)
    {
    return;
    }
    //qDebug() <<"value2 " <<value2;
    m_sliderLastValue.h_lastValue1 = value1;
    m_sliderLastValue.h_lastValue2 = value2;
    unsigned int layerFirst = ui->openGLWidget->m_sequential_view.endpoints.first;
    ui->openGLWidget->horizonsliderMove(value1 + layerFirst, value2 + layerFirst);
}

void viewWidget::setExportPic()
{
    //reSetGcodePath("T:\\_tmp\\1.gcode",false,0);
    //this->ui->openGLWidget->clearGcodeSource();
    //return;
    QString dir = QDir::homePath();
    QString defaultName = dir + "/" + oStlName;
    QString savePath ;

    if(isAiMode){
        savePath = QFileDialog::getSaveFileName(NULL, "", defaultName, "*.acode");
    }else{
        savePath = QFileDialog::getSaveFileName(NULL, "", defaultName, "*.gcode");
    }

    if (savePath.isEmpty())
    {
        return;
    }
#if 0
    {
    AkUtil::TDebug("-------------IgnoreEvent start--------------");
    IgnoreEvent ign(this);
    QProgressDialog *p = new QProgressDialog();
    p->setAttribute(Qt::WA_DeleteOnClose);
    //connect( this,SIGNAL(startOffRender(const QString&,bool,bool)),ui->openGLWidget,SLOT(exportOffRender(const QString&,bool,bool)));
    p->setWindowModality(Qt::ApplicationModal);
    connect( ui->openGLWidget,SIGNAL(setValue(int)),p,SLOT(setValue(int)));
    p->show();
    ui->openGLWidget->off_render(savePath,isAiMode);
    delete p;
    p = nullptr;
    AkUtil::TDebug("-------------IgnoreEvent end--------------");
    }
    ui->openGLWidget->update();
#else
    mpDlg = new ProgressDialog(nullptr);//(QDialog*)this
    IgnoreEvent ign(ui->openGLWidget);
    //boolLock orL(offRenderLock);
    if(isAiMode){
        mpDlg->setText(tr("Generate AI Image"));
    }else{
        mpDlg->setText(tr("Export G-Code"));
    }
    mpDlg->setCancelVisible(false);
    connect( ui->openGLWidget,SIGNAL(setValue(int)),mpDlg,SLOT(setValue(int)));
    connect(mpDlg, &ProgressDialog::destroyed, this, [&]()->void { mpDlg = NULL; });
    //connect( this,SIGNAL(startOffRender(const QString&,bool,bool)),ui->openGLWidget,SLOT(exportOffRender(const QString&,bool,bool)));
    //emit startOffRender(savePath,isAiMode,false);
    QTimer::singleShot(100, this, [&,savePath](){
               //emit startOffRender(savePath,isAiMode,false);
               ui->openGLWidget->off_render(savePath,isAiMode);
            });
   mpDlg->exec();
#endif
}


//#include<QSettings>
void viewWidget::setSavePicTrue()
{
    //    QAction* viewAction = qobject_cast<QAction*> (sender());
    //    viewAction->setIcon(QIcon(checked ? ":/pic/ui/fdm_ai_icon_s.png" : ":/pic/ui/fdm_ai_icon_n.png"));

    int curLayer = this->verticalSlider->getUpValue();
    std::array<float, 3> setPosition;
    std::array<float, 3> viewCenter;
    std::array<float, 3> camLookUp;
    setPosition[0] = 292.78;
    setPosition[2] = 2.07 + ui->openGLWidget->m_layers.get_z_at(curLayer - 1);
    setPosition[1] = 20.57 + ui->openGLWidget->m_sequential_view.current_position[1];
    viewCenter[0] = -0.866;
    viewCenter[1] = 0;
    viewCenter[2] = -0.5;
    camLookUp[0] = -0.5;
    camLookUp[1] = 0.0;
    camLookUp[2] = 0.866;

    ui->openGLWidget->set_look_at(std::move(setPosition), std::move(viewCenter), std::move(camLookUp));
    //ui->openGLWidget->set_save_pic(true);

}


void viewWidget::setOriginalStlName(const QString& oStlName)
{
    this->oStlName = oStlName;
    this->ui->openGLWidget->setOriginalStlName(oStlName);
}

void viewWidget::setLoadingProgress()
{
    timer_value += 895.2 * (1024 / 4.7) / f_size * 0.3;
    qDebug()<< "timer sum set Value " <<timer_value;
    if(timer_value < 30.0)
    {
        emit this->ui->openGLWidget->setValue((int)timer_value);
    }else{
        tp_timer->stop();
        timer_value = 0.0;
    }
}

void viewWidget::reSetGcodePath( std::string gcodePath,bool isAiMode, int gcode_size)
{
    
    QDir dir(saveTempPath);
    if (dir.exists())
    {
        dir.removeRecursively();
    }
    
    if (this->mPlayTimer != nullptr)
    {
    this->mPlayTimer->stop();
    }
    
    QFileInfo fi(QString::fromStdString(gcodePath));
    f_size = fi.size() / 1024; 
    if(tp_timer == nullptr)
    {
       tp_timer = new QTimer(this);
    }
    connect(tp_timer, &QTimer::timeout, this, &viewWidget::setLoadingProgress,Qt::UniqueConnection);
    tp_timer->start(100);

    QThreadPool::globalInstance()->start([=](){
        processor->reset();
        processor->enable_producers(true);
        processor->process_file(gcodePath, false);
        this->ui->openGLWidget->waitForSceneInit();
        auto poolThread = QThread::currentThread();
        
        QMetaObject::invokeMethod(QCoreApplication::instance(), [&] {
            this->ui->openGLWidget->cnt = new QOpenGLContext();
            this->ui->openGLWidget->m_offscreenSurface = new QOffscreenSurface();
            this->ui->openGLWidget->m_offscreenSurface->setFormat(this->ui->openGLWidget->context()->format());
            this->ui->openGLWidget->m_offscreenSurface->create();
            this->ui->openGLWidget->doneCurrent();
            this->ui->openGLWidget->cnt->setFormat(this->ui->openGLWidget->context()->format());
            this->ui->openGLWidget->cnt->setShareContext(this->ui->openGLWidget->context());
            this->ui->openGLWidget->cnt->create();
            this->ui->openGLWidget->cnt->moveToThread(poolThread);

        }, Qt::BlockingQueuedConnection);
        this->ui->openGLWidget->clearGcodeSource();
        this->setToolPath(std::move(processor->extract_result()), gcodePath, isAiMode,gcode_size);
        
        this->tp_timer->stop();
        this->timer_value = 0.0;
        this->ui->openGLWidget->loadGcode();

        this->ui->openGLWidget->cnt->moveToThread(QThread::currentThread());
        this->ui->openGLWidget->cnt->destroyed();
        this->ui->openGLWidget->cnt = nullptr;
        emit processFile(QString::fromStdString(gcodePath),isAiMode);
    });
    if(processing){
        connect(this,&viewWidget::processFile,this,[&](const QString& _gcodePath,bool _isAiMode){
            changeSlider();
            processing = false;
            
            QVariant totleTime = allTimeValue;
            m_rpc.data()->setProperty("gcodeTotalTime",totleTime);
            QVariant totleFilament = allFilamentValue;
            m_rpc.data()->setProperty("gcodeTotalFilament",totleFilament);

            float extruderTemperature, bedTemperature;
            getGcodeTargetTemperature(extruderTemperature, bedTemperature);
            QVariant gcodeExtruderTargetTemperature = extruderTemperature;
            m_rpc.data()->setProperty("gcodeExtruderTargetTemperature",gcodeExtruderTargetTemperature);
            QVariant gcodeBedTargetTemperature = bedTemperature;
            m_rpc.data()->setProperty("gcodeBedTargetTemperature",gcodeBedTargetTemperature);
        }, Qt::QueuedConnection);
    }
}

void viewWidget::getGcodeTargetTemperature(float& extruderTemperature, float& bedTemperature)
{
    if(ui == nullptr || ui->openGLWidget == nullptr)
    {
        return;
    }

    extruderTemperature = 0.0f;
    bedTemperature = 0.0f;
    for(int i=0;i< ui->openGLWidget->gcode_result.moves.size();++i)
    {
        if(extruderTemperature<0.01)
        {
            extruderTemperature = ui->openGLWidget->gcode_result.moves[i].temperature;
        }
        if(bedTemperature<0.01)
        {
            bedTemperature = ui->openGLWidget->gcode_result.moves[i].bed_temperature;
        }

        if(extruderTemperature > 0.01 && bedTemperature > 0.01)
        {
            break;
        }
    }
}

void viewWidget::setDefaultScene()
{
     PrintMachineBoxParam de_box;
     de_box.m_color = QColor(0.313725 * 255, 0.313725 * 255, 0.313725 * 255,1);
     de_box.m_height = 250;
     de_box.m_length = 235;
     de_box.m_width = 235;
     de_box.num = 11513528;
     de_box.m_lineWidth = 1;
    this->m_sceneParam.m_eye = QVector3D(0, 0, 0);
     this->m_sceneParam.m_front = QVector3D(0, 0, 0);
     this->m_sceneParam.m_up = QVector3D(0, 0, 0);
    this->m_sceneParam.m_printMachineBox = de_box;
}

bool viewWidget::setToolPath(Anker::GCodeProcessor::Result&& gcode_result, std::string gcodePath,bool isAiMode, int gcode_size)
{
    this->isAiMode = isAiMode;
    auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString dirPath = writableLocation + "/AICamera";
    QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz");
    dirPath = dirPath + "/" + timestamp;
    saveTempPath = dirPath;
    //QString dirPath = "./AICamera";
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    QRegularExpression re("[^/]+(?!.*/)");
    QString QgcodePath = QString::fromStdString(gcodePath).replace("\\","/");
    QRegularExpressionMatch match = re.match(QgcodePath);
     //noly name  without path
    QString DefaultGcodeName = match.captured(0);
    ui->openGLWidget->setOriginalGcodeName(DefaultGcodeName);
    if (match.hasMatch())
    {
        QgcodePath = dirPath + '/'+"temp_"+match.captured(0);
    }
    runTimesId = gcode_size;
    //qDebug() << "QgcodePath"<<QgcodePath;
    bool IsTrue = QFile::exists(QgcodePath);
    if(IsTrue)
    {
        QFile::remove(QgcodePath);
    }
    QFile::copy(QString::fromStdString(gcodePath.c_str()), QgcodePath);
    QString str;
    QDebug(&str) << "---QString::fromStdString(gcodePath.c_str()) "<<QString::fromStdString(gcodePath.c_str());
    QDebug(&str) << "---QgcodePath "<<QgcodePath;
    AkUtil::TDebug( str);
    if (ui->openGLWidget->setGcodePath(QgcodePath.toStdString()))
    {
        ui->openGLWidget->gcode_result = std::move(gcode_result);
        return true;
    }
    return false;
    //    ui->openGLWidget->load_toolpaths( ui->openGLWidget->gcode_result);
}

void viewWidget::changeSlider()
{
    

    unsigned int layersize = static_cast<unsigned int>(ui->openGLWidget->m_layers.size());
    if(layersize < 1)
    {
        this->verticalSlider->hide();
        this->spinBox->hide();
        this->spinBox_2->hide();
        this->horizontalSlider->hide();
        this->pushButton_9->hide();//play button
        
        if(!printModeInit || !innerModeInit)
        {
            control::MessageDialog a("warning",tr("G-Code failed to open. Try again."), control::MessageDialog::BUTTONFLAG::OK);
            a.exec();
        }
        if(printModeInit){
            printModeInit = false;
        }else{
            innerModeInit = false;
        }
        this->ExitButton->setEnabled(false);
        this->ExportButton->setEnabled(false);

        return;
    }else{
        this->verticalSlider->show();
        this->spinBox->show();
        this->spinBox_2->show();
        this->horizontalSlider->show();
        this->pushButton_9->show();//play button
        this->ExitButton->setEnabled(true);
        this->ExportButton->setEnabled(true);
    }

    this->verticalSlider->setRange(1, layersize);
    this->ui->openGLWidget->refresh_render_paths(true,false);
    this->spinBox->lineEdit->setRange(1, layersize);
    this->spinBox->lineEdit->setValue(1);//setText(QString::number(1));
    this->spinBox->label->setText(" / "+QString::number(1));

//	this->spinBox_2->setMinimum(1);
//	this->spinBox_2->setMaximum(layersize);
    this->spinBox_2->lineEdit->setRange(1, layersize);//lineEdit->setValidator(new QIntValidator(1, layersize, this));
    this->spinBox_2->lineEdit->setValue(layersize);
    this->spinBox_2->label->setText(" / "+QString::number(layersize));
    getTimeCount();
    setTimeStr();
    getFilamentCount();
    setFilamentStr();
    getGcodeSize();
    setGcodeSize();

    this->update();
}

//no need
void viewWidget::closeEvent(QCloseEvent* event)
{
    //Q_EMIT closePreviewEvent(runTimesId);
    //m_rpc.
    int num = m_rpc->property("linkNum").toInt();
    QVariant ln = num - 1;
    m_rpc->setProperty("linkNum",ln);
    QString str;
    QDebug(&str) << "m_rpc->property(LinkNum).toInt() "<<num;
    QDebug(&str) << "ln "<<ln.toInt();
    //QDebug(&str) << "p.value<passSceneParam>().m_printMachineBox.m_height "<<var_test.value<passSceneParam>().m_printMachineBox.m_height;
    AkUtil::TDebug( str);

    ui->openGLWidget->resetGpu();
    event->accept();
}


void viewWidget::paintEvent(QPaintEvent* event)
{
    ui->openGLWidget->update();
}

void viewWidget::wheelEvent(QWheelEvent* event)
{
   if(this->verticalSlider->hasFocus())
   {
    QWheelEvent *wevent = new QWheelEvent(event->posF(),event->delta(),Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(this->verticalSlider,wevent);
   }
   if(this->horizontalSlider->hasFocus())
   {
    QWheelEvent *wevent = new QWheelEvent(event->posF(),event->delta(),Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(this->horizontalSlider,wevent);
   }
   event->accept();
}

void viewWidget::changeEvent(QEvent * event)
{
  if(this->windowState() == Qt::WindowNoState && !this->printMode && !this->innerMode)
  {
    if(QApplication::desktop()->width() < 1920 || QApplication::desktop()->height() - 80 < 1030)
    {

        this->resize(QApplication::desktop()->width() * 0.8, (QApplication::desktop()->height() - 80)*0.8);
        QString dstr;
        QDebug(&dstr) << "------changeEvent:size-------"<<this->size() ;
        AkUtil::TDebug(dstr);
        //this->setWindowState(Qt::WindowMaximized);
    }
  } else if (event->type() == QEvent::LanguageChange) {
    if (ExportButton) {
        ExportButton->setText(tr("Export"));
    }
    if (ExitButton) {
        ExitButton->setText(tr("Print"));
    }
    setWindowTitle(tr("AnkerMake G-Code Preview"));
#ifdef USE_EXTRA_UI
    if (linetypeCombox) {
        int index = linetypeCombox->currentIndex();
        linetypeCombox->clear();
        linetypeCombox->addItem(tr("Line Type"));
        linetypeCombox->addItem(tr("Speed"));
        linetypeCombox->addItem(tr("Trapezoid"));
        linetypeCombox->addItem(tr("Flow"));
        linetypeCombox->setCurrentIndex(index);
    }
    if (supportLabel_speed) {
        supportLabel_speed->setText(tr("Support"));
    }
    if (unkonwnLabel_speed) {
        unkonwnLabel_speed->setText(tr("Unknown"));
    }
    if (colorPatch_label_unit) {
        colorPatch_label_unit->setText(tr("mm/s"));
    }
    if (innerWallLabel_trapezoid) {
        innerWallLabel_trapezoid->setText(tr("Inner Wall"));
    }
    if (outerWallLabel_trapezoid) {
        outerWallLabel_trapezoid->setText(tr("Outer Wall"));
    }
    if (skinLabel_trapezoid) {
        skinLabel_trapezoid->setText(tr("Skin"));
    }
    if (skinLabel_trapezoid) {
        skinLabel_trapezoid->setText(tr("Skin"));
    }
    if (helperLabel_Flow) {
        helperLabel_Flow->setText(tr("Adhesion"));
    }
    if (fillLabel_Flow) {
        fillLabel_Flow->setText(tr("Fill"));
    }
    if (supportLabel_Flow) {
        supportLabel_Flow->setText(tr("Support"));
    }
    if (supportLabel_Flow) {
        supportLabel_Flow->setText(tr("Support"));
    }
    if (unkonwnLabel_Flow) {
        unkonwnLabel_Flow->setText(tr("Unknown"));
    }
    if (colorPatch_label_unit_Flow) {
        colorPatch_label_unit_Flow->setText(tr("mm/s"));
    }
#endif
    if (linetypelabel) {
        linetypelabel->setText(tr("Line Type"));
    }
    if (innerWallLabel) {
        innerWallLabel->setText(tr("Inner Wall"));
    }
    if (outerWallLabel) {
        outerWallLabel->setText(tr("Outer Wall"));
    }
    if (skinLabel) {
        skinLabel->setText(tr("Skin"));
    }
    if (helperLabel) {
        helperLabel->setText(tr("Adhesion"));
    }
    if (fillLabel) {
        fillLabel->setText(tr("Fill"));
    }
    if (ConsumedLabelabel) {
        ConsumedLabelabel->setText(tr("Consumables"));
    }
    if (filamentLabel) {
        filamentLabel->setText(tr("Filament"));
    }
    if (timelabel) {
        timelabel->setText(tr("Time"));
    }
    if (travelLabel) {
        travelLabel->setText(tr("Travel"));
    }
    if (travelLabel) {
        travelLabel->setText(tr("Travel"));
    }
    if (zlapLabel) {
        zlapLabel->setText(tr("zlap"));
    }
    if (supportLabel) {
        supportLabel->setText(tr("Support"));
    }
    if (unkonwnLabel) {
        unkonwnLabel->setText(tr("Unknown"));
    }
  }
  return QWidget::changeEvent(event);
}

void viewWidget::resizeEvent(QResizeEvent* event)
{
    AkUtil::TDebug("-------------viewWidget::resizeEvent--------------");
    //event->accept();
}

void viewWidget::on_pushButton_quit_clicked()
{//change for print function  TODO: add show print widget

    
    qint64 id = QDateTime::currentSecsSinceEpoch();
    
    //this->ui->openGLWidget->off_render_single();
    QString sendStr;

    if(!isAiMode)
    {
        sendStr = this->ui->openGLWidget->sendOriginalStlNamedFile(true);
    }else{
        sendStr = this->ui->openGLWidget->sendAcodeToPrintCtr();
        if(sendStr.size() <= 1)
        {
            control::MessageDialog a("warning","NO GCODE HERE!", 0x00400000);
            a.exec();
            return ;
        }
        m_progessDlg =  new  ProgressDialog(nullptr);//(QDialog*)this
        IgnoreEvent ign(ui->openGLWidget);
        //boolLock orL(offRenderLock);
        m_progessDlg->setText(tr("Generate AI Image"));
        m_progessDlg->setCancelVisible(false);
        connect( ui->openGLWidget,SIGNAL(setValue(int)),m_progessDlg,SLOT(setValue(int)));
        //connect( this,SIGNAL(startOffRender(const QString&,bool,bool)),ui->openGLWidget,SLOT(exportOffRender(const QString&,bool,bool)));
        //emit startOffRender(savePath,isAiMode,false);
        QTimer::singleShot(100, this, [&,sendStr](){
                   //emit startOffRender(savePath,isAiMode,false);
                   ui->openGLWidget->off_render(sendStr,isAiMode);
                });
        m_progessDlg->exec();
    }
    AkUtil::TDebug("sendStr: " + sendStr);
    emit printGcode(sendStr);
    //qs->displayDeviceListWidget(id, sendStr);
    return;
}

void viewWidget::setFilamentStr()
{
    double showValue = 0.0;
    if (this->innerWallcheckBox->checkState())
    {
        showValue += showLabelCount[0];
    }
    if (this->outerWallcheckBox->checkState())
    {
        showValue += showLabelCount[1];
    }
    if (this->skincheckBox->checkState())
    {
        showValue += showLabelCount[2];
    }
    if (this->helpercheckBox->checkState())
    {
        showValue += showLabelCount[3];
    }
    if (this->fillcheckBox->checkState())
    {
        showValue += showLabelCount[4];
    }
    if (this->travelcheckBox->checkState())
    {
        showValue += showLabelCount[5];
    }
    if (this->supportcheckBox->checkState())
    {
        showValue += showLabelCount[6];
    }
    if (this->unkonwncheckBox->checkState())
    {
        showValue += showLabelCount[7];
    }
    double mm_per_g = 0.00298;
    QString showStr = QString("%1 g").arg(int(showValue * mm_per_g), 6);
    filamentValue->setText(showStr);
    double sum = std::accumulate(std::begin(showLabelCount), std::end(showLabelCount), 0.0);
    allFilamentValue = sum * mm_per_g;
    this->allfilamentLabel->setText(QString("%1 g").arg(int(sum * mm_per_g), 6));
    this->allfilamentLabel->setStyleSheet("background:transparent");
}

void viewWidget::getGcodeSize()
{
    showAllSize[0] = this->ui->openGLWidget->m_boundingBox.max[0] - this->ui->openGLWidget->m_boundingBox.min[0] + 1;
    showAllSize[1] = this->ui->openGLWidget->m_boundingBox.max[1] - this->ui->openGLWidget->m_boundingBox.min[1] + 1;
    showAllSize[2] = this->ui->openGLWidget->m_boundingBox.max[2] - this->ui->openGLWidget->m_boundingBox.min[2] + 1;
}

void viewWidget::setGcodeSize()
{
    QString showStr = QString("%1*%2*%3 mm").arg(QString::number(showAllSize[0]), QString::number(showAllSize[1]), QString::number(showAllSize[2]));
    this->allSizeLabel->setStyleSheet("background:transparent");
    this->allSizeLabel->setText(showStr);
}

void viewWidget::setPerformance(bool setValue)
{
    this->isHighPerformance = setValue;
    this->ui->openGLWidget->setPerformance(setValue);
}

void viewWidget::setAimode(bool _isAimode)
{
    AkUtil::TDebug("setAimode  : "+ QString::number(_isAimode));
    this->isAiMode = _isAimode;
}

void viewWidget::setLoggingStatus(bool status)
{
    loggingStatus = status;
}

void viewWidget::pauseGcodePlay(int index)
{
    qDebug()<<  "pauseGcodePlay index :" <<index;
   
    if(index == 1)
    {
       return;
    }
    
    if (this->mPlayTimer != nullptr)
    {
        this->mPlayTimer->stop();
    }
    this->pushButton_9->setChecked(false);
}

void viewWidget::setTimeStr()
{
    double showValue = 0;
    if (this->innerWallcheckBox->checkState())
    {
        showValue += showTimeLabelCount[0];
    }
    if (this->outerWallcheckBox->checkState())
    {
        showValue += showTimeLabelCount[1];
    }
    if (this->skincheckBox->checkState())
    {
        showValue += showTimeLabelCount[2];
    }
    if (this->helpercheckBox->checkState())
    {
        showValue += showTimeLabelCount[3];
    }
    if (this->fillcheckBox->checkState())
    {
        showValue += showTimeLabelCount[4];
    }
    if (this->travelcheckBox->checkState())
    {
        showValue += showTimeLabelCount[5];
    }
    if (this->supportcheckBox->checkState())
    {
        showValue += showTimeLabelCount[6];
    }
    if (this->unkonwncheckBox->checkState())
    {
        showValue += showTimeLabelCount[7];
    }
    QString showStr = getTimesDhms(showValue);
    timeValue->setText(showStr);
    float sum = std::accumulate(std::begin(showTimeLabelCount), std::end(showTimeLabelCount), 0.0);
    allTimeValue = sum;
    this->allTimeLabel->setText(getTimesDhms(sum));
    this->allTimeLabel->setStyleSheet("background:transparent");
}

QString viewWidget::getTimesDhms(float timeInSecs)
{
    int days = (int)(timeInSecs / 86400.0f);
    timeInSecs -= (float)days * 86400.0f;
    int hours = (int)(timeInSecs / 3600.0f);
    timeInSecs -= (float)hours * 3600.0f;
    int minutes = (int)(timeInSecs / 60.0f);
    timeInSecs -= (float)minutes * 60.0f;

    QString bufferStr;
    if (days > 0)
        bufferStr = QString("%1d %2h %3m %4s").arg(QString::number(days), QString::number(hours), QString::number(minutes), QString::number((int)timeInSecs));
    else if (hours > 0)
        bufferStr = QString("%1h %2m %3s").arg(QString::number(hours), QString::number(minutes), QString::number((int)timeInSecs));
    else if (minutes > 0)
        bufferStr = QString(" %1m %2s").arg(QString::number(minutes), QString::number((int)timeInSecs));
    else
        bufferStr = QString("%1s").arg(QString::number((int)timeInSecs));

    return bufferStr;
}

void viewWidget::getTimeCount()
{
    this->ui->openGLWidget->getTimeAndPercent();
    const std::vector<float> times = this->ui->openGLWidget->times;
    const  std::vector<Anker::ExtrusionRole>& roles = this->ui->openGLWidget->m_roles;
    const std::map<uint8_t, featureType> roleDict = {
    {ExtrusionRole::erNone ,featureType::unknown},
    {ExtrusionRole::erPerimeter,featureType::innerWall},
    {ExtrusionRole::erExternalPerimeter,featureType::outerWall},
    {ExtrusionRole::erOverhangPerimeter ,featureType::unknown},
    {ExtrusionRole::erInternalInfill ,featureType::fill},
    {ExtrusionRole::erSolidInfill,featureType::skin},
    {ExtrusionRole::erTopSolidInfill,featureType::unknown},
    {ExtrusionRole::erIroning,featureType::unknown},
    {ExtrusionRole::erBridgeInfill,featureType::unknown},
    {ExtrusionRole::erInternalBridgeInfill,featureType::unknown},
    {ExtrusionRole::erThinWall,featureType::unknown},
    {ExtrusionRole::erGapFill,featureType::unknown},
    {ExtrusionRole::erSkirt,featureType::helper},
    {ExtrusionRole::erSupportMaterial,featureType::support},
    {ExtrusionRole::erSupportMaterialInterface,featureType::support},
    {ExtrusionRole::erWipeTower,featureType::helper},
    {ExtrusionRole::erMilling,featureType::unknown},
    {ExtrusionRole::erCustom,featureType::unknown},
    {ExtrusionRole::erMixed,featureType::unknown}
    };
    showTimeLabelCount = {};
    for (int i = 0; i < roles.size(); i++)
    {
        auto feature = roleDict.find(roles[i]);
        showTimeLabelCount[feature->second] += times[i];

    }

}

void viewWidget::getFilamentCount()
{
    const std::vector<std::array<double, erCount>>& filamentCount = this->ui->openGLWidget->m_layers.get_filamentcost();

    const std::map<uint8_t, featureType> roleDict = {
    {ExtrusionRole::erNone ,featureType::unknown},
    {ExtrusionRole::erPerimeter,featureType::innerWall},
    {ExtrusionRole::erExternalPerimeter,featureType::outerWall},
    {ExtrusionRole::erOverhangPerimeter ,featureType::unknown},
    {ExtrusionRole::erInternalInfill ,featureType::fill},
    {ExtrusionRole::erSolidInfill,featureType::skin},
    {ExtrusionRole::erTopSolidInfill,featureType::unknown},
    {ExtrusionRole::erIroning,featureType::unknown},
    {ExtrusionRole::erBridgeInfill,featureType::unknown},
    {ExtrusionRole::erInternalBridgeInfill,featureType::unknown},
    {ExtrusionRole::erThinWall,featureType::unknown},
    {ExtrusionRole::erGapFill,featureType::unknown},
    {ExtrusionRole::erSkirt,featureType::helper},
    {ExtrusionRole::erSupportMaterial,featureType::support},
    {ExtrusionRole::erSupportMaterialInterface,featureType::support},
    {ExtrusionRole::erWipeTower,featureType::helper},
    {ExtrusionRole::erMilling,featureType::unknown},
    {ExtrusionRole::erCustom,featureType::unknown},
    {ExtrusionRole::erMixed,featureType::unknown}
    };

    std::fill( std::begin( showLabelCount ), std::end( showLabelCount ), 0 );

    for (auto it = filamentCount.begin(); it != filamentCount.end(); it++)
    {
        for (uint8_t fname = erNone; fname < erCount; fname++) {
            auto feature = roleDict.find(fname);
            showLabelCount[feature->second] += (*it)[fname];
        }
    }

    //  qDebug() << "showLabelCount[0]"<< showLabelCount[0]<< " ";
    //  qDebug() << "showLabelCount[1]"<< showLabelCount[1]<< " ";
    //  qDebug() << "showLabelCount[2]"<< showLabelCount[2]<< " ";
    //  qDebug() << "showLabelCount[3]"<< showLabelCount[3]<< " ";
    //  qDebug() << "showLabelCount[4]"<< showLabelCount[4]<< " ";
    //  qDebug() << "showLabelCount[5]"<< showLabelCount[5]<< " ";
    //  qDebug() << "showLabelCount[6]"<< showLabelCount[6]<< " ";
    //  qDebug() << "showLabelCount[7]"<< showLabelCount[7]<< " ";
}


void viewWidget::setSceneParams(const SceneParam& param)
{
    m_sceneParam = param;
}
