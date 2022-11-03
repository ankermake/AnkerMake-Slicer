/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/

#ifndef DOCUMENT_PRPERTY_H
#define DOCUMENT_PRPERTY_H

#include "../ml_document/mesh_document.h"
#include <string>
#include <list>
#include <QColor>
#include <QMatrix4x4>
#include <QString>

class DocumentProperty
{
public:
    DocumentProperty();
    ~DocumentProperty();

    inline void clear()
    {
        models.clear();
        docName = "";
        pathName = "";
    }
    struct ShowProperty
    {
        QString meshName;
        bool visible;
        int meshId;
        QString meshPath;
        QColor meshColor;
        QMatrix4x4 matrix;
        CMeshO cm;
    };

    struct MeshProperty : public  ShowProperty
    {
        ShowMode showMode;
        QVector3D rotCenter;
        std::vector<float> params;

        std::vector<ShowProperty> supports;
        inline MeshProperty& operator= (const MeshProperty& _meshProp)
        {
            meshName = _meshProp.meshName;
            visible = _meshProp.visible;
            meshId = _meshProp.meshId;
            meshPath = _meshProp.meshPath;
            meshColor = _meshProp.meshColor;
            matrix = _meshProp.matrix;
            cm = _meshProp.cm;

            showMode = _meshProp.showMode;
            rotCenter = _meshProp.rotCenter;
            params = _meshProp.params;
            supports = _meshProp.supports;
            return *this;
        }

        enum ShowMode
        {
            MeshShowVertex = 0,
            MeshShowEdge,
            MeshShowFace
        };
    };

    std::list<MeshProperty> models;
    QString docName;
    QString pathName;


    inline DocumentProperty& operator= (const DocumentProperty& _docProp)
    {
        models = _docProp.models;
        docName = _docProp.docName;
        pathName = _docProp.pathName;

        return *this;
    }
};




#endif // !DOCUMENT_PRPERTY_H
