/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
#include <common/mlapplication.h>
#include <common/GLExtensionsManager.h>
#include <QMessageBox>
#include "mainwindow.h"
#include "translator.h"
#include <QGLFormat>
#include <QSurfaceFormat>
#include <QString>
#include <QTextCodec>

#include <clocale>
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include "ankermainwindow.h"
#endif //_WIN32
#include "LocalServer.h"

#include "settingmanager.h"
#include "common/utilities/tlogger.h"
#include "ankermainwindow.h"
#include "controls/useragreementwidget.h"
#include "common/utilities/ioapi.h"
#include "common/Socket/HeartBeatThead.h"

#ifdef _WIN32
#define ANKERMAKE_EXE "AnkerSlicer.exe"
#elif __APPLE__
#define ANKERMAKE_EXE "AnkerSlicer.app"
#endif

char LocalSeverName[]="AnkerMakerLocalServer";

using namespace AkUtil;
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

void handleCriticalError(const MLException& exc);
void wait(unsigned int TimeMS);



int main(int argc, char *argv[]){

    
    {
        bool ret = false;
#ifdef _WIN32
        QProcess process;
        process.start("tasklist", QStringList() << "/FI" << "imagename eq " + QString(ANKERMAKE_EXE));
        process.waitForFinished();

        QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput());
        ret = outputStr.contains("PID");
        ret &= outputStr.count(ANKERMAKE_EXE) > 1; 
        qDebug() << "outputStr: " << outputStr;
#elif __APPLE__
/*         std::string strCmd = "ps -ef|grep " + QString(ANKERMAKE_EXE).toStdString() + " |grep -v grep |awk '{print $2}'";
        const char* strFindName = strCmd.c_str();
        FILE *pPipe = popen(strFindName, "r");
        if(pPipe != NULL)
        {
            char name[512] = { 0 };
            while(fgets(name, sizeof(name), pPipe) != NULL)
            {
                int nLen = strlen(name);
                if(nLen > 0 && name[nLen - 1] == '\n')
                {
                    name[nLen - 1] = '\0';
                    ret = true;
                    break;
                }
            }
            pclose(pPipe);
        } */
#endif
        if(ret)
        {
            if(argc == 1)
            {
                
                qDebug() << "ret:" << ret << ", Exit.";
                return 0;
            }
            else
            {
                
                LocalClient client;
                if(client.ConnectToServer(LocalSeverName))
                {
                    for(int i=1; i<argc;++i)
                    {
                        wait(500);
                        client.sendMessage(QString::fromLocal8Bit(argv[i]));
                        qDebug() <<"tell the existing process to open:"<<QString::fromLocal8Bit(argv[i]) ;
                    }
                }
                else
                {
                    qDebug() <<"fail to connect local server" ;
                }
                return 0;
            }
        }
    }

    
    {
        
        QCoreApplication::setOrganizationName(MeshLabApplication::organization());
        QString tmp = MeshLabApplication::appArchitecturalName(MeshLabApplication::HW_ARCHITECTURE(QSysInfo::WordSize));
        QCoreApplication::setApplicationName(tmp);

        
        std::setlocale(LC_ALL, "C");
        QLocale::setDefault(QLocale::C);

    }


    MeshLabApplication app(argc, argv);



    
    int clearLogCreatedMoreThanXDays = 5;
    QString logPath = TLogger::instance()->getLogPath();
    QStringList filter;
    filter << "*.log";
    QSet<QString> excludeSet;
    IoApi::clearFilesUnderPath(clearLogCreatedMoreThanXDays*24*60*AkConst::Time::MINUTE_SECOND, logPath, filter,excludeSet);

    AkUtil::TWarning("\n\n\n\n");
    QString dbgStr;
    {QDebug debug(&dbgStr); debug.noquote(); for(int i=0; i<argc;++i){debug<<argv[i];} qDebug() << dbgStr;}
    AkUtil::TWarning(dbgStr);
    TDebug("Application exec Start...");

    
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

//    QStringList m_fontList;
//    int lcdFontId = QFontDatabase::addApplicationFont(":/qss/Roboto-Regular-14.ttf");
//    //  int lcdFontId1 = QFontDatabase::addApplicationFont(":/qss/Roboto-Medium-12.ttf");
//    if(lcdFontId != -1) {
//        m_fontList << QFontDatabase::applicationFontFamilies(lcdFontId);
//        QFont font(m_fontList.at(0));
//        font.setPixelSize(14);
//        app.setFont(font);
//    }
    QFont font("Microsoft YaHei");
    font.setPixelSize(14);
    app.setFont(font);


    auto m_translator =  Translator::instance();
    SettingManager setting;
    int index =  setting.getCurrentLanguage();
    if(index == 1) {
        m_translator->loadLanguage(Language::CH);
    }
    else {
        m_translator->loadLanguage(Language::EN);
    }
    //add qss
    QFile file(":/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    bool flag = setting.getAcceptUserAgreement();
    if(!flag) {
        UserAgreementWidget userAgreementWidget;
        userAgreementWidget.setFixedSize(658,353);
        userAgreementWidget.setStyleSheet("background-color:#151619;border-radius:10px");
        int i = userAgreementWidget.exec();
        if(i == QDialog::Rejected) {
            setting.setAcceptUserAgreement(false);
            return -1;
        }
        setting.setAcceptUserAgreement(true);
    }
#ifdef __APPLE__
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);
#endif


    std::unique_ptr<AnkerMainWindow> window;
    try {
        TDebug("AnkerMainWindow start.");
        window = std::unique_ptr<AnkerMainWindow>(new AnkerMainWindow());
        TDebug("AnkerMainWindow end.");
    }
    catch (const MLException& exc) {
        handleCriticalError(exc);
        return -1;
    }

    window->showMaximized();

    // click .stl startup exe
    QStringList argvFileList;
    for(int i=1; i<argc;++i)
    {
       argvFileList.append(QString::fromLocal8Bit(argv[i]));
    }
    window->openFileList(argvFileList);

    LocalServer server;
    QObject::connect(&server, &LocalServer::processArgvfileNameMsg, window.get(), &AnkerMainWindow::openFileList);
    server.RunServer(LocalSeverName);

    app.processEvents();

	return app.exec();
}

void handleCriticalError(const MLException& exc){

	QString message = "MeshLab was not able to start.\n";
	if (QString::fromStdString(exc.what()).contains("GLEW initialization failed")){
		message.append("Please check your Graphics drivers.\n");
	}
	message.append("\nError: " + QString::fromStdString(exc.what()));

	QMessageBox messageBox(
				QMessageBox::Critical,
				"Critical Error",
				message);
	messageBox.addButton(QMessageBox::Ok);

#ifdef _WIN32
	bool openGLProblem = QString::fromStdString(exc.what()).contains("GLEW initialization failed");
	QCheckBox *cb = nullptr;
	if (openGLProblem) {
		cb = new QCheckBox("Use CPU OpenGL and restart MeshLab");
		messageBox.setCheckBox(cb);
	}
#endif //_WIN32

	messageBox.exec();

#ifdef _WIN32
	if (openGLProblem && cb->isChecked()) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(
					nullptr, "Are you really sure?",
					"By accepting, you are disabling hardware acceleration for MeshLab.\n\n"
					"Before proceeding, you should check the default graphic card you are "
					"using (if a dedicated graphic card is available, be sure you are using "
					"it instead of the integrated one) and if the drivers could be updated. "
					"If none of there options are available, you can proceed disabling "
					"hardware acceleration for MeshLab.\n\n"
					"You can undo this operation by renaming the file 'opengl32.dll' to "
					"'opengl32sw.dll' inside the MeshLab folder.",
					QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			//start a new process "UseCPUOpenGL.exe" to copy opengl32.dll
			SHELLEXECUTEINFO sei;

			ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

			sei.cbSize = sizeof(SHELLEXECUTEINFO);
			sei.lpVerb = _T("runas");
			sei.lpFile = _T("UseCPUOpenGL.exe"); //obviously not the actual file name
			//but it can be substituted and will work just fine in windows 7
			sei.lpParameters = _T("1");

			ShellExecuteEx(&sei);
		}
	}
#endif //_WIN32
}


void wait(unsigned int TimeMS)
{
#ifdef _WIN32
    Sleep(TimeMS);
#else
    struct timespec ts = { TimeMS/ 1000, (TimeMS% 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

