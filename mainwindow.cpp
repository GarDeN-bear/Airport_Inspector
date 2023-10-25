#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataBase_ = new DataBase(this);
    msgBox_ = new QMessageBox(this);
    sw_ = new Stopwatch(this);
    isFailConnection_ = false;
    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase_->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    connect(dataBase_, &DataBase::sig_SendTableFromDB, this, &MainWindow::ScreenTableFromDB);
    connect(dataBase_, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase_, &DataBase::sig_SendQueryFromDB, this, &MainWindow::ScreenQueryFromDB);
    connect(sw_->getQTimer(), &QTimer::timeout, this, &MainWindow::RunConnectionToDB);

    ui->lb_status->setText("Отключено от БД");
    ui->lb_status->setStyleSheet("color:red");
    ui->statusbar->addWidget(ui->lb_status);

    sw_->Start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ScreenQueryFromDB(QSqlQueryModel *model)
{
    ui->tb_main->setModel(model);
}


void MainWindow::ScreenTableFromDB(QSqlTableModel *model)
{
    ui->tb_main->setModel(model);
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if (status)
    {
        ui->lb_status->setText("Подключено к БД");
        ui->lb_status->setStyleSheet("color:green");
        ui->statusbar->addWidget(ui->lb_status);
        sw_->Stop();
    }
    else
    {
        isFailConnection_ = !isFailConnection_;
        ui->lb_status->setText("Отключено от БД");
        ui->lb_status->setStyleSheet("color:red");
        ui->statusbar->addWidget(ui->lb_status);
        dataBase_->DisconnectFromDB(DB_NAME);
        msgBox_->setIcon(QMessageBox::Critical);
        msgBox_->setText(dataBase_->GetLastError().text());
        sw_->Stop();
        msgBox_->exec();
        sw_->setTime(0);
        sw_->Start();
    }
}

void MainWindow::RunConnectionToDB()
{
    if (!dataBase_->status_ && !isFailConnection_)
    {
        auto conn = [&]{dataBase_->ConnectToDB();};
        QtConcurrent::run(conn);

    }
    if (isFailConnection_)
    {
        sw_->setTime(sw_->getCurrentTime() + 1);
        if(sw_->getCurrentTime() >= 5)
        {
            isFailConnection_ = !isFailConnection_;
        }
    }
}
