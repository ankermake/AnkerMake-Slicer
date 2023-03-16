#ifndef CCGLOBAL_SERIAL_DRILL_H
#define CCGLOBAL_SERIAL_DRILL_H
#include "ccglobal/serial/serialtrimesh.h"

struct CCDrillParam
{
	int count;
	float radius;
	float depth;
	trimesh::vec3 position;
	trimesh::vec3 normal;
	bool onlyOneLayer;
};

void cc_load_drill(const std::string& file, trimesh::fxform& xf, trimesh::TriMesh& mesh, std::vector<CCDrillParam>& params)
{
	auto f = [&xf, &mesh, &params](std::fstream& in) {
		loadFXform(in, xf);
		loadTrimesh(in, mesh);
		loadVectorT(in, params);
	};
	serialLoad(file, f);
}

void cc_save_drill(const std::string& file, trimesh::fxform& xf, trimesh::TriMesh& mesh, std::vector<CCDrillParam>& params)
{
	auto f = [&xf, &mesh, &params](std::fstream& out) {
		saveFXform(out, xf);
		saveTrimesh(out, mesh);
		saveVectorT(out, params);
	};
	serialSave(file, f);
}

struct CCSliceSModel
{
	trimesh::TriMesh mesh;
	trimesh::fxform xf;
};

void cc_load_multislice(const std::string& file, std::vector<CCSliceSModel>& models)
{
	auto f = [&models](std::fstream& in) {
		int size = 0;
		loadT(in, size);
		if (size > 0)
			models.resize(size);
		for (int i = 0; i < size; ++i)
		{
			CCSliceSModel& model = models.at(i);
			loadFXform(in, model.xf);
			loadTrimesh(in, model.mesh);
		}
	};
	serialLoad(file, f);
}

void cc_save_multislice(const std::string& file, std::vector<CCSliceSModel>& models)
{
	auto f = [&models](std::fstream& out) {
		int size = models.size();
		saveT(out, size);
		for (int i = 0; i < size; ++i)
		{
			CCSliceSModel& model = models.at(i);
			saveFXform(out, model.xf);
			saveTrimesh(out, model.mesh);
		}
	};
	serialSave(file, f);
}
#endif // CCGLOBAL_SERIAL_DRILL_H
