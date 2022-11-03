/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004-2020                                           \/)\/    *
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

#include "richparameterlistframe.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QClipboard>
#include <QColorDialog>
#include <QToolBar>


using namespace vcg;

RichParameterListFrame::RichParameterListFrame(
		const RichParameterList& curParSet,
		const RichParameterList& defParSet,
		QWidget* p,
		QWidget* gla) :
	QFrame(p), gla(gla), hiddenFrame(nullptr)
{
	loadFrameContent(curParSet, defParSet);
}

RichParameterListFrame::RichParameterListFrame(
		const RichParameterList& curParSet,
		QWidget* p,
		QWidget* gla) :
	QFrame(p), gla(gla)
{
	loadFrameContent(curParSet);
}

RichParameterListFrame::RichParameterListFrame(
		const RichParameter& curPar,
		const RichParameter& defPar,
		QWidget* p,
		QWidget* gla) :
	QFrame(p), gla(gla)
{
	loadFrameContent(curPar, defPar);
}

RichParameterListFrame::~RichParameterListFrame()
{
}

/**
 * @brief RichParameterListFrame::readValues
 * From GUI to RichParameterList
 * @param curParSet
 */
void RichParameterListFrame::writeValuesOnParameterList(RichParameterList &curParSet)
{
	assert(curParSet.size() == (unsigned int)stdfieldwidgets.size());
	for(auto& p : stdfieldwidgets) {
		curParSet.setValue(p.first,(p.second)->widgetValue());
	}
}

void RichParameterListFrame::resetValues()
{
	for(auto& p : stdfieldwidgets) {
		p.second->resetValue();
	}
}

void RichParameterListFrame::toggleHelp()
{
	for(int i = 0; i < helpList.count(); i++)
		helpList.at(i)->setVisible(!helpList.at(i)->isVisible()) ;
	updateGeometry();
}

unsigned int RichParameterListFrame::size() const
{
	return stdfieldwidgets.size();
}

RichParameterListFrame::iterator RichParameterListFrame::begin()
{
	return stdfieldwidgets.begin();
}

RichParameterListFrame::iterator RichParameterListFrame::end()
{
	return stdfieldwidgets.end();
}

void RichParameterListFrame::toggleAdvancedParameters()
{
	if (hiddenFrame) {
		if (hiddenFrame->isVisible()){
			hiddenFrame->setVisible(false);
            showHiddenFramePushButton->setText("V");
		}
		else {
			hiddenFrame->setVisible(true);
            showHiddenFramePushButton->setText("A");
		}
		QFrame* p = dynamic_cast<QFrame*>(parent());
		if (p){
			p->setMinimumSize(p->sizeHint());
		}
	}
}

void RichParameterListFrame::loadFrameContent(
		const RichParameterList& curParSet,
		const RichParameterList& defParSet)
{
	if(layout())
		delete layout();
	QGridLayout* glay = new QGridLayout(this);


	//collect parameters per category
	std::map<QString, std::vector<const RichParameter*>> visibleParameters;
	std::map<QString, std::vector<const RichParameter*>> hiddenParameters;

	for(const RichParameter& fpi : curParSet) {
		if (!fpi.isAdvanced()){
			visibleParameters[fpi.category()].push_back(&fpi);
		}
		else {
			hiddenParameters[fpi.category()].push_back(&fpi);
		}
	}

	int i = 0;
	//parameters are organized into categories
	for (const auto& p : visibleParameters) {
		//if not the default category, the category name must be printed in the dialog
		//before the list of parameter widgets
		if (!p.first.isEmpty()) {
			QString labltext = "<P><b>" + p.first + ":</b></P>";
			QLabel* l = new QLabel(labltext, this);
			glay->addWidget(l,i++,0,Qt::AlignLeft);
		}
		//put the parameter widgets into the grid layout
		for (const RichParameter* fpi : p.second){
			const RichParameter& defrp = defParSet.getParameterByName(fpi->name());
			RichParameterWidget* wd = createWidgetFromRichParameter(this, *fpi, defrp);
			stdfieldwidgets[fpi->name()] = wd;
			helpList.push_back(wd->helpLab);
			wd->addWidgetToGridLayout(glay,i++);
		}
	}
	if (hiddenParameters.size() > 0){
		hiddenFrame = new QFrame(this);
		hiddenFrame->setContentsMargins(0,0,0,0);
		QGridLayout* flay = new QGridLayout();
		hiddenFrame->setLayout(flay);

		int j = 0;
		for (const auto& p : hiddenParameters) {
			if (!p.first.isEmpty()) {
				QString labltext = "<P><b>" + p.first + ":</b></P>";
				QLabel* l = new QLabel(labltext, this);
				flay->addWidget(l,j++,0,Qt::AlignLeft);
			}
			for (const RichParameter* fpi : p.second){
				const RichParameter& defrp = defParSet.getParameterByName(fpi->name());
				RichParameterWidget* wd = createWidgetFromRichParameter(this, *fpi, defrp);
				stdfieldwidgets[fpi->name()] = wd;
				helpList.push_back(wd->helpLab);
				wd->addWidgetToGridLayout(flay,j++);
			}
		}
		//hiddenFrame->setMinimumSize(hiddenFrame->sizeHint());
		glay->addWidget(hiddenFrame, i++, 0, 1, 3);
		hiddenFrame->setVisible(false);
		showHiddenFramePushButton = new QPushButton("", this);
		showHiddenFramePushButton->setFlat(true);
        showHiddenFramePushButton->setText("V");
		glay->addWidget(showHiddenFramePushButton, i++, 0, 1, 3);
		connect(showHiddenFramePushButton, SIGNAL(clicked()), this, SLOT(toggleAdvancedParameters()));
	}
	setLayout(glay);
}

/* creates widgets for the standard parameters */
void RichParameterListFrame::loadFrameContent(const RichParameterList &curParSet)
{
	loadFrameContent(curParSet, curParSet);
}

void RichParameterListFrame::loadFrameContent(const RichParameter& curPar, const RichParameter& defPar)
{
	RichParameterList crpl;
	crpl.addParam(curPar);
	RichParameterList drpl;
	drpl.addParam(defPar);
	loadFrameContent(crpl, drpl);
}

RichParameterWidget* RichParameterListFrame::createWidgetFromRichParameter(
		QWidget* parent,
		const RichParameter& pd,
		const RichParameter& def)
{
	if (pd.isOfType<RichAbsPerc>()){
		return new AbsPercWidget(parent, (const RichAbsPerc&)pd, (const RichAbsPerc&)def);
	}
	else if (pd.isOfType<RichDynamicFloat>()){
		return new DynamicFloatWidget(parent, (const RichDynamicFloat&)pd, (const RichDynamicFloat&)def);
	}
	else if (pd.isOfType<RichEnum>()){
		return new EnumWidget(parent, (const RichEnum&)pd, (const RichEnum&)def);
	}
	else if (pd.isOfType<RichBool>()){
		return new BoolWidget(parent, (const RichBool&)pd, (const RichBool&)def);
	}
	else if (pd.isOfType<RichInt>()){
		return new IntWidget(parent, (const RichInt&)pd, (const RichInt&)def);
	}
	else if (pd.isOfType<RichFloat>()){
		return new FloatWidget(parent, (const RichFloat&)pd, (const RichFloat&)def);
	}
	else if (pd.isOfType<RichString>()){
		return new StringWidget(parent, (const RichString&)pd, (const RichString&)def);
	}
	else if (pd.isOfType<RichMatrix44f>()){
		return new Matrix44fWidget(parent, (const RichMatrix44f&)pd, (const RichMatrix44f&)def, reinterpret_cast<RichParameterListFrame*>(parent)->gla);
	}
	else if (pd.isOfType<RichPoint3f>()){
		return new Point3fWidget(parent, (const RichPoint3f&)pd, (const RichPoint3f&)def, reinterpret_cast<RichParameterListFrame*>(parent)->gla);
	}
	else if (pd.isOfType<RichShotf>()){
		return new ShotfWidget(parent, (const RichShotf&)pd, (const RichShotf&)def, reinterpret_cast<RichParameterListFrame*>(parent)->gla);
	}
	else if (pd.isOfType<RichColor>()){
		return new ColorWidget(parent, (const RichColor&)pd, (const RichColor&)def);
	}
	else if (pd.isOfType<RichOpenFile>()){
		return new OpenFileWidget(parent, (const RichOpenFile&)pd, (const RichOpenFile&)def);
	}
	else if (pd.isOfType<RichSaveFile>()){
		return new SaveFileWidget(parent, (const RichSaveFile&)pd, (const RichSaveFile&)def);
	}
	else if (pd.isOfType<RichMesh>()){
		return new MeshWidget(parent, (const RichMesh&)pd, (const RichMesh&)def);
	}
	else {
		std::cerr << "RichParameter type not supported for widget creation.\n";
		assert(0);
		return nullptr;
	}
}
