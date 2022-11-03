/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/

#ifndef VIEW_PROPERTY_H
#define VIEW_PROPERTY_H

#include <string>
#include <QVector3D>
#include <QMatrix4x4>
class ViewProperty
{
public:
	ViewProperty();
	~ViewProperty();

    inline void clear()
    {
        viewName = "";
        m_scene_property.m_verticalAngle = 0.0;
        m_scene_property.senseRadius = 0.0;
        m_scene_property.camera.m_eye = QVector3D(0.0, 0.0, 0.0);
        m_scene_property.camera.m_front = QVector3D(1.0, 0.0, 0.0);
        m_scene_property.camera.m_up = QVector3D(0.0, 0.0, 1.0);
    }

	struct SceneProperty
	{
        struct CameraProperty
        {
            QVector3D m_eye;
            QVector3D m_up;
            QVector3D m_front;


            inline CameraProperty& operator= (const CameraProperty& cameraProp)
            {
                m_eye = cameraProp.m_eye;
                m_up = cameraProp.m_up;
                m_front = cameraProp.m_front;

                return *this;
            }
        };

		CameraProperty camera;
        double m_verticalAngle;
        double senseRadius;//?????????????????

        inline SceneProperty& operator= (const SceneProperty& _sceneProp)
        {
            camera = _sceneProp.camera;
            m_verticalAngle = _sceneProp.m_verticalAngle;
            senseRadius = _sceneProp.senseRadius;
            return *this;
        }
       
	};

	SceneProperty m_scene_property;
    std::string viewName;
    inline ViewProperty& operator= (const ViewProperty& _viewProp)
    {
        m_scene_property = _viewProp.m_scene_property;

        return *this;
    }
};




#endif // !VIEW_PROPERTY_H
