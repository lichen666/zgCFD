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
    tool_btn_list.append(ui->toolButtonSolve);
    tool_btn_list.append(ui->toolButtonPostProcessor);

    ui->plainTextEdit->setReadOnly(true);

    on_toolButtonMesh_clicked();
    ui->tbnMeshModelHiddenLines->click();

    InitializeMeshWidget();
    InitializeComboBoxBoundary();
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

void MainWindow::InitializeComboBoxBoundary()
{
    ui->comboBoxP->addItem("FixedValue");
    ui->comboBoxP->addItem("ZeroGradient");
    ui->comboBoxP->addItem("Inlet");
    ui->comboBoxP->addItem("Outlet");
    ui->comboBoxP->addItem("Empty");

    ui->comboBoxV->addItem("FixedValue");
    ui->comboBoxV->addItem("ZeroGradient");
    ui->comboBoxV->addItem("Inlet");
    ui->comboBoxV->addItem("Outlet");
    ui->comboBoxV->addItem("Slip");
    ui->comboBoxV->addItem("NoSlip");
    ui->comboBoxV->addItem("Empty");
}

void MainWindow::InitializeVariable()
{
    p_ = new zgCFD::ScalarVolumeField(*mesh_, "P", mesh_->case_directory_);
    v_ = new zgCFD::VectorVolumeField(*mesh_, "V", mesh_->case_directory_);
}

void MainWindow::InitializeBoundaryWidget()
{
    ui->toolButtonBoundary->setEnabled(true);

    QListWidgetItem* item;

    ui->listWidgetBoundary->clear();

    int number_of_bounary = mesh_->number_of_boundaries_;

    for (int i = 0; i < number_of_bounary; ++i)
    {
        QString name = QString::fromStdString(mesh_->boundaries_[i].boundary_name_);
        item = new QListWidgetItem();
        item->setText(name);
        ui->listWidgetBoundary->addItem(item);
    }
}

void MainWindow::InitializeSolverWidget()
{
    ui->toolButtonSolve->setEnabled(true);
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

void MainWindow::on_toolButtonSolve_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
    ui->toolButtonSolve->setStyleSheet("border-image:url(:/icon/image/toolbar_hover.png)");
    for (int i = 0; i < tool_btn_list.size(); i++)
    {
        if (tool_btn_list[i] != ui->toolButtonSolve)
        {
            tool_btn_list[i]->setStyleSheet("");
        }
    }
}

void MainWindow::on_toolButtonPostProcessor_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
    ui->toolButtonPostProcessor->setStyleSheet("border-image:url(:/icon/image/toolbar_hover.png)");
    for (int i = 0; i < tool_btn_list.size(); i++)
    {
        if (tool_btn_list[i] != ui->toolButtonPostProcessor)
        {
            tool_btn_list[i]->setStyleSheet("");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString current_path = QCoreApplication::applicationDirPath();
    QString title = "打开文件";
    QString filter = "OpenFOAM Mesh(*.of);;Fluent Mesh(*.msh);;所有文件(*.*)";
    QString file_path_name = QFileDialog::getOpenFileName(this, title, current_path, filter);
    QFileInfo file_info(file_path_name);
    QString file_path = file_info.absolutePath();
    QString file_suffix = file_info.suffix();

    if (file_suffix == "of")
    {
        try
        {
            mesh_ = new zgCFD::OfMesh(file_path.toStdString());
            emit(PushMesh(*mesh_));
        }
        catch (std::exception e)
        {
        }

        if (mesh_->isSuccessfulRead())
        {
            InitializeBoundaryWidget();
            InitializeSolverWidget();
            InitializeVariable();
        }
    }
}

void MainWindow::on_listWidgetBoundary_itemClicked(QListWidgetItem* item)
{
    QString name = item->text();
    current_item = item->text();
    zgCFD::Scalar p_value = p_->boundary_condation_[name.toStdString()].bc_value_;
    zgCFD::Scalar v_value_x = v_->boundary_condation_[name.toStdString()].bc_value_.x();
    zgCFD::Scalar v_value_y = v_->boundary_condation_[name.toStdString()].bc_value_.y();
    zgCFD::Scalar v_value_z = v_->boundary_condation_[name.toStdString()].bc_value_.z();

    if (ui->comboBoxP->currentText() == "FixedValue")
    {
        QString p_value_t = QString::number(p_value, 'f', 3);
        ui->lineEditP->setText(p_value_t);
    }
    if (ui->comboBoxV->currentText() == "FixedValue")
    {
        QString v_value_x_t = QString::number(v_value_x, 'f', 3);
        QString v_value_y_t = QString::number(v_value_y, 'f', 3);
        QString v_value_z_t = QString::number(v_value_z, 'f', 3);
        ui->lineEditVX->setText(v_value_x_t);
        ui->lineEditVY->setText(v_value_y_t);
        ui->lineEditVZ->setText(v_value_z_t);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //压力边界条件
    if (ui->comboBoxP->currentText() == "FixedValue")
    {
        p_->boundary_condation_[current_item.toStdString()].bc_value_ = ui->lineEditP->text().toDouble();
        p_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::SPECIFIEDVALUE;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info = cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item +
                       "压力边界条件为:" + "FixedValue;" + "值为:" + ui->lineEditP->text();
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxP->currentText() == "ZeroGradient")
    {
        p_->boundary_condation_[current_item.toStdString()].bc_value_ = 0;
        p_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::ZEROGRADIENT;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info = cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item +
                       "压力边界条件为:" + "ZeroGradient";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxP->currentText() == "Inlet")
    {
        p_->boundary_condation_[current_item.toStdString()].bc_value_ = 0;
        p_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::INLET;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "压力边界条件为:" + "Inlet";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxP->currentText() == "Outlet")
    {
        p_->boundary_condation_[current_item.toStdString()].bc_value_ = 0;
        p_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::OUTLET;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "压力边界条件为:" + "Outlet";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxP->currentText() == "Empty")
    {
        p_->boundary_condation_[current_item.toStdString()].bc_value_ = 0;
        p_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::EMPTY;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "压力边界条件为:" + "Empty";
        ui->plainTextEdit->appendPlainText(info);
    }

    //速度边界条件
    if (ui->comboBoxV->currentText() == "FixedValue")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = ui->lineEditVX->text().toDouble();
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = ui->lineEditVY->text().toDouble();
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = ui->lineEditVZ->text().toDouble();
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::SPECIFIEDVALUE;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info = cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item +
                       "速度边界条件为:" + "FixedValue;" + "值为:(" + ui->lineEditVX->text() + " " +
                       ui->lineEditVY->text() + " " + ui->lineEditVZ->text() + ")";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxV->currentText() == "ZeroGradient")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::ZEROGRADIENT;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info = cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item +
                       "速度边界条件为:" + "ZeroGradient";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxV->currentText() == "Inlet")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = ui->lineEditVX->text().toDouble();
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = ui->lineEditVY->text().toDouble();
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = ui->lineEditVZ->text().toDouble();
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::INLET;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info = cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item +
                       "速度边界条件为:" + "Inlet;" + "值为:(" + ui->lineEditVX->text() + " " + ui->lineEditVY->text() +
                       " " + ui->lineEditVZ->text() + ")";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxV->currentText() == "Outlet")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::OUTLET;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "速度边界条件为:" + "Outlet";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxV->currentText() == "Slip")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::SLIP;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "速度边界条件为:" + "Slip";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxV->currentText() == "NoSlip")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::NOSLIP;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "速度边界条件为:" + "NoSlip";
        ui->plainTextEdit->appendPlainText(info);
    }
    else if (ui->comboBoxV->currentText() == "Empty")
    {
        v_->boundary_condation_[current_item.toStdString()].bc_value_.x() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.y() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_value_.z() = 0;
        v_->boundary_condation_[current_item.toStdString()].bc_type_ = zgCFD::BoundaryCondationType::EMPTY;

        QDateTime cur_date_Time = QDateTime::currentDateTime();
        QString info =
            cur_date_Time.toString("yyyy-MM-dd hh:mm:ss\t") + "设置" + current_item + "速度边界条件为:" + "Empty";
        ui->plainTextEdit->appendPlainText(info);
    }
}

void MainWindow::on_pushButton_boundary_info_clicked()
{
    int number_of_bounary = mesh_->number_of_boundaries_;
    QDateTime cur_date_Time = QDateTime::currentDateTime();
    ui->plainTextEdit->appendPlainText(cur_date_Time.toString("yyyy-MM-dd hh:mm:ss  边界信息:"));

    for (int i = 0; i < number_of_bounary; ++i)
    {
        QString name = QString::fromStdString(mesh_->boundaries_[i].boundary_name_);

        zgCFD::Scalar p_value = p_->boundary_condation_[name.toStdString()].bc_value_;
        zgCFD::Scalar v_value_x = v_->boundary_condation_[name.toStdString()].bc_value_.x();
        zgCFD::Scalar v_value_y = v_->boundary_condation_[name.toStdString()].bc_value_.y();
        zgCFD::Scalar v_value_z = v_->boundary_condation_[name.toStdString()].bc_value_.z();

        QString p_boundary;
        QString v_boundary;

        switch (p_->boundary_condation_[mesh_->boundaries_[i].boundary_name_].bc_type_)
        {
            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                p_boundary = "FixedValue;值为:" + QString::number(p_value, 'f', 3) + ";";
                break;
            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                p_boundary = "ZeroGradient;";
                break;
            case zgCFD::BoundaryCondationType::INLET:
                p_boundary = "Inlet;";
                break;
            case zgCFD::BoundaryCondationType::OUTLET:
                p_boundary = "Outlet;";
                break;
            case zgCFD::BoundaryCondationType::EMPTY:
                p_boundary = "Empty;";
                break;
            default:
                break;
        }

        switch (v_->boundary_condation_[mesh_->boundaries_[i].boundary_name_].bc_type_)
        {
            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                v_boundary = "FixedValue;值为:(" + QString::number(v_value_x, 'f', 3) + " " +
                             QString::number(v_value_y, 'f', 3) + " " + QString::number(v_value_z, 'f', 3) + ");";
                break;
            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                v_boundary = "ZeroGradient;";
                break;
            case zgCFD::BoundaryCondationType::INLET:
                v_boundary = "Inlet;值为:(" + QString::number(v_value_x, 'f', 3) + " " +
                             QString::number(v_value_y, 'f', 3) + " " + QString::number(v_value_z, 'f', 3) + ");";
                break;
            case zgCFD::BoundaryCondationType::OUTLET:
                v_boundary = "Outlet;";
                break;
            case zgCFD::BoundaryCondationType::EMPTY:
                v_boundary = "Empty;";
                break;
            default:
                break;
        }

        QString info = QString::fromStdString(mesh_->boundaries_[i].boundary_name_) + ":\n" +
                       "压力边界条件为:" + p_boundary + "速度边界条件为:" + v_boundary;
        ui->plainTextEdit->appendPlainText(info);
    }
}

void MainWindow::on_pushButtonInitial_clicked()
{
    QDateTime cur_date_Time = QDateTime::currentDateTime();
    ui->plainTextEdit->appendPlainText(cur_date_Time.toString("yyyy-MM-dd hh:mm:ss  初始化信息:"));

    start_time_ = ui->lineEditStartTime->text().toDouble();
    end_time_ = ui->lineEditTotleTime->text().toDouble();
    delta_t_ = ui->lineEditDeltat->text().toDouble();
    output_time_ = ui->lineEditOutTime->text().toDouble();
    nu_ = ui->lineEditNu->text().toDouble();
    number_of_correctors_ = ui->lineEditPISO->text().toInt();

    QString info =
        "设置开始时间为:" + ui->lineEditStartTime->text() + "\t设置时间间隔为:" + ui->lineEditDeltat->text() +
        "\n设置结束时间为:" + ui->lineEditTotleTime->text() + "\t设置输出间隔为:" + ui->lineEditOutTime->text() +
        "\n设置粘度为:" + ui->lineEditNu->text() + "\t设置PISO循环次数为:" + ui->lineEditPISO->text();
    ui->plainTextEdit->appendPlainText(info);
}

void MainWindow::on_pushButtonRun_clicked()
{
    zgCFD::Piso piso(start_time_, delta_t_, end_time_, output_time_, number_of_correctors_, nu_, v_, p_);

    piso.run();
}
