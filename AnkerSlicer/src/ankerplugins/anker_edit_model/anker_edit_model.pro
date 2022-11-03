include (../../shared.pri)

HEADERS += \
	CHCustomLineEdit.h\
	CHModelMirrorTransformParamsSetUI.h\
	CHModelMoveTransformParamsSetUI.h\
	CHModelRotationTransformParamsSetUI.h\
	CHModelZoomTransformParamsSetUI.h\
	edit_meshmirrortransform.h\
	edit_meshmovetransform.h\
	edit_meshrotationtransform.h\
	edit_meshtransform_factory.h\
	edit_meshzoomtransform.h\
        BoxBorder.h

SOURCES += \
    CHCustomLineEdit.cpp\
	CHModelMirrorTransformParamsSetUI.cpp\
	CHModelMoveTransformParamsSetUI.cpp\
	CHModelRotationTransformParamsSetUI.cpp\
	CHModelZoomTransformParamsSetUI.cpp\
	edit_meshmirrortransform.cpp\
	edit_meshmovetransform.cpp\
	edit_meshrotationtransform.cpp\
	edit_meshtransform_factory.cpp\
	edit_meshzoomtransform.cpp\
        BoxBorder.cpp

RESOURCES += \
    anker_edit_model.qrc


TARGET = anker_edit_model

QT += widgets


