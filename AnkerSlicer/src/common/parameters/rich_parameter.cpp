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

#include "rich_parameter.h"
#include "../ml_document/mesh_document.h"
#include "../python/python_utils.h"

/**** RichParameter Class ****/

RichParameter::RichParameter(const RichParameter& rp) :
	pName(rp.pName),
	val(rp.value().clone()),
	fieldDesc(rp.fieldDesc),
	tooltip(rp.tooltip),
	advanced(rp.advanced),
	pCategory(rp.pCategory)
{
	qobj = qobj.create(this);
}

RichParameter::RichParameter(RichParameter&& rp) :
	pName(std::move(rp.pName)),
	fieldDesc(std::move(rp.fieldDesc)),
	tooltip(std::move(rp.tooltip)),
	pCategory(std::move(rp.pCategory))
{
	val = rp.val;
	rp.val = nullptr;
	advanced = rp.advanced;
	qobj = qobj.create(this);
}

RichParameter::RichParameter(
	const QString& nm,
	const Value& v,
	const QString& desc,
	const QString& tltip,
	bool isAdvanced,
	const QString& category) :
	pName(nm),
	val(v.clone()),
	fieldDesc(desc),
	tooltip(tltip),
	advanced(isAdvanced),
	pCategory(category)
{
	qobj = qobj.create(this);
}

RichParameter::~RichParameter()
{
	delete val;
}

const QString& RichParameter::name() const
{
	return pName;
}

const Value& RichParameter::value() const
{
	return *val;
}

const QString& RichParameter::fieldDescription() const
{
	return fieldDesc;
}

const QString& RichParameter::toolTip() const
{
	return tooltip;
}

bool RichParameter::isAdvanced() const
{
	return advanced;
}

const QString& RichParameter::category() const
{
	return pCategory;
}

void RichParameter::setName(const QString& newName)
{
	pName = newName;
}

void RichParameter::setValue(const Value& ov)
{
	assert(val->typeName() == ov.typeName());
	delete val;
	val = ov.clone();
}

QDomElement RichParameter::fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip) const
{
	QDomElement parElem = doc.createElement("Param");
	parElem.setAttribute("name", pName);
	parElem.setAttribute("type", stringType());
	if (saveDescriptionAndTooltip) {
		parElem.setAttribute("description", fieldDesc);
		parElem.setAttribute("tooltip", tooltip);
	}
	val->fillToXMLElement(parElem);
	return parElem;
}

/**
 * @brief returns the name of the parameter used in python (for pymeshlab)
 */
QString RichParameter::pythonName() const
{
	return pymeshlab::computePythonName(pName);
}

/**
 * @brief returns the type if the parameter (as a string) used in python
 * (for pymeshlab)
 */
QString RichParameter::pythonType() const
{
	return pymeshlab::computePythonTypeString(*this);
}

RichParameter& RichParameter::operator=(const RichParameter& rp)
{
	if (&rp != this) {
		delete val;
		val = rp.value().clone();
		pName = rp.pName;
		fieldDesc = rp.fieldDesc;
		tooltip = rp.tooltip;
	}
	return *this;
}

RichParameter& RichParameter::operator=(RichParameter&& rp)
{
	assert(&rp != this);
	val = rp.val;
	rp.val = nullptr;
	pName = std::move(rp.pName);
	fieldDesc = std::move(rp.fieldDesc);
	tooltip = std::move(rp.tooltip);
	return *this;
}

/**** RichBool Class ****/

RichBool::RichBool(
	const QString& nm,
	const bool defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, BoolValue(defval), desc, tltip, hidden, category)
{
}

RichBool::~RichBool()
{
}

QString RichBool::stringType() const
{
	return "RichBool";
}

RichBool* RichBool::clone() const
{
	return new RichBool(*this);
}

bool RichBool::operator==(const RichParameter& rb)
{
	return (rb.value().isBool() && (pName == rb.name()) && (value().getBool() == rb.value().getBool()));
}

/**** RichInt Class ****/

RichInt::RichInt(
	const QString& nm,
	const int defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, IntValue(defval), desc, tltip, hidden, category)
{
}

RichInt::~RichInt()
{
}

QString RichInt::stringType() const
{
	return "RichInt";
}

RichInt* RichInt::clone() const
{
	return new RichInt(*this);
}

bool RichInt::operator==(const RichParameter& rb)
{
	return (rb.value().isInt() && (pName == rb.name()) && (value().getInt() == rb.value().getInt()));
}

/**** RichFloat Class ****/

RichFloat::RichFloat(
	const QString& nm,
	const Scalarm defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, FloatValue(defval), desc, tltip, hidden, category)
{
}

RichFloat::~RichFloat()
{
}

QString RichFloat::stringType() const
{
	return "RichFloat";
}

RichFloat* RichFloat::clone() const
{
	return new RichFloat(*this);
}

bool RichFloat::operator==(const RichParameter& rb)
{
	return (rb.value().isFloat() && (pName == rb.name()) && (value().getFloat() == rb.value().getFloat()));
}

/**** RichString Class ****/

RichString::RichString(
	const QString& nm,
	const QString& defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, StringValue(defval), desc, tltip, hidden, category)
{
}

RichString::~RichString()
{
}

QString RichString::stringType() const
{
	return "RichString";
}

RichString* RichString::clone() const
{
	return new RichString(*this);
}

bool RichString::operator==(const RichParameter& rb)
{
	return (rb.value().isString() && (pName == rb.name()) && (value().getString() == rb.value().getString()));
}

/**** RichMatrix44f Class ****/

RichMatrix44f::RichMatrix44f(
	const QString& nm,
	const Matrix44m& defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, Matrix44fValue(defval), desc, tltip, hidden, category)
{
}

RichMatrix44f::~RichMatrix44f()
{
}

QString RichMatrix44f::stringType() const
{
	return "RichMatrix44f";
}

RichMatrix44f* RichMatrix44f::clone() const
{
	return new RichMatrix44f(*this);
}

bool RichMatrix44f::operator==(const RichParameter& rb)
{
	return (rb.value().isMatrix44f() && (pName == rb.name()) && (value().getMatrix44f() == rb.value().getMatrix44f()));
}

/**** RichPoint3f Class ****/

RichPoint3f::RichPoint3f(
	const QString& nm,
	const Point3m& defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, Point3fValue(defval), desc, tltip, hidden, category)
{
}

RichPoint3f::~RichPoint3f()
{
}

QString RichPoint3f::stringType() const
{
	return "RichPoint3f";
}

RichPoint3f* RichPoint3f::clone() const
{
	return new RichPoint3f(*this);
}

bool RichPoint3f::operator==(const RichParameter& rb)
{
	return (rb.value().isPoint3f() && (pName == rb.name()) && (value().getPoint3f() == rb.value().getPoint3f()));
}

/**** RichShotf Class ****/

RichShotf::RichShotf(
	const QString& nm,
	const Shotm& defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, ShotfValue(defval), desc, tltip, hidden, category)
{
}

RichShotf::~RichShotf()
{
}

QString RichShotf::stringType() const
{
	return "RichShotf";
}

RichShotf* RichShotf::clone() const
{
	return new RichShotf(*this);
}

bool RichShotf::operator==(const RichParameter& rb)
{
	return (rb.value().isShotf() && (pName == rb.name()));
	// TODO REAL TEST OF EQUALITY // && (value().getShotf() == rb.value().getShotf()));
}

/**** RichColor Class ****/

RichColor::RichColor(
	const QString& nm,
	const QColor& defval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, ColorValue(defval), desc, tltip, hidden, category)
{
}

RichColor::~RichColor()
{
}

QString RichColor::stringType() const
{
	return "RichColor";
}

RichColor* RichColor::clone() const
{
	return new RichColor(*this);
}

bool RichColor::operator==(const RichParameter& rb)
{
	return (rb.value().isColor() && (pName == rb.name()) && (value().getColor() == rb.value().getColor()));
}

/**** RichAbsPerc Class ****/

RichAbsPerc::RichAbsPerc(
	const QString& nm,
	const Scalarm defval,
	const Scalarm minval,
	const Scalarm maxval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, FloatValue(defval), desc, tltip, hidden, category), min(minval), max(maxval)
{
}

RichAbsPerc::~RichAbsPerc()
{
}

QString RichAbsPerc::stringType() const
{
	return "RichAbsPerc";
}

QDomElement RichAbsPerc::fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip) const
{
	QDomElement parElem = RichParameter::fillToXMLDocument(doc, saveDescriptionAndTooltip);
	parElem.setAttribute("min", QString::number(min));
	parElem.setAttribute("max", QString::number(max));
	return parElem;
}

RichAbsPerc* RichAbsPerc::clone() const
{
	return new RichAbsPerc(*this);
}

bool RichAbsPerc::operator==(const RichParameter& rb)
{
	return (rb.isOfType<RichAbsPerc>() && (pName == rb.name()) && (value().getFloat() == rb.value().getFloat()));
}

/**** RichEnum Class ****/

RichEnum::RichEnum(
	const QString& nm,
	const int defval,
	const QStringList& values,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, IntValue(defval), desc, tltip, hidden, category), enumvalues(values)
{
}

RichEnum::~RichEnum()
{
}

QString RichEnum::stringType() const
{
	return "RichEnum";
}

QDomElement RichEnum::fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip) const
{
	QDomElement parElem = RichParameter::fillToXMLDocument(doc, saveDescriptionAndTooltip);
	parElem.setAttribute("enum_cardinality", enumvalues.size());
	for (int ii = 0; ii < enumvalues.size(); ++ii)
		parElem.setAttribute(QString("enum_val") + QString::number(ii), enumvalues.at(ii));
	return parElem;
}

RichEnum* RichEnum::clone() const
{
	return new RichEnum(*this);
}

bool RichEnum::operator==(const RichParameter& rb)
{
	return (rb.isOfType<RichEnum>() && (pName == rb.name()) && (value().getInt() == rb.value().getInt()));
}

/**** RichDynamicFloat Class ****/

RichDynamicFloat::RichDynamicFloat(
	const QString& nm,
	const Scalarm defval,
	const Scalarm minval,
	const Scalarm maxval,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, FloatValue(defval), desc, tltip, hidden, category), min(minval), max(maxval)
{
}

RichDynamicFloat::~RichDynamicFloat()
{
}

QString RichDynamicFloat::stringType() const
{
	return "RichDynamicFloat";
}

QDomElement RichDynamicFloat::fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip) const
{
	QDomElement parElem = RichParameter::fillToXMLDocument(doc, saveDescriptionAndTooltip);
	parElem.setAttribute("min", QString::number(min));
	parElem.setAttribute("max", QString::number(max));
	return parElem;
}

RichDynamicFloat* RichDynamicFloat::clone() const
{
	return new RichDynamicFloat(*this);
}

bool RichDynamicFloat::operator==(const RichParameter& rb)
{
	return (rb.isOfType<RichDynamicFloat>() && (pName == rb.name()) && (value().getFloat() == rb.value().getFloat()));
}

/**** RichOpenFile Class ****/

RichOpenFile::RichOpenFile(
	const QString& nm,
	const QString& directorydefval,
	const QStringList& exts,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, StringValue(directorydefval), desc, tltip, hidden, category), exts(exts)
{
}

RichOpenFile::~RichOpenFile()
{
}

QString RichOpenFile::stringType() const
{
	return "RichOpenFile";
}

QDomElement RichOpenFile::fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip) const
{
	QDomElement parElem = RichParameter::fillToXMLDocument(doc, saveDescriptionAndTooltip);
	parElem.setAttribute("exts_cardinality", exts.size());
	for (int ii = 0; ii < exts.size(); ++ii)
		parElem.setAttribute(QString("ext_val") + QString::number(ii), exts[ii]);
	return parElem;
}

RichOpenFile* RichOpenFile::clone() const
{
	return new RichOpenFile(*this);
}

bool RichOpenFile::operator==(const RichParameter& rb)
{
	return (rb.isOfType<RichOpenFile>() && (pName == rb.name()) && (value().getString() == rb.value().getString()));
}

/**** RichSaveFile Class ****/

RichSaveFile::RichSaveFile(
	const QString& nm,
	const QString& filedefval,
	const QString& ext,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, StringValue(filedefval), desc, tltip, hidden, category), ext(ext)
{
}

RichSaveFile::~RichSaveFile()
{
}

QString RichSaveFile::stringType() const
{
	return "RichSaveFile";
}

QDomElement RichSaveFile::fillToXMLDocument(QDomDocument& doc, bool saveDescriptionAndTooltip) const
{
	QDomElement parElem = RichParameter::fillToXMLDocument(doc, saveDescriptionAndTooltip);
	parElem.setAttribute("ext", ext);
	return parElem;
}

RichSaveFile* RichSaveFile::clone() const
{
	return new RichSaveFile(*this);
}

bool RichSaveFile::operator==(const RichParameter& rb)
{
	return (rb.isOfType<RichSaveFile>() && (pName == rb.name()) && (value().getString() == rb.value().getString()));
}

/**** RichMesh Class ****/

RichMesh::RichMesh(
	const QString& nm,
	unsigned int meshind,
	const MeshDocument* doc,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, IntValue(meshind), desc, tltip, hidden, category), meshdoc(doc)
{
}

RichMesh::RichMesh(
	const QString& nm,
	unsigned int meshind,
	const QString& desc,
	const QString& tltip,
	bool hidden,
	const QString& category) :
	RichParameter(nm, IntValue(meshind), desc, tltip, hidden, category), meshdoc(nullptr)
{
}

RichMesh::~RichMesh()
{
}

QString RichMesh::stringType() const
{
	return "RichMesh";
}

RichMesh* RichMesh::clone() const
{
	return new RichMesh(*this);
}

bool RichMesh::operator==(const RichParameter& rb)
{
	return (rb.isOfType<RichMesh>() && (pName == rb.name()) && (value().getInt() == rb.value().getInt()));
}

/**** RichParameterAdapter Class ****/

bool RichParameterAdapter::create(const QDomElement& np, RichParameter** par)
{
	QString name = np.attribute("name");
	QString type = np.attribute("type");
	QString desc = np.attribute("description");
	QString tooltip = np.attribute("tooltip");

	// qDebug("    Reading Param with name %s : %s", qUtf8Printable(name), qUtf8Printable(type));

	bool corrconv = false;
	if (type == "RichBool") {
		QString val = np.attribute("value").toLower();
		if ((val != QString("true")) && (val != QString("false")))
			return false;
		*par = new RichBool(name, np.attribute("value") != QString("false"), desc, tooltip);
		return true;
	}

	if (type == "RichInt") {
		int val = np.attribute("value").toInt(&corrconv);
		if (!corrconv)
			return false;
		*par = new RichInt(name, val, desc, tooltip);
		return true;
	}

	if (type == "RichFloat") {
		float val = np.attribute("value").toFloat(&corrconv);
		if (!corrconv)
			return false;
		*par = new RichFloat(name, val, desc, tooltip);
		return true;
	}

	if (type == "RichString") {
		*par = new RichString(name, np.attribute("value"), desc, tooltip);
		return true;
	}

	if (type == "RichAbsPerc") {
		float val = np.attribute("value").toFloat(&corrconv);
		if ((!corrconv) && (val >= 0.0f) && (val <= 100.0f))
			return false;
		float min = np.attribute("min").toFloat(&corrconv);
		if (!corrconv)
			return false;
		float max = np.attribute("max").toFloat(&corrconv);
		if (!corrconv)
			return false;
		*par = new RichAbsPerc(name, val, min, max, desc, tooltip);
		return true;
	}

	if (type == "RichColor") {
		unsigned int r = np.attribute("r").toUInt(&corrconv);
		if ((!corrconv) && (r <= 255))
			return false;
		unsigned int g = np.attribute("g").toUInt(&corrconv);
		if ((!corrconv) && (g <= 255))
			return false;
		unsigned int b = np.attribute("b").toUInt(&corrconv);
		if ((!corrconv) && (b <= 255))
			return false;
		unsigned int a = np.attribute("a").toUInt(&corrconv);
		if ((!corrconv) && (a <= 255))
			return false;
		QColor col(r, g, b, a);
		*par = new RichColor(name, col, desc, tooltip);
		return true;
	}

	if (type == "RichMatrix44f") {
		Matrix44m mm;
		for (int i = 0; i < 16; ++i)
		{
			Scalarm val = np.attribute(QString("val") + QString::number(i)).toFloat(&corrconv);
			if (!corrconv)
				return false;
			mm.V()[i] = val;
		}
		*par = new RichMatrix44f(name, mm, desc, tooltip);
		return true;
	}

	if (type == "RichEnum") {
		QStringList list;
		int enum_card = np.attribute(QString("enum_cardinality")).toUInt(&corrconv);
		if (!corrconv)
			return false;

		for (int i = 0; i < enum_card; ++i)
			list << np.attribute(QString("enum_val") + QString::number(i));

		int val = np.attribute("value").toInt(&corrconv);
		if ((!corrconv) && (val >= 0) && (val < enum_card))
			return false;
		*par = new RichEnum(name, val, list, desc, tooltip);
		return true;
	}

	if (type == "RichMesh") {
		int val = np.attribute("value").toInt(&corrconv);

		if (!corrconv)
			return false;

		*par = new RichMesh(name, val, desc, tooltip);
		return true;
	}

	if (type == "RichDynamicFloat") {
		float min = np.attribute("min").toFloat(&corrconv);
		if (!corrconv)
			return false;
		float max = np.attribute("max").toFloat(&corrconv);
		if (!corrconv)
			return false;

		float val = np.attribute("value").toFloat(&corrconv);
		if ((!corrconv) && (val >= min) && (val <= max))
			return false;

		*par = new RichDynamicFloat(name, val, min, max, desc, tooltip);
		return true;
	}

	if (type == "RichOpenFile") {
		QStringList list;
		int exts_card = np.attribute(QString("exts_cardinality")).toUInt(&corrconv);
		if (!corrconv)
			return false;

		for (int i = 0; i < exts_card; ++i)
			list << np.attribute(QString("exts_val") + QString::number(i));

		QString defdir = np.attribute("value");

		*par = new RichOpenFile(name, defdir, list, desc, tooltip);
		return true;
	}

	if (type == "RichSaveFile") {
		QString deffile = np.attribute("value");
		QString ext = np.attribute("ext");
		*par = new RichSaveFile(name, deffile, ext, desc, tooltip);
		return true;
	}

	if (type == "RichPoint3f") {
		vcg::Point3f val;
		val[0] = np.attribute("x").toFloat(&corrconv);
		if (!corrconv)
			return false;
		val[1] = np.attribute("y").toFloat(&corrconv);
		if (!corrconv)
			return false;
		val[2] = np.attribute("z").toFloat(&corrconv);
		if (!corrconv)
			return false;

		*par = new RichPoint3f(name, val, desc, tooltip);
		return true;
	}
	if (type == "RichShotf") {
		Shotm val;
		assert(0); //TODO!!!!
		*par = new RichShotf(name, val, desc, tooltip);
		return true;
	}

	return false;
}

//TODO!!!Add By Aden Hu
RichSceneParam::RichSceneParam(const QString& nm, const SceneParam& sceneParam) : RichParameter(nm, SceneValue(sceneParam))
{
	m_sceneParam = sceneParam;
}

RichSceneParam::RichSceneParam(
	const QString& nm,
	const PrintMachineBoxParam& bPrintParam,
	const QVector3D& inEye,
	const QVector3D& inUp,
	const QVector3D& inFront,
	const CMeshO& logoMesh,
	const QColor& backgroundColor
) : RichParameter(nm, SceneValue(bPrintParam, inEye, inUp, inFront, logoMesh, backgroundColor))
{
	m_sceneParam.m_printMachineBox = bPrintParam;
	m_sceneParam.m_eye = inEye;
	m_sceneParam.m_up = inUp;
	m_sceneParam.m_front = inFront;
	m_sceneParam.logoMesh = logoMesh;
	m_sceneParam.m_backgroundColor = backgroundColor;
}

RichSceneParam::RichSceneParam(
	const QString& nm,
	const float boxLength, const float boxWidth, const float boxHeight, const QColor& boxColor, const float boxLineWidth, const int bLineNum,
	const QVector3D& inEye,
	const QVector3D& inUp,
	const QVector3D& inFront,
	const CMeshO& logoMesh,
	const QColor& backgroundColor) :
	RichParameter(nm,
		SceneValue(boxLength, boxWidth, boxHeight, boxColor, boxLineWidth, bLineNum,
			inEye, inUp, inFront, logoMesh, backgroundColor))
{
	m_sceneParam.m_printMachineBox.m_length = boxLength;
	m_sceneParam.m_printMachineBox.m_width = boxWidth;
	m_sceneParam.m_printMachineBox.m_height = boxHeight;
	m_sceneParam.m_printMachineBox.m_color = boxColor;
	m_sceneParam.m_printMachineBox.m_lineWidth = boxLineWidth;
	m_sceneParam.m_printMachineBox.num = bLineNum;

	m_sceneParam.m_eye = inEye;
	m_sceneParam.m_up = inUp;
	m_sceneParam.m_front = inFront;
	m_sceneParam.logoMesh = logoMesh;
	m_sceneParam.m_backgroundColor = backgroundColor;
}

RichSceneParam::~RichSceneParam()
{

}


QString RichSceneParam::stringType() const
{
	return QString("RichSceneParam");
}
RichSceneParam* RichSceneParam::clone() const
{
	return new RichSceneParam(*this);
}

bool RichSceneParam::operator==(const RichParameter& rp)
{
	return (rp.value().isScene() && (pName == rp.name()) && (value().getSceneParam() == rp.value().getSceneParam()));
}
