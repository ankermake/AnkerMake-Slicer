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

#include "mesh_document.h"

template <class LayerElement>
QString nameDisambiguator(std::list<LayerElement> &elemList, QString meshLabel)
{
	QString newName=std::move(meshLabel);
	typename std::list<LayerElement>::iterator mmi;

	for(mmi=elemList.begin(); mmi!=elemList.end(); ++mmi)
	{
		if((*mmi).label() == newName) // if duplicated name found
		{
			QFileInfo fi((*mmi).label());
			QString baseName = fi.baseName(); //  all characters in the file up to the first '.' Eg "/tmp/archive.tar.gz" -> "archive"
			QString suffix = fi.suffix();
			bool ok;

			// if name ends with a number between parenthesis (XXX),
			// it was himself a duplicated name, and we need to
			// just increase the number between parenthesis
			int numDisamb;
			int startDisamb;
			int endDisamb;

			startDisamb = baseName.lastIndexOf("(");
			endDisamb   = baseName.lastIndexOf(")");
			if((startDisamb!=-1)&&(endDisamb!=-1))
				numDisamb = baseName.midRef((startDisamb+1),(endDisamb-startDisamb-1)).toInt(&ok);
			else
				numDisamb = 0;

			if(startDisamb!=-1)
				newName = baseName.left(startDisamb)+ "(" + QString::number(numDisamb+1) + ")";
			else
				newName = baseName + "(" + QString::number(numDisamb+1) + ")";

			if (suffix != QString(""))
				newName = newName + "." + suffix;

			// now recurse to see if the new name is free
			newName = nameDisambiguator(elemList, newName);
		}
	}
	return newName;
}

MeshDocument::MeshDocument() 
{
	meshIdCounter=0;
	rasterIdCounter=0;
	currentMesh = nullptr;
	currentRaster = nullptr;
	busy=false;
}

MeshDocument::~MeshDocument()
{
}

bool MeshDocument::writeAllMeshModelsToStlFile(const QString& path, bool binary)
{
	FILE* fp = 0;
	fp = fopen(path.toStdString().c_str(), "wb");
	if (fp == 0)
	{
		return false;
	}

	if (binary)
	{
		// Write Header
		char header[128] = "VCG                                                                                                  ";
		
		fwrite(header, 80, 1, fp);
		// write number of facets
		int faceSum = 0;
		for (std::list<MeshModel>::iterator meshIt = meshList.begin(); meshIt != meshList.end(); meshIt++)
		{
			if (!meshIt->m_bePrintObj)
			{
				continue;
			}
			faceSum += meshIt->cm.fn;
		}
		fwrite(&faceSum, 1, sizeof(int), fp);

		for (std::list<MeshModel>::iterator meshIt = meshList.begin(); meshIt != meshList.end(); meshIt++)
		{
			if (!meshIt->m_bePrintObj)
			{
				continue;
			}
			Matrix44m norTran = vcg::Inverse(meshIt->cm.Tr).transpose();

			vcg::Point3f p;
			unsigned short attributes = 0;
			for (vcgTriMesh::FaceIterator fi = meshIt->cm.face.begin(); fi != meshIt->cm.face.end(); ++fi)
			{
				if (!(*fi).IsD())
				{
					// For each triangle write the normal, the three coords and a short set to zero
					p.Import(norTran * vcg::TriangleNormal(*fi).Normalize());
					fwrite(p.V(), 3, sizeof(float), fp);

					for (int k = 0; k < 3; ++k)
					{
						p.Import(meshIt->cm.Tr * ((*fi).V(k)->P()));
						fwrite(p.V(), 3, sizeof(float), fp);
					}

					fwrite(&attributes, 1, sizeof(short), fp);
				}
			}
		}
	}
	else
	{
		fprintf(fp, "solid vcg\n");

		for (std::list<MeshModel>::iterator meshIt = meshList.begin(); meshIt != meshList.end(); meshIt++)
		{
			if (!meshIt->m_bePrintObj)
			{
				continue;
			}
			Matrix44m norTran = vcg::Inverse(meshIt->cm.Tr).transpose();

			vcg::Point3f p;
			vcgTriMesh::FaceIterator fi;
			for (fi = meshIt->cm.face.begin(); fi != meshIt->cm.face.end(); ++fi)
			{
				if (!(*fi).IsD())
				{
					// For each triangle write the normal, the three coords and a short set to zero
					p.Import(norTran * TriangleNormal(*fi).Normalize());
					fprintf(fp, "  facet normal %13e %13e %13e\n", p[0], p[1], p[2]);
					fprintf(fp, "    outer loop\n");
					for (int k = 0; k < 3; ++k)
					{
						p.Import(meshIt->cm.Tr * ((*fi).V(k)->P()));
						fprintf(fp, "      vertex  %13e %13e %13e\n", p[0], p[1], p[2]);
					}
					fprintf(fp, "    endloop\n");
					fprintf(fp, "  endfacet\n");
				}
			}
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

void MeshDocument::clear()
{
	meshList.clear();
	rasterList.clear();

	meshIdCounter=0;
	rasterIdCounter=0;
	currentMesh = nullptr;
	currentRaster = nullptr;
	busy=false;
	filterHistory.clear();
	fullPathFilename = "";
	documentLabel = "";
	meshDocStateData().clear();
}

const MeshModel* MeshDocument::getMesh(unsigned int id) const
{
	for (const MeshModel& m : meshList)
		if (m.id() == id)
			return &m;
	return nullptr;
}

MeshModel* MeshDocument::getMesh(unsigned int id)
{
	for (MeshModel& m : meshList) {
		if (m.id() == id) {
			return &m;
		}
	}
	return nullptr;
}

void MeshDocument::setCurrentMesh(int new_curr_id)
{
	if(new_curr_id<0) {
		currentMesh = nullptr;
		return;
	}
	currentMesh = getMesh(new_curr_id);
	emit currentMeshChanged(new_curr_id);
	assert(currentMesh);
}

void MeshDocument::setVisible(int meshId, bool val)
{
	getMesh(meshId)->setVisible(val);
	emit meshSetChanged();
}

//returns the raster at a given position in the list
RasterModel *MeshDocument::getRaster(int i)
{
	for(RasterModel& rmp : rasterList) {
		if(rmp.id() == i) {
			return &rmp;
		}
	}
	return nullptr;
}

void MeshDocument::setCurrentRaster(int new_curr_id)
{
	if(new_curr_id<0) {
		currentRaster = nullptr;
		return;
	}

	currentRaster = getRaster(new_curr_id);
	assert(currentRaster);
}

void MeshDocument::setCurrent(MeshModel* newCur)
{
	setCurrentMesh(newCur->id());
}

void MeshDocument::setCurrent(RasterModel* newCur)
{
	setCurrentRaster(newCur->id());
}

MeshModel* MeshDocument::nextVisibleMesh(MeshModel* _m)
{
	MeshModel *newM = nextMesh(_m);
	if(newM==0)
		return newM;

	if(newM->isVisible())
		return newM;
	else
		return nextVisibleMesh(newM);
}

MeshModel* MeshDocument::nextMesh(MeshModel* _m)
{
	if(_m == nullptr && meshList.size()>0)
		return &meshList.front();
	for (auto it = meshList.begin(); it != meshList.end(); ++it) {
		if (&(*it) == _m) {
			auto next = it;
			next++;
			if(next != meshList.end())
				return &(*next);
		}
	}
	return nullptr;
}

RasterModel* MeshDocument::nextRaster(RasterModel* _rm)
{
	if(_rm == nullptr && rasterList.size()>0)
		return &rasterList.front();
	for (auto it = rasterList.begin(); it != rasterList.end(); ++it) {
		if (&(*it) == _rm) {
			auto next = it;
			next++;
			if(next != rasterList.end())
				return &(*next);
		}
	}
	return nullptr;
}

MeshModel* MeshDocument::mm()
{
	return currentMesh;
}

const MeshModel* MeshDocument::mm() const
{
	return currentMesh;
}

RasterModel* MeshDocument::rm()
{
	return currentRaster;
}

const RasterModel* MeshDocument::rm() const
{
	return currentRaster;
}

void MeshDocument::requestUpdatingPerMeshDecorators(int mesh_id)
{
	emit updateDecorators(mesh_id);
}

MeshDocumentStateData& MeshDocument::meshDocStateData()
{
	return mdstate;
}

void MeshDocument::setDocLabel(const QString& docLb)
{
	documentLabel = docLb;
}

QString MeshDocument::docLabel() const
{
	return documentLabel;
}

QString MeshDocument::pathName() const
{
	QFileInfo fi(fullPathFilename);
	return fi.absolutePath();
}

void MeshDocument::setFileName(const QString& newFileName)
{
	fullPathFilename = newFileName;
}

unsigned int MeshDocument::meshNumber() const
{
	return meshList.size();
}

unsigned int MeshDocument::rasterNumber() const
{
	return rasterList.size();
}

bool MeshDocument::isBusy()
{
	return busy;
}

void MeshDocument::setBusy(bool _busy)
{
	busy=_busy;
}

/**
 * @brief Adds a new mesh to the MeshDocument. The added mesh is a COPY of the mesh
 * passed as parameter.
 */
MeshModel* MeshDocument::addNewMesh(
		const CMeshO& mesh,
		const QString& label,
		bool setAsCurrent)
{
	MeshModel* m = addNewMesh("", label, setAsCurrent);
	m->cm = mesh;
	m->updateBoxAndNormals();
	m->updateDataMask();
	return m;
}

MeshModel* MeshDocument::addNewMesh(
		QString fullPath,
		const QString& label,
		bool setAsCurrent)
{
	QString newlabel = nameDisambiguator(this->meshList, label);

	if(!fullPath.isEmpty())
	{
		QFileInfo fi(fullPath);
		fullPath = fi.absoluteFilePath();
	}

	meshList.push_back(MeshModel(newMeshId(), fullPath,newlabel));
	MeshModel& newMesh = meshList.back();

	if(setAsCurrent)
		this->setCurrentMesh(newMesh.id());

	emit meshSetChanged();
	emit meshAdded(newMesh.id());
	return &newMesh;
}

MeshModel* MeshDocument::addOrGetMesh(
		const QString& fullPath,
		const QString& label,
		bool setAsCurrent)
{
	MeshModel* newMesh = nullptr;
	for (MeshModel& m : meshList)
		if (m.shortName() == label)
			newMesh = &m;
	if(newMesh) {
		if(setAsCurrent)
			this->setCurrentMesh(newMesh->id());
		return newMesh;
	}
	return addNewMesh(fullPath,label,setAsCurrent);
}

/**
 * Returns the ordered list of the meshes loaded from the same
 * file in which has been loaded the model mm.
 */
std::list<MeshModel*> MeshDocument::getMeshesLoadedFromSameFile(MeshModel& mm)
{
	std::list<MeshModel*> ml;
	if (mm.idInFile() == -1){
		ml.push_back(&mm);
	}
	else {
		if (!mm.fullName().isEmpty()){
			for (MeshModel& m : meshList){
				if (m.fullName() == mm.fullName())
					ml.push_back(&m);
			}
		}
	}
	return ml;
}

bool MeshDocument::delMesh(unsigned int id)
{
	auto pos = meshList.end();
	for (auto it = meshList.begin(); it != meshList.end(); ++it)
		if (it->id() == id)
			pos = it;
	if (pos == meshList.end())
		return false;

	if((currentMesh == &(*pos)) && (!meshList.empty()))
		setCurrentMesh(this->meshList.front().id());
	else if (meshList.empty())
		setCurrentMesh(-1);

	meshList.erase(pos);

	emit meshSetChanged();
	emit meshRemoved(id);
	return true;
}

MeshDocument::MeshIterator MeshDocument::eraseMesh(MeshIterator it)
{
	return meshList.erase(it);
}

RasterModel * MeshDocument::addNewRaster(/*QString fullPathFilename*/)
{
	QFileInfo info(fullPathFilename);
	QString newLabel=info.fileName();

	rasterList.push_back(RasterModel(newRasterId(), newLabel));
	RasterModel& newRaster = rasterList.back();

	this->setCurrentRaster(newRaster.id());

	emit rasterSetChanged();
	return &newRaster;
}

bool MeshDocument::delRaster(unsigned int id)
{
	auto pos = rasterList.end();
	for (auto it = rasterList.begin(); it != rasterList.end(); ++it)
		if (it->id() == id)
			pos = it;
	if (pos == rasterList.end())
		return false;

	if((currentRaster == &(*pos)) && (!rasterList.empty()))
		setCurrentRaster(rasterList.front().id());
	else if (rasterList.empty())
		setCurrentRaster(-1);

	rasterList.erase(pos);

	emit rasterSetChanged();

	return true;
}

MeshDocument::RasterIterator MeshDocument::eraseRaster(RasterIterator it)
{
	return rasterList.erase(it);
}

int MeshDocument::vn() const
{
	int tot=0;
	for(const MeshModel& mmp : meshList)
		tot+= mmp.cm.vn;
	return tot;
}

int MeshDocument::fn() const
{
	int tot=0;
	for(const MeshModel& mmp : meshList)
		tot+= mmp.cm.fn;
	return tot;
}

Box3m MeshDocument::bbox() const
{
	Box3m FullBBox;
	for(const MeshModel& mp : meshList)
		FullBBox.Add(mp.cm.Tr,mp.cm.bbox);
	return FullBBox;
}

bool MeshDocument::hasBeenModified() const
{
	for(const MeshModel& m : meshList)
		if(m.meshModified()) return true;
	return false;
}

MeshDocument::MeshIterator MeshDocument::meshBegin()
{
	return meshList.begin();
}

MeshDocument::MeshIterator MeshDocument::meshEnd()
{
	return meshList.end();
}

MeshDocument::RasterIterator MeshDocument::rasterBegin()
{
	return rasterList.begin();
}

MeshDocument::RasterIterator MeshDocument::rasterEnd()
{
	return rasterList.end();
}

MeshDocument::ConstMeshIterator MeshDocument::meshBegin() const
{
	return meshList.begin();
}

MeshDocument::ConstMeshIterator MeshDocument::meshEnd() const
{
	return meshList.end();
}

MeshDocument::ConstRasterIterator MeshDocument::rasterBegin() const
{
	return rasterList.begin();
}

MeshDocument::ConstRasterIterator MeshDocument::rasterEnd() const
{
	return rasterList.end();
}

MeshDocument::MeshRangeIterator MeshDocument::meshIterator()
{
	return MeshRangeIterator(*this);
}

MeshDocument::ConstMeshRangeIterator MeshDocument::meshIterator() const
{
	return ConstMeshRangeIterator(*this);
}

MeshDocument::RasterRangeIterator MeshDocument::rasterIterator()
{
	return RasterRangeIterator(*this);
}

MeshDocument::ConstRasterRangeIterator MeshDocument::rasterIterator() const
{
	return ConstRasterRangeIterator(*this);
}

unsigned int MeshDocument::newMeshId()
{
	return meshIdCounter++;
}

unsigned int MeshDocument::newRasterId()
{
	return rasterIdCounter++;
}
