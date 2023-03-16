#ifndef CCGLOBAL_SERIAL_TRIMESH_H
#define CCGLOBAL_SERIAL_TRIMESH_H
#include "ccglobal/serial/serial.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"

void loadTrimesh(std::fstream& in, trimesh::TriMesh& mesh)
{
	loadVectorT(in, mesh.vertices);
	loadVectorT(in, mesh.faces);
}

void saveTrimesh(std::fstream& out, trimesh::TriMesh& mesh)
{
	saveVectorT(out, mesh.vertices);
	saveVectorT(out, mesh.faces);
}

void loadFXform(std::fstream& in, trimesh::fxform& xf)
{
	in.read((char*)xf.data(), 16 * sizeof(float));
}

void saveFXform(std::fstream& out, trimesh::fxform& xf)
{
	out.write((const char*)xf.data(), 16 * sizeof(float));
}
#endif  // CCGLOBAL_SERIAL_TRIMESH_H
