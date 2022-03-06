#include "ZuoGuangCFD/ui/mainwindow.h"

#include <QSize>
#include <QVariant>
#include <QtWidgets>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(PushMesh(zgCFD::Mesh&)), ui->openGLWidget, SLOT(GetMesh(zgCFD::Mesh&)));

    connect(ui->tbnMeshModelPoints, SIGNAL(clicked()), ui->openGLWidget, SLOT(ShowPoints()));
    connect(ui->tbnMeshModelFlatLines, SIGNAL(clicked()), ui->openGLWidget, SLOT(ShowFlatLines()));
    connect(ui->tbnMeshModelFlat, SIGNAL(clicked()), ui->openGLWidget, SLOT(ShowFlat()));
    connect(ui->tbnMeshModelHiddenLines, SIGNAL(clicked()), ui->openGLWidget, SLOT(ShowHiddenLines()));

    ui->tabWidget->tabBar()->hide();

    QButtonGroup gp;
    gp.addButton(ui->tbnMeshModelFlat);
    gp.addButton(ui->tbnMeshModelFlatLines);
    gp.addButton(ui->tbnMeshModelHiddenLines);

    tool_btn_list.append(ui->toolButtonMesh);
    tool_btn_list.append(ui->toolButtonBoundary);

    on_toolButtonMesh_clicked();
    ui->tbnMeshModelFlat->click();

    InitializeMeshWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitializeMeshWidget()
{
    //    QSize size(60, 60);
    //    ui->toolButton->setIconSize(size);
    //    ui->toolButton->resize(size);

    //    ui->toolButton_2->setIconSize(size);
    //    ui->toolButton_2->resize(size);
    //    ui->toolButton_3->setIconSize(size);
    //    ui->toolButton_3->resize(size);
}

void MainWindow::on_toolButtonMesh_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->toolButtonMesh->setStyleSheet("border-image:url(:/icon/image/toolbar_hover.png)");
    for (int i = 0; i < tool_btn_list.size(); i++)
    {
        if (tool_btn_list[i] != ui->toolButtonMesh)
        {
            tool_btn_list[i]->setStyleSheet("");
        }
    }
}

void MainWindow::on_toolButtonBoundary_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->toolButtonBoundary->setStyleSheet("border-image:url(:/icon/image/toolbar_hover.png)");
    for (int i = 0; i < tool_btn_list.size(); i++)
    {
        if (tool_btn_list[i] != ui->toolButtonBoundary)
        {
            tool_btn_list[i]->setStyleSheet("");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString current_path = QCoreApplication::applicationDirPath();
    QString title = "打开文件";
    QString filter = "OpenFOAM Mesh(*.of);;所有文件(*.*)";
    //    QString file_name = QFileDialog::getOpenFileName(this, title, current_path, filter);
    QString file_path = QFileDialog::getExistingDirectory(this, title, current_path);
    mesh_ = new zgCFD::OfMesh(file_path.toStdString());
    emit(PushMesh(*mesh_));
}
