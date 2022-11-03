/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
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

#ifndef MESHLAB_RICH_PARAMETER_LIST_H
#define MESHLAB_RICH_PARAMETER_LIST_H

#include "rich_parameter.h"

/**
 * @brief The RichParameterList class
 *
 * It is a list container of RichParameters.
 * Since RichParameter is a base abstract class, you cannot have an
 * instance of a RichParameter object, but you can reference a generic
 * RichParameter object. Therefore, this container works with references.
 *
 * You can, for example, iterate along its RichParameters:
 * \code{.cpp}
 * for (const RichParameter& rp : list){
 *     if (rp.value().isBool()){
 *         //...
 *     }
 * }
 * \endcode
 *
 * You can get a RichParameter by name:
 * \code{.cpp}
 * RichParameter& rp = list.getParameterByName("myFloat");
 * \endcode
 *
 * See the documentation of every member function for details.
 */
class RichParameterList
{

public:
	class iterator {
	public:
		iterator() : it() {}
		friend class RichParameterList;
		bool operator==(const iterator& ot) {return it == ot.it;}
		bool operator!=(const iterator& ot) {return it != ot.it;}
		iterator& operator++() {++it; return *this;}
		iterator operator++(int) {auto tmp = it; ++it; return tmp;}
		iterator& operator--() {--it; return *this;}
		iterator operator--(int) {auto tmp = it; --it; return tmp;}
		RichParameter& operator*() {return **it;}
		RichParameter* operator->() {return *it;}
	private:
		iterator(std::list<RichParameter*>::iterator it) : it(it){}
		std::list<RichParameter*>::iterator it;
	};

	class const_iterator {
	public:
		const_iterator() : it() {}
		friend class RichParameterList;
		bool operator==(const const_iterator& ot) {return it == ot.it;}
		bool operator!=(const const_iterator& ot) {return it != ot.it;}
		const_iterator& operator++() {++it; return *this;}
		const_iterator operator++(int) {auto tmp = it; ++it; return tmp;}
		const_iterator& operator--() {--it; return *this;}
		const_iterator operator--(int) {auto tmp = it; --it; return tmp;}
		const RichParameter& operator*() {return **it;}
		const RichParameter* operator->() {return *it;}
	private:
		const_iterator(std::list<RichParameter*>::const_iterator it) : it(it){}
		std::list<RichParameter*>::const_iterator it;
	};

	RichParameterList();
	RichParameterList(const RichParameterList& rps);
	RichParameterList(RichParameterList&& rps);
	~RichParameterList();

	bool isEmpty() const;
	unsigned int size() const;
	void clear();

	bool getBool(const QString& name) const;
	int getInt(const QString& name) const;
	Scalarm getFloat(const QString& name) const;
	QString getString(const QString& name) const;
	Matrix44m getMatrix44(const QString& name) const;
	Point3m getPoint3m(const QString& name) const;
	Shotm getShotf(const QString& name) const;
	QColor getColor(const QString& name) const;
	vcg::Color4b getColor4b(const QString& name) const;
	Scalarm getAbsPerc(const QString& name) const;
	int getEnum(const QString& name) const;
	unsigned int getMeshId(const QString& name) const;
	Scalarm getDynamicFloat(const QString& name) const;
	QString getOpenFileName(const QString& name) const;
	QString getSaveFileName(const QString& name) const;

	bool hasParameter(const QString& name) const;
	RichParameter& getParameterByName(const QString& name);
	const RichParameter& getParameterByName(const QString& name) const;
	iterator findParameter(const QString& name);
	const_iterator findParameter(const QString& name) const;
	RichParameter& at(unsigned int i);
	const RichParameter& at(unsigned int i) const;
	unsigned int numberAdvancedParameters() const;

	void setValue(const QString& name, const Value& val);
	RichParameter& addParam(const RichParameter& pd);
	void join(const RichParameterList& rps);

	void pushFromQDomElement(QDomElement np);

	void swap(RichParameterList& oth);
	bool operator==(const RichParameterList& rps);
	RichParameterList& operator=(RichParameterList rps);

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

private:
	std::list<RichParameter*> paramList;
};

#endif // MESHLAB_RICH_PARAMETER_LIST_H
