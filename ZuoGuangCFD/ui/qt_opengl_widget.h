#ifndef QTOPENGLWIDGET_H
#define QTOPENGLWIDGET_H

#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include "ZuoGuangCFD/struct/base_data_structure.h"

class QtOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
  public:
    QtOpenGLWidget(QWidget* parent);
    ~QtOpenGLWidget();

  private:
    void Init(void);

  public:
    QSize minimumSizeHint(void) const Q_DECL_OVERRIDE;
    QSize sizeHint(void) const Q_DECL_OVERRIDE;
    const double& Radius(void) const;
    const zgCFD::GeneralVector& Center(void) const;
    const double* GetModelviewMatrix(void) const;
    void ResetModelviewMatrix(void);
    void CopyModelViewMatrix(void);
    void LoadCopyModelViewMatrix(void);
    const double* GetProjectionMatrix(void) const;

    enum ProjectionMode
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };
    void SetProjectionMode(const ProjectionMode& pm);
    const ProjectionMode& GetProjectionMode(void) const;

    enum DrawMode
    {
        POINTS,
        WIREFRAME,
        HIDDENLINES,
        FLATLINES,
        FLAT,
        SMOOTH
    };
    void SetDrawMode(const DrawMode& dm);
    const DrawMode& GetDrawMode(void) const;

  protected:
    enum MaterialType
    {
        MaterialDefault,
        MaterialGold,
        MaterialSilver,
        MaterialEmerald,
        MaterialTin
    };
    void SetMaterial(const MaterialType& mattype = MaterialGold) const;
    void SetDefaultLight(void) const;
    void initializeGL(void) Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL(void) Q_DECL_OVERRIDE;
    virtual void DrawScene(void);
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

  private:
    void Translation(const QPoint& p);
    void Translate(const zgCFD::GeneralVector& trans);
    void Rotation(const QPoint& p);
    void Rotate(const zgCFD::GeneralVector& axis, const double& angle);
    bool MapToSphere(const QPoint& point, zgCFD::GeneralVector& result);
    void UpdateProjectionMatrix(void);

  public:
    void SetScenePosition(const zgCFD::GeneralVector& c, const double& r);
    void ViewAll(void);

  protected:
    DrawMode draw_mode;
    ProjectionMode projection_mode;
    double window_left;
    double window_right;
    double windo_wtop;
    double window_bottom;
    Qt::MouseButton mouse_mode;
    zgCFD::GeneralVector center;
    double radius;
    std::vector<double> projection_matrix;
    std::vector<double> modelview_matrix;
    std::vector<double> copymodelview_matrix;
    QPoint lastpoint2;
    zgCFD::GeneralVector lastpoint3;
    bool last_point_ok;

    QOpenGLFunctions_3_3_Core* qgl_functions{nullptr};

  private:
    static const double track_ball_radius;
    static const double pi;
};

#endif  // QTOPENGLWIDGET_H
