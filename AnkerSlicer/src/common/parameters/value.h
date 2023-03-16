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

#ifndef MESHLAB_VALUE_H
#define MESHLAB_VALUE_H

#include <QString>
#include <QColor>
#include "../ml_document/cmesh.h"
#include "../GeoAndShow/SceneParam.h"
class MeshModel;
class MeshDocument;
class QDomElement;
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

/**
 * @brief The Value class
 *
 * Represents a generic parameter value for meshlab.
 * Specializations inherit from this class, depending on the type of the
 * value. Value class is an attribute of the RichParameter class.
 */
class COMMONLIB_EXPORT Value
{
public:
	virtual ~Value() {}

	virtual bool getBool() const { assert(0); return bool(); }
	virtual int getInt() const { assert(0); return int(); }
	virtual Scalarm getFloat() const { assert(0); return Scalarm(); }
	virtual QString getString() const { assert(0); return QString(); }
	virtual Matrix44m getMatrix44f() const { assert(0); return Matrix44m(); }
	virtual Point3m getPoint3f() const { assert(0); return Point3m(); }
	virtual Shotm getShotf() const { assert(0); return Shotm(); }
	virtual QColor getColor() const { assert(0); return QColor(); }

	//TODO!!!Add By Aden Hu
	virtual SceneParam getSceneParam() const { assert(0); return SceneParam(); }
	virtual bool isScene() const { assert(0); return false; }

	virtual bool isBool() const { return false; }
	virtual bool isInt() const { return false; }
	virtual bool isFloat() const { return false; }
	virtual bool isString() const { return false; }
	virtual bool isMatrix44f() const { return false; }
	virtual bool isPoint3f() const { return false; }
	virtual bool isShotf() const { return false; }
	virtual bool isColor() const { return false; }

	virtual QString typeName() const = 0;
	virtual void set(const Value& p) = 0;
	virtual Value* clone() const = 0;
	virtual void fillToXMLElement(QDomElement& element) const = 0;
};

class COMMONLIB_EXPORT BoolValue : public Value
{
public:
	BoolValue(const bool val) : pval(val) {};
	~BoolValue() {}

	inline bool getBool() const { return pval; }
	inline bool isBool() const { return true; }
	inline QString typeName() const { return QString("Bool"); }
	inline void	set(const Value& p) { pval = p.getBool(); }
	inline BoolValue* clone() const { return new BoolValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	bool pval;
};

class COMMONLIB_EXPORT IntValue : public Value
{
public:
	IntValue(const int val) : pval(val) {}
	~IntValue() {}

	inline int	getInt() const { return pval; }
	inline bool isInt() const { return true; }
	inline QString typeName() const { return QString("Int"); }
	inline void	set(const Value& p) { pval = p.getInt(); }
	inline IntValue* clone() const { return new IntValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	int pval;
};

class COMMONLIB_EXPORT FloatValue : public Value
{
public:
	FloatValue(const float val) :pval(val) {}
	~FloatValue() {}

	inline Scalarm getFloat() const { return pval; }
	inline bool isFloat() const { return true; }
	inline QString typeName() const { return QString("Float"); }
	inline void	set(const Value& p) { pval = p.getFloat(); }
	inline FloatValue* clone() const { return new FloatValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	Scalarm pval;
};

class COMMONLIB_EXPORT StringValue : public Value
{
public:
	StringValue(const QString& val) :pval(val) {}
	~StringValue() {}

	inline QString getString() const { return pval; }
	inline bool isString() const { return true; }
	inline QString typeName() const { return QString("String"); }
	inline void	set(const Value& p) { pval = p.getString(); }
	inline StringValue* clone() const { return new StringValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	QString pval;
};

class COMMONLIB_EXPORT Matrix44fValue : public Value
{
public:
	Matrix44fValue(const Matrix44m& val) :pval(val) {}
	//Matrix44fValue(const vcg::Matrix44d& val) :pval(vcg::Matrix44f::Construct(val)) {}
	~Matrix44fValue() {}

	inline Matrix44m getMatrix44f() const { return pval; }
	inline bool isMatrix44f() const { return true; }
	inline QString typeName() const { return QString("Matrix44f"); }
	inline void	set(const Value& p) { pval = p.getMatrix44f(); }
	inline Matrix44fValue* clone() const { return new Matrix44fValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	Matrix44m pval;
};

class COMMONLIB_EXPORT Point3fValue : public Value
{
public:
	Point3fValue(const Point3m& val) : pval(val) {}
	~Point3fValue() {}

	inline Point3m getPoint3f() const { return pval; }
	inline bool isPoint3f() const { return true; }
	inline QString typeName() const { return QString("Point3f"); }
	inline void	set(const Value& p) { pval = p.getPoint3f(); }
	inline Point3fValue* clone() const { return new Point3fValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	Point3m pval;
};

class COMMONLIB_EXPORT ShotfValue : public Value
{
public:
	ShotfValue(const Shotm& val) : pval(val) {}
	~ShotfValue() {}

	inline Shotm getShotf() const { return pval; }
	inline bool isShotf() const { return true; }
	inline QString typeName() const { return QString("Shotf"); }
	inline void	 set(const Value& p) { pval = p.getShotf(); }
	inline ShotfValue* clone() const { return new ShotfValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	Shotm pval;
};

class COMMONLIB_EXPORT ColorValue : public Value
{
public:
	ColorValue(QColor val) :pval(val) {}
	~ColorValue() {}

	inline QColor getColor() const { return pval; }
	inline bool isColor() const { return true; }
	inline QString typeName() const { return QString("Color"); }
	inline void	set(const Value& p) { pval = p.getColor(); }
	inline ColorValue* clone() const { return new ColorValue(*this); }
	void fillToXMLElement(QDomElement& element) const;

private:
	QColor pval;
};

//TODO!!! Add By Aden Hu 
class COMMONLIB_EXPORT SceneValue : public Value
{
public:
	SceneValue(SceneParam param) : m_params(param) {}
	SceneValue(
		const PrintMachineBoxParam& bPrintParam,
		const QVector3D& inEye,
		const QVector3D& inUp,
		const QVector3D& inFront,
		const CMeshO& logoMesh,
		const QColor& backgroundColor
	)
	{
		m_params.m_printMachineBox = bPrintParam;
		m_params.m_eye = inEye;
		m_params.m_up = inUp;
		m_params.m_front = inFront;
		m_params.logoMesh = logoMesh;
		m_params.m_backgroundColor = backgroundColor;
	}
	SceneValue(
		const float boxLength, const float boxWidth, const float boxHeight, const QColor& boxColor, const float boxLineWidth, const int bLineNum,
		const QVector3D& inEye,
		const QVector3D& inUp,
		const QVector3D& inFront,
		const CMeshO& logoMesh,
		const QColor& backgroundColor)
	{
		m_params.m_printMachineBox.m_length = boxLength;
		m_params.m_printMachineBox.m_width = boxWidth;
		m_params.m_printMachineBox.m_height = boxHeight;
		m_params.m_printMachineBox.m_color = boxColor;
		m_params.m_printMachineBox.m_lineWidth = boxLineWidth;
		m_params.m_printMachineBox.num = bLineNum;

		m_params.m_eye = inEye;
		m_params.m_up = inUp;
		m_params.m_front = inFront;
		m_params.logoMesh = logoMesh;
		m_params.m_backgroundColor = backgroundColor;
	}

	~SceneValue() {}
	inline QString typeName() const { return QString("Scene"); }
	inline bool isScene() const { return true; }
	inline void set(const Value& p) { m_params = p.getSceneParam(); }
	inline SceneValue* clone() const { return new SceneValue(*this); }
	void fillToXMLElement(QDomElement& element) const;
	inline SceneParam getSceneParam() const { return m_params; }

private:
	SceneParam m_params;
};

#endif //MESHLAB_VALUE_H
