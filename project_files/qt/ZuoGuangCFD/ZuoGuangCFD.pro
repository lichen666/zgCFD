QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../../ZuoGuangCFD/ZuoGuangCFD.cpp \
    ../../../ZuoGuangCFD/algorithm/piso.cpp \
    ../../../ZuoGuangCFD/finite_volume_method/explicit_calculus.cpp \
    ../../../ZuoGuangCFD/finite_volume_method/implicit_calculus.cpp \
    ../../../ZuoGuangCFD/finite_volume_method/implicit_convection.cpp \
    ../../../ZuoGuangCFD/finite_volume_method/implicit_diffusion.cpp \
    ../../../ZuoGuangCFD/mesh/boundary.cpp \
    ../../../ZuoGuangCFD/mesh/element.cpp \
    ../../../ZuoGuangCFD/mesh/face.cpp \
    ../../../ZuoGuangCFD/mesh/mesh.cpp \
    ../../../ZuoGuangCFD/mesh/of_mesh.cpp \
    ../../../ZuoGuangCFD/mesh/point.cpp \
    ../../../ZuoGuangCFD/output/output_of.cpp \
    ../../../ZuoGuangCFD/output/output_vtu.cpp \
    ../../../ZuoGuangCFD/solver/solver.cpp \
    ../../../ZuoGuangCFD/struct/base_point.cpp \
    ../../../ZuoGuangCFD/struct/matrix_coo.cpp \
    ../../../ZuoGuangCFD/struct/matrix_csr.cpp \
    ../../../ZuoGuangCFD/ui/mainwindow.cpp \
    ../../../ZuoGuangCFD/ui/mesh_viewer_widget.cpp \
    ../../../ZuoGuangCFD/ui/qt_opengl_widget.cpp

HEADERS += \
    ../../../ZuoGuangCFD/ZuoGuangCFD.h \
    ../../../ZuoGuangCFD/algorithm/piso.h \
    ../../../ZuoGuangCFD/finite_volume_method/explicit_calculus.h \
    ../../../ZuoGuangCFD/finite_volume_method/implicit_calculus.h \
    ../../../ZuoGuangCFD/finite_volume_method/implicit_convection.h \
    ../../../ZuoGuangCFD/finite_volume_method/implicit_diffusion.h \
    ../../../ZuoGuangCFD/mesh/boundary.h \
    ../../../ZuoGuangCFD/mesh/element.h \
    ../../../ZuoGuangCFD/mesh/face.h \
    ../../../ZuoGuangCFD/mesh/mesh.h \
    ../../../ZuoGuangCFD/mesh/of_mesh.h \
    ../../../ZuoGuangCFD/mesh/point.h \
    ../../../ZuoGuangCFD/output/output_of.h \
    ../../../ZuoGuangCFD/output/output_vtu.h \
    ../../../ZuoGuangCFD/solver/solver.h \
    ../../../ZuoGuangCFD/struct/base_data_structure.h \
    ../../../ZuoGuangCFD/struct/base_field.h \
    ../../../ZuoGuangCFD/struct/base_field_exception.h \
    ../../../ZuoGuangCFD/struct/base_fieldl.h \
    ../../../ZuoGuangCFD/struct/base_point.h \
    ../../../ZuoGuangCFD/struct/enums.h \
    ../../../ZuoGuangCFD/struct/fvm_matrix.h \
    ../../../ZuoGuangCFD/struct/fvm_matrixl.h \
    ../../../ZuoGuangCFD/struct/matrix_coo.h \
    ../../../ZuoGuangCFD/struct/matrix_csr.h \
    ../../../ZuoGuangCFD/struct/matrix_csr_exception.h \
    ../../../ZuoGuangCFD/struct/scalar_field.h \
    ../../../ZuoGuangCFD/struct/scalar_fieldl.h \
    ../../../ZuoGuangCFD/struct/scalar_vector_tensor_operatorl.h \
    ../../../ZuoGuangCFD/struct/tensor_field.h \
    ../../../ZuoGuangCFD/struct/tensor_fieldl.h \
    ../../../ZuoGuangCFD/struct/vector_field.h \
    ../../../ZuoGuangCFD/struct/vector_fieldl.h \
    ../../../ZuoGuangCFD/struct/zg_field.h \
    ../../../ZuoGuangCFD/struct/zg_fieldl.h \
    ../../../ZuoGuangCFD/struct/zg_scalar.h \
    ../../../ZuoGuangCFD/struct/zg_tensor.h \
    ../../../ZuoGuangCFD/struct/zg_tensorl.h \
    ../../../ZuoGuangCFD/struct/zg_vector.h \
    ../../../ZuoGuangCFD/struct/zg_vectorl.h \
    ../../../ZuoGuangCFD/ui/mainwindow.h \
    ../../../ZuoGuangCFD/ui/mesh_viewer_widget.h \
    ../../../ZuoGuangCFD/ui/qt_opengl_widget.h

FORMS += \
    ../../../ZuoGuangCFD/ui/mainwindow.ui

INCLUDEPATH += ../../../

LIBS += -lopengl32 -lGLU32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../../../ZuoGuangCFD/ui/icon.qrc
