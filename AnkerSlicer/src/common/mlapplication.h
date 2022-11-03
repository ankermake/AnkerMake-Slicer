#ifndef ML_APPLICATION_H
#define ML_APPLICATION_H

#include <QApplication>
#include <QDate>
#include <QString>
#include <wrap/gl/gl_mesh_attributes_info.h>
#include "ml_document/cmesh.h"

class MeshLabApplication : public QApplication
{
public:
	enum HW_ARCHITECTURE {HW_32BIT = 32,HW_64BIT = 64};
	MeshLabApplication(int &argc, char *argv[]): QApplication(argc,argv){}
	~MeshLabApplication(){}
#ifdef NDEBUG
	bool notify(QObject * rec, QEvent * ev);
#endif	
    static const QString appName(){return tr("AnkerMake"); }
	static const QString architecturalSuffix(const HW_ARCHITECTURE hw) {return "_" + QString::number(int(hw)) + "bit";}
	static const QString appArchitecturalName(const HW_ARCHITECTURE hw) {return appName() + architecturalSuffix(hw) + "_" + MeshLabScalarTest<Scalarm>::floatingPointPrecision();}
    static const QString appVer(bool includeName = false, bool includeDate = false);
	static const QString compilerVersion();
	static const QString qtVersion();
	
    static const QString shortName() { return appName() + "_" + appVer(); }
	static const QString completeName(const HW_ARCHITECTURE hw){return appArchitecturalName(hw) + " v" + appVer(); }
    static const QString organization(){return "AnkerSlicer";} 
	static const QString organizationHost() {return tr("http://vcg.isti.cnr.it");}
	static const QString webSite() {return tr("http://www.meshlab.net/");}
	static const QString downloadSite() {return tr("http://www.meshlab.net/#download");}
	
	static const QString pluginsPathRegisterKeyName() {return tr("pluginsPath");}
	static const QString versionRegisterKeyName() {return tr("version");}
	static const QString wordSizeKeyName() {return tr("wordSize");}
	
	static const QString extraPluginsLocation();
	static const QString extraShadersLocation();
	
private:
	static std::string versionString(int a, int b, int c);
};

#endif
