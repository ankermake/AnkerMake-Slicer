/*



*/



#ifndef CHDoc_H
#define CHDoc_H

#include "CH3DPrintModel.h"
#include "CHPrintMachineBox.h"
#include "QObject"


enum ModelStatusChangedType
{
    ClearDoc,
    DeleteMesh,
    AddMesh,
    VisuableChanged,
    ResetMesh
};


DEF_PTR(CHDoc)
class CHDoc : public QObject
{
    Q_OBJECT

public:
    CHDoc();
    ~CHDoc();


public:

    
    
    
    bool writeAllMeshModelsToStlFile(const QString& path, bool binary = true);
    bool writeVisibleMeshesToStlFile(const QString& path, bool binary = true);

    CH3DPrintModelPtr addNewMesh(const QString& fileName, const QString& infoFileName);

    
    void clearDocMeshModels();

    
    void resetAllMeshModels();
    void resetObjs(std::vector<CH3DPrintModelPtr> objs);

    void deleteObjs(std::vector<CH3DPrintModelPtr> objs);
    void deleteObj(CH3DPrintModelPtr obj);

    void getRealObjectName(const CH3DPrintModelPtr &modelPtr);
    void addObjs(std::vector<CH3DPrintModelPtr> objs);
    void addObj(CH3DPrintModelPtr obj);
    void appendObj(CH3DPrintModelPtr obj);

    void setObjsVisuable(std::vector<CHMeshShowObjPtr> objs, bool visual);
    void setObjsVisuable(std::vector<CH3DPrintModelPtr> objs, bool visual);
    void setObjVisuable(CHMeshShowObjPtr obj, bool visual);
    void setObjVisuable(CH3DPrintModelPtr obj, bool visual);

    
    void calcVisibleModelCount();

Q_SIGNALS:
    
    void modelObjsStatusChanged(ModelStatusChangedType changeType);

    
    void visibleModelCountChanged(int modelCount);

    
    void modelTransformChanged();

    
    void modelOutOfRangeChanged(bool allInPrintBox);

    
    void modelCheckSceneInChanged();

    
    void modelSupportNumberChanged(int sptNum);

public Q_SLOTS:
    void modelCheckSceneIn();

public:

    std::vector<CH3DPrintModelPtr> m_printObjs;
    CHPrintMachineBoxPtr m_machineBox;
    CHAssemblyPtr m_showPickedAABB;

    QString m_path;
};


#endif
