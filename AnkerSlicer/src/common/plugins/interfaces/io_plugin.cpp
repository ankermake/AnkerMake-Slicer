#include "io_plugin.h"
#include "../../ml_document/mesh_document.h"
#include <qDebug>

unsigned int IOPlugin::numberMeshesContainedInFile(
		const QString&,
		const QString&,
		const RichParameterList&) const
{
	return 1;
}

void IOPlugin::open(
		const QString& format,
		const QString& fileName,
		const std::list<MeshModel*>& meshModelList,
		std::list<int>& maskList,
		const RichParameterList& par,
		vcg::CallBackPos* cb)
{
	// This implementation assumes that the number of meshes contained in
	// the file is 1, and will call the open function that takes a single
	// MeshModel* as parameter.
    assert(meshModelList.size() == 1);
    maskList.clear();
    MeshModel *mm = meshModelList.front();
	int mask = 0;
    open(format, fileName, *mm, mask, par, cb);
    maskList.push_back(mask);
}

void IOPlugin::reportWarning(const QString& warningMessage) const
{
	if (!warningMessage.isEmpty()){
		MeshLabPluginLogger::log(GLLogStream::WARNING, warningMessage.toStdString());
		warnString += "\n" + warningMessage;
	}
}

void IOPlugin::wrongOpenFormat(const QString& format) const
{
	throw MLException("Internal error: unknown open format " + format + " to " + pluginName() + " plugin.");
}

void IOPlugin::wrongSaveFormat(const QString& format) const
{
	throw MLException("Internal error: unknown save format " + format + " to " + pluginName() + " plugin.");
}

QString IOPlugin::warningMessageString() const
{
	QString tmp = warnString;
	warnString.clear();
	return tmp;
}
