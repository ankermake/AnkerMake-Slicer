#include "CHModelTransformParamsSetUI.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QValidator"
#include <QSignalMapper>



CHModelTransformParamsSetUI::CHModelTransformParamsSetUI(QWidget *parent)
	: QDialog(parent)
{
	move(120, 100);
	setFixedSize(300, 400);
	setWindowTitle(QString::fromLocal8Bit("??????"));
	setModal(false); 
	setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
	//setWindowFlags(Qt::WindowMinimizeButtonHint);
	//setWindowFlags(/*!Qt::WindowCloseButtonHint &*/ Qt::WindowStaysOnTopHint);


	
	//????????
	/*QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(0x00, 0xff, 0x00, 0x00));
	setPalette(pal);*/

	//setWindowOpacity(0.7);

	//this->setAttribute(Qt::WA_TranslucentBackground, true);

	QGroupBox* groupbox = createParamsSetUI();

	QVBoxLayout* vlayout = new QVBoxLayout(this);
	vlayout->addWidget(groupbox);
	vlayout->addStretch(1);

}

CHModelTransformParamsSetUI::~CHModelTransformParamsSetUI()
{
	
}

void CHModelTransformParamsSetUI::showAxisChanged()
{
	if (m_showLocalAxis->checkState() == Qt::Checked)
	{
		emit showAxis(true);
	}
	else
	{
		emit showAxis(false);
	}
}

void CHModelTransformParamsSetUI::receiveParams(vector<float> params)
{
	//???????????
	if (params[0] < 0)
	{
		m_xMirror->setCheckState(Qt::Checked);
	}
	else
	{
		m_xMirror->setCheckState(Qt::Unchecked);
	}
	if (params[1] < 0)
	{
		m_yMirror->setCheckState(Qt::Checked);
	}
	else
	{
		m_yMirror->setCheckState(Qt::Unchecked);
	}
	if (params[2] < 0)
	{
		m_zMirror->setCheckState(Qt::Checked);
	}
	else
	{
		m_zMirror->setCheckState(Qt::Unchecked);
	}
	m_xScall->setText(QString::number(fabs(params[0]), 'f', 2));
	m_yScall->setText(QString::number(fabs(params[1]), 'f', 2));
	m_zScall->setText(QString::number(fabs(params[2]), 'f', 2));

	for (int i = 3; i < m_allLineEdits.size(); i++)
	{
		m_allLineEdits[i]->setText(QString::number(params[i], 'f', 2));
	}	
}

void CHModelTransformParamsSetUI::submit()
{
	vector<float> params(9);
	for (int i = 0; i < m_allLineEdits.size(); i++)
	{
		params[i] = m_allLineEdits[i]->text().toFloat();
	}

	//??????
	if (m_xMirror->checkState() == Qt::Checked)
	{
		params[0] = -params[0];
	}
	if (m_yMirror->checkState() == Qt::Checked)
	{
		params[1] = -params[1];
	}
	if (m_zMirror->checkState() == Qt::Checked)
	{
		params[2] = -params[2];
	}

	//????
	adjustSingleAngle(params[3]);
	adjustSingleAngle(params[4]);
	adjustSingleAngle(params[5]);
	receiveParams(params);

	emit sendParams(params);
}

void CHModelTransformParamsSetUI::sameScale(int index)
{
	if (m_lockTheSameScall->checkState() == Qt::Checked)
	{
		for (int i = 0; i < m_allScaleEdits.size(); i++)
		{
			m_allScaleEdits[i]->setText(m_allScaleEdits[index]->text());
		}
	}

	submit();
}

QGroupBox * CHModelTransformParamsSetUI::createParamsSetUI()
{
	QGroupBox* groupbox = new QGroupBox(this);
	groupbox->setTitle(QString::fromLocal8Bit("????"));
	groupbox->setAlignment(Qt::AlignHCenter);

	QGridLayout* layout = new QGridLayout(groupbox);
	//layout->setSpacing(20);//????

	QLabel* xLabel = new QLabel(QString::fromLocal8Bit("X"));
	xLabel->setAlignment(Qt::AlignHCenter);
	QLabel* yLabel = new QLabel(QString::fromLocal8Bit("Y"));
	yLabel->setAlignment(Qt::AlignHCenter);
	QLabel* zLabel = new QLabel(QString::fromLocal8Bit("Z"));
	zLabel->setAlignment(Qt::AlignHCenter);

	QLabel* mirror = new QLabel(QString::fromLocal8Bit("???"));
	m_xMirror = new QCheckBox(this);
	m_yMirror = new QCheckBox(this);
	m_zMirror = new QCheckBox(this);	

	QLabel* scale = new QLabel(QString::fromLocal8Bit("???"));
	m_xScall = new QLineEdit(this);
	m_yScall = new QLineEdit(this);
	m_zScall = new QLineEdit(this);

	QLabel* rot = new QLabel(QString::fromLocal8Bit("???"));
	m_xRot= new QLineEdit(this);
	m_yRot = new QLineEdit(this);
	m_zRot = new QLineEdit(this);
	QLabel* rotUnit = new QLabel(QString::fromLocal8Bit("?"));
	
	QLabel* location = new QLabel(QString::fromLocal8Bit("???"));
	m_xPos = new QLineEdit(this);
	m_yPos = new QLineEdit(this);
	m_zPos = new QLineEdit(this);
	QLabel* locUnit = new QLabel(QString::fromLocal8Bit("mm"));

	QLabel* lock = new QLabel(QString::fromLocal8Bit("???????"));
	m_lockTheSameScall = new QCheckBox(this);
	m_lockTheSameScall->setCheckState(Qt::Checked);
	QLabel* showAxis = new QLabel(QString::fromLocal8Bit("????????"));
	m_showLocalAxis = new QCheckBox(this);
	//m_showLocalAxis->setCheckState(Qt::Checked);

	m_allScaleEdits.push_back(m_xScall);
	m_allScaleEdits.push_back(m_yScall);
	m_allScaleEdits.push_back(m_zScall);
	m_allLineEdits.push_back(m_xScall);
	m_allLineEdits.push_back(m_yScall);
	m_allLineEdits.push_back(m_zScall);
	m_allLineEdits.push_back(m_xRot);
	m_allLineEdits.push_back(m_yRot);
	m_allLineEdits.push_back(m_zRot);
	m_allLineEdits.push_back(m_xPos);
	m_allLineEdits.push_back(m_yPos);
	m_allLineEdits.push_back(m_zPos);
	QDoubleValidator* dv = new QDoubleValidator();
	dv->setDecimals(2);
	for (int i = 0; i < m_allLineEdits.size(); i++)
	{
		m_allLineEdits[i]->setValidator(dv);
	}

	layout->addWidget(xLabel, 0, 1);
	layout->addWidget(yLabel, 0, 2);
	layout->addWidget(zLabel, 0, 3);
	layout->addWidget(mirror, 1, 0);
	layout->addWidget(m_xMirror, 1, 1, Qt::AlignHCenter);
	layout->addWidget(m_yMirror, 1, 2, Qt::AlignHCenter);
	layout->addWidget(m_zMirror, 1, 3, Qt::AlignHCenter);
	layout->addWidget(scale, 2, 0);
	layout->addWidget(m_xScall, 2, 1);
	layout->addWidget(m_yScall, 2, 2);
	layout->addWidget(m_zScall, 2, 3);
	layout->addWidget(rot, 3, 0);
	layout->addWidget(m_xRot, 3, 1);
	layout->addWidget(m_yRot, 3, 2);
	layout->addWidget(m_zRot, 3, 3);
	layout->addWidget(rotUnit, 3, 4);
	layout->addWidget(location, 4, 0);
	layout->addWidget(m_xPos, 4, 1);
	layout->addWidget(m_yPos, 4, 2);
	layout->addWidget(m_zPos, 4, 3);
	layout->addWidget(locUnit, 4, 4);
	layout->addWidget(lock, 5, 0);
	layout->addWidget(m_lockTheSameScall, 5, 1);
	layout->addWidget(showAxis, 6, 0);
	layout->addWidget(m_showLocalAxis, 6, 1);

	//??
	QSignalMapper* myMapper = new QSignalMapper(this);
	for (int i = 0; i < m_allScaleEdits.size(); i++)
	{
		connect(m_allScaleEdits[i], SIGNAL(editingFinished()), myMapper, SLOT(map()));
		myMapper->setMapping(m_allScaleEdits[i], i);
	}
	connect(myMapper, SIGNAL(mapped(int)), this, SLOT(sameScale(int)));

	for (int i = 3; i < m_allLineEdits.size(); i++)
	{
		connect(m_allLineEdits[i], SIGNAL(editingFinished()), this, SLOT(submit()));
	}

	connect(m_xMirror, SIGNAL(clicked()), this, SLOT(submit()));
	connect(m_yMirror, SIGNAL(clicked()), this, SLOT(submit()));
	connect(m_zMirror, SIGNAL(clicked()), this, SLOT(submit()));
	connect(m_showLocalAxis, SIGNAL(clicked()), this, SLOT(showAxisChanged()));

	return groupbox;
}

void CHModelTransformParamsSetUI::adjustSingleAngle(float & angle)
{
	int n = (int)(angle / 360.0);
	if (angle < 0)
	{
		n--;
	}
	angle = angle - 360.0*n;
}






