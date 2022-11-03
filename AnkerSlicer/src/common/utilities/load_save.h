/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005-2021                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#ifndef MESHLAB_LOAD_SAVE_H
#define MESHLAB_LOAD_SAVE_H

#include "../ml_shared_data_context/ml_shared_data_context.h"
#include "../plugins/interfaces/io_plugin.h"

/**
 * Utility functions to load/save meshes using plugins loaded in the plugin
 * manager.
 */

namespace meshlab {

std::list<std::string> loadMesh(
		const QString& fileName,
		IOPlugin* ioPlugin,
		const RichParameterList& prePar,
		const std::list<MeshModel*>& meshList,
		std::list<int>& maskList,
		vcg::CallBackPos *cb);

std::list<MeshModel*> loadMeshWithStandardParameters(
		const QString& filename,
        PluginManager& pm,
		MeshDocument& md,
		vcg::CallBackPos *cb = nullptr,
		RichParameterList prePar = RichParameterList());

void reloadMesh(
		const QString& filename,
		const std::list<MeshModel*>& meshList,
		GLLogStream* log = nullptr,
		vcg::CallBackPos* cb = nullptr);

void saveMeshWithStandardParameters(
		const QString& fileName,
		MeshModel& m,
		GLLogStream* log = nullptr,
		vcg::CallBackPos* cb = nullptr);

void saveAllMeshes(
		const QString& basePath,
		MeshDocument& md,
		bool onlyVisible = false,
		GLLogStream* log = nullptr,
		vcg::CallBackPos* cb = nullptr);

QImage loadImage(
		const QString& filename,
		GLLogStream* log = nullptr,
		vcg::CallBackPos *cb = nullptr);

void saveImage(
		const QString& filename,
		const QImage& image,
		int quality = -1,
		GLLogStream* log = nullptr,
		vcg::CallBackPos* cb = nullptr);

void loadRaster(
		const QString& filename,
		RasterModel& rm,
		GLLogStream* log = nullptr,
		vcg::CallBackPos *cb = nullptr);

std::vector<MeshModel*> loadProject(
		const QStringList& filenames,
        PluginManager& pm,
		IOPlugin* ioPlugin,
		MeshDocument& md,
		std::vector<MLRenderingData>& rendOpt,
		GLLogStream* log = nullptr,
		vcg::CallBackPos *cb = nullptr);

std::vector<MeshModel*> loadProject(
		const QStringList& filenames,
        PluginManager& pm,
		MeshDocument& md,
		GLLogStream* log = nullptr,
		vcg::CallBackPos *cb = nullptr);

std::vector<MeshModel*> loadProject(
		const QString& filename,
        PluginManager& pm,
		MeshDocument& md,
		GLLogStream* log = nullptr,
		vcg::CallBackPos *cb = nullptr);
std::vector<MeshModel*> openProject(
        const QString& format,
        PluginManager& pm,
        const QStringList& filenames,
        MeshDocument& md,
        std::vector<MLRenderingData>& rendOpt,
        vcg::CallBackPos* cb);
std::vector<MeshModel*> loadMLP(
        const QString& filename,
        PluginManager& pm,
        MeshDocument& md,
        std::vector<MLRenderingData>& rendOpt,
        std::vector<std::string>& unloadedImgList,
        vcg::CallBackPos* cb);
std::vector<MeshModel*> loadALN(
        const QString& filename,
        PluginManager& pm,
        MeshDocument& md,
        vcg::CallBackPos* cb);
void saveProject(
		const QString& filename,
		const MeshDocument& md,
		bool onlyVisibleMeshes,
		std::vector<MLRenderingData> renderData = std::vector<MLRenderingData>());

}

#endif // MESHLAB_LOAD_SAVE_H
