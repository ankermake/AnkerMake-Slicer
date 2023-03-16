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

#ifndef MESHLAB_GLOBALS_H
#define MESHLAB_GLOBALS_H

#define meshlab_xstr(a) mlstringify(a)
#define mlstringify(a) #a

#include <QString>
#include <QVariantMap>
#ifndef MESHLAB_VERSION
#error "MESHLAB_VERSION needs to be defined!"
#endif
#ifndef MESHLAB_SCALAR
#error "MESHLAB_SCALAR needs to be defined!"
#endif
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

class RichParameterList;
class PluginManager;

namespace meshlab {

COMMONLIB_EXPORT QString  defaultPluginPath();
COMMONLIB_EXPORT QString  defaultShadersPath();
COMMONLIB_EXPORT RichParameterList&  defaultGlobalParameterList();
COMMONLIB_EXPORT PluginManager& pluginManagerInstance();

// keep these functions inlined please
// each plugin that uses them need to have their own definition
// plugins cannot link them!!
inline std::string meshlabVersion() 
{
	return std::string(meshlab_xstr(MESHLAB_VERSION));
}

inline bool builtWithDoublePrecision()
{
	return std::string(meshlab_xstr(MESHLAB_SCALAR)) == std::string("double");
}

COMMONLIB_EXPORT QVariantMap &  AppInfo();
}


#endif // MESHLAB_GLOBALS_H
