#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")

#include <QDateTime>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QToolButton>
#include <exception>
#include <sstream>

#include "ZuoGuangCFD/algorithm/piso.h"
#include "ZuoGuangCFD/mesh/of_mesh.h"
#include "ZuoGuangCFD/struct/base_data_structure.h"
#include "ZuoGuangCFD/struct/zg_field.h"
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
    zgCFD::ScalarVolumeField* p_;
    zgCFD::VectorVolumeField* v_;

    zgCFD::Scalar start_time_;
    zgCFD::Scalar end_time_;
    zgCFD::Scalar delta_t_;
    zgCFD::Scalar output_time_;

    zgCFD::Scalar nu_;

    // PISO算法相关
    int number_of_correctors_;

  private:
    QString current_item;

  private slots:

    void on_toolButtonMesh_clicked();

    void on_toolButtonBoundary_clicked();

    void on_pushButton_clicked();

    void on_toolButtonSolve_clicked();

    void on_toolButtonPostProcessor_clicked();

    void on_listWidgetBoundary_itemClicked(QListWidgetItem* item);

    void on_pushButton_2_clicked();

    void on_pushButton_boundary_info_clicked();

    void on_pushButtonInitial_clicked();

    void on_pushButtonRun_clicked();

  signals:
    void PushMesh(zgCFD::Mesh& _mesh);

  private:
    void InitializeMeshWidget();
    void InitializeComboBoxBoundary();
    void InitializeVariable();
    void InitializeBoundaryWidget();
    void InitializeSolverWidget();

    Ui::MainWindow* ui;
    QList<QToolButton*> tool_btn_list;
};

#endif  // MAINWINDOW_H
