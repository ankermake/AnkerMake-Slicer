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

#include "ui_save_snapshot_dialog.h"
#include "save_snapshot_dialog.h"
#include <QDialogButtonBox>

SaveSnapshotDialog::SaveSnapshotDialog(QWidget * parent): 
	QDialog(parent),
	ui(new Ui::SaveSnapShotDialog)
{
	ui->setupUi(this);
	//connect(
	//	ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), 
	//	this, SLOT(accept()));
}

void SaveSnapshotDialog::setValues(const SnapshotSetting& ss)
{
	settings=ss;
	ui->baseNameLineEdit->setText(settings.basename);
	ui->outDirLineEdit->setText(settings.outdir);
	ui->resolutionSpinBox->setValue(settings.resolution);
	ui->counterSpinBox->setValue(settings.counter);
	ui->backgroundComboBox->setCurrentIndex(settings.background);
	ui->alllayersCheckBox->setChecked(settings.snapAllLayers);
	ui->tiledSaveCheckBox->setChecked(settings.tiledSave);
	ui->addToRastersCheckBox->setChecked(settings.addToRasters);
}

SnapshotSetting SaveSnapshotDialog::getValues()
{
	settings.basename=ui->baseNameLineEdit->text();
	settings.outdir=ui->outDirLineEdit->text();
	settings.counter=ui->counterSpinBox->value();
	settings.resolution=ui->resolutionSpinBox->value();
	settings.background = ui->backgroundComboBox->currentIndex();
	settings.snapAllLayers=ui->alllayersCheckBox->isChecked();
	settings.tiledSave=ui->tiledSaveCheckBox->isChecked();
	settings.addToRasters=ui->addToRastersCheckBox->isChecked();
	return settings;
}



SaveSnapshotDialog::~SaveSnapshotDialog()
{
	delete ui;
}

void SaveSnapshotDialog::on_browseDir_clicked()
{
	QFileDialog fd(0,"Choose output directory");
	fd.setFileMode(QFileDialog::DirectoryOnly);

	QStringList selection;
	if (fd.exec())
	{
		selection = fd.selectedFiles();
		ui->outDirLineEdit->setText(selection.at(0));	
	}
}
