/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/
#ifndef FILE_PROPERTY_H
#define FILE_PROPERTY_H

#include "app_params_property.h"
#include "document_property.h"
#include "view_property.h"
#include "plugin_pamas_property.h"
#include "../GeoAndShow/CHDoc.h"


class FileProperty
{
public:
    FileProperty();
    ~FileProperty();

    inline void clear()
    {
        m_appParams_property.clear();
        //m_document_property.clear();
        m_view_property.clear();
        m_plugins_property.clear();
        m_load_project_property.clear();
    }

    struct ProjectLoadProperty
    {
        bool m_loadProjected = false;
        QString m_lastProjectFile = "";

        inline void clear()
        {
            m_loadProjected = false;
            m_lastProjectFile = "";
        }

        inline ProjectLoadProperty& operator= (const ProjectLoadProperty& _property)
        {
            m_loadProjected = _property.m_loadProjected;
            m_lastProjectFile = _property.m_lastProjectFile;
            return *this;
        }
    };

    AppParamsProperty m_appParams_property;
    // DocumentProperty m_document_property;
    CHDocPtr m_docPtr;
    ViewProperty m_view_property;
    PluginPamasProperty m_plugins_property;

    ProjectLoadProperty m_load_project_property;

    inline FileProperty& operator= (const FileProperty& _fileProp)
    {
        m_appParams_property = _fileProp.m_appParams_property;
        // m_document_property = _fileProp.m_document_property;
        m_docPtr = _fileProp.m_docPtr;
        m_view_property = _fileProp.m_view_property;
        m_plugins_property = _fileProp.m_plugins_property;
        m_load_project_property = _fileProp.m_load_project_property;
        m_docPtr = _fileProp.m_docPtr;
        return *this;
    }
};


#endif // !FILE_PROPERTY_H
