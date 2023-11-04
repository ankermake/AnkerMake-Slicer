#include "fdmsettingvisibilityconfig.h"


namespace fdmsettings {

	FdmSettingVisibilityConfig::FdmSettingVisibilityConfig()
	{

	}

	QList<FdmParamNode*> FdmSettingVisibilityConfig::loadList(FdmParamNode* root, const QString& filepath)
	{
		lineList.clear();
		nodeList.clear();
		lineList = readConfigFile(filepath);
		if (lineList.isEmpty()) {
			return QList<FdmParamNode*>();
		}
		insertlevel(root, 0);
		return  nodeList;
	}

	QStringList FdmSettingVisibilityConfig::readConfigFile(const QString& filepath)
	{
		QFile file(filepath);
		if (!file.open(QIODevice::ReadOnly)) {
			return QStringList();
		}
		QStringList configList;
		while (!file.atEnd()) {
            
            QString line = remove(file.readLine());
            if(!line.isEmpty())
                configList.append(line);
		}
		file.close();
		return configList;
	}


	void FdmSettingVisibilityConfig::insertlevel(FdmParamNode* node, int level)
	{
		auto isEnable = [node]() -> bool
		{
			QVariant qmlEnable = node->getProperty(fdmPrinterJsonEnabled);
			return qmlEnable.isValid() ? qmlEnable.toBool() : true;
		};

		bool visibility = lineList.contains(node->getNodeName());
		//visibility = visibility && isEnable();
        //visibility = true;

		QList<FdmParamNode*> child = QList<FdmParamNode*>();
		if (visibility) {
			QString category = node->getProperty(fdmPrinterJsonType).toString();
			if (category == "category") {
				level = 0;

				//             child = node->allChildrenNode();
				//             if(child.isEmpty()) {
				//                 qDebug() << " chile ==" << node->getProperty(fdmPrinterJsonLabel).toString();
				//                 return;
				//             }
			}
			insertDatas(node, level);
		}

		child = node->allChildrenNode();
		if (child.isEmpty()) {
			return;
		}
		for (auto i = child.begin(); i != child.end(); ++i) {
			insertlevel(*i, level + 1);
		}
	}


	void FdmSettingVisibilityConfig::insertIcon(FdmParamNode* node)
	{
		QString category = node->getProperty(fdmPrinterJsonType).toString();
		if (category != "category") {
			return;
		}

		QString icon = node->getProperty(fdmPrinterJsonIcon).toString();
		if (icon.isEmpty()) {
			return;
		}
		QString iconPath;

		if (icon == "PrintQuality") {
			iconPath = "qrc:/images/images/fdm_quality_icon_u.png";
		}
		else if (icon == "PrintShell") {
			iconPath = "qrc:/images/images/fdm_walls_icon_u.png";
		}
		else if (icon == "PrintTopBottom") {
			iconPath = "qrc:/images/images/fdm_top_icon_u.png";
		}
		else if (icon == "Infill1") {
			iconPath = "qrc:/images/images/fdm_infill_icon_u.png";
		}
		else if (icon == "Spool") {
			iconPath = "qrc:/images/images/fdm_material_icon_u.png";
		}
		else if (icon == "Fan") {
			iconPath = "qrc:/images/images/fdm_cooling_icon_u.png";
		}
		else if (icon == "Support") {
			iconPath = "qrc:/images/images/fdm_support_icon_u.png";
		}
		else if (icon == "Adhesion") {
			iconPath = "qrc:/images/images/fdm_build_icon_u.png";
		}
		else if (icon == "DualExtrusion") {
			iconPath = "qrc:/images/images/fdm_dual_icon_u.png";
		}
		else if (icon == "Bandage") {
			iconPath = "qrc:/images/images/fdm_mesh_icon_u.png";
		}
		else if (icon == "BlackMagic") {
			iconPath = "qrc:/images/images/fdm_special_icon_u.png";
		}
		else if (icon == "SpeedOMeter") {
			iconPath = "qrc:/images/images/fdm_speed_icon_u.png";
		}
		else if (icon == "Experiment") {
			iconPath = "qrc:/images/images/fdm_experimental_icon_u.png";
		}
		else if (icon == "PrintTravel") {
			iconPath = "qrc:/images/images/fdm_travel_icon_u.png";
		}
        
        else if (icon == "AnkerFdmSettings") {
            iconPath = "qrc:/images/images/fdm_special_icon_u.png";
        }
        else if (icon == "MachinePrinter") {
            iconPath = "qrc:/images/images/fdm_special_icon_u.png";
        }

		if (iconPath.isEmpty()) {
			return;
		}

		node->setProperty(fdmNodeIcon, iconPath);
	}

	void FdmSettingVisibilityConfig::insertDatas(FdmParamNode* node, int level)
	{
		node->setProperty(fdmNodeLevel, level);
		insertIcon(node);
		QString category = node->getProperty(fdmPrinterJsonType).toString();
		if (category != "category") {
			node->insert(fdmNodeIsVisibility, false);

		}
		else {
			node->insert(fdmNodeExpand, false);
			node->insert(fdmNodeIsVisibility, true);

		}
		node->insert(fdmNodeValueRangeFlag, 0);
		if (node->getProperty(fdmPrinterJsonUnit).isNull()) {
			node->insert(fdmPrinterJsonUnit, QString());
		}
		nodeList.append(node);
	}

	void FdmSettingVisibilityConfig::print()
	{
		for (auto i = nodeList.begin(); i != nodeList.end(); ++i) {
			(*i)->dumpParam();
		}
	}

	QString FdmSettingVisibilityConfig::remove(QString str)
	{
        if(str.startsWith(";")){return QString();} //  add @2022-02-27 by CL
		QString temp;
		temp = str.trimmed();
		temp.remove("[");
		temp.remove("]");
		return temp;
	}



}
