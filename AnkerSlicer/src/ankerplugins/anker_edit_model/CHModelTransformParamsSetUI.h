/*
*?????????????
*?????
*???2021?11?16?
*/


#ifndef CHModelTransformParamsSetUI_H
#define CHModelTransformParamsSetUI_H


#include <QDialog>
#include <QGroupBox>
#include <QScrollArea>
#include "QLabel"
#include "QLineEdit"
#include "QComboBox"
#include "QCheckBox"
#include <vector>


using namespace std;

class CHModelTransformParamsSetUI : public QDialog
{
    Q_OBJECT

public:
	CHModelTransformParamsSetUI(QWidget *parent = Q_NULLPTR);
	~CHModelTransformParamsSetUI() ;
	




	public
		Q_SLOTS:
	void showAxisChanged();
	void receiveParams(vector<float> params);
	void submit();
	void sameScale(int index);

Q_SIGNALS:
	void sendParams(vector<float> params);
	void showAxis(bool showed);

private:
	QGroupBox * createParamsSetUI();
	void adjustSingleAngle(float& angle);

private:
		
	QLineEdit* m_xScall;
	QLineEdit* m_yScall;
	QLineEdit* m_zScall;
	QLineEdit* m_xRot;
	QLineEdit* m_yRot;
	QLineEdit* m_zRot;
	QLineEdit* m_xPos;
	QLineEdit* m_yPos;
	QLineEdit* m_zPos;
	std::vector<QLineEdit*> m_allLineEdits;
	std::vector<QLineEdit*> m_allScaleEdits;

	QCheckBox* m_xMirror;
	QCheckBox* m_yMirror;
	QCheckBox* m_zMirror;
	QCheckBox* m_lockTheSameScall;
	QCheckBox* m_showLocalAxis;
};
#endif
