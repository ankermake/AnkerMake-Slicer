/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005-2021                                           \/)\/    *
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

#include "plugin_manager.h"

#include <QObject>
#include <QDir>
#include <QApplication>

#include <vcg/complex/algorithms/create/platonic.h>

#include "../mlexception.h"
#include "../globals.h"
#include "GeoAndShow/CHScene.h"
#include "utilities/tlogger.h"

static QStringList fileNamePluginDLLs() {
	QStringList l;
#if defined(Q_OS_WIN)
	l.push_back("*.dll");
	return l;
#elif defined(Q_OS_MAC)
	l.push_back("*.dylib");
	l.push_back("*.so");
	return l;
#else //linux
	l.push_back("*.so");
	return l;
#endif
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
	for (auto& plugin : allPluginLoaders){
		plugin->unload();
		delete plugin;
	}
}

/**
 * @brief Checks if the given file is a valid MeshLab plugin.
 * It does not add the plugin to the plugin manger.
 * 
 * Note: this function is called automatically before loading a plugin.
 * 
 * Throws a MLException if the file is not a valid MeshLab plugin.
 */
MeshLabPluginType PluginManager::checkPlugin(const QString& filename)
{
	QFileInfo fin(filename);
	if (!fin.exists()){
		throw MLException(filename + " does not exists.");
	}
	QPluginLoader loader(fin.absoluteFilePath());
	QObject *plugin = loader.instance();
	if (!plugin) {
		throw MLException(fin.fileName() + " does not seem to be a Qt Plugin.\n\n" + loader.errorString());
	}

	MeshLabPlugin* ifp = dynamic_cast<MeshLabPlugin *>(plugin);
	if (!ifp){
		throw MLException(fin.fileName() + " is not a MeshLab plugin.");
	}

	//check floating point precision of the plugin
	if (ifp->getMLVersion().second != MeshLabScalarTest<Scalarm>::doublePrecision()) {
		throw MLException(fin.fileName() + " has different floating point precision from the running MeshLab version.");
	}

	std::string mlVersionPlug = ifp->getMLVersion().first;
	/** Rules for the future: **/
	//  check version of the plugin
	//   - needs to be the same major version and
	//     <= minor version wrt MeshLab version
	//   example: 2021.01 plugin can run on 2021.03 MeshLab
	//          2021.03 plugin cannot run on 2021.01 MeshLab
	//          2021.12 plugin cannot run on 2022.01 MeshLab
//	std::string majorVersionPlug = mlVersionPlug.substr(0, 4); //4 is the position of '.' in meshlab version
//	std::string majorVersionML = meshlab::meshlabVersion().substr(0, 4);
//	if (majorVersionML != majorVersionPlug){
//		throw MLException(fin.fileName() + " has different major version from the running MeshLab version.");
//	}
//	std::string minorVersionPlug = mlVersionPlug.substr(5, mlVersionPlug.size());
//	std::string minorVersionML = meshlab::meshlabVersion().substr(5, meshlab::meshlabVersion().size());
//	if (std::stoi(minorVersionPlug) > std::stoi(minorVersionML)){
//		throw MLException(fin.fileName() + " has greater version from the running MeshLab version. Please update MeshLab to use it.");
//	}

	/** Rules for now: plugin needs to have same version of meshlab **/
	if (mlVersionPlug.substr(0, 6) != meshlab::meshlabVersion().substr(0, 6)){
		throw MLException(fin.fileName() + " has different version from the running MeshLab version.");
	}
	
	MeshLabPluginType type(ifp);
	
	if (!type.isValid()){
		throw MLException(fin.fileName() + " has none of the known plugin types known by this MeshLab version.");
	}
	
	//ToDo: proper checks also for other plugin types...
	if (type.isFilterPlugin()){
		checkFilterPlugin(qobject_cast<FilterPlugin *>(plugin));
	}

	loader.unload();
	return type;
}

/**
 * @brief Loads the plugins contained in the default meshlab plugin directory.
 * 
 * If at least one plugin fails to be loaded, a MLException is thrown.
 * In any case, all the other valid plugins contained in the directory are loaded.
 */
void PluginManager::loadPlugins()
{
	// without adding the correct library path in the mac the loading of jpg (done via qt plugins) fails
	// ToDo: get rid of any qApp here
    if(meshlab::defaultPluginPath().isEmpty()){ return; }
	qApp->addLibraryPath(meshlab::defaultPluginPath());
	loadPlugins(QDir(meshlab::defaultPluginPath()));
}

/**
 * @brief Loads the plugins contained in the given directory.
 * 
 * If at least one plugin fails to be loaded, a MLException is thrown.
 * In any case, all the other valid plugins contained in the directory are loaded.
 */
void PluginManager::loadPlugins(QDir pluginsDirectory)
{
	if (pluginsDirectory.exists()){
		QStringList nameFiltersPlugins = fileNamePluginDLLs();
		
		//only the file with extension pluginfilters will be listed by function entryList()
		pluginsDirectory.setNameFilters(nameFiltersPlugins);
		
		//qDebug("Current Plugins Dir is: %s ", qUtf8Printable(pluginsDirectory.absolutePath()));
		std::list<std::pair<QString, QString>> errors;
		for(QString fileName : pluginsDirectory.entryList(QDir::Files)) {
			try {
				loadPlugin(pluginsDirectory.absoluteFilePath(fileName));
			}
			catch(const MLException& e){
				errors.push_back(std::make_pair(fileName, e.what()));
			}
		}
		if (errors.size() > 0){
            QString singleError = "Unable to load the following plugins:";
			for (const auto& p : errors){
                singleError += "\t" + p.first + ": " + p.second ;
			}
			throw MLException(singleError);
		}
	}
}

/**
 * @brief Loads the plugin specified in the given file and adds the plugin into the
 * PluginManager.
 * 
 * Note: better to give the absolute path of the plugin file.
 * 
 * Throws a MLException if the load of the plugin fails.
 */
MeshLabPlugin* PluginManager::loadPlugin(const QString& fileName)
{
	QFileInfo fin(fileName);
	if (pluginFiles.find(fin.absoluteFilePath()) != pluginFiles.end())
		throw MLException(fin.fileName() + " has been already loaded.");

    //checkPlugin(fileName);
    //auto x = fin.absoluteFilePath();
    AkUtil::TInfo(QString("LOADING PLUGIN START: ") + fin.absoluteFilePath());

	//load the plugin depending on the type (can be more than one type!)
	QPluginLoader* loader = new QPluginLoader(fin.absoluteFilePath());
	QObject *plugin = loader->instance();
	MeshLabPlugin* ifp = dynamic_cast<MeshLabPlugin *>(plugin);
	MeshLabPluginType type(ifp);
	
    
    if(!plugin || !ifp){
        loader->deleteLater();
        AkUtil::TError(QString("LOADING PLUGIN ERROR: ")  + fin.absoluteFilePath());
        throw MLException(QString("\n") + fin.absoluteFilePath());
    }
    qDebug() << __FUNCTION__ << __LINE__ << ifp->pluginName() << fin.absoluteFilePath();
    AkUtil::TInfo(QString("LOADING PLUGIN SUCCESS: ") + ifp->pluginName() + "--" + fin.absoluteFilePath());
	if (type.isDecoratePlugin()){
		decoratePlugins.pushDecoratePlugin(qobject_cast<DecoratePlugin *>(plugin));
	}
	if (type.isEditPlugin()){
		editPlugins.pushEditPlugin(qobject_cast<EditPlugin *>(plugin));
	}
	if (type.isFilterPlugin()){
		filterPlugins.pushFilterPlugin(qobject_cast<FilterPlugin *>(plugin));
	}
	if (type.isIOPlugin()){
		ioPlugins.pushIOPlugin(qobject_cast<IOPlugin *>(plugin));
	}
	if (type.isRenderPlugin()){
		renderPlugins.pushRenderPlugin(qobject_cast<RenderPlugin *>(plugin));
	}

    if(plugin && ifp){  //  add @2021-10-21 by CL
        if(auto p = qobject_cast<AnkerEditPlugin *>(plugin)){
            ankerEditPlugins.append(p);
        }
        else if(auto p = qobject_cast<AnkerPlugin *>(plugin)){
            ankerPlugins.append(p);
        }

        QObject::connect(loader->instance(), SIGNAL(sendMsg2Manager(PluginMessageData)),
                         this, SLOT(recMsgfromPlugin(PluginMessageData)) );
    }

	//set the QFileInfo to the plugin, and add it to the continer
	//of all plugins
	ifp->plugFileInfo = fin;
	allPlugins.push_back(ifp);
	allPluginLoaders.push_back(loader);
	pluginFiles.insert(fin.absoluteFilePath());
	return ifp;
}

void PluginManager::unloadPlugin(MeshLabPlugin* ifp)
{
	auto it = std::find(allPlugins.begin(), allPlugins.end(), ifp);
	if (it != allPlugins.end()){
		unsigned int index = it - allPlugins.begin();
		MeshLabPluginType type(ifp);
		if (type.isDecoratePlugin()){
			decoratePlugins.eraseDecoratePlugin(dynamic_cast<DecoratePlugin *>(ifp));
		}
		if (type.isEditPlugin()){
			editPlugins.eraseEditPlugin(dynamic_cast<EditPlugin *>(ifp));
		}
		if (type.isFilterPlugin()){
			filterPlugins.eraseFilterPlugin(dynamic_cast<FilterPlugin *>(ifp));
		}
		if (type.isIOPlugin()){
			ioPlugins.eraseIOPlugin(dynamic_cast<IOPlugin *>(ifp));
		}
		if (type.isRenderPlugin()){
			renderPlugins.eraseRenderPlugin(dynamic_cast<RenderPlugin *>(ifp));
		}
		QPluginLoader* l = allPluginLoaders[index];
		allPluginLoaders.erase(allPluginLoaders.begin() + index);
		allPlugins.erase(it);
		l->unload();
		delete l;
	}
}

void PluginManager::enablePlugin(MeshLabPlugin* ifp)
{
	auto it = std::find(allPlugins.begin(), allPlugins.end(), ifp);
	if (it  != allPlugins.end() && !ifp->isEnabled()){
		ifp->enable();
		//ToDo other checks...
	}
}

void PluginManager::disablePlugin(MeshLabPlugin* ifp)
{
	auto it = std::find(allPlugins.begin(), allPlugins.end(), ifp);
	if (it  != allPlugins.end() && ifp->isEnabled()){
		ifp->disable();
		//ToDo other checks...
	}
}

unsigned int PluginManager::size() const
{
	return allPlugins.size();
}

int PluginManager::numberIOPlugins() const
{
	return ioPlugins.size();
}

// Search among all the decorator plugins the one that contains a decoration with the given name
DecoratePlugin *PluginManager::getDecoratePlugin(const QString& name)
{
	return decoratePlugins.decoratePlugin(name);
}

QAction* PluginManager::filterAction(const QString& name)
{
	return filterPlugins.filterAction(name);
}

IOPlugin* PluginManager::inputMeshPlugin(const QString& inputFormat) const
{
	return ioPlugins.inputMeshPlugin(inputFormat);
}

IOPlugin* PluginManager::outputMeshPlugin(const QString& outputFormat) const
{
	return ioPlugins.outputMeshPlugin(outputFormat);
}

IOPlugin* PluginManager::inputImagePlugin(const QString inputFormat) const
{
	return ioPlugins.inputImagePlugin(inputFormat);
}

IOPlugin* PluginManager::outputImagePlugin(const QString& outputFormat) const
{
	return ioPlugins.outputImagePlugin(outputFormat);
}

IOPlugin* PluginManager::inputProjectPlugin(const QString& inputFormat) const
{
	return ioPlugins.inputProjectPlugin(inputFormat);
}

IOPlugin* PluginManager::outputProjectPlugin(const QString& outputFormat) const
{
	return ioPlugins.outputProjectPlugin(outputFormat);
}

bool PluginManager::isInputMeshFormatSupported(const QString inputFormat) const
{
	return ioPlugins.isInputMeshFormatSupported(inputFormat);
}

bool PluginManager::isOutputMeshFormatSupported(const QString outputFormat) const
{
	return ioPlugins.isOutputMeshFormatSupported(outputFormat);
}

bool PluginManager::isInputImageFormatSupported(const QString inputFormat) const
{
	return ioPlugins.isInputImageFormatSupported(inputFormat);
}

bool PluginManager::isOutputImageFormatSupported(const QString outputFormat) const
{
	return ioPlugins.isOutputImageFormatSupported(outputFormat);
}

bool PluginManager::isInputProjectFormatSupported(const QString inputFormat) const
{
	return ioPlugins.isInputProjectFormatSupported(inputFormat);
}

bool PluginManager::isOutputProjectFormatSupported(const QString outputFormat) const
{
	return ioPlugins.isOutputProjectFormatSupported(outputFormat);
}

QStringList PluginManager::inputMeshFormatList() const
{
	return ioPlugins.inputMeshFormatList();
}

QStringList PluginManager::outputMeshFormatList() const
{
	return ioPlugins.outputMeshFormatList();
}

QStringList PluginManager::inputImageFormatList() const
{
	return ioPlugins.inputImageFormatList();
}

QStringList PluginManager::outputImageFormatList() const
{
	return ioPlugins.outputImageFormatList();
}

QStringList PluginManager::inputProjectFormatList() const
{
	return ioPlugins.inputProjectFormatList();
}

QStringList PluginManager::outputProjectFormatList() const
{
	return ioPlugins.outputProjectFormatList();
}

QStringList PluginManager::inputMeshFormatListDialog() const
{
	return inputFormatListDialog(ioPluginIterator());
}

QStringList PluginManager::outputMeshFormatListDialog() const
{
	return outputFormatListDialog(ioPluginIterator());
}

QStringList PluginManager::inputImageFormatListDialog() const
{
	return inputImageFormatListDialog(ioPluginIterator());
}

QStringList PluginManager::inputProjectFormatListDialog() const
{
	return inputProjectFormatListDialog(ioPluginIterator());
}

QStringList PluginManager::outputProjectFormatListDialog() const
{
	return outputProjectFormatListDialog(ioPluginIterator());
}

MeshLabPlugin* PluginManager::operator[](unsigned int i) const
{
	return allPlugins[i];
}

PluginManager::PluginRangeIterator PluginManager::pluginIterator(bool iterateAlsoDisabledPlugins) const
{
	return PluginRangeIterator(this, iterateAlsoDisabledPlugins);
}

FilterPluginContainer::FilterPluginRangeIterator PluginManager::filterPluginIterator(bool iterateAlsoDisabledPlugins) const
{
	return filterPlugins.filterPluginIterator(iterateAlsoDisabledPlugins);
}

IOPluginContainer::IOPluginRangeIterator PluginManager::ioPluginIterator(bool iterateAlsoDisabledPlugins) const
{
	return ioPlugins.ioPluginIterator(iterateAlsoDisabledPlugins);
}

RenderPluginContainer::RenderPluginRangeIterator PluginManager::renderPluginIterator(bool iterateAlsoDisabledPlugins) const
{
	return renderPlugins.renderPluginIterator(iterateAlsoDisabledPlugins);
}

DecoratePluginContainer::DecoratePluginRangeIterator PluginManager::decoratePluginIterator(bool iterateAlsoDisabledPlugins) const
{
	return decoratePlugins.decoratePluginIterator(iterateAlsoDisabledPlugins);
}

EditPluginContainer::EditPluginFactoryRangeIterator PluginManager::editPluginFactoryIterator(bool iterateAlsoDisabledPlugins) const
{
	return editPlugins.editPluginIterator(iterateAlsoDisabledPlugins);
}

void PluginManager::checkFilterPlugin(FilterPlugin* iFilter)
{
	for(QAction *filterAction : iFilter->actions()) {
		if(iFilter->getClass(filterAction)==FilterPlugin::Generic){
			throw MLException("Missing class for " + iFilter->pluginName() + " " + filterAction->text());
		}
		if(iFilter->getRequirements(filterAction) == int(MeshModel::MM_UNKNOWN)){
			throw MLException("Missing requirements for " + iFilter->pluginName() + " " + filterAction->text());
		}
		if(iFilter->getPreConditions(filterAction) == int(MeshModel::MM_UNKNOWN)){
			throw MLException("Missing preconditions for "+ iFilter->pluginName() + " " + filterAction->text());
		}
		if(iFilter->postCondition(filterAction) == int(MeshModel::MM_UNKNOWN)) {
			throw MLException("Missing postcondition for "+ iFilter->pluginName() + " " + filterAction->text());
		}
		if(iFilter->filterArity(filterAction) == FilterPlugin::UNKNOWN_ARITY) {
			throw MLException("Missing Arity for " + iFilter->pluginName() + " " + filterAction->text());
		}
	}
}

template<typename RangeIterator>
QStringList PluginManager::inputFormatListDialog(RangeIterator iterator)
{
    QString allKnownFormats = QString("All known formats (");
	QStringList inputFormatsDialogStringList;
	for (auto io : iterator){
		QString allKnownFormatsFilter;
		for (const FileFormat& currentFormat : io->importFormats()){
			QString currentFilterEntry = currentFormat.description + " (";
			for (QString currentExtension : currentFormat.extensions) {
				currentExtension = currentExtension.toLower();
                allKnownFormatsFilter.append(QString(" *."));
				allKnownFormatsFilter.append(currentExtension);
                currentFilterEntry.append(QString(" *."));
				currentFilterEntry.append(currentExtension);
			}
			currentFilterEntry.append(')');
			inputFormatsDialogStringList.append(currentFilterEntry);
		}
		allKnownFormats += allKnownFormatsFilter;
	}
	allKnownFormats.append(')');
	inputFormatsDialogStringList.push_front(allKnownFormats);
	return inputFormatsDialogStringList;
}

template<typename RangeIterator>
QStringList PluginManager::outputFormatListDialog(RangeIterator iterator)
{
	QStringList outputFormatsDialogStringList;
	for (auto io : iterator){
		for (const FileFormat& currentFormat : io->exportFormats()){
			QString currentFilterEntry = currentFormat.description + " (";
			for (QString currentExtension : currentFormat.extensions) {
				currentExtension = currentExtension.toLower();
                currentFilterEntry.append(QString(" *."));
				currentFilterEntry.append(currentExtension);
			}
			currentFilterEntry.append(')');
			outputFormatsDialogStringList.append(currentFilterEntry);
		}
	}
	return outputFormatsDialogStringList;
}

template<typename RangeIterator>
QStringList PluginManager::inputImageFormatListDialog(RangeIterator iterator)
{
    QString allKnownFormats = QString("All known formats (");
	QStringList inputImageFormatsDialogStringList;
	for (auto io : iterator){
		QString allKnownFormatsFilter;
		for (const FileFormat& currentFormat : io->importImageFormats()){
			QString currentFilterEntry = currentFormat.description + " (";
			for (QString currentExtension : currentFormat.extensions) {
				currentExtension = currentExtension.toLower();
                allKnownFormatsFilter.append(QString(" *."));
				allKnownFormatsFilter.append(currentExtension);
                currentFilterEntry.append(QString(" *."));
				currentFilterEntry.append(currentExtension);
			}
			currentFilterEntry.append(')');
			inputImageFormatsDialogStringList.append(currentFilterEntry);
		}
		allKnownFormats += allKnownFormatsFilter;
	}
	allKnownFormats.append(')');
	inputImageFormatsDialogStringList.push_front(allKnownFormats);
	return inputImageFormatsDialogStringList;
}

template<typename RangeIterator>
QStringList PluginManager::inputProjectFormatListDialog(RangeIterator iterator)
{
    QString allKnownFormats = QString("All known formats (");
	QStringList inputProjectFormatsDialogStringList;
	for (auto io : iterator){
		QString allKnownFormatsFilter;
		for (const FileFormat& currentFormat : io->importProjectFormats()){
			QString currentFilterEntry = currentFormat.description + " (";
			for (QString currentExtension : currentFormat.extensions) {
				currentExtension = currentExtension.toLower();
                allKnownFormatsFilter.append(QString(" *."));
				allKnownFormatsFilter.append(currentExtension);
                currentFilterEntry.append(QString(" *."));
				currentFilterEntry.append(currentExtension);
			}
			currentFilterEntry.append(')');
			inputProjectFormatsDialogStringList.append(currentFilterEntry);
		}
		allKnownFormats += allKnownFormatsFilter;
	}
	allKnownFormats.append(')');
	inputProjectFormatsDialogStringList.push_front(allKnownFormats);
	return inputProjectFormatsDialogStringList;
}

template<typename RangeIterator>
QStringList PluginManager::outputProjectFormatListDialog(RangeIterator iterator)
{
	QStringList outputProjectFormatsDialogStringList;
	for (auto io : iterator){
		for (const FileFormat& currentFormat : io->exportProjectFormats()){
			QString currentFilterEntry = currentFormat.description + " (";
			for (QString currentExtension : currentFormat.extensions) {
				currentExtension = currentExtension.toLower();
                currentFilterEntry.append(QString(" *."));
				currentFilterEntry.append(currentExtension);
			}
			currentFilterEntry.append(')');
			outputProjectFormatsDialogStringList.append(currentFilterEntry);
		}
	}
	return outputProjectFormatsDialogStringList;
}

ConstPluginIterator<MeshLabPlugin> PluginManager::PluginRangeIterator::begin()
{
	return ConstPluginIterator<MeshLabPlugin>(pm->allPlugins, pm->allPlugins.begin(), b);
}

ConstPluginIterator<MeshLabPlugin> PluginManager::PluginRangeIterator::end()
{
	return ConstPluginIterator<MeshLabPlugin>(pm->allPlugins, pm->allPlugins.end(), b);
}

PluginManager::PluginRangeIterator::PluginRangeIterator(const PluginManager* pm, bool iterateAlsoDisabledPlugins) :
	pm(pm), b(iterateAlsoDisabledPlugins)
{
}
