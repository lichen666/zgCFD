#include "ZuoGuangCFD/ui/qt_opengl_widget.h"

const double QtOpenGLWidget::track_ball_radius = 0.6;
const double QtOpenGLWidget::pi = 3.14159265358979323846;

QtOpenGLWidget::QtOpenGLWidget(QWidget* parent) :
    QOpenGLWidget(parent), draw_mode(FLATLINES), projection_mode(PERSPECTIVE), window_left(-1), window_right(1),
    windo_wtop(1), window_bottom(-1), mouse_mode(Qt::NoButton), center(0, 0, 0), radius(0), projection_matrix(16, 0.0),
    modelview_matrix(16, 0.0), copymodelview_matrix(16, 0.0), lastpoint2(0, 0), lastpoint3(0, 0, 0),
    last_point_ok(false)
{
}

QtOpenGLWidget::~QtOpenGLWidget() {}

QSize QtOpenGLWidget::minimumSizeHint() const
{
    return QSize(10, 10);
}

QSize QtOpenGLWidget::sizeHint() const
{
    QRect rect = QApplication::desktop()->screenGeometry();
    return QSize(int(rect.width() * 0.8), int(rect.height() * 1.0));
}

const double& QtOpenGLWidget::Radius() const
{
    return radius;
}

const zgCFD::GeneralVector& QtOpenGLWidget::Center() const
{
    return center;
}

const double* QtOpenGLWidget::GetModelviewMatrix() const
{
    return &modelview_matrix[0];
}

void QtOpenGLWidget::ResetModelviewMatrix()
{
    makeCurrent();
    glLoadIdentity();
    qgl_functions->glGetDoublev(GL_MODELVIEW_MATRIX, &modelview_matrix[0]);
}

void QtOpenGLWidget::CopyModelViewMatrix()
{
    copymodelview_matrix = modelview_matrix;
}

void QtOpenGLWidget::LoadCopyModelViewMatrix()
{
    modelview_matrix = copymodelview_matrix;
}

const double* QtOpenGLWidget::GetProjectionMatrix() const
{
    return &projection_matrix[0];
}

void QtOpenGLWidget::SetProjectionMode(const QtOpenGLWidget::ProjectionMode& pm)
{
    projection_mode = pm;
    UpdateProjectionMatrix();
    ViewAll();
}

const QtOpenGLWidget::ProjectionMode& QtOpenGLWidget::GetProjectionMode() const
{
    return projection_mode;
}

void QtOpenGLWidget::SetDrawMode(const QtOpenGLWidget::DrawMode& dm)
{
    draw_mode = dm;
    update();
}

const QtOpenGLWidget::DrawMode& QtOpenGLWidget::GetDrawMode() const
{
    return draw_mode;
}

void QtOpenGLWidget::SetMaterial(const QtOpenGLWidget::MaterialType& mattype) const
{
    std::vector<GLfloat> matAmbient, matDiffuse, matSpecular;
    GLfloat matShininess;
    switch (mattype)
    {
        default:
        case MaterialDefault:
            matAmbient = {1.0f, 1.0f, 1.0f, 1.0f};
            matDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
            matSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
            matShininess = 120.0f;
            break;
        case MaterialGold:
            matAmbient = {0.9f, 0.667f, 0.0f, 1.0f};
            matDiffuse = {0.9f, 0.749f, 0.251f, 1.0f};
            matSpecular = {0.9f, 0.816f, 0.451f, 1.0f};
            matShininess = 50.0f;
            break;
        case MaterialSilver:
            matAmbient = {0.3f, 0.3f, 0.3f, 1.0f};
            matDiffuse = {0.7f, 0.7f, 0.7f, 1.0f};
            matSpecular = {0.7f, 0.7f, 0.7f, 1.0f};
            matShininess = 51.2f;
            break;
        case MaterialEmerald:
            matAmbient = {0.143f, 0.549f, 0.143f, 1.0f};
            matDiffuse = {0.143f, 0.549f, 0.143f, 1.0f};
            matSpecular = {0.733f, 0.927811f, 0.733f, 1.0f};
            matShininess = 76.8f;
            break;
        case MaterialTin:
            matAmbient = {0.405882f, 0.358824f, 0.413725f, 1.0f};
            matDiffuse = {0.727451f, 0.770588f, 0.841176f, 1.0f};
            matSpecular = {0.633333f, 0.633333f, 0.821569f, 1.0f};
            matShininess = 59.84615f;
            break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular.data());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
}

void QtOpenGLWidget::SetDefaultLight() const
{
    // Lighting
    glLoadIdentity();
    GLfloat pos1[] = {10.0f, 10.0f, -10.0f, 0.0f};
    GLfloat pos2[] = {-10.0f, 10.0f, -10.0f, 0.0f};
    GLfloat pos3[] = {0.0f, 0.0f, 10.0f, 0.0f};
    GLfloat col1[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat col2[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat col3[] = {0.8f, 0.8f, 0.8f, 1.0f};

    qgl_functions->glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

    qgl_functions->glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, col2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, col2);

    qgl_functions->glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, pos3);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
    glLightfv(GL_LIGHT2, GL_SPECULAR, col3);
}

void QtOpenGLWidget::initializeGL()
{
    qgl_functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    qgl_functions->initializeOpenGLFunctions();

    // OpenGL state
    qgl_functions->glClearColor(1.0, 1.0, 1.0, 1.0);
    qgl_functions->glClearDepth(1.0);

    // glDisable(GL_DITHER);
    qgl_functions->glEnable(GL_DEPTH_TEST);
    qgl_functions->glEnable(GL_MULTISAMPLE);
    qgl_functions->glEnable(GL_LINE_SMOOTH);
    qgl_functions->glEnable(GL_BLEND);
    qgl_functions->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    qgl_functions->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    qgl_functions->glEnable(GL_POINT_SMOOTH);
    qgl_functions->glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    // Material
    SetMaterial();

    // Lighting
    glLoadIdentity();
    SetDefaultLight();

    // scene pos and size
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);

    // for initialize all the viewports
    qgl_functions->glGetDoublev(GL_MODELVIEW_MATRIX, &modelview_matrix[0]);
    CopyModelViewMatrix();

    SetScenePosition(zgCFD::GeneralVector(0.0, 0.0, 0.0), 1.0);
    // LoadTexture();
}

void QtOpenGLWidget::resizeGL(int width, int height)
{
    qgl_functions->glViewport(0, 0, width, height);
    UpdateProjectionMatrix();
    update();
}

void QtOpenGLWidget::paintGL()
{
    qgl_functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawScene();
}

void QtOpenGLWidget::DrawScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(&projection_matrix[0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(&modelview_matrix[0]);
    switch (draw_mode)
    {
        case WIREFRAME:
            qgl_functions->glDisable(GL_LIGHTING);
            // glutWireTeapot(0.5);
            break;
        case FLAT:
            qgl_functions->glEnable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            // glutSolidTeapot(0.5);
            break;
        case FLATLINES:
            qgl_functions->glEnable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            // glutSolidTeapot(0.5);
            break;
        case HIDDENLINES:
            qgl_functions->glDisable(GL_LIGHTING);
            break;
        case SMOOTH:
            qgl_functions->glEnable(GL_LIGHTING);
            glShadeModel(GL_SMOOTH);
            // glutSolidTeapot(0.5);
            break;
        default:
            break;
    }
}

void QtOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    lastpoint2 = event->pos();
    last_point_ok = MapToSphere(lastpoint2, lastpoint3);
    mouse_mode = event->button();
}

void QtOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint newPoint2D = event->pos();
    if (last_point_ok)
    {
        switch (mouse_mode)
        {
            case Qt::LeftButton:
                Rotation(newPoint2D);
                break;
            case Qt::RightButton:
                Translation(newPoint2D);
                break;
            default:
                break;
        }
    }

    // remember this point
    lastpoint2 = newPoint2D;
    last_point_ok = MapToSphere(lastpoint2, lastpoint3);
    // trigger redraw
    update();
}

void QtOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    mouse_mode = Qt::NoButton;
    last_point_ok = false;
}

void QtOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    double d = -event->delta() / 120.0 * 0.05 * radius;
    Translate(zgCFD::GeneralVector(0.0, 0.0, d));
    update();
}

void QtOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            qApp->quit();
            break;
    }
    event->ignore();
}

void QtOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    event->ignore();
}

void QtOpenGLWidget::Translation(const QPoint& p)
{
    double z = -(modelview_matrix[2] * center.x() + modelview_matrix[6] * center.y() +
                 modelview_matrix[10] * center.z() + modelview_matrix[14]) /
               (modelview_matrix[3] * center.x() + modelview_matrix[7] * center.y() +
                modelview_matrix[11] * center.z() + modelview_matrix[15]);

    double w = width();
    double h = height();
    double aspect = w / h;
    double near_plane = 0.01 * radius;
    double top = tan(45.0 / 2.0f * pi / 180.0f) * near_plane;
    double right = aspect * top;

    double dx = p.x() - lastpoint2.x();
    double dy = p.y() - lastpoint2.y();

    Translate(zgCFD::GeneralVector(2.0 * dx / w * right / near_plane * z, -2.0 * dy / h * top / near_plane * z, 0.0f));
}

void QtOpenGLWidget::Translate(const zgCFD::GeneralVector& trans)
{
    makeCurrent();
    glLoadIdentity();
    trans.x();
    glTranslated(trans.x(), trans.y(), trans.z());
    glMultMatrixd(&modelview_matrix[0]);
    qgl_functions->glGetDoublev(GL_MODELVIEW_MATRIX, &modelview_matrix[0]);
}

void QtOpenGLWidget::Rotation(const QPoint& p)
{
    zgCFD::GeneralVector newPoint3D;
    bool newPoint_hitSphere = MapToSphere(p, newPoint3D);
    if (newPoint_hitSphere)
    {
        zgCFD::GeneralVector axis = lastpoint3 ^ newPoint3D;
        if ((axis.Magnitude() * axis.Magnitude()) < 1e-7)
        {
            axis = zgCFD::GeneralVector(1, 0, 0);
        }
        else
        {
            axis.Normalize();
        }
        // find the amount of rotation
        zgCFD::GeneralVector d = lastpoint3 - newPoint3D;
        double t = 0.5 * d.Magnitude() / track_ball_radius;
        if (t < -1.0)
            t = -1.0;
        else if (t > 1.0)
            t = 1.0;
        double phi = 2.0 * asin(t);
        double angle = phi * 180.0 / pi;
        Rotate(axis, angle);
    }
}

void QtOpenGLWidget::Rotate(const zgCFD::GeneralVector& axis, const double& angle)
{
    zgCFD::GeneralVector t(modelview_matrix[0] * center.x() + modelview_matrix[4] * center.y() +
                               modelview_matrix[8] * center.z() + modelview_matrix[12],
                           modelview_matrix[1] * center.x() + modelview_matrix[5] * center.y() +
                               modelview_matrix[9] * center.z() + modelview_matrix[13],
                           modelview_matrix[2] * center.x() + modelview_matrix[6] * center.y() +
                               modelview_matrix[10] * center.z() + modelview_matrix[14]);

    makeCurrent();
    glLoadIdentity();
    glTranslatef(t.x(), t.y(), t.z());
    glRotated(angle, axis.x(), axis.y(), axis.z());
    glTranslatef(-t.x(), -t.y(), -t.z());
    glMultMatrixd(&modelview_matrix[0]);
    qgl_functions->glGetDoublev(GL_MODELVIEW_MATRIX, &modelview_matrix[0]);
}

bool QtOpenGLWidget::MapToSphere(const QPoint& point, zgCFD::GeneralVector& vector3)
{
    double x = (2.0 * point.x() - width()) / width();
    double y = -(2.0 * point.y() - height()) / height();
    double xval = x;
    double yval = y;
    double x2y2 = xval * xval + yval * yval;

    const double rsqr = track_ball_radius * track_ball_radius;
    vector3.x() = xval;
    vector3.y() = yval;
    if (x2y2 < 0.5 * rsqr)
    {
        vector3.z() = sqrt(rsqr - x2y2);
    }
    else
    {
        vector3.z() = 0.5 * rsqr / sqrt(x2y2);
    }

    return true;
}

void QtOpenGLWidget::UpdateProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (PERSPECTIVE == projection_mode)
    {
        glFrustum(-0.01 * radius * (sqrt(2.0) - 1) * width() / height(),
                  0.01 * radius * (sqrt(2.0) - 1) * width() / height(), -0.01 * radius * (sqrt(2.0) - 1),
                  0.01 * radius * (sqrt(2.0) - 1), 0.01 * radius, 100.0 * radius);
    }
    else if (ORTHOGRAPHIC == projection_mode)  // not work for
    {
        glOrtho(window_left, window_right, window_bottom, windo_wtop, -1, 1);
    }
    qgl_functions->glGetDoublev(GL_PROJECTION_MATRIX, &projection_matrix[0]);
    glMatrixMode(GL_MODELVIEW);
}

void QtOpenGLWidget::SetScenePosition(const zgCFD::GeneralVector& c, const double& r)
{
    center = c;
    radius = r;

    UpdateProjectionMatrix();
    ViewAll();
}

void QtOpenGLWidget::ViewAll()
{
    zgCFD::GeneralVector trans =
        zgCFD::GeneralVector(-(modelview_matrix[0] * center.x() + modelview_matrix[4] * center.y() +
                               modelview_matrix[8] * center.z() + modelview_matrix[12]),
                             -(modelview_matrix[1] * center.x() + modelview_matrix[5] * center.y() +
                               modelview_matrix[9] * center.z() + modelview_matrix[13]),
                             -(modelview_matrix[2] * center.x() + modelview_matrix[6] * center.y() +
                               modelview_matrix[10] * center.z() + modelview_matrix[14] + 2.0 * radius));

    makeCurrent();
    glLoadIdentity();
    glTranslated(trans.x(), trans.y(), trans.z());
    glMultMatrixd(&modelview_matrix[0]);
    qgl_functions->glGetDoublev(GL_MODELVIEW_MATRIX, &modelview_matrix[0]);
}
