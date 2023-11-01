#include "database.h"

DataBase::DataBase(QObject *parent) : QObject{parent}
{
    dataBase_ = new QSqlDatabase();
    modelQueryMain= new QSqlQueryModel(this);
    modelQueryData = new QSqlQueryModel(this);
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

    QString request = "SELECT airport_name->>'ru' AS name, airport_code FROM bookings.airports_data ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain->setQuery(*query);

    emit sig_SendDataToAirports(modelQueryMain);
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
    QString parsedDate = ParseInputDate(date);
    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                      "WHERE (f.arrival_airport  = '" + airportCode + "' AND f.scheduled_arrival::date = date('" + parsedDate + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain->setQuery(*query);
    emit sig_SendDataToArrivals(modelQueryMain);
    delete query;
}

void DataBase::GetDataDepartures(const QString &airportCode, const QString& date)
{
    QString parsedDate = ParseInputDate(date);
    QString request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                      "WHERE (f.departure_airport  = '" + airportCode + "' AND f.scheduled_departure::date = date('" + parsedDate + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain->setQuery(*query);
    emit sig_SendDataToDepartures(modelQueryMain);
    delete query;
}

void DataBase::GetDataPerYear(const QString &airportCode)
{
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) AS Month "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Month";

    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryData->setQuery(*query);
    emit sig_SendDataPerYear(modelQueryData);
    delete query;
}

void DataBase::GetDataPerMonth(const QString &airportCode)
{
    QString request = "SELECT count(flight_no), date_part('month', date_trunc('day', scheduled_departure)), date_trunc('day', scheduled_departure) AS Day "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Day";

    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryData->setQuery(*query);
    emit sig_SendDataPerMonth(modelQueryData);
    delete query;
}


QSqlError DataBase::GetLastError()
{
    return dataBase_->lastError();
}

QString DataBase::ParseInputDate(const QString &date)
{
    QString day, month, year;
    for(int i = 0; i < date.size(); ++i)
    {
        if(i < 2)
        {
            day+=date[i];
        }
        if(i > 2 && i < 5)
        {
            month+=date[i];
        }
        if(i > 5)
        {
            year+=date[i];
        }
    }
    QString resultStr = year + "-" + month + "-" + day;
    return resultStr;
}
