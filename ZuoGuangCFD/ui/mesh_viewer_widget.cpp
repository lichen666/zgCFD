#include "ZuoGuangCFD/ui/mesh_viewer_widget.h"

MeshViewerWidget::MeshViewerWidget(QWidget* parent) : QtOpenGLWidget(parent) {}

MeshViewerWidget::~MeshViewerWidget() {}

bool MeshViewerWidget::InitializeShader(float ratio)
{
    qgl_functions->glGenVertexArrays(1, &VAO);
    qgl_functions->glGenBuffers(1, &VBO);
    qgl_functions->glGenBuffers(1, &EBO);

    qgl_functions->glBindVertexArray(VAO);
    qgl_functions->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector<float> vertices;

    for (int i = 0; i < mesh_->number_of_points_; ++i)
    {
        vertices.push_back(mesh_->points_[i].x_ / (ratio));
        vertices.push_back(mesh_->points_[i].y_ / (ratio));
        vertices.push_back(mesh_->points_[i].z_ / (ratio));
    }

    qgl_functions->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    qgl_functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    qgl_functions->glEnableVertexAttribArray(0);
    //    ff->glBindBuffer(GL_ARRAY_BUFFER, 0);

    qgl_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    std::vector<int> indices;

    for (int i = 0; i < mesh_->number_of_faces_; ++i)
    {
        for (int j = 0; j < mesh_->faces_[i].points_.size(); ++j)
        {
            indices.push_back(mesh_->faces_[i].points_[j]);
        }
    }

    qgl_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    qgl_functions->glBindVertexArray(0);

    return true;
}

bool MeshViewerWidget::ScreenShot()
{
    update();
    QString filename = strMeshPath + "/" + QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz") + QString(".png");
    QImage image = grabFramebuffer();
    image.save(filename);
    std::cout << "保存截图到 " << filename.toStdString() << std::endl;
    return true;
}

void MeshViewerWidget::SetDrawBoundingBox(bool b)
{
    isDrawBoundingBox = b;
    update();
}

void MeshViewerWidget::SetDrawBoundary(bool b)
{
    isDrawBoundary = b;
    update();
}

void MeshViewerWidget::EnableLighting(bool b)
{
    isEnableLighting = b;
    update();
}

void MeshViewerWidget::EnableDoubleSide(bool b)
{
    isTwoSideLighting = b;
    update();
}

void MeshViewerWidget::ResetView()
{
    ResetModelviewMatrix();
    ViewCenter();
    update();
}

void MeshViewerWidget::ViewCenter()
{
    SetScenePosition((ptMin + ptMax) * 0.5, (ptMin - ptMax).Magnitude() * 0.5);
    update();
}

void MeshViewerWidget::CopyRotation()
{
    CopyModelViewMatrix();
}

void MeshViewerWidget::LoadRotation()
{
    LoadCopyModelViewMatrix();
    update();
}

void MeshViewerWidget::DrawScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(&projection_matrix[0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(&modelview_matrix[0]);

    if (isEnableLighting)
        qgl_functions->glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, isTwoSideLighting);
    DrawSceneMesh();
    if (isEnableLighting)
        qgl_functions->glDisable(GL_LIGHTING);
}

void MeshViewerWidget::DrawSceneMesh()
{
    SetMaterial();
    qgl_functions->glBindVertexArray(VAO);

    switch (draw_mode)
    {
        case POINTS:
            DrawPoints();
            break;
        case WIREFRAME:
            DrawWireframe();
            break;
        case HIDDENLINES:
            DrawHiddenLines();
            break;
        case FLATLINES:
            DrawFlatLines();
            break;
        case FLAT:
            glColor3d(0.8, 0.8, 0.8);
            DrawFlat();
            break;
        default:
            break;
    }
    qgl_functions->glBindVertexArray(0);
}

void MeshViewerWidget::ShowPoints()
{
    SetDrawMode(DrawMode::POINTS);
}

void MeshViewerWidget::ShowWireframe()
{
    SetDrawMode(DrawMode::WIREFRAME);
}

void MeshViewerWidget::ShowHiddenLines()
{
    SetDrawMode(DrawMode::HIDDENLINES);
}

void MeshViewerWidget::ShowFlatLines()
{
    SetDrawMode(DrawMode::FLATLINES);
}

void MeshViewerWidget::ShowFlat()
{
    SetDrawMode(DrawMode::FLAT);
}

void MeshViewerWidget::DrawPoints()
{
    glColor3d(1.5, 0.5, 0.5);
    qgl_functions->glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
    qgl_functions->glPointSize(5);
    qgl_functions->glDrawElements(GL_POINTS, mesh_->number_of_faces_ * 4, GL_UNSIGNED_INT, 0);
}

void MeshViewerWidget::DrawWireframe()
{
    glColor3d(0.2, 0.2, 0.2);
    qgl_functions->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    qgl_functions->glDrawElements(GL_QUADS, mesh_->number_of_faces_ * 4, GL_UNSIGNED_INT, 0);
}

void MeshViewerWidget::DrawHiddenLines()
{
    qgl_functions->glLineWidth(1.0);
    float backcolor[4];
    qgl_functions->glGetFloatv(GL_COLOR_CLEAR_VALUE, backcolor);
    glColor4fv(backcolor);
    qgl_functions->glDepthRange(0.01, 1.0);
    qgl_functions->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (qgl_functions->glIsEnabled(GL_LIGHTING))
    {
        qgl_functions->glDisable(GL_LIGHTING);
        DrawFlat();
        qgl_functions->glEnable(GL_LIGHTING);
    }
    else
    {
        DrawFlat();
    }
    qgl_functions->glDepthRange(0.0, 1.0);
    qgl_functions->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3d(.3, .3, .3);
    DrawFlat();
    qgl_functions->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MeshViewerWidget::DrawFlatLines()
{
    qgl_functions->glEnable(GL_POLYGON_OFFSET_FILL);
    qgl_functions->glPolygonOffset(1.5f, 2.0f);
    glShadeModel(GL_FLAT);

    glColor3d(1.0, 1.0, 1.0);
    DrawFlat();
    qgl_functions->glDisable(GL_POLYGON_OFFSET_FILL);
    if (qgl_functions->glIsEnabled(GL_LIGHTING))
    {
        qgl_functions->glDisable(GL_LIGHTING);
        DrawWireframe();
        qgl_functions->glEnable(GL_LIGHTING);
    }
    else
    {
        DrawWireframe();
    }
}

void MeshViewerWidget::DrawFlat()
{
    qgl_functions->glDrawElements(GL_QUADS, number_of_faces * 4, GL_UNSIGNED_INT, 0);
}

bool MeshViewerWidget::GetMesh(zgCFD::Mesh& _mesh)
{
    mesh_ = &_mesh;

    number_of_faces = mesh_->number_of_faces_;

    bool read_OK = mesh_->isSuccessfulRead();

    if (read_OK)
    {
        ptMin.x_ = ptMin.y_ = ptMin.z_ = DBL_MAX;
        ptMax.x_ = ptMax.y_ = ptMax.z_ = -DBL_MAX;

        for (int i = 0; i < mesh_->number_of_points_; ++i)
        {
            ptMin.x_ = ptMin.x_ > mesh_->points_[i].x_ ? mesh_->points_[i].x_ : ptMin.x_;
            ptMin.y_ = ptMin.y_ > mesh_->points_[i].y_ ? mesh_->points_[i].y_ : ptMin.y_;
            ptMin.z_ = ptMin.z_ > mesh_->points_[i].z_ ? mesh_->points_[i].z_ : ptMin.z_;

            ptMax.x_ = ptMax.x_ < mesh_->points_[i].x_ ? mesh_->points_[i].x_ : ptMax.x_;
            ptMax.y_ = ptMax.y_ < mesh_->points_[i].y_ ? mesh_->points_[i].y_ : ptMax.y_;
            ptMax.z_ = ptMax.z_ < mesh_->points_[i].z_ ? mesh_->points_[i].z_ : ptMax.z_;
        }

        double radio = std::max(std::max((ptMax - ptMin).x(), (ptMax - ptMin).y()), (ptMax - ptMin).z());

        ptMin.x_ = ptMin.x_ / radio;
        ptMax.x_ = ptMax.x_ / radio;
        ptMin.y_ = ptMin.y_ / radio;
        ptMax.y_ = ptMax.y_ / radio;
        ptMin.z_ = ptMin.z_ / radio;
        ptMax.z_ = ptMax.z_ / radio;

        SetScenePosition((ptMin + ptMax) * 0.5, (ptMax - ptMin).Magnitude() * 0.5);

        InitializeShader(radio);

        update();
        return true;
    }
    return false;
}
