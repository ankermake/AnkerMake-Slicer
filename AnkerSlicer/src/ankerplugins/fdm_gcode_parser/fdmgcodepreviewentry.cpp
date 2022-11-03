#include "fdmgcodepreviewentry.h"
#include "viewwidget.h"
#include "common/parameters/rich_parameter_list.h"
#include "common/GeoAndShow/SceneParam.h"
#include "../common/ak_const.h"
#include "../common/utilities/tlogger.h"
#include <QMainWindow>
#include <QTranslator>
#include "GcodeViewer.h"

#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

extern "C"
{
#ifdef _WIN32
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
#elif __APPLE__
unsigned long NvOptimusEnablement = 0x00000001;
#endif
}
extern "C"
{
#ifdef _WIN32
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#elif __APPLE__
int AmdPowerXpressRequestHighPerformance = 1;
#endif
}

struct OpenGLInfo{
    QString Vendor = "";
    QString Render = "";
};

bool isHighPerformance(const OpenGLInfo& _info_opengl_t){
#ifdef _WIN32
    
    if(_info_opengl_t.Vendor.contains("Intel",Qt::CaseSensitivity::CaseInsensitive)){
     
        AkUtil::TDebug("m_opengl_info.Vendorr Intel");
        #ifdef USE_EXTRA_UI
            return true;
        #endif
        return false;
    }else if(_info_opengl_t.Vendor.contains("NVIDIA",Qt::CaseSensitivity::CaseInsensitive)){
     
         AkUtil::TDebug("m_opengl_info.Vendorr NVIDIA");
         return true;
    }else if(_info_opengl_t.Vendor.contains("AMD",Qt::CaseSensitivity::CaseInsensitive)){
        AkUtil::TDebug("m_opengl_info.Vendorr AMD");
        return true;
    }
#elif __APPLE__
    
    return true;
#endif

}

FdmGcodePreviewEntry::FdmGcodePreviewEntry()
{

}

FdmGcodePreviewEntry::~FdmGcodePreviewEntry()
{
    if(this->ww != nullptr){
        delete  ww;
        ww = nullptr;
    }
}

//void FdmGcodePreviewEntry::createView(int argc, char *argv[])
void FdmGcodePreviewEntry::createView(QStringList& args)
{
    //QCoreApplication::setOrganizationName("AnkerSlicer");
    //QString appName = QString("AnkerMake_%1bit_fp").arg(QString::number(QSysInfo::WordSize));
    //QCoreApplication::setApplicationName(appName);

    //QApplication a(argc, argv);
    
    
    QCommandLineParser parser;
    parser.setApplicationDescription("gcode helper");
    parser.addHelpOption();
    QCommandLineOption gcodePath("p", "the Gcode path", "gcode_path","");
    QCommandLineOption originalStlName("o", "the original stl name", "original_stl_name","");
    QCommandLineOption aiMode("ai", "is ai mode");
    QCommandLineOption printMode("pmode", "is print contral mode");
    QCommandLineOption innerMode("imode", "is inner mianwindows mode");
    QCommandLineOption runtimes("n", "run times ", "rt","0");
    QCommandLineOption _hostUrl("host", "host url adress ", "hurl","");
    parser.addOption(gcodePath);
    parser.addOption(originalStlName);
    parser.addOption(aiMode);
    parser.addOption(printMode);
    parser.addOption(runtimes);
    parser.addOption(_hostUrl);
    parser.addOption(innerMode);

    parser.process(args);

    qDebug() << parser.isSet(aiMode);
    qDebug() << parser.value(runtimes);
    qDebug() << parser.value(gcodePath);

    std::string gcodePath_std = std::move(parser.value(gcodePath).toStdString());
    QString oStlName = parser.value(originalStlName);
    bool isAimode = parser.isSet(aiMode);
    bool isPmode = parser.isSet(printMode);
    bool isImode = parser.isSet(innerMode);
    int rts = parser.value(runtimes).toInt();
    QString hostUrl = parser.value(_hostUrl);
    SceneParam iniSceneParam;
    std::unique_ptr<Anker::GCodeProcessor> processor;
    processor = std::make_unique<Anker::GCodeProcessor>();
    processor->reset();
    processor->enable_producers(true);
    processor->process_file(gcodePath_std, false);
    OpenGLInfo m_opengl_info;
    {
        QOffscreenSurface surf;
        surf.create();

        QOpenGLContext ctx;
        ctx.create();
        ctx.makeCurrent(&surf);
        m_opengl_info.Vendor = QString::fromStdString((const char*)ctx.functions()->glGetString(GL_VENDOR));
        m_opengl_info.Render =  QString::fromStdString((const char*)ctx.functions()->glGetString(GL_RENDERER));
        //(const char*)ctx.functions()->glGetString(GL_EXTENSIONS);
    }
    bool is_hight_performance = isHighPerformance(m_opengl_info);
    AkUtil::TDebug("m_opengl_info.vendor"+m_opengl_info.Vendor);
        //#ifdef __APPLE__
        QSurfaceFormat glFormat;
        glFormat.setVersion(3, 3);
if(is_hight_performance){
        glFormat.setSamples(128);
}else {
        AkUtil::TDebug("m_opengl_info.Vendor no sample");
        glDisable(GL_MULTISAMPLE);
        
}
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(glFormat);
        //#endif
        //static viewWidget w(nullptr, iniSceneParam,isPmode, hostUrl, isImode);
        
//        if (nullptr != vw)
//        {
//            delete vw;
//            vw = nullptr;
//        }
        ww = new viewWidget(nullptr, iniSceneParam, isPmode, hostUrl, isImode);
        viewWidget& w = *ww;
        w.setPerformance(is_hight_performance);
        //add qss
        QFile file(":/pic/ui/default.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QString::fromLatin1(file.readAll());
        //a.setStyleSheet(styleSheet);
        
        //font.setPixelSize(14);
        //a.setFont(font);

        QTranslator translator;
        translator.load(":/../language/tr_zh.qm");
        qDebug() << " is empty =" << translator.isEmpty();
        //a.installTranslator(&translator);
        w.setOriginalStlName(oStlName);
        if(isPmode | isImode)
        {
            w.setWindowFlags(Qt::FramelessWindowHint| Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        }
        w.setToolPath(std::move(processor->extract_result()), gcodePath_std, isAimode,rts);
//        if(isPmode){
//            w.show();
//            w.changeSlider();
//        }
        //a.processEvents();

        //return a.exec();
        return ;

}
