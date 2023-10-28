#include "database.h"

DataBase::DataBase(QObject *parent) : QObject{parent}
{
    dataBase_ = new QSqlDatabase();
    modelQuery_= new QSqlQueryModel(this);
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
    modelTable_->setTable("bookings.flights");
    modelTable_->setFilter("flight_id < 10");
    modelTable_->select();
    emit sig_SendTableFromDB(modelTable_);

    QString request = "SELECT airport_name->>'ru', airport_code FROM bookings.airports_data";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQuery_->setQuery(*query);

    emit sig_SendDataToAirports(modelQuery_);
    emit sig_SendStatusConnection(status_);
    delete query;

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

void DataBase::GetDataArrivals(const QString& airportCode, const QString& date)
{
    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name "
                      "FROM bookings.flights f ";
    "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport"
    "WHERE (f.arrival_airport  = '" + airportCode + "' AND f.shceduled_arrival::date = date('" + date + "'))";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQuery_->setQuery(*query);
    emit sig_SendDataToArrivals(modelQuery_);
    delete query;
}

void DataBase::GetDataDepartures(const QString &airportCode, const QString& date)
{
    QString request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name "
                      "FROM bookings.flights f ";
    "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport"
    "WHERE f.departure_airport  = '" + airportCode + "' AND f.scheduled_departure::date = date('" + date + "'))";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQuery_->setQuery(*query);
    emit sig_SendDataToArrivals(modelQuery_);
    delete query;
}


QSqlError DataBase::GetLastError()
{
    return dataBase_->lastError();
}
