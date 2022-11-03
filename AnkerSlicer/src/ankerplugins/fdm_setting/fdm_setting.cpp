#include "fdm_setting.h"


#include "filemanage/fdmqmlsourcetree.h"
#include "filemanage/fdmqmlengine.h"
#include "fdmparamsettingswidget.h"
#include "service/fdmparameterprofileservice.h"
#include "profilemanage/fdmmachineprofilemanager.h"
#include "profilemanage/fdmmaterialprofilemanager.h"
#include "profilemanage/fdmparameterprofilemanager.h"
#include "service/fdmpreferencedialogservice.h"
#include "service/fdmrightparameterservice.h"

#include <QPushButton>
#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include "common/ak_const.h"
#include "savepanel.h"
#include "common/utilities/ioapi.h"
#include "common/utilities/tlogger.h"
using namespace AkUtil;

namespace fdmsettings {
	
	bool FdmSettingPlugin::saveProject(const QString& proPath)
	{
		auto realProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
		QString profilePath = QDir(proPath).absoluteFilePath("realProfile.ini");
		realProfile->save(profilePath);
		return true;
	}
	
	bool FdmSettingPlugin::loadProject(const QString& proPath)
	{
		QDir dir(proPath);
		if (!dir.exists())
		{
			
			return true;
		}

		QStringList filters;
		filters << "*.ini";
		QFileInfoList fInfos = dir.entryInfoList(filters, QDir::Files | QDir::Readable, QDir::Name);
		if (fInfos.size() < 1)
		{
			return true;
		}
		
		auto filePath = fInfos[0].absoluteFilePath();

        
        loadRealTimeProfile(filePath);

		return true;
	}

    
    bool FdmSettingPlugin::loadRealTimeProfile(const QString& filePath)
    {
        auto newProfile = FdmRealTimeProfile(filePath);
        FdmRightParameterService::instance()->reload(&newProfile);
        //if ()

        
        auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
        auto meterialName = realtimeProfile->getMaterialName();
        auto meterialProfile = FdmMaterialProfileManager::Instance().getProfileByName(meterialName);
        if (!meterialName.isEmpty() && meterialProfile == nullptr)
        {
             QList<FdmProfileCategory> categories;
            auto pCategory = realtimeProfile->getCategory(AkConst::Category::MATERIAL);
            categories.push_back(*pCategory);
            auto newProfile = FdmMaterialProfileManager::Instance().createProfile(meterialName, categories);
            newProfile->setVisible(true);
            newProfile->save();
        }

        
        auto machineName = realtimeProfile->getMachineName();
        auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(machineName);
        if (!machineName.isEmpty() && machineProfile == nullptr)
        {
            QList<FdmProfileCategory> categories;
            auto pCategory = realtimeProfile->getCategory(AkConst::Category::MACHINE_SETTINGS);
            categories.push_back(*pCategory);
            auto newProfile = FdmMachineProfileManager::Instance().createProfile(machineName, categories);
            newProfile->setVisible(true);
            newProfile->save();
        }

        
        auto nameInProfile = realtimeProfile->getProfileName();
        
        if (nameInProfile != AkConst::ProfileName::SIMPLE_MODE)
        {
            QString validName = FdmParameterProfileManager::Instance().getValidName(nameInProfile);
            FdmRightParameterService::instance()->onSaveAsBtnClicked(validName);
        }
		else
		{
			
			FdmRightParameterService::instance()->applySimpleModeData();
		}

        return true;
    }

	void FdmSettingPlugin::recMsgfromManager(PluginMessageData metaData) {
		//do get_custom_setting request from fdm_slicer
		if (metaData.from == AkConst::Plugin::FDM_SLICER
			&& metaData.msg == AkConst::Msg::GET_CUSTOM_SETTING) {
			QString setting = FdmQmlSourceTree().instance().getFdmQmlTreeApi_Right().getCustomSetting();
            
            AkConst::EGlobalSupportState gSptState = (AkConst::EGlobalSupportState)FdmRightParameterService::instance()->getGlobalSupportState();
            
            if (gSptState == AkConst::EGlobalSupportState::Unactived)
            {
                
                setting += getSupportConfig();
            }

			
			PluginMessageData response = metaData;
			response.from = metaData.dest;
			response.dest = metaData.from;
			response.map.insert(AkConst::Param::CUSTOM_SETTING, setting);
			emit sendMsg2Manager(response);
		}

		
		if (metaData.msg == AkConst::Msg::SLICE_RESET
			|| metaData.msg == AkConst::Msg::SLICE_FAILED) {
			emit savePanelShow();
		}

        
		if (metaData.msg == AkConst::Msg::SLICE_SUCCESS
			|| metaData.msg == AkConst::Msg::SLICE_START) {
            //emit savePanelHide();
		}

        
        if (metaData.msg == AkConst::Msg::MODEL_SUPPORT_NUMBER_CHANGED) {
            int sptNum = metaData.map.value(AkConst::Param::SUPPORT_MESH_NUMBER, 0).toInt();
            FdmRightParameterService::instance()->setSupportMeshCount(sptNum);
            emit FdmRightParameterService::instance()->globalSupportStateChanged();
            emit FdmRightParameterService::instance()->globalSupportTextCodeChanged();
        }

        
        if (metaData.msg == AkConst::Msg::GLOBAL_SUPPORT_DISABLED) {
            bool GlobalSptForbidden = metaData.map.value(AkConst::Param::GLOBAL_SUPPORT_DISABLED_STATUS, true).toBool();
            FdmRightParameterService::instance()->setGlobalSupportForbidden(GlobalSptForbidden);
            emit FdmRightParameterService::instance()->globalSupportStateChanged();
            emit FdmRightParameterService::instance()->globalSupportTextCodeChanged();
        }

        
        if (metaData.msg == AkConst::Msg::PREVIEW_GCODE_BY_DRAW) {
            QString gcodeFile = metaData.map.value(AkConst::Param::GCODE_FILE, "").toString();
            if (gcodeFile == "")
            {
                return;
            }
            
            auto realTimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
            bool extractParamFromGCode =  realTimeProfile->getSetting("",AkConst::SettingKey::EXTRACT_PARAM_FROM_GCODE).toBool();
            if (extractParamFromGCode)
            {
                auto extractFile = extractRealTimeParamFile(gcodeFile);
                
                loadRealTimeProfile(extractFile);
            }
        }

        
        if(AkConst::Plugin::FDM_SEND2ALL == metaData.dest){
            
            if(AkConst::Msg::AIMODE_CHANGED == metaData.msg){
                if(metaData.map.contains(AkConst::Param::AIMODE_STATE)){
                    qDebug() << "AkConst::Msg::AIMODE_STATE" << metaData.map;
                    QTimer::singleShot(10, [metaData](){
                        bool aiCheck = metaData.map[AkConst::Param::AIMODE_STATE].toBool();
                        FdmQmlSourceTree::instance().setAiCheck(aiCheck);
                    });
                }
            }
            
            if(AkConst::Msg::LANGUAGE_CHANGED == metaData.msg){
                if(metaData.map.contains(AkConst::Param::LANGUAGE_STATE)){
                    qDebug() << "AkConst::Msg::LANGUAGE_CHANGED" << metaData.map;

                    
                    
                    
                    bool refreshLanguage = false;
                    do{
                        QVariantMap & refreshLanguageArg = FdmQmlSourceTree::instance().refreshLanguageArg;
                        
                        if(refreshLanguageArg.contains("Realtime")){
                            refreshLanguage = true;
                            break;
                        }
                        else if(!refreshLanguageArg.contains("OnlyAtAtartup")){
                            refreshLanguageArg.remove("Realtime");
                            refreshLanguageArg.insert("OnlyAtAtartup", true);
                            refreshLanguage = true;
                            break;
                        }
                        //else{refreshLanguage = false; break;}
                    }while(false);

                    if(refreshLanguage){
                        int languageIndex = metaData.map[AkConst::Param::LANGUAGE_STATE].toInt();
                        FdmQmlSourceTree::instance().setLanguage(languageIndex);
                    }
                }
            }

            if (AkConst::Msg::MAIN_WINDOW_INIT_FINISHED == metaData.msg) {
                FdmRightParameterService::instance()->doMainWindowInitFinished();
            }
        }
    }

    
    QString FdmSettingPlugin::extractRealTimeParamFile(QString gcodeFile)
    {
        
        
        
        QString tempPath =  QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("temp");
        
        QSet<QString> excludeSet;
        IoApi::clearFoldersUnderPath(10*AkConst::Time::MINUTE_SECOND,tempPath,excludeSet);
        
        QDir destDir = IoApi::createTimestampFolderUnderPath(tempPath);
        QString drawGcodeName = "draw.gcode";
        QString destFile = destDir.absoluteFilePath(drawGcodeName);
        bool copyResult = QFile::copy(gcodeFile, destFile);
        if (!copyResult)
        {
            TDebug("copy file " + gcodeFile + "failed");
            return "";
        }
        
        QString paramContent =  IoApi::subContent(destFile,";paramBegin",";paramEnd",true);
        
        
        paramContent = IoApi::mergeStr(paramContent, ";");
        QString cpParamFile = destDir.absoluteFilePath("realtimeContent.ini");
        IoApi::write(cpParamFile, QByteArray::fromBase64(paramContent.toUtf8()));

        return cpParamFile;
    }



    QString FdmSettingPlugin::getSupportConfig()
    {
        QString profileStorePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/parameter/support");
        return getConfig(profileStorePath);
    }

    QString FdmSettingPlugin::getConfig(QString configPath)
    {
       QStringList settingList;
       QMap<QString, QString> paramMap;
       
       QStringList filter;
       filter << "*.ini";
       auto fileInfoList = IoApi::getFiles(configPath,filter,IoApi::SearchOption::AllDirectories);
       for (int i = 0; i < fileInfoList.size(); i++)
       {
           QString configContent = IoApi::readAll(fileInfoList[i].absoluteFilePath());
           QString key = "[values]";
           int valueStartIdx = configContent.indexOf(key);
           if (valueStartIdx<0)
           {
               continue;
           }
           QString settingContent = configContent.mid(valueStartIdx + key.length());


           QRegularExpression exp("^(.*?)=(.*?)$", QRegularExpression::MultilineOption|QRegularExpression::InvertedGreedinessOption);
           auto kvMatches = exp.globalMatch(settingContent);

           //var kvMatchs = Regex.Matches(settingContent, regStr, RegexOptions.Multiline);
           while (kvMatches.hasNext())
           {
               auto kvMatch = kvMatches.next();
               if (kvMatch.hasMatch())
               {
                   QString name = kvMatch.captured(1).trimmed();
                   QString value = kvMatch.captured(2).trimmed();
                   paramMap[name] = value;
               }
           }
       }

       //infill_pattern = ='lines' if infill_sparse_density > 25 else 'grid'
       if (paramMap.contains("infill_sparse_density"))
       {
           float density = paramMap["infill_sparse_density"].toFloat();
           if (density>25)
           {
               paramMap["infill_pattern"] = "lines";
           }
           else
           {
               paramMap["infill_pattern"] = "grid";
           }
       }

       for(auto it = paramMap.constBegin();it!=paramMap.constEnd();it++)
       {
           settingList.append(QString(" -s %1=%2 ").arg(it.key()).arg(it.value()));
       }

       return settingList.join(" ");
    }


	//
	void FdmSettingPlugin::sendSliceResetMsg()
	{
		PluginMessageData data;
		data.from = AkConst::Plugin::FDM_SETTING;
		data.dest = AkConst::Plugin::FDM_SLICER;
		data.msg = AkConst::Msg::PARAMETER_CHANGED;
		emit sendMsg2Manager(data);
	}

    
    void FdmSettingPlugin::getGlobalSupportSetting()
	{
		PluginMessageData data;
		data.from = AkConst::Plugin::FDM_SETTING;
        data.dest = AkConst::Plugin::FDM_EDIT_MODEL;
        data.msg = AkConst::Msg::GET_GLOBAL_SUPPORT_DISABLED_STATUS;
		emit sendMsg2Manager(data);
	}

    void FdmSettingPlugin::initPlugin(ControlInterface* , RichParameterList* )
	{
		{//FdmQmlEngine
			FdmQmlEngine::instance();
			FdmQml_registerType();
		}

		{
            FdmQmlSourceTree::instance().init();
		}

        
        //FdmParameterProfileService::instance()->exportIniFromTree("c:/workspace/log/tree.ini");


		
		//qDebug()<< "enter initGui" ;
        FdmMachineProfileManager::Instance().init();
        FdmMaterialProfileManager::Instance().init();
        FdmParameterProfileManager::Instance().init();

        
        FdmPreferenceDialogService::instance()->init();
        FdmRightParameterService::instance()->init();
		FdmParameterProfileService::instance()->init();

		//anyNodeValueChange
        //connect(&(FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference()), &FdmQmlTreeApi::anyNodeValueChange, FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::onSourceTreeApiNodeValueChanged);
		//connect(&(FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right()), &FdmQmlTreeApi::anyNodeValueChange, FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::onSourceTreeApiNodeValueChanged);

		//qDebug()<< "exit initGui" ;
	}


	void FdmSettingPlugin::initGui(ControlInterface* controlmanager, RichParameterList* globalParameterList) {

		
        m_controlmanager = controlmanager;

		fdmParamSettingsWidget = new FdmParamSettingsWidget();
        connect(fdmParamSettingsWidget,&FdmParamSettingsWidget::openPreferenceIndex,this,&FdmSettingPlugin::slotOpenPreferenceIndex);
		MachineWidget* machine = new MachineWidget();
		controlmanager->addPageToPreferences(machine, 1);
		MaterialWidget* material = new MaterialWidget();
		controlmanager->addPageToPreferences(material, 2);
		ParametersWidget* parameter = new ParametersWidget();
		controlmanager->addPageToPreferences(parameter, 3);

       // fdmParamSettingsWidget->setCurrent(controlmanager);
        
        SavePanel* panel = new SavePanel();
        
        controlmanager->addWidgetToRightSettings(AkConst::EFdmRightWidgetIndex::SavePanel, panel);
        connect(panel,&SavePanel::save, FdmRightParameterService::instance(),&FdmRightParameterService::onSaveBtnClicked);
        connect(panel,&SavePanel::saveAs, FdmRightParameterService::instance(),&FdmRightParameterService::onSaveAsBtnClicked);
        
        connect(FdmRightParameterService::instance(),&FdmRightParameterService::currentParameterSwitched, panel,&SavePanel::doParameterChanged);
        
        panel->init(FdmRightParameterService::instance()->getCurrentSelectedName());

		connect(this, &FdmSettingPlugin::savePanelShow, panel, &SavePanel::doPanelShow);
		connect(this, &FdmSettingPlugin::savePanelHide, panel, &SavePanel::doPanelHide);


        controlmanager->addWidgetToRightSettings(AkConst::EFdmRightWidgetIndex::SettingPanel, fdmParamSettingsWidget);
		//    controlmanager->addActionToToolBar(btnLoadIni);
		//    controlmanager->addActionToToolBar(btnSaveIni);

			
		connect(controlmanager, &ControlInterface::openPreferencesDialog, FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::onPreferenceWindowOpen);
		connect(controlmanager, &ControlInterface::switchPreferencesPage, FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::onPreferencesPageSwitch);

		connect(parameter, &PageWidget::saveButtonClicked, FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::onSaveBtnClicked);
		connect(parameter, &PageWidget::closeButtonClicked, FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::onCloseBtnClicked);

		
		FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
		connect(&rightTree, &FdmQmlTreeApi::anyNodeValueChange, this, &FdmSettingPlugin::sendSliceResetMsg);
		connect(FdmRightParameterService::instance(), &FdmRightParameterService::currentMachineSwitched, this, &FdmSettingPlugin::sendSliceResetMsg);
		connect(FdmRightParameterService::instance(), &FdmRightParameterService::currentMaterialSwitched, this, &FdmSettingPlugin::sendSliceResetMsg);
		connect(FdmRightParameterService::instance(), &FdmRightParameterService::currentParameterSwitched, this, &FdmSettingPlugin::sendSliceResetMsg);

		auto getLoadIniFilePath = []() -> QString
		{
			QString fileName = QFileDialog::getOpenFileName(
				nullptr,
                QString::fromLocal8Bit("Import ini config file"),
				QString(),
				QString("Ini File(*.ini)"));
            qDebug() << QString::fromLocal8Bit("Import ini config file \t") << fileName;
			return fileName;
		};

		auto getSaveIniFilePath = []() -> QString
		{
			QString fileName = QFileDialog::getSaveFileName(
				nullptr,
                QString::fromLocal8Bit("Export ini config file"),
				QString(),
				QString("Ini File(*.ini)"));
            qDebug() << QString::fromLocal8Bit("Export ini config file \t") << fileName;
			return fileName;
		};


		//    QPushButton *btnLoadIni = dockFdmParameter->findChild<QPushButton *>("btnLoadIni");
		//    QPushButton *btnSaveIni = dockFdmParameter->findChild<QPushButton *>("btnSaveIni");

			
		//    QObject::connect(btnLoadIni, &QAction::triggered, [&](){
		//        QString fileName = getLoadIniFilePath();
		//        if(QFileInfo::exists(fileName)){
		//            //FdmQmlSourceTree().instance().resetCustomIni(fileName);
		//            FdmParameterProfileService::instance()->onImportBtnClicked(fileName);
		//        }
		//    });

		//    QObject::connect(btnSaveIni, &QAction::triggered, [&](){
		//        QString fileName = getSaveIniFilePath();
		//        //QString fileName = "C:/Users/Administrator/Desktop/78.ini";
		//        //if(QFileInfo::exists(fileName))
		//        //QFile::remove(fileName);
		//        //FdmQmlSourceTree().instance().saveCustomIni(fileName);
		//        FdmParameterProfileService::instance()->onExportBtnClicked(fileName);
		//    });

			//btnLoadIni();

        this->globalParameterList = globalParameterList;
        if (this->globalParameterList) {
            _initParam();
        }

        
        getGlobalSupportSetting();
	}

	void FdmSettingPlugin::_initParam()
	{
		auto findNode = [](const QString& nodeName) -> FdmParamNode*
		{
			if (FdmQmlSourceTree().instance().qmlSourceTree_Right()) {
				auto node = FdmQmlSourceTree().instance().qmlSourceTree_Right()->findChildNode(nodeName);
				return node;
			}
			return nullptr;
		};

		{   
			FdmParamNode* machineSizeX = findNode("machine_width");
			FdmParamNode* machineSizeY = findNode("machine_depth");
			FdmParamNode* machineSizeZ = findNode("machine_height");

			auto getMachineSize = [machineSizeX, machineSizeY, machineSizeZ]() -> Point3m
			{
				Point3m size(machineSizeX->getFdmValue().toDouble(), machineSizeY->getFdmValue().toDouble(), machineSizeZ->getFdmValue().toDouble());
				return size;
			};

			auto updateMachineSize = [this, getMachineSize]() {
				Point3m size = getMachineSize();
				Point3fValue value(size);
				const RichParameter& param = globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE);
                param.qobj->setValue(value);
			};

            
            RichParameter& param = globalParameterList->addParam(RichPoint3f(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE, getMachineSize()));
			QObject::connect(machineSizeX, &FdmParamNode::fdmValueChange, updateMachineSize);
			QObject::connect(machineSizeY, &FdmParamNode::fdmValueChange, updateMachineSize);
			QObject::connect(machineSizeZ, &FdmParamNode::fdmValueChange, updateMachineSize);

			if (0) 
			{
				const RichParameter& param = globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE);
				
				QObject::connect(param.qobj.data(), &RichParameterQObject::valueChange, [](const Value& value) {
					Point3m size = value.getPoint3f();
					qDebug() << size.X() << size.Y() << size.Z() << AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE << "&RichParameterQObject::valueChange";
				});

				for (int i = 5; i < 30; i += 5) {  
					QTimer::singleShot(i * 1000, [i, machineSizeX, machineSizeY, machineSizeZ, getMachineSize]() {
						Point3m size = getMachineSize();
						if (i % 3 == 0)
							machineSizeX->nodeValueChange_fromCpp(size.X() + 20);
						if (i % 3 == 1)
							machineSizeY->nodeValueChange_fromCpp(size.Y() + 30);
						if (i % 3 == 2)
							machineSizeZ->nodeValueChange_fromCpp(size.Z() + 40);
					});
				}
			}
		}

		{  
			FdmParamNode* support_enable = findNode("support_enable");
			bool enable = support_enable->getFdmValue().toBool();
			globalParameterList->addParam(RichBool(AkConst::GlobalParameterKeys::ANKER_SUPPORT_ENABLE, enable));
			QObject::connect(support_enable, &FdmParamNode::fdmValueChange, [this](QVariant var) {
				bool enable = var.toBool();
				BoolValue value(enable);
				const RichParameter& param = globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_SUPPORT_ENABLE);
				param.qobj->setValue(value);
			});


			FdmParamNode* support_angle = findNode("support_angle");
			float angle = support_angle->getFdmValue().toFloat();
			globalParameterList->addParam(RichFloat(AkConst::GlobalParameterKeys::ANKER_SUPPORT_ANGLE, angle));
			QObject::connect(support_angle, &FdmParamNode::fdmValueChange, [this](QVariant var) {
				float angle = var.toFloat();
				FloatValue value(angle);
				const RichParameter& param = globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_SUPPORT_ANGLE);
				param.qobj->setValue(value);
			});

			if (0) 
			{
				const RichParameter& param = globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_SUPPORT_ENABLE);
				
				QObject::connect(param.qobj.data(), &RichParameterQObject::valueChange, [](const Value& value) {
					bool enable = value.getBool();
					qDebug() << enable << AkConst::GlobalParameterKeys::ANKER_SUPPORT_ENABLE << "&RichParameterQObject::valueChange";
				});

				for (int i = 4; i < 30; i += 3) {  
					QTimer::singleShot(i * 1000, [i, support_enable, support_angle]() {
						bool enable = support_enable->getFdmValue().toBool();
						support_enable->nodeValueChange_fromCpp(!enable);
					});
				}
			}

			if (0) 
			{
				const RichParameter& param = globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_SUPPORT_ANGLE);
				
				QObject::connect(param.qobj.data(), &RichParameterQObject::valueChange, [](const Value& value) {
					float angle = value.getFloat();
					qDebug() << angle << AkConst::GlobalParameterKeys::ANKER_SUPPORT_ANGLE << "&RichParameterQObject::valueChange";
				});

				for (int i = 4; i < 30; i += 3) {  
					QTimer::singleShot(i * 1000, [i, support_enable, support_angle]() {
						float angle = support_angle->getFdmValue().toFloat();
						support_angle->nodeValueChange_fromCpp(angle + 3.6);
					});
				}
			}
		}

		{   

		}
	}
}
