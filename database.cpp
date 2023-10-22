#include "database.h"

DataBase::DataBase(QObject *parent) : QObject{parent}
{
    dataBase_ = new QSqlDatabase();
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
}

void DataBase::DisconnectFromDB(QString nameDb)
{
    *dataBase_ = QSqlDatabase::database(nameDb);
    dataBase_->close();
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase_ = QSqlDatabase::addDatabase(driver, nameDB);
}


QSqlError DataBase::GetLastError()
{
    return dataBase_->lastError();
}
