#ifndef MESHVIEWERWIDGET_H
#define MESHVIEWERWIDGET_H

#include <QDateTime>
#include <QWidget>
#include <vector>

#include "ZuoGuangCFD/mesh/of_mesh.h"
#include "ZuoGuangCFD/ui/qt_opengl_widget.h"

class MeshViewerWidget : public QtOpenGLWidget
{
    Q_OBJECT

  public:
    MeshViewerWidget(QWidget* parent);
    ~MeshViewerWidget();
    bool InitializeShader(float ratio);
    bool ScreenShot(void);
    void SetDrawBoundingBox(bool b);
    void SetDrawBoundary(bool b);
    void EnableLighting(bool b);
    void EnableDoubleSide(bool b);
    void ResetView(void);
    void ViewCenter(void);
    void CopyRotation(void);
    void LoadRotation(void);

  protected:
    virtual void DrawScene(void) Q_DECL_OVERRIDE;
    void DrawSceneMesh(void);

  private:
    void DrawPoints(void);
    void DrawWireframe(void);
    void DrawHiddenLines(void);
    void DrawFlatLines(void);
    void DrawFlat(void);

    int number_of_faces;

  protected:
    zgCFD::Mesh mesh;
    zgCFD::Mesh* mesh_;
    QString strMeshFileName;
    QString strMeshBaseName;
    QString strMeshPath;
    zgCFD::point ptMin;
    zgCFD::point ptMax;
    bool isEnableLighting;
    bool isTwoSideLighting;
    bool isDrawBoundingBox;
    bool isDrawBoundary;
    unsigned int VAO, VBO, EBO;

  signals:
    void LoadMeshOKSignal(bool, QString);

  private slots:
    bool GetMesh(zgCFD::Mesh& _mesh);
    void ShowPoints(void);
    void ShowWireframe(void);
    void ShowHiddenLines(void);
    void ShowFlatLines(void);
    void ShowFlat(void);
};

#endif  // MESHVIEWERWIDGET_H
