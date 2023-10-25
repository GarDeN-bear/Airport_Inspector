#include "database.h"

DataBase::DataBase(QObject *parent) : QObject{parent}
{
    dataBase_ = new QSqlDatabase();
    modelQuery= new QSqlQueryModel(this);
    status_ = false;
}

DataBase::~DataBase()
{
    delete dataBase_;
}

void DataBase::ConnectToDB()
{
    //Данные для подключения к БД.
    QVector<QString> data;
    data.resize(port + 1);

    data[hostName] = "981757-ca08998.tmweb.ru";
    data[port] = "5432";
    data[dbName] = "demo";
    data[login] = "netology_usr_cpp";
    data[pass] = "CppNeto3";

    dataBase_->setHostName(data[hostName]);
    dataBase_->setPort(data[port].toInt());
    dataBase_->setDatabaseName(data[dbName]);
    dataBase_->setUserName(data[login]);
    dataBase_->setPassword(data[pass]);

    status_ = dataBase_->open();
//    modelQuery->setQuery("SELECT * FROM bookings.bookings", *dataBase_);
    modelTable_->setTable("bookings.flights");
    modelTable_->select();
    emit sig_SendTableFromDB(modelTable_);
//    emit sig_SendQueryFromDB(modelQuery);

    emit sig_SendStatusConnection(status_);

}

void DataBase::DisconnectFromDB(QString nameDb)
{
    *dataBase_ = QSqlDatabase::database(nameDb);
    dataBase_->close();
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase_ = QSqlDatabase::addDatabase(driver, nameDB);
    modelTable_ = new QSqlTableModel(this, *dataBase_);
}


QSqlError DataBase::GetLastError()
{
    return dataBase_->lastError();
}
