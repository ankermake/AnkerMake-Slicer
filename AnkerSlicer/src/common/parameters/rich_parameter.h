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

#ifndef MESHLAB_RICH_PARAMETER_H
#define MESHLAB_RICH_PARAMETER_H

#include "value.h"
#include <QDomElement>
#include <QSharedPointer>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif


class MeshDocument;

/**
 * @brief The RichParameter class
 *
 * The RichParameter class is a representation of a MeshLab parameter that contains
 * a value of a certain type (see the Value class and its specializations) plus
 * some other decorators, like a parameter description and a tooltip.
 *
 * All these decorators allow MeshLab to automatically design GUI interfaces for
 * every RichParameter spcialization.
 *
 * Every specialization must implement the following member functions:
 * - QString stringType() const : returns a string representation of the type of the RichParameter
 * - RichParameter* clone() const : returns a *new* object which is a clone of the RichParameter
 * - bool operator==(const RichParameter& rp): returns true if the two RichParameter are the same
 */
class RichParameterQObject;
class COMMONLIB_EXPORT RichParameter
{
public:
	RichParameter(const RichParameter& rp);
	RichParameter(RichParameter&& rp);
	RichParameter(
		const QString& nm,
		const Value& v,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool isAdvanced = false,
		const QString& category = QString());
	virtual ~RichParameter();

	const QString& name() const;
	const Value& value() const;
	const QString& fieldDescription() const;
	const QString& toolTip() const;
	bool isAdvanced() const;
	const QString& category() const;

	template <class RichParam>
	bool isOfType() const
	{
		const RichParam* t = dynamic_cast<const RichParam*>(this);
		return (t != nullptr);
	}

	virtual QString stringType() const = 0;

	void setName(const QString& newName);
	void setValue(const Value& ov);

	virtual QDomElement fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip = true) const;

	//python names of paraeter
	QString pythonName() const;
	QString pythonType() const;

	virtual RichParameter* clone() const = 0;
	RichParameter& operator=(const RichParameter& rp);
	RichParameter& operator=(RichParameter&& rp);
	virtual bool operator==(const RichParameter& rp) = 0;

	QSharedPointer<RichParameterQObject> qobj;
protected:
	QString pName;
	Value* val;
	QString fieldDesc;
	QString tooltip;
	bool advanced;
	QString pCategory;
};

/**
 * @brief The RichParameterQObject class
 * add @2022-01-27 by CL

 */
class COMMONLIB_EXPORT RichParameterQObject : public QObject {
	Q_OBJECT
		RichParameter* param{ nullptr };
public:
	RichParameterQObject(RichParameter* param) :param(param) {}

	const Value& getValue() {
		return param->value();
	}
public slots:
	void setValue(const Value& value) {
		param->setValue(value);
		emit valueChange(value);
	}
signals:
	void valueChange(const Value& value);
};

class COMMONLIB_EXPORT RichBool : public RichParameter
{
public:
	RichBool(
		const QString& nm,
		const bool defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichBool();

	QString stringType() const;

	RichBool* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichInt : public RichParameter
{
public:
	RichInt(
		const QString& nm,
		const int defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichInt();

	QString stringType() const;

	RichInt* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichFloat : public RichParameter
{
public:
	RichFloat(
		const QString& nm,
		const Scalarm defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichFloat();

	QString stringType() const;

	RichFloat* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichString : public RichParameter
{
public:
	RichString(
		const QString& nm,
		const QString& defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichString();

	QString stringType() const;

	RichString* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichMatrix44f : public RichParameter
{
public:
	RichMatrix44f(
		const QString& nm,
		const Matrix44m& defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichMatrix44f();

	QString stringType() const;

	RichMatrix44f* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichPoint3f : public RichParameter
{
public:
	RichPoint3f(
		const QString& nm,
		const Point3m& defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichPoint3f();

	QString stringType() const;

	RichPoint3f* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichShotf : public RichParameter
{
public:
	RichShotf(
		const QString& nm,
		const Shotm& defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichShotf();

	QString stringType() const;

	RichShotf* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichColor : public RichParameter
{
public:
	RichColor(
		const QString& nm,
		const QColor& defval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichColor();

	QString stringType() const;

	RichColor* clone() const;
	bool operator==(const RichParameter& rb);
};

class COMMONLIB_EXPORT RichAbsPerc : public RichParameter
{
public:
	RichAbsPerc(
		const QString& nm,
		const Scalarm defval,
		const Scalarm minval,
		const Scalarm maxval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichAbsPerc();

	QString stringType() const;
	QDomElement fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip = true) const;

	RichAbsPerc* clone() const;
	bool operator==(const RichParameter& rb);
	Scalarm min;
	Scalarm max;
};

class COMMONLIB_EXPORT RichEnum : public RichParameter
{
public:
	RichEnum(
		const QString& nm,
		const int defval,
		const QStringList& values,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichEnum();

	QString stringType() const;
	QDomElement fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip = true) const;

	RichEnum* clone() const;
	bool operator==(const RichParameter& rb);
	QStringList enumvalues;
};

class COMMONLIB_EXPORT RichDynamicFloat : public RichParameter
{
public:
	RichDynamicFloat(
		const QString& nm,
		const Scalarm defval,
		const Scalarm minval,
		const Scalarm maxval,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichDynamicFloat();

	QString stringType() const;
	QDomElement fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip = true) const;

	RichDynamicFloat* clone() const;
	bool operator==(const RichParameter& rb);
	Scalarm min;
	Scalarm max;
};

class COMMONLIB_EXPORT RichOpenFile : public RichParameter
{
public:
	RichOpenFile(
		const QString& nm,
		const QString& directorydefval,
		const QStringList& exts,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichOpenFile();

	QString stringType() const;
	QDomElement fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip = true) const;

	RichOpenFile* clone() const;
	bool operator==(const RichParameter& rb);
	QStringList exts;
};

class COMMONLIB_EXPORT RichSaveFile : public RichParameter
{
public:
	RichSaveFile(
		const QString& nm,
		const QString& filedefval,
		const QString& ext,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichSaveFile();

	QString stringType() const;
	QDomElement fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip = true) const;

	RichSaveFile* clone() const;
	bool operator==(const RichParameter& rb);
	QString ext;
};

class COMMONLIB_EXPORT RichMesh : public RichParameter
{
public:
	RichMesh(
		const QString& nm,
		unsigned int meshindex,
		const MeshDocument* doc,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	//WARNING: IT SHOULD BE USED ONLY BY MESHLABSERVER!!!!!!!
	RichMesh(
		const QString& nm,
		unsigned int meshind,
		const QString& desc = QString(),
		const QString& tltip = QString(),
		bool hidden = false,
		const QString& category = QString());
	~RichMesh();

	QString stringType() const;

	RichMesh* clone() const;
	bool operator==(const RichParameter& rb);
	const MeshDocument* meshdoc;
};

//TODO!!!Add By Aden Hu
class COMMONLIB_EXPORT RichSceneParam : public RichParameter
{
public:
	RichSceneParam(const QString& nm, const SceneParam& sceneParam);
	RichSceneParam(
		const QString& nm,
		const PrintMachineBoxParam& bPrintParam,
		const QVector3D& inEye,
		const QVector3D& inUp,
		const QVector3D& inFront,
		const CMeshO& logoMesh,
		const QColor& backgroundColor
	);

	RichSceneParam(
		const QString& nm,
		const float boxLength, const float boxWidth, const float boxHeight, const QColor& boxColor, const float boxLineWidth, const int bLineNum,
		const QVector3D& inEye,
		const QVector3D& inUp,
		const QVector3D& inFront,
		const CMeshO& logoMesh,
		const QColor& backgroundColor);

	~RichSceneParam();

	QString stringType() const;
	RichSceneParam* clone() const;
	bool operator==(const RichParameter& rp);
private:
	SceneParam m_sceneParam;
};

class COMMONLIB_EXPORT RichParameterAdapter
{
public:
	static bool create(const QDomElement& np, RichParameter** par);
};

#endif // MESHLAB_RICH_PARAMETER_H
