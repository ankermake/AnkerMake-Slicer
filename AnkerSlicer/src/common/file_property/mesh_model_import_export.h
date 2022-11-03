/****************************************************************************
Revision 0.1  2022/01/07 16:38:03  Aden Hu
****************************************************************************/

#ifndef MESH_MODEL_IMPORT_EXPORT_H
#define MESH_MODEL_IMPORT_EXPORT_H

#include <QObject>
#include <wrap/io_trimesh/import_obj.h>
#include <wrap/io_trimesh/export_obj.h>
#include <wrap/io_trimesh/export_stl.h>
#include <wrap/io_trimesh/import_stl.h>

#include "../common/ml_document/cmesh.h"
#include <../common/GeoAndShow/CH3DPrintModel.h>
#include <wrap/callback.h>
#include "thread_safe.h"

class  PVertex;
class  PEdge;
class  PFace;

struct PUsedTypes : public vcg::UsedTypes<vcg::Use<PVertex>   ::AsVertexType,
    vcg::Use<PEdge>     ::AsEdgeType,
    vcg::Use<PFace>     ::AsFaceType> {};

class PVertex : public vcg::Vertex<PUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::Qualityf, vcg::vertex::Color4b, vcg::vertex::BitFlags  > {};
class PEdge : public vcg::Edge< PUsedTypes, vcg::edge::VertexRef, vcg::edge::BitFlags> {};
class PFace :public vcg::Face<
    PUsedTypes,
    vcg::face::PolyInfo, // this is necessary  if you use component in vcg/simplex/face/component_polygon.h
    vcg::face::PFVAdj,   // Pointer to the vertices (just like FVAdj )
    vcg::face::Color4b,
    vcg::face::BitFlags, // bit flags
    vcg::face::Normal3f, // normal
    vcg::face::WedgeTexCoord2f
> {};

class PMesh : public vcg::tri::TriMesh< std::vector<PVertex>, std::vector<PEdge>, std::vector<PFace>   > {};

class MeshModelImport_Export : QObject
{
    Q_OBJECT
public:
    static bool open(const QString& formatName, const QString& fileName, CMeshO& cm, vcg::CallBackPos* cb);
    static void save(const QString& formatName, const QString& fileName, CMeshO& cm, int mask, vcg::CallBackPos* cb);
    static void loadMesh(const QString& fileName, const std::list<CH3DPrintModelPtr>& printModelsList, std::list<int>& mask, vcg::CallBackPos* cb);
private:

};

/*class ModelImportThread : ThreadObject
{
public:
    ModelImportThread(const QString& formatName, const QString& fileName, CMeshO& cm, vcg::CallBackPos* cb)	:
        m_formatName(formatName), m_fileName(fileName), m_cm(cm), m_cb(cb)
    {

    }

private slots:
    void init()
    {
        qDebug() << QThread::currentThreadId();

    }

public slots:
    void import_slot()
    {
        qDebug() << tr("Import: ") << QThread::currentThreadId();
        MeshModelImport_Export::open(m_formatName, m_fileName, m_cm, m_cb);
    }

private:
    QString m_formatName;
    QString m_fileName;
    CMeshO &m_cm;
    vcg::CallBackPos* m_cb;
};

class ModelExportThread : ThreadObject
{
public:
    ModelExportThread(const QString& formatName, const QString& fileName, CMeshO& cm, int mask, vcg::CallBackPos* cb) :
        m_formatName(formatName), m_fileName(fileName), m_cm(cm), m_mask(mask), m_cb(cb)
    {

    }

private slots:
    void init()
    {
        qDebug() << QThread::currentThreadId();
    }

public slots:
    void export_slot()
    {
        qDebug() << tr("Export: ") << QThread::currentThreadId();
        MeshModelImport_Export::save(m_formatName, m_fileName, m_cm, m_mask, m_cb);
    }

private:
    QString m_formatName;
    QString m_fileName;
    CMeshO& m_cm;
    int m_mask;
    vcg::CallBackPos* m_cb;
};
   */


#endif // !MESH_MODEL_IMPORT_EXPORT_H
