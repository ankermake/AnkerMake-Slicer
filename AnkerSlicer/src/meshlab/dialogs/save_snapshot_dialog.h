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

#ifndef MESHLAB_SAVE_SNAPSHOT_DIALOG_H
#define MESHLAB_SAVE_SNAPSHOT_DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "snapshotsetting.h"

namespace Ui 
{
	class SaveSnapShotDialog;
} 

class SaveSnapshotDialog: public QDialog
{
Q_OBJECT

public:
	SaveSnapshotDialog(QWidget *parent = 0);
	~SaveSnapshotDialog();
	void setValues(const SnapshotSetting &ss);
	SnapshotSetting getValues();
	
private slots:
	void on_browseDir_clicked();
private:
	Ui::SaveSnapShotDialog* ui;
	SnapshotSetting settings;
};

#endif //MESHLAB_SAVE_SNAPSHOT_DIALOG_H
