/*



*/


#ifndef CHScene_H
#define CHScene_H


#include "CHPickOperationCommand.h"
#include "CHDoc.h"
#include "CHMeshShowObj.h"
#include "CHCurveShowObj.h"
#include "CHPointShowObj.h"
#include "../plugins/interfaces/edit_plugin.h"
#include "../plugins/interfaces/anker_edit_plugin.h"

#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

class COMMONLIB_EXPORT CHScene
{
public:
    CHScene();
    ~CHScene();


public:
    virtual void refresh() = 0;

    virtual void getCurNearFarPoint(int x, int y, QVector3D& np, QVector3D& fp) = 0;
    virtual void pixelToWorld(float x, float y, float z, QVector3D& worldCoord) = 0;
    virtual void worldToPixel(QVector3D worldCoord, float& x, float& y, float& z) = 0;
    virtual bool getCurMouseInWidget(int x, int y) = 0;
    virtual bool isActivteAppWindow() = 0;
    virtual void getMechineBoxSize(float &length, float &width, float &height) = 0;

    void updateShowObj(CHShowObjPtr showobj);
    void updateDoc();


protected:
    virtual void updatePoint(CHPointShowObjPtr pb) = 0;
    virtual void updateCurve(CHCurveShowObjPtr showcurve) = 0;
    virtual void updateMesh(CHMeshShowObjPtr mb) = 0;

public:
    CHDocPtr m_doc;
    CHPickOperationCommandPtr m_pickCommand;

};

extern COMMONLIB_EXPORT ActionEditTool * currActionEdit;
extern COMMONLIB_EXPORT CHScene*  curScene;
extern COMMONLIB_EXPORT CHPickOperationCommandPtr getGlobalPick();
extern COMMONLIB_EXPORT CHDocPtr getDoc();

inline ActionEditTool * inAnkerEditMode(){
    return (currActionEdit && currActionEdit->isChecked()) ? currActionEdit : nullptr;
}
inline ActionEditTool * readyAnkerEditMode(){
    return (currActionEdit && !currActionEdit->isChecked()) ? currActionEdit : nullptr;
}

#endif
