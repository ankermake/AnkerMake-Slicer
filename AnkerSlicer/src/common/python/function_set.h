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
#ifndef PYMESHLAB_FUNCTION_SET_H
#define PYMESHLAB_FUNCTION_SET_H

#include "../plugins/plugin_manager.h"
#include "function.h"

namespace pymeshlab {

/**
 * @brief The FunctionSet class represents a container of "python function names".
 * Each one of these function is binded with a particular action of a plugin,
 * which can be a filter action, an input/output mesh format or an input raster
 * format. Therefore, given a PluginManager, a FunctionSet will be constructed
 * from it considering only Filter and IO type of plugins.
 *
 * Note: there is not the actual link between the function and the action here.
 * This container is meant just as a container of *names*, therefore from here
 * you can check if a particular function exists, if it has some parameters and
 * the parameter types, and get the *name* of the binded action in meshlab.
 *
 * This cointainer just allows to access and iterate through all the contained
 * Functions. Please refer to the pymeshlab::Function class to see how to manage
 * a Function.
 */
class FunctionSet
{
public:
	FunctionSet();
	FunctionSet(const PluginManager& pm);

	//load plugins
	void loadFilterPlugin(FilterPlugin* fp);
	void loadIOPlugin(IOPlugin* iop);

	std::list<std::string> pythonFilterFunctionNames() const;

	const Function& filterFunction(const QString& pythonFunctionName) const;
	bool containsFilterFunction(const QString& pythonFunctionName) const;

	const Function& loadMeshFunction(const QString& pythonFunctionName) const;
	bool containsLoadMeshFunction(const QString& pythonFunctionName) const;

	const Function& saveMeshFunction(const QString& pythonFunctionName) const;
	bool containsSaveMeshFunction(const QString& pythonFunctionName) const;

	const Function& loadRasterFunction(const QString& pythonFunctionName) const;
	bool containsLoadRasterFunction(const QString& pythonFunctionName) const;

	//iterators
	class FunctionRangeIterator
	{
		friend class FunctionSet;
	public:
		std::set<Function>::const_iterator begin() { return s.begin();}
		std::set<Function>::const_iterator end() {return s.end();}
	private:
		FunctionRangeIterator(const std::set<Function>& s) : s(s) {};
		const std::set<Function>& s;
	};

	FunctionRangeIterator filterFunctionIterator() const;
	FunctionRangeIterator loadMeshFunctionIterator() const;
	FunctionRangeIterator saveMeshFunctionIterator() const;
	FunctionRangeIterator loadRasterFunctionIterator() const;

private:
	void updateSaveParameters(
			IOPlugin* plugin,
			const QString& outputFormat,
			Function& f);

	void initDummyMeshDocument();

	MeshDocument dummyMeshDocument;

	std::set<Function> filterSet;
	std::set<Function> loadMeshSet;
	std::set<Function> saveMeshSet;
	std::set<Function> loadImageSet;
};

}

#endif // PYMESHLAB_FUNCTION_SET_H
