#ifndef FDMGCODEPREVIEWENTRY_H
#define FDMGCODEPREVIEWENTRY_H
#include <QtCore/qglobal.h>
#include <QStringList>
#include "glview/viewWidget.h"
//#if defined(TOOLLIB_LIBRARY)
//#  define TOOLLIB_EXPORT Q_DECL_EXPORT
//#else
//#  define TOOLLIB_EXPORT Q_DECL_IMPORT
//#endif

//entry class. it's main
//class Q_DECL_EXPORT FdmGcodePreviewEntry
class FdmGcodePreviewEntry
{
public:
    FdmGcodePreviewEntry();

    //void createView(int argc, char *argv[]);
    void createView(QStringList& args);
    viewWidget* ww = nullptr;
};



#endif // FDMGCODEPREVIEWENTRY_H
