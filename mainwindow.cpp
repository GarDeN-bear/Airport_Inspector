#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataBase_ = new DataBase(this);
    msgBox_ = new QMessageBox(this);


    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase_->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    changeConnectionStatus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeConnectionStatus()
{
    if (ui->lb_status->text() == "Отключено от БД")
    {
        ui->lb_status->setText("Подключено к БД");
        ui->lb_status->setStyleSheet("color:green");
        ui->statusbar->addWidget(ui->lb_status);
        ui->pb_connect->setText("Отключиться от БД");
    }
    else
    {
        ui->lb_status->setText("Отключено от БД");
        ui->lb_status->setStyleSheet("color:red");
        ui->statusbar->addWidget(ui->lb_status);
        ui->pb_connect->setText("Подключиться к БД");
    }
}


void MainWindow::on_pb_connect_clicked()
{
    if (ui->pb_connect->text() == "Подключиться к БД")
    {
        dataBase_->ConnectToDB();

        if(!dataBase_->status_)
        {
            msgBox_->setIcon(QMessageBox::Critical);
            msgBox_->setText(dataBase_->GetLastError().text());
            msgBox_->exec();
        }
        else
        {
            changeConnectionStatus();
        }
    }
    else
    {
        dataBase_->DisconnectFromDB(DB_NAME);
        changeConnectionStatus();
    }
}

