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

#ifndef ML_STD_PAR_DIALOG
#define ML_STD_PAR_DIALOG

#include<QDockWidget>
#include <common/plugins/interfaces/filter_plugin.h>
#include <common/ml_document/mesh_model_state.h>
#include "rich_parameter_gui/richparameterlistframe.h"

class MainWindow;
class MeshlabStdDialog : public QDockWidget
{
	Q_OBJECT

public:
	MeshlabStdDialog(QWidget *p = NULL);
	~MeshlabStdDialog();

	void createFrame();
	void loadFrameContent();

	bool showAutoDialog(FilterPlugin *mfi, MeshModel *mm, MeshDocument * md, QAction *q, MainWindow *mwi, QWidget *gla=0);
	bool isPreviewable();

public slots:
	void closeClick();

private slots:
	void applyClick();
	void resetValues();
	void toggleHelp();
	void togglePreview();
	void applyDynamic();
	void changeCurrentMesh(int meshInd);

public:
	QFrame *qf;
	RichParameterListFrame *stdParFrame;
	QAction *curAction;
	MeshModelState meshState;
	MeshModelState meshCacheState;
	QCheckBox *previewCB;

	void closeEvent ( QCloseEvent * event );

	uint curmask;
	MeshModel *curModel;
	MeshDocument * curMeshDoc;
	FilterPlugin *curmfi;
	MainWindow *curmwi;
	QWidget * curgla;
	RichParameterList curParSet;
	RichParameterList prevParSet;
	bool validcache;

};

#endif
