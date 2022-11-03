#ifndef SAVE_PROJECT_H
#define SAVE_PROJECT_H

#include <iostream>
#include <fstream>
#include <common/ml_document/mesh_document.h>
#include <QFile>
//#include <QCoreApplication>
//#include "include/JlCompress.h"

void saveMLP(
		const QString& filename,
		const MeshDocument& md,
		bool onlyVisibleLayers,
		const std::vector<MLRenderingData>& rendOpt,
		vcg::CallBackPos* cb);

void saveALN(
		const QString& filename,
		const MeshDocument& md,
		bool onlyVisibleLayers,
		vcg::CallBackPos* cb);
void saveANKER(
        const QString& filename,
        const MeshDocument& md,
        bool onlyVisibleLayers,
        const std::vector<MLRenderingData>& rendOpt,
        vcg::CallBackPos* cb);

#endif // SAVE_PROJECT_H
