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

#ifndef MESHLAB_EDIT_PLUGIN_H
#define MESHLAB_EDIT_PLUGIN_H

#include <QTabletEvent>

#include "meshlab_plugin_logger.h"
#include "meshlab_plugin.h"
#include "../../ml_document/mesh_document.h"


class GLArea;

/**
 * @brief The EditTool class is used to provide tools that needs some kind of
 * interaction with the mesh. Editing tools are exclusive (only one at a time) 
 * and can grab the mouse events and customize the rendering process.
 **/

class EditTool : public MeshLabPluginLogger
{
public:
	EditTool() : MeshLabPluginLogger() {}
	virtual ~EditTool() {}

	//should return a sentence describing what the editing tool does
	static const QString info();

    virtual void suggestedRenderingData(MeshModel &/*m*/) {}

	// Called when the user press the first time the button
    virtual bool startEdit(MeshModel &/*m*/) { return true; }
    virtual bool startEdit(MeshDocument &md)
	{
		//assert(NULL != md.mm());
		if (md.mm() != NULL)
            return (startEdit(*(md.mm())));
		else return false;
	}
	// Called when the user press the second time the button
    virtual void endEdit(MeshModel &/*m*/) {}
    virtual void endEdit(MeshDocument &/*m*/) {}

	// There are two classes of editing tools, the one that works on a single layer at a time
	// and the ones that works on all layers and have to manage in a correct way the action of changing the current layer.
	// For the edit tools that works ona single layer changing the layer means the restart of the edit tool.
	virtual bool isSingleMeshEdit() const { return true; }

	// Called when the user changes the selected layer
	//by default it calls end edit with the layer that was selected and start with the new layer that is
	//selected.  This ensures that plugins who don't support layers do not get sent pointers to meshes
	//they are not expecting.
	// If your editing plugins is not singleMesh you MUST reimplement this to correctly handle the change of layer.
    virtual void layerChanged(MeshDocument &md, MeshModel &oldMeshModel)
	{
		assert(this->isSingleMeshEdit());
        endEdit(oldMeshModel);
        startEdit(md);
	}

	virtual void decorate(MeshModel &m, GLArea *parent, QPainter * /*p*/) { decorate(m, parent); }
	virtual void decorate(MeshModel &/*m*/, GLArea * /*parent*/) {}

	virtual void keyReleaseEvent(QKeyEvent *, MeshModel &/*m*/, GLArea *) {}
	virtual void keyPressEvent(QKeyEvent *, MeshModel &/*m*/, GLArea *) {}
	virtual void wheelEvent(QWheelEvent*, MeshModel &/*m*/, GLArea *) {}
	virtual void tabletEvent(QTabletEvent * e, MeshModel &/*m*/, GLArea *) { e->ignore(); }
    virtual void mousePressEvent(QMouseEvent *event, MeshModel &/*m*/, GLArea *) {};
    virtual void mouseMoveEvent(QMouseEvent *event, MeshModel &/*m*/, GLArea *) {};
    virtual void mouseReleaseEvent(QMouseEvent *event, MeshModel &/*m*/, GLArea *) {};
};


/** 
 * @brief The EditPlugin class  is used to generate an action for each
 * starting of an editing tool.
 *
 * This is needed because editing tools have a internal state, so if you want
 * to have an editing tool for two different documents you have to instance 
 * two objects. This class is used by the framework to generate an independent 
 * EditTool for each document.
 */
class EditPlugin : public MeshLabPlugin
{
public:
	EditPlugin() {}
	virtual ~EditPlugin() {}

	//gets a list of actions available from this plugin
	virtual std::list<QAction *> actions() const {return actionList;};

	//get the edit tool for the given action
	virtual EditTool* getEditTool(const QAction *) = 0;

	//get the description for the given action
	virtual QString getEditToolDescription(const QAction *) = 0;

protected:
	std::list<QAction*> actionList;
};

#define EDIT_PLUGIN_IID  "vcg.meshlab.EditPlugin/1.0"
Q_DECLARE_INTERFACE(EditPlugin, EDIT_PLUGIN_IID)


#endif // MESHLAB_EDIT_PLUGIN_H
