#include "mlapplication.h"
#include "mlexception.h"
#include <vcg/complex/complex.h>
#include <QStandardPaths>
#include <QDir>
#include "globals.h"

#if defined(__clang__)
#define ML_COMPILER "Clang"
#define ML_COMPILER_VER MeshLabApplication::versionString(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__) || defined(__GNUG__) 
#define ML_COMPILER "GCC"
#define ML_COMPILER_VER MeshLabApplication::versionString(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(_MSC_VER)
#define ML_COMPILER "MSVC"
#define ML_COMPILER_VER std::to_string(_MSC_VER)
#else
#define ML_COMPILER "Unknown Compiler"
#define ML_COMPILER_VER std::string()
#endif

#ifdef NDEBUG
bool MeshLabApplication::notify( QObject * rec, QEvent * ev )
{
	try {
		return QApplication::notify(rec,ev);
	}
	catch (MLException& e) {
		qCritical("%s",e.what());
	}
	catch (vcg::MissingComponentException &e ) {
		qCritical("%s",e.what());
		abort();
	}
	catch (...) {
		qCritical("Something really bad happened!!!!!!!!!!!!!");
		abort();
	}
	return false;
}
#endif

//  change @2022-06-06 by CL
const QString MeshLabApplication::appVer(bool includeName, bool includeDate)
{
    QString ver("V0.9.14");
#if USE_EXTRA_UI
    ver = ver + ".1";
#endif

    if(includeName){
        ver = appName() + "_" + ver;
    }
    if(includeDate){
        ver +=  "_" + QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy").toString("yyyyMMdd");
    }
    qDebug() << __DATE__ << ver ;
    return ver;
    //return QString::fromStdString(meshlab::meshlabVersion());
}

const QString MeshLabApplication::compilerVersion()
{
	return QString(ML_COMPILER) + QString(" ") + QString::fromStdString(ML_COMPILER_VER);
}

const QString MeshLabApplication::qtVersion()
{
	return QString(QT_VERSION_STR);
}

const QString MeshLabApplication::extraPluginsLocation()
{
	QDir appDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());
	appDir.mkpath(appDir.absolutePath());
	
	appDir.mkdir("MeshLabExtraPlugins");
	appDir.cd("MeshLabExtraPlugins");
	
	//QString major = appVer().left(4);
	//appDir.mkdir(major);
	//appDir.cd(major);

	//just for first versions, compatibility of plugins is fixed for same version of meshlab
	appDir.mkdir(appVer());
	appDir.cd(appVer());

	return appDir.absolutePath();
}

const QString MeshLabApplication::extraShadersLocation()
{
	QDir appDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());
	appDir.mkpath(appDir.absolutePath());
	
	appDir.mkdir("MeshLabExtraShaders");
	appDir.cd("MeshLabExtraShaders");
	
	return appDir.absolutePath();
}

std::string MeshLabApplication::versionString(int a, int b, int c)
{
	std::ostringstream ss;
	ss << a << '.' << b << '.' << c;
	return ss.str();
}

