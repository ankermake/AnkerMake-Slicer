#ifndef FDMGCODEPREVIEWENTRY_H
#define FDMGCODEPREVIEWENTRY_H
#include <QtCore/qglobal.h>
#include <QStringList>
#include "glview/viewWidget.h"

class FdmGcodePreviewEntry
{
public:
    FdmGcodePreviewEntry();
    ~FdmGcodePreviewEntry();
    void createView(QStringList& args);
    viewWidget* ww = nullptr;
private:

};



#endif // FDMGCODEPREVIEWENTRY_H
