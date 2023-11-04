#ifndef ML_APPLICATION_H
#define ML_APPLICATION_H

#include <QApplication>
#include <QDate>
#include <QString>
#include <wrap/gl/gl_mesh_attributes_info.h>
#include "ml_document/cmesh.h"
#include <QFileOpenEvent>
#include "utilities/tlogger.h"
#include <QFontDatabase>

class COMMONLIB_EXPORT MeshLabApplication : public QApplication
{
    Q_OBJECT
public:
	enum HW_ARCHITECTURE {HW_32BIT = 32,HW_64BIT = 64};
	MeshLabApplication(int &argc, char *argv[]): QApplication(argc,argv){}
	~MeshLabApplication(){
        QFontDatabase::removeAllApplicationFonts();
    }
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
    static const QString organization(){return "AnkerMake";} 
	static const QString organizationHost() {return tr("http://vcg.isti.cnr.it");}
	static const QString webSite() {return tr("http://www.meshlab.net/");}
	static const QString downloadSite() {return tr("http://www.meshlab.net/#download");}
	
    static const QString pluginsPathRegisterKeyName() {return "pluginsPath";}
    static const QString versionRegisterKeyName() {return "version";}
    static const QString wordSizeKeyName() {return "wordSize";}
	
	static const QString extraPluginsLocation();
	static const QString extraShadersLocation();

    bool event(QEvent *event)
    {
        if (event->type() == QEvent::LanguageChange) {
            emit languageChangeSignal();
        }
       
        if(event->type() == QEvent::FileOpen){
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            AkUtil::TDebug("Open File: " + openEvent->file());
            QString fileName = openEvent->file();
            if(!fileName.isEmpty())
            {
                fileList.append(fileName);
                emit openFileSignal(fileName);
            }
        }
        return QApplication::event(event);
    }

    QStringList fileList;
signals:
    void openFileSignal(const QString &fileName);
    void languageChangeSignal();
	
private:
	static std::string versionString(int a, int b, int c);
};

#endif
