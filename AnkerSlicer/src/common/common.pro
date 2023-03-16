include (../general.pri)

QT += opengl \
    widgets
QT += xml
QT += network
QT += gui-private
QT += remoteobjects

TEMPLATE = lib
TARGET = meshlab-common
DEPENDPATH += .

#QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8

DESTDIR = $$MESHLAB_DISTRIB_DIRECTORY/lib

win32-msvc:QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
#win32-msvc:CONFIG += staticlib
win32:CONFIG += staticlib
#CONFIG += staticlib
win32-g++:DLLDESTDIR = $$MESHLAB_DISTRIB_DIRECTORY/lib

linux:CONFIG += dll

CONFIG += c++17

INCLUDEPATH *= \
	../.. \
	$$$$MESHLAB_EXTERNAL_DIRECTORY/easyexif \
	$$VCGDIR \
	$$EIGENDIR

!CONFIG(system_glew) {
	INCLUDEPATH *= $$GLEWDIR/include
	GLEWCODE = $$GLEWDIR/src/glew.c
}

!CONFIG(system_glew) {
	SOURCES += $$GLEWCODE
	DEFINES += GLEW_STATIC
}

message(MeshLab Version: $$MESHLAB_VERSION)

# Input
HEADERS += 	\
    GeoAndShow/AkTransform.h \
    GeoAndShow/AkTransformMath.h \
    GeoAndShow/CH3DPrintModel.h \
    GeoAndShow/CHAABB3D.h \
    GeoAndShow/CHAssembly.h \
    GeoAndShow/CHBase.h \
    GeoAndShow/CHBaseAlg.h \
    GeoAndShow/CHCircleShowObj.h \
    GeoAndShow/CHCone.h \
	GeoAndShow/CHCuboid.h \
	GeoAndShow/CHCylinder.h \
	GeoAndShow/CHSphere.h \
	GeoAndShow/CHTetrahedron.h \
	GeoAndShow/CHTrapezoidBody.h \
    GeoAndShow/CHCurveShowObj.h \
    GeoAndShow/CHDoc.h \
    GeoAndShow/CHLine3D.h \
    GeoAndShow/CHLineSegment3DShowObj.h \
    GeoAndShow/CHMeshShowObj.h \
    GeoAndShow/CHObject.h \
    GeoAndShow/CHPickOperationCommand.h \
    GeoAndShow/CHPlane.h \
    GeoAndShow/CHPointShowObj.h \
    GeoAndShow/CHPrintMachineBox.h \
    GeoAndShow/CHScene.h \
    GeoAndShow/CHShowObj.h \
    GeoAndShow/TransformPack.h \
	GeoAndShow/SceneParam.h	\
	GeoAndShow/coordinate.h	\
    GeoAndShow/sphere.h \
    GeoAndShow/support/SupportAssemblyMeshes.h \
    GeoAndShow/support/SupportCone.h \
    GeoAndShow/support/SupportCuboid.h \
    GeoAndShow/support/SupportCylinder.h \
    GeoAndShow/support/SupportMesh.h \
    GeoAndShow/support/SupportSphere.h \
    GeoAndShow/support/SupportTetrahedron.h \
    GeoAndShow/support/SupportTrapezoidBody.h \
    ak_const.h \
    controlInterface/bubbleWidget.h \
        controlInterface/controlInterface.h \
    controlInterface/line.h \
    controlInterface/messageDialog.h \
    controlInterface/pagewidget.h \
    controlInterface/progressbar.h \
    controlInterface/progressdialog.h \
    controlInterface/radiusdiaolg.h \
    controlInterface/switchbutton.h \
    controlInterface/titlewidget.h \
	globals.h \
	plugins/containers/generic_container_iterator.h \
	plugins/containers/decorate_plugin_container.h \
	plugins/containers/edit_plugin_container.h \
	plugins/containers/filter_plugin_container.h \
	plugins/containers/io_plugin_container.h \
	plugins/containers/render_plugin_container.h \
    plugins/interfaces/anker_edit_plugin.h \
	plugins/interfaces/anker_plugin.h \
	plugins/interfaces/meshlab_plugin.h \
	plugins/interfaces/meshlab_plugin_logger.h \
	plugins/interfaces/decorate_plugin.h \
	plugins/interfaces/edit_plugin.h \
	plugins/interfaces/filter_plugin.h \
	plugins/interfaces/io_plugin.h \
	plugins/interfaces/render_plugin.h \
	plugins/meshlab_plugin_type.h \
	plugins/plugin_manager.h \
           ml_document/helpers/mesh_document_state_data.h \
           ml_document/helpers/mesh_model_state_data.h \
           ml_document/base_types.h \
           ml_document/cmesh.h \
           ml_document/mesh_model.h \
           ml_document/mesh_model_state.h \
           ml_document/mesh_document.h \
           ml_document/raster_model.h \
           ml_document/render_raster.h \
#	ml_shared_data_context/ml_plugin_gl_context.h \
#	ml_shared_data_context/ml_scene_gl_shared_data_context.h \
#	ml_shared_data_context/ml_shared_data_context.h \
	parameters/rich_parameter_list.h \
	parameters/value.h \
	parameters/rich_parameter.h \
	python/function.h \
	python/function_parameter.h \
	python/function_set.h \
	python/python_utils.h \
    support/supportdata.h \
    support/supportline.h \
    support/supportmeshfactory.h \
    utilities/AkMatrix3i.h \
    utilities/AkPoint3i.h \
    utilities/akUtilSpace.h \
    utilities/math_utils.h \
	utilities/eigen_mesh_conversions.h \
	utilities/file_format.h \
	utilities/ioapi.h \
#	utilities/load_save.h \
	GLExtensionsManager.h \
	filterscript.h \
	GLLogStream.h \
	mlexception.h \
	mlapplication.h \
#	ml_selection_buffers.h \
	utilities/tlogger.h	\
	file_property/app_params_property.h	\
	file_property/document_property.h	\
	file_property/file_property.h	\
	file_property/plugin_pamas_property.h	\
	file_property/view_property.h	\
	file_property/project_load_save.h	\
	file_property/xml_stream.h	\
	file_property/mesh_model_import_export.h	\
	file_property/thread_safe.h	\
        file_property/import_export_support.h   \
        file_property/import_model_thread.h \
        file_property/export_model_thread.h \
        file_property/import_project_thread.h   \
        file_property/export_project_thread.h   \
        file_property/read_mesh.h   \
    Socket/HeartBeatThead.h   \
    Socket/QMainSocket2.h  \
    Socket/SocketBase.h


SOURCES += \
	GeoAndShow/AkTransform.cpp \
	GeoAndShow/AkTransformMath.cpp \
	GeoAndShow/CH3DPrintModel.cpp \
	GeoAndShow/CHAABB3D.cpp \
	GeoAndShow/CHAssembly.cpp \
	GeoAndShow/CHBaseAlg.cpp \
	GeoAndShow/CHCircleShowObj.cpp \
	GeoAndShow/CHCone.cpp \
	GeoAndShow/CHCuboid.cpp \
	GeoAndShow/CHCylinder.cpp \
	GeoAndShow/CHSphere.cpp \
	GeoAndShow/CHTetrahedron.cpp \
	GeoAndShow/CHTrapezoidBody.cpp \
	GeoAndShow/CHCurveShowObj.cpp \
	GeoAndShow/CHDoc.cpp \
	GeoAndShow/CHLine3D.cpp \
	GeoAndShow/CHLineSegment3DShowObj.cpp \
	GeoAndShow/CHMeshShowObj.cpp \
	GeoAndShow/CHObject.cpp \
	GeoAndShow/CHPickOperationCommand.cpp \
	GeoAndShow/CHPlane.cpp \
	GeoAndShow/CHPointShowObj.cpp \
	GeoAndShow/CHPrintMachineBox.cpp \
	GeoAndShow/CHScene.cpp \
	GeoAndShow/CHShowObj.cpp \
	GeoAndShow/TransformPack.cpp \
	GeoAndShow/coordinate.cpp	\
    GeoAndShow/sphere.cpp \
	GeoAndShow/support/SupportAssemblyMeshes.cpp \
	GeoAndShow/support/SupportCone.cpp \
	GeoAndShow/support/SupportCuboid.cpp \
	GeoAndShow/support/SupportCylinder.cpp \
	GeoAndShow/support/SupportMesh.cpp \
	GeoAndShow/support/SupportSphere.cpp \
	GeoAndShow/support/SupportTetrahedron.cpp \
	GeoAndShow/support/SupportTrapezoidBody.cpp \
	controlInterface/bubbleWidget.cpp \
	controlInterface/line.cpp \
	controlInterface/messageDialog.cpp \
	controlInterface/pagewidget.cpp \
	controlInterface/progressbar.cpp \
	controlInterface/progressdialog.cpp \
    controlInterface/radiusdiaolg.cpp \
	controlInterface/switchbutton.cpp \
    controlInterface/titlewidget.cpp \
	globals.cpp \
	plugins/containers/decorate_plugin_container.cpp \
	plugins/containers/edit_plugin_container.cpp \
	plugins/containers/filter_plugin_container.cpp \
	plugins/containers/io_plugin_container.cpp \
	plugins/containers/render_plugin_container.cpp \
	plugins/interfaces/meshlab_plugin_logger.cpp \
	plugins/interfaces/decorate_plugin.cpp \
	plugins/interfaces/filter_plugin.cpp \
	plugins/interfaces/io_plugin.cpp \
	plugins/meshlab_plugin_type.cpp \
	plugins/plugin_manager.cpp \
           ml_document/helpers/mesh_document_state_data.cpp \
           ml_document/cmesh.cpp \
           ml_document/mesh_model.cpp \
           ml_document/mesh_model_state.cpp \
           ml_document/mesh_document.cpp \
           ml_document/raster_model.cpp \
           ml_document/render_raster.cpp \
#	ml_shared_data_context/ml_plugin_gl_context.cpp \
#	ml_shared_data_context/ml_scene_gl_shared_data_context.cpp \
#	ml_shared_data_context/ml_shared_data_context.cpp \
	parameters/rich_parameter.cpp \
	parameters/rich_parameter_list.cpp \
	parameters/value.cpp \
	python/function.cpp \
	python/function_parameter.cpp \
	python/function_set.cpp \
	python/python_utils.cpp \
	support/supportdata.cpp \
	support/supportline.cpp \
	support/supportmeshfactory.cpp \
    utilities/AkMatrix3i.cpp \
    utilities/AkPoint3i.cpp \
	utilities/eigen_mesh_conversions.cpp \
	utilities/ioapi.cpp \
#	utilities/load_save.cpp \
        utilities/math_utils.cpp \
	GLExtensionsManager.cpp \
	filterscript.cpp \
	GLLogStream.cpp \
	mlapplication.cpp \
	searcher.cpp \
#	ml_selection_buffers.cpp \
	$$MESHLAB_EXTERNAL_DIRECTORY/easyexif/exif.cpp \
	utilities/tlogger.cpp	\
	file_property/app_params_property.cpp	\
	file_property/document_property.cpp	\
	file_property/file_property.cpp	\
	file_property/plugin_pamas_property.cpp	\
	file_property/view_property.cpp	\
	file_property/project_load_save.cpp	\
	file_property/xml_stream.cpp	\
	file_property/mesh_model_import_export.cpp	\
	file_property/thread_safe.cpp	\
        file_property/import_export_support.cpp \
        file_property/import_model_thread.cpp   \
        file_property/export_model_thread.cpp   \
        file_property/import_project_thread.cpp   \
        file_property/export_project_thread.cpp \
        file_property/read_mesh.cpp \
        Socket/HeartBeatThead.cpp   \
        Socket/QMainSocket2.cpp

RESOURCES += \
        meshlab-common.qrc
        #ankerimages/ankerMake.qrc

macx:QMAKE_POST_LINK = "\
        if [ -d  $$MESHLAB_DISTRIB_DIRECTORY/AnkerMake.app/Contents/Frameworks/ ]; \
	then  \
		echo "Copying";  \
	else  \
                mkdir -p $$MESHLAB_DISTRIB_DIRECTORY/AnkerMake.app/Contents/Frameworks;  \
	fi;   \
        cp $$MESHLAB_DISTRIB_DIRECTORY/lib/libmeshlab-common.* $$MESHLAB_DISTRIB_DIRECTORY/AnkerMake.app/Contents/Frameworks/ ;\
        #cp $$MESHLAB_DISTRIB_DIRECTORY/../../distrib/.* $$MESHLAB_DISTRIB_DIRECTORY/AnkerMake.app/Contents/MacOS/ ;\
#	if [ -d ../external/ ];\
#	then \
#		echo "ok external dir exists"; \
#	else \
#		ln -s ../../meshlab/src/external ../external;\
#		echo "linked external dir"; \
#	fi;\
#	if [ -d $$MESHLAB_DISTRIB_DIRECTORY/shaders/ ];\
#	then \
#		echo "ok shader dir exists"; \
#	else \
#		ln -s ../../../meshlab/src/distrib/shaders ../distrib/shaders ;\
#		echo "linked shader dir"; \
#	fi;\
	"

DISTFILES += \
    img/dummy.png \
    img/fdm_nav_del_icon.png
