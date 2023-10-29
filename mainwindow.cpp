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
    statistics_ = new Statistics();
    isFailConnection_ = false;

    dataBase_->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    ui->tb_main->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_main->verticalHeader()->setVisible(false);
    ui->groupBox->setEnabled(false);
    ui->lb_status->setText("Отключено от БД");
    ui->lb_status->setStyleSheet("color:red");
    ui->statusbar->addWidget(ui->lb_status);
    ui->rb_arrival->setChecked(true);

    sw_->Start();

    connect(dataBase_, &DataBase::sig_SendTableFromDB, this, &MainWindow::ScreenTableFromDB);
    connect(dataBase_, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase_, &DataBase::sig_SendQueryFromDB, this, &MainWindow::ScreenQueryFromDB);
    connect(dataBase_, &DataBase::sig_SendDataToAirports, this, &MainWindow::FillDataInAirports);
    connect(dataBase_, &DataBase::sig_SendDataToArrivals, this, &MainWindow::ScreenQueryFromDB);
    connect(dataBase_, &DataBase::sig_SendDataToDepartures, this, &MainWindow::ScreenQueryFromDB);
    connect(sw_->getQTimer(), &QTimer::timeout, this, &MainWindow::RunConnectionToDB);

    connect(statistics_, &Statistics::sig_CloseStatistics, this, &MainWindow::StatisticsIsClosed);

    connect(dataBase_, &DataBase::sig_SendStatisticsPerYear, this, &MainWindow::GetStatisticsPerYear);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete statistics_;
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
        ui->groupBox->setEnabled(true);
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

void MainWindow::FillDataInAirports(QSqlQueryModel *model)
{
    for(int i = 0; i < model->rowCount(); ++i)
    {
        ui->cb_airports->addItem(model->data(model->index(i,0)).toString());
        airports_[model->data(model->index(i,0)).toString()] = model->data(model->index(i,1)).toString();
    }

}

void MainWindow::StatisticsIsClosed()
{
    ui->centralwidget->setEnabled(true);
    statistics_->close();
}

void MainWindow::GetStatisticsPerYear(QSqlQueryModel *model)
{
    QVector<double> data;
    for(int i = 0; i < model->rowCount(); ++i)
    {
        data.append(model->data(model->index(i,0)).toDouble());
    }
    statistics_->SetStatisticsPerYear(data);
}

void MainWindow::on_pb_getList_clicked()
{
    QString airportCode = airports_[ui->cb_airports->currentText()];
    QString date = ui->de_date->text();
    if(ui->rb_arrival->isChecked())
    {
        dataBase_->GetDataArrivals(airportCode, date);
    }
    else if(ui->rb_departure->isChecked())
    {
        dataBase_->GetDataDepartures(airportCode, date);
    }
}


void MainWindow::on_pb_showLoad_clicked()
{
    ui->centralwidget->setEnabled(false);
    QString airportCode = ui->cb_airports->currentText() +
            " (" + airports_[ui->cb_airports->currentText()] + ")";
    statistics_->SetAirportText(airportCode);
    dataBase_->GetStatisticsPerYear(airports_[ui->cb_airports->currentText()]);
    statistics_->show();
}

