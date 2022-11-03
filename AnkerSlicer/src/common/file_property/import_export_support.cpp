#include "import_export_support.h"
#include "../common/utilities/tlogger.h"
#include "mesh_model_import_export.h"
#include "../common/GeoAndShow/TransformPack.h"
#include <QApplication>
#include <iostream>
#include <fstream>

bool _QCallBack(const int pos, const char* str)
{
    int static lastPos = -1;
    if (pos == lastPos) return true;
    lastPos = pos;
    qApp->processEvents();
    return true;
}
bool Import_Export_Support::importSupport(const QString& inPathName, CHMeshShowObjPtr modelPtr)
{
    if (modelPtr == nullptr)
    {
        return false;
    }
    QString pathName = inPathName + QString("_") + QString::number(modelPtr->getId()) + QString(".stl");
    CMeshO cmesh;
    MeshModelImport_Export::open(QString("stl"), pathName, cmesh, _QCallBack);
    modelPtr->cmeshoToMeshShowObj(cmesh);
    return true;//readSupportFileToMesh(modelPtr, pathName);
}

bool Import_Export_Support::exportSupport(const QString& outPathName, CHMeshShowObjPtr modelPtr)
{
    if (modelPtr == nullptr)
    {
        return false;
    }
    QDir dir;
    if (!dir.exists(outPathName))
    {
        dir.mkpath(outPathName);
    }

    QString fullPath = outPathName + QString("_") + QString::number(modelPtr->getId()) + QString(".stl");

    return writeMeshToSupportFile(modelPtr, fullPath, true);
}

bool Import_Export_Support::writeMeshToSupportFile(const CHMeshShowObjPtr& meshPtr, const QString& path, bool binary)
{
    if (meshPtr == nullptr)
    {
        return false;
    }
    FILE* fp = NULL;
    std::string filepath = std::string(path.toLocal8Bit().constData());

    fp = fopen(filepath.c_str(), "wb");
    if (fp == NULL)
    {
        AkUtil::TError(QString("Open File %1 Failed, errno=%2, reason is %3").arg(path).arg(errno).arg(strerror(errno)));
        return false;
    }
    if (binary)
    {
        // Write Header
        char header[128] = "VCG                                                                                                  ";
        fwrite(header, 80, 1, fp);
        int faceSum = 0;
        faceSum += meshPtr->m_trians.size();
        fwrite(&faceSum, 1, sizeof(int), fp);

        for (int i = 0; i < faceSum; i++)
        {
            QVector3D pt1 = meshPtr->m_vertices[meshPtr->m_trians[i].m_index1];
            QVector3D pt2 = meshPtr->m_vertices[meshPtr->m_trians[i].m_index2];
            QVector3D pt3 = meshPtr->m_vertices[meshPtr->m_trians[i].m_index3];

            
            QVector3D tnor = QVector3D::crossProduct(pt2 - pt1, pt3 - pt1).normalized();
            fwrite(&tnor, 3, sizeof(float), fp);

            fwrite(&pt1, 3, sizeof(float), fp);
            fwrite(&pt2, 3, sizeof(float), fp);
            fwrite(&pt3, 3, sizeof(float), fp);

            unsigned short attributes = 0;
            fwrite(&attributes, 1, sizeof(short), fp);
        }
    }
    else
    {
        fprintf(fp, "solid vcg\n");
        for (int i = 0; i < meshPtr->m_trians.size(); i++)
        {
            QVector3D pt1 = meshPtr->m_vertices[meshPtr->m_trians[i].m_index1];
            QVector3D pt2 = meshPtr->m_vertices[meshPtr->m_trians[i].m_index2];
            QVector3D pt3 = meshPtr->m_vertices[meshPtr->m_trians[i].m_index3];

            QVector3D tnor = QVector3D::crossProduct(pt2 - pt1, pt3 - pt1).normalized();
            fprintf(fp, "  facet normal %13e %13e %13e\n", tnor[0], tnor[1], tnor[2]);
            fprintf(fp, "    outer loop\n");

            fprintf(fp, "      vertex  %13e %13e %13e\n", pt1[0], pt1[1], pt1[2]);
            fprintf(fp, "      vertex  %13e %13e %13e\n", pt2[0], pt2[1], pt2[2]);
            fprintf(fp, "      vertex  %13e %13e %13e\n", pt3[0], pt3[1], pt3[2]);

            fprintf(fp, "    endloop\n");
            fprintf(fp, "  endfacet\n");
        }
        fprintf(fp, "endsolid vcg\n");
    }
    bool result = true;
    if (ferror(fp))
    {
        result = false;
    }
    fclose(fp);
    return result;
}

bool Import_Export_Support::readSupportFileToMesh(CHMeshShowObjPtr& mesh, const QString& inPath)
{
    std::string filepath = std::string(inPath.toLocal8Bit().constData());
    std::ifstream instream(filepath.c_str(), std::ifstream::in);

    if (!instream)
    {
        return false;
    }

    std::string headStr;
    getline(instream, headStr, ' ');
    instream.close();

    if (headStr.empty())
        return false;

    bool success = false;
    if (headStr[0] = 's')
    {
        success = readAscii(mesh, filepath);
    }
    else
    {
        success = readBinary(mesh, filepath);
    }
    return success;
}


bool Import_Export_Support::readAscii(CHMeshShowObjPtr& mesh, const std::string& inPath)
{
    if (mesh == nullptr)
    {
        return false;
    }
    int i = 0, j = 0, cnt = 0, pCnt = 0;
    char a[100] = { 0 };
    char str[100] = { 0 };
    double x = 0.0, y = 0.0, z = 0.0;

    std::ifstream instream(inPath.c_str(), std::ifstream::in);
    if (!instream)
    {
        return false;
    }
    do
    {
        i = 0;
        cnt = 0;
        instream.getline(a, 100, '\n');
        while (a[i] != '\n')
        {
            if (!islower((int)a[i]) && !isupper((int)a[i]) && a[i] != ' ') 
            {
                break;
            }
            cnt++;
            i++;
        }

        while (a[cnt] != '\0')
        {
            str[j] = a[cnt];
            cnt++;
            j++;
        }
        str[j] = '\0';
        j = 0;

        if (sscanf(str, "%lf%lf%lf", &x, &y, &z) == 3)
        {
            switch (pCnt % 4)
            {
            case 0: mesh->m_nors.push_back(QVector3D(x, y, z));  break;
            case 1:
            case 2:
            case 3:
                mesh->m_vertices.push_back(QVector3D(x, y, z));
                break;

            default:
                break;
            }
            pCnt = ++pCnt % 4;
        }

    } while (!instream.eof());

    return true;
}

bool Import_Export_Support::readBinary(CHMeshShowObjPtr& mesh, const std::string& inPath)
{
    if (mesh == nullptr)
    {
        return false;
    }
    char str[80];
    std::ifstream instream(inPath.c_str(), std::ifstream::in | std::ifstream::binary);

    if (!instream)
    {
        return false;
    }
    instream.read(str, 80);   

    //number of triangles
    int triangles;
    instream.read((char*)&triangles, sizeof(int));

    if (triangles == 0)
    {
        return false;
    }

    for (int i = 0; i < triangles; i++)
    {
        float coorXYZ[12] = { 0.0 };
        instream.read((char*)coorXYZ, 12 * sizeof(float));
        mesh->m_nors.push_back(QVector3D(coorXYZ[0], coorXYZ[1], coorXYZ[2]));
        mesh->m_vertices.push_back(QVector3D(coorXYZ[3], coorXYZ[4], coorXYZ[5]));
        mesh->m_vertices.push_back(QVector3D(coorXYZ[6], coorXYZ[7], coorXYZ[8]));
        mesh->m_vertices.push_back(QVector3D(coorXYZ[9], coorXYZ[10], coorXYZ[11]));

        instream.read((char*)coorXYZ, 2);
    }
    instream.close();
    return true;
}
