/****************************************************************************
* MeshLab                                                           o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004-2021                                           \/)\/    *
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

#ifndef MESH_DOCUMENT_H
#define MESH_DOCUMENT_H

#include "mesh_model.h"
#include "raster_model.h"

#include "helpers/mesh_document_state_data.h"
//#include "../GeoAndShow/CHScene.h"
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

class COMMONLIB_EXPORT MeshDocument : public QObject
{
	Q_OBJECT

public:

	// Iterators
	typedef std::list<MeshModel>::iterator MeshIterator;
	typedef std::list<MeshModel>::const_iterator ConstMeshIterator;
	typedef std::list<RasterModel>::iterator RasterIterator;
	typedef std::list<RasterModel>::const_iterator ConstRasterIterator;

	class MeshRangeIterator
	{
		friend class MeshDocument;
	public:
		MeshIterator begin() {return md.meshBegin();}
		MeshIterator end() {return md.meshEnd();}
	private:
		MeshRangeIterator(MeshDocument& md) : md(md){}
		MeshDocument& md;
	};

	class ConstMeshRangeIterator
	{
		friend class MeshDocument;
	public:
		ConstMeshIterator begin() {return md.meshBegin();}
		ConstMeshIterator end()   {return md.meshEnd();}
	private:
		ConstMeshRangeIterator(const MeshDocument& md) : md(md){}
		const MeshDocument& md;
	};

	class RasterRangeIterator
	{
		friend class MeshDocument;
	public:
		RasterIterator begin() {return md.rasterBegin();}
		RasterIterator end()   {return md.rasterEnd();}
	private:
		RasterRangeIterator(MeshDocument& md) : md(md){}
		MeshDocument& md;
	};

	class ConstRasterRangeIterator
	{
		friend class MeshDocument;
	public:
		ConstRasterIterator begin() {return md.rasterBegin();}
		ConstRasterIterator end()   {return md.rasterEnd();}
	private:
		ConstRasterRangeIterator(const MeshDocument& md) : md(md){}
		const MeshDocument& md;
	};

	MeshDocument();

	//deletes each meshModel
	~MeshDocument();

	
	
	
	bool writeAllMeshModelsToStlFile(const QString& path, bool binary = true);

	void clear();

	///returns the mesh with the given unique id
	const MeshModel* getMesh(unsigned int id) const;
	MeshModel* getMesh(unsigned int id);

	//set the current mesh to be the one with the given ID
	void setCurrentMesh( int new_curr_id );
	
	void setVisible(int meshId, bool val);

	/// returns the raster with the given unique id
	RasterModel *getRaster(int i);

	//set the current raster to be the one with the given ID
	void setCurrentRaster( int new_curr_id );
	void setCurrent(MeshModel   *newCur);
	void setCurrent(RasterModel *newCur);

	/// methods to access the set of Meshes in a ordered fashion.
	MeshModel   *nextVisibleMesh(MeshModel *_m = nullptr);

	MeshModel   *nextMesh(MeshModel *_m = nullptr);
	/// methods to access the set of Meshes in a ordered fashion.
	RasterModel   *nextRaster(RasterModel *_rm = nullptr);

	MeshModel* mm();

	const MeshModel* mm() const;

	//Could return 0 if no raster has been selected
	RasterModel *rm();
	const RasterModel* rm() const;

	void requestUpdatingPerMeshDecorators(int mesh_id);

	MeshDocumentStateData& meshDocStateData();
	void setDocLabel(const QString& docLb);
	QString docLabel() const;
	QString pathName() const;
	void setFileName(const QString& newFileName);

	unsigned int meshNumber() const;
	unsigned int rasterNumber() const;

	bool isBusy();  // used in processing. To disable access to the mesh by the rendering thread
	void setBusy(bool _busy);

	///add a new mesh with the given name
	MeshModel* addNewMesh(const CMeshO& mesh, const QString& Label, bool setAsCurrent=true);
	MeshModel *addNewMesh(QString fullPath, const QString& Label, bool setAsCurrent=true);
	MeshModel *addOrGetMesh(const QString& fullPath, const QString& Label, bool setAsCurrent=true);
	std::list<MeshModel*> getMeshesLoadedFromSameFile(MeshModel& mm);

	///remove the mesh from the list and delete it from memory
	bool delMesh(unsigned int id);
	MeshIterator eraseMesh(MeshIterator it);

	///add a new raster model
	RasterModel *addNewRaster(/*QString rasterName*/);

	///remove the raster from the list and delete it from memory
	bool delRaster(unsigned int id);
	RasterIterator eraseRaster(RasterIterator it);

	int vn() const; /// Sum of all the vertices of all the meshes

	int fn() const;

	Box3m bbox() const;

	bool hasBeenModified() const;

	//iterator member functions
	MeshIterator meshBegin();
	MeshIterator meshEnd();
	RasterIterator rasterBegin();
	RasterIterator rasterEnd();
	ConstMeshIterator meshBegin() const;
	ConstMeshIterator meshEnd() const;
	ConstRasterIterator rasterBegin() const;
	ConstRasterIterator rasterEnd() const;

	MeshRangeIterator meshIterator();
	ConstMeshRangeIterator meshIterator() const;
	RasterRangeIterator rasterIterator();
	ConstRasterRangeIterator rasterIterator() const;

	GLLogStream Log;
	FilterScript filterHistory;

	
	//CHScene* m_scene;

private:
	/// The very important member:
	/// The list of MeshModels.
	std::list<MeshModel> meshList;
	/// The list of the raster models of the project
	std::list<RasterModel> rasterList;

	int meshIdCounter;
	int rasterIdCounter;

	/**
	All the files referred in a document are relative to the folder containing the project file.
	this is the full path to the document.
	*/
	QString fullPathFilename;

	//it is the label of the document. it should only be something like Project_n (a temporary name for a new empty document) or the fullPathFilename.
	QString documentLabel;

	MeshDocumentStateData mdstate;

	bool busy;

	MeshModel* currentMesh;
	//the current raster model
	RasterModel* currentRaster;

	unsigned int newMeshId();
	unsigned int newRasterId();

signals:
	///whenever the current mesh is changed (e.g. the user click on a different mesh)
	// this signal will send out with the index of the newest mesh
	void currentMeshChanged(int index);

	/// whenever the document (or even a single mesh) is modified by a filter
	void meshDocumentModified();

	///whenever the meshList is changed
	void meshSetChanged();

	void meshAdded(int index);
	void meshRemoved(int index);

	///whenever the rasterList is changed
	void rasterSetChanged();

	//this signal is emitted when a filter request to update the mesh in the renderingState
	void documentUpdated();
	void updateDecorators(int mesh_id);
};// end class MeshDocument


#endif // MESH_DOCUMENT_H
