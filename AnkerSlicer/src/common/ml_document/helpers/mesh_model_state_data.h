/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005-2020                                           \/)\/    *
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

#ifndef MESHLAB_MESH_MODEL_STATE_DATA_H
#define MESHLAB_MESH_MODEL_STATE_DATA_H

#include <cstddef>

struct MeshModelStateData
{
	int _mask;
	size_t _nvert;
	size_t _nface;
	size_t _nedge;

	MeshModelStateData(int mask, size_t nvert, size_t nface, size_t nedge):
		_mask(mask), _nvert(nvert), _nface(nface), _nedge(nedge)
	{}
};


#endif // MESHLAB_MESH_MODEL_STATE_DATA_H
