#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QToolButton>

#include "ZuoGuangCFD/mesh/of_mesh.h"
#include "ZuoGuangCFD/ui/qt_opengl_widget.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    zgCFD::Mesh* mesh_;

  private slots:

    void on_toolButtonMesh_clicked();

    void on_toolButtonBoundary_clicked();

    void on_pushButton_clicked();

  signals:
    void PushMesh(zgCFD::Mesh& _mesh);

  private:
    void InitializeMeshWidget();

    Ui::MainWindow* ui;
    QList<QToolButton*> tool_btn_list;
};

#endif  // MAINWINDOW_H
