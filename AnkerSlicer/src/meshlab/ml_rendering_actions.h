/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
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

#ifndef ML_RENDERING_ACTIONS_H
#define ML_RENDERING_ACTIONS_H

#include <QAction>
#include "../common/ml_shared_data_context/ml_scene_gl_shared_data_context.h"


class MLRenderingAction : public QAction
{
    Q_OBJECT
public:
    MLRenderingAction(QObject* parent);
    MLRenderingAction(int meshid,QObject* parent);

	virtual void createSisterAction(MLRenderingAction*& sisteract,QObject* par) = 0;
    virtual void updateRenderingData(MLRenderingData& rd) = 0;
    virtual bool isRenderingDataEnabled(const MLRenderingData& rd) const = 0;
    virtual bool isVisibleConditionValid(MeshModel*) const {return true;}
	virtual void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY /*pr*/) {}
	
	int meshId() const;
    void setMeshId(int meshid);
	bool isSameType(const MLRenderingAction& act) const;


protected:
    bool isRenderingDataEnabled(MLRenderingData::PRIMITIVE_MODALITY pm,MLRenderingData::ATT_NAMES att,const MLRenderingData& rd) const;
};

class MLRenderingGlobalAction : public QAction
{
	Q_OBJECT
public:
	MLRenderingGlobalAction(const QString& text,const QIcon& icon,QObject* par);

	void addMainAction(MLRenderingAction* act);
	void addRelatedAction(MLRenderingAction* act);

	QList<MLRenderingAction*>& mainActions();
	QList<MLRenderingAction*>& relatedActions();

private:
	QList<MLRenderingAction*> _mainactions;
	QList<MLRenderingAction*> _relatedactions;
};

class MLRenderingBBoxAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingBBoxAction(QObject* parent);
    MLRenderingBBoxAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    //bool isOnOffOption(MLRenderingAction* onact = 0,MLRenderingAction* offact = 0);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingPointsAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPointsAction(QObject* parent);
    MLRenderingPointsAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingWireAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingWireAction(QObject* parent);
    MLRenderingWireAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingSolidAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingSolidAction(QObject* parent);
    MLRenderingSolidAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingFauxEdgeWireAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingFauxEdgeWireAction(QObject* parent);
    MLRenderingFauxEdgeWireAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    bool isVisibleConditionValid(MeshModel*) const;
};

class MLRenderingPerFaceNormalAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerFaceNormalAction(QObject* parent);
    MLRenderingPerFaceNormalAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
	bool isVisibleConditionValid(MeshModel*) const;

};

class MLRenderingPerVertexNormalAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerVertexNormalAction(MLRenderingData::PRIMITIVE_MODALITY pm,QObject* parent);
    MLRenderingPerVertexNormalAction(MLRenderingData::PRIMITIVE_MODALITY pm,int meshid,QObject* parent);

	void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY pr);
	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
	bool isVisibleConditionValid(MeshModel*) const;
private:
    MLRenderingData::PRIMITIVE_MODALITY _pm;
};

class MLRenderingPointsDotAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPointsDotAction(QObject* parent);
    MLRenderingPointsDotAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};


class MLRenderingPerVertTextCoordAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerVertTextCoordAction(MLRenderingData::PRIMITIVE_MODALITY pm,QObject* parent);
    MLRenderingPerVertTextCoordAction(MLRenderingData::PRIMITIVE_MODALITY pm,int meshid,QObject* parent);

	void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY pr);
	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    bool isVisibleConditionValid(MeshModel*) const;
private:
    MLRenderingData::PRIMITIVE_MODALITY _pm;
};

class MLRenderingPerWedgeTextCoordAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerWedgeTextCoordAction(QObject* parent);
    MLRenderingPerWedgeTextCoordAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    bool isVisibleConditionValid(MeshModel*) const;
};

class MLRenderingSingleLightingAction : public MLRenderingAction
{
	Q_OBJECT
public:
	MLRenderingSingleLightingAction(QObject* parent);
	MLRenderingSingleLightingAction(int meshid, QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
	void updateRenderingData(MLRenderingData& rd);
	bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingDoubleLightingAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingDoubleLightingAction(QObject* parent);
    MLRenderingDoubleLightingAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingFancyLightingAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingFancyLightingAction(QObject* parent);
    MLRenderingFancyLightingAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingFaceCullAction : public MLRenderingAction
{
	Q_OBJECT
public:
	MLRenderingFaceCullAction(QObject* parent);
	MLRenderingFaceCullAction(int meshid, QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
	void updateRenderingData(MLRenderingData& rd);
	bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingNoShadingAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingNoShadingAction(MLRenderingData::PRIMITIVE_MODALITY pm,QObject* parent);
    MLRenderingNoShadingAction(MLRenderingData::PRIMITIVE_MODALITY pm,int meshid,QObject* parent);

	void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY pr);
	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
private:
    MLRenderingData::PRIMITIVE_MODALITY _pm;
};



class MLRenderingPerMeshColorAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerMeshColorAction(MLRenderingData::PRIMITIVE_MODALITY pm,QObject* parent);
    MLRenderingPerMeshColorAction(MLRenderingData::PRIMITIVE_MODALITY pm,int meshid,QObject* parent);

	void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY pr);
	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    void setColor(const QColor& col);
    void setColor(const vcg::Color4b& col);

    vcg::Color4b& getColor();
private:
    vcg::Color4b _col;
    MLRenderingData::PRIMITIVE_MODALITY _pm;
};

class MLRenderingBBoxPerMeshColorAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingBBoxPerMeshColorAction(QObject* parent);
    MLRenderingBBoxPerMeshColorAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    void setColor(const QColor& col);
    void setColor(const vcg::Color4b& col);

    vcg::Color4b& getColor();
private:
    vcg::Color4b _col;
};

class MLRenderingPerVertexColorAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerVertexColorAction(MLRenderingData::PRIMITIVE_MODALITY pm,QObject* parent);
    MLRenderingPerVertexColorAction(MLRenderingData::PRIMITIVE_MODALITY pm,int meshid,QObject* parent);

	void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY pr);
	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
	bool isVisibleConditionValid(MeshModel * mm) const;
private:
    MLRenderingData::PRIMITIVE_MODALITY _pm;
};

class MLRenderingPerFaceColorAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingPerFaceColorAction(QObject* parent);
    MLRenderingPerFaceColorAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    bool isVisibleConditionValid(MeshModel*) const;
};

class MLRenderingUserDefinedGeneralColorAction : public MLRenderingAction
{
	Q_OBJECT
public:
	MLRenderingUserDefinedGeneralColorAction(QObject* parent);
	MLRenderingUserDefinedGeneralColorAction(int meshid, QObject* parent);
	MLRenderingUserDefinedGeneralColorAction(MLRenderingUserDefinedGeneralColorAction* origin, QObject * par);

	virtual void createSisterAction(MLRenderingAction*& sisteract, QObject* par) = 0;
	virtual void updateRenderingData(MLRenderingData& rd) = 0;
	virtual bool isRenderingDataEnabled(const MLRenderingData& rd) const = 0;

	void setColor(const vcg::Color4b& col);
	void setColor(const QColor& col);

	vcg::Color4b& getColor();
	virtual void readColor(const MLRenderingData& rd, vcg::Color4b& col) = 0;
protected:
	vcg::Color4b _coluser;
};

class MLRenderingUserDefinedColorAction : public MLRenderingUserDefinedGeneralColorAction
{
    Q_OBJECT
public:
    MLRenderingUserDefinedColorAction(MLRenderingData::PRIMITIVE_MODALITY pm,QObject* parent);
    MLRenderingUserDefinedColorAction(MLRenderingData::PRIMITIVE_MODALITY pm,int meshid,QObject* parent);
	MLRenderingUserDefinedColorAction(MLRenderingUserDefinedColorAction* origin, QObject * par);

	void switchPrimitive(MLRenderingData::PRIMITIVE_MODALITY pr);
	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;

	void readColor(const MLRenderingData& rd, vcg::Color4b& col);
private:
    MLRenderingData::PRIMITIVE_MODALITY _pm;
};

class MLRenderingBBoxUserDefinedColorAction : public MLRenderingUserDefinedGeneralColorAction
{
    Q_OBJECT
public:
    MLRenderingBBoxUserDefinedColorAction(QObject* parent);
    MLRenderingBBoxUserDefinedColorAction(int meshid,QObject* parent);
	MLRenderingBBoxUserDefinedColorAction(MLRenderingBBoxUserDefinedColorAction* origin, QObject * par);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
 
	void readColor(const MLRenderingData& rd, vcg::Color4b& col);
};


class MLRenderingFloatAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingFloatAction(QObject* parent);
    MLRenderingFloatAction(int meshid,QObject* parent);

    void setValue(float value) {_value = value;}
    float getValue() {return _value;}

	virtual void createSisterAction(MLRenderingAction*& sisteract, QObject* par) = 0;
    virtual void updateRenderingData(MLRenderingData& rd) = 0;
    virtual bool isRenderingDataEnabled(const MLRenderingData& rd) const = 0;
    virtual float getValueFromRenderingData(const MLRenderingData& rd) const = 0;
protected:
    float _value;
};

class MLRenderingPointsSizeAction : public MLRenderingFloatAction
{
    Q_OBJECT
public:
    MLRenderingPointsSizeAction(QObject* parent);
    MLRenderingPointsSizeAction(int meshid,QObject* parent);
	MLRenderingPointsSizeAction(MLRenderingPointsSizeAction* origin,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    float getValueFromRenderingData(const MLRenderingData& rd) const;
};

class MLRenderingWireWidthAction : public MLRenderingFloatAction
{
    Q_OBJECT
public:
    MLRenderingWireWidthAction(QObject* parent);
    MLRenderingWireWidthAction(int meshid,QObject* parent);
	MLRenderingWireWidthAction(MLRenderingWireWidthAction* origin, QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
    float getValueFromRenderingData(const MLRenderingData& rd) const;
};

class MLRenderingSelectionAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingSelectionAction(QObject* parent);
    MLRenderingSelectionAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingVertSelectionAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingVertSelectionAction(QObject* parent);
    MLRenderingVertSelectionAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingFaceSelectionAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingFaceSelectionAction(QObject* parent);
    MLRenderingFaceSelectionAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingEdgeDecoratorAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingEdgeDecoratorAction(QObject* parent);
    MLRenderingEdgeDecoratorAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingEdgeBoundaryAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingEdgeBoundaryAction(QObject* parent);
    MLRenderingEdgeBoundaryAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingFaceBoundaryAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingFaceBoundaryAction(QObject* parent);
    MLRenderingFaceBoundaryAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingEdgeManifoldAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingEdgeManifoldAction(QObject* parent);
    MLRenderingEdgeManifoldAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingVertManifoldAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingVertManifoldAction(QObject* parent);
    MLRenderingVertManifoldAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};


class MLRenderingTexBorderAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingTexBorderAction(QObject* parent);
    MLRenderingTexBorderAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

//class MLRenderingLightColorAction : public MLRenderingAction
//{
//    Q_OBJECT
//public:
//    MLRenderingLightColorAction(QObject* parent);
//    MLRenderingLightColorAction(int meshid,QObject* parent);
//
//    void updateRenderingData(MLRenderingData& rd);
//    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
//};

class MLRenderingDotAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingDotAction(QObject* parent);
    MLRenderingDotAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

class MLRenderingBBoxQuotedInfoAction : public MLRenderingAction
{
    Q_OBJECT
public:
    MLRenderingBBoxQuotedInfoAction(QObject* parent);
    MLRenderingBBoxQuotedInfoAction(int meshid,QObject* parent);

	void createSisterAction(MLRenderingAction*& sisteract, QObject* par);
    void updateRenderingData(MLRenderingData& rd);
    bool isRenderingDataEnabled(const MLRenderingData& rd) const;
};

#endif
