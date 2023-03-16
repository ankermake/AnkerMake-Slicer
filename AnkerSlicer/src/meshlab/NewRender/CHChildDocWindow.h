/*
*???
*?????
*???2021?12?29?
*/


#ifndef CHChildDocWindow_H
#define CHChildDocWindow_H


#include "common/GeoAndShow/CHScene.h"
#include "common/plugins/interfaces/edit_plugin.h"
#include "CHRenderData.h"
#include "CH3dView.h"
#include "common/GeoAndShow/CHCurveShowObj.h"
#include <QMimeData>



class CHChildDocWindow :public QWidget, public CHScene
{
	Q_OBJECT

public:
	CHChildDocWindow(QWidget* parent = nullptr);
	~CHChildDocWindow();

public:
	virtual void refresh();
	virtual void getCurNearFarPoint(int x, int y, QVector3D& np, QVector3D& fp);
	virtual void pixelToWorld(float x, float y, float z, QVector3D& worldCoord);
	virtual void worldToPixel(QVector3D worldCoord, float& x, float& y, float& z);
    virtual bool getCurMouseInWidget(int x, int y);
    virtual bool isActivteAppWindow();
    virtual void getMechineBoxSize(float &length, float &width, float &height);

	QOpenGLShaderProgram& getShaderProgram();
	CHRenderDataPtr getRenderData();

	
	void generateBasePointBuffer();

	void createMachineBox(const QString& logPath, float x, float y, float z, QColor color = QColor(229, 229, 229));
    void updateMechineBox(float x, float y, float z);

	void setView(const CH3dView::ViewType& _type, const CHAABB3D& aabb);

protected:
	virtual void updatePoint(CHPointShowObjPtr pb);
	virtual void updateCurve(CHCurveShowObjPtr showcurve);
	virtual void updateMesh(CHMeshShowObjPtr mb);
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

signals:
	void openFiles(QStringList fileList);
    void machinePlatformSizeSignal(float length, float width, float height);

private:
	virtual void resizeEvent(QResizeEvent* event);

	void writeToGPUBUffer(CHMeshShowObjPtr mesh, QOpenGLBuffer& outvbo, QOpenGLBuffer& outnorbo,
		QOpenGLVertexArrayObject& outvao, GLuint& outibo, int& fnum, bool normalFlip = false);
	void writeToGPUBUffer(CHCurveShowObjPtr curvebody, CHRenderCurvePtr rendercurve);

private:

	CHRenderDataPtr m_renderData;

	CH3dView* m_view;
};


#endif
