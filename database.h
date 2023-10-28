#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"


class DataBase : public QObject
{
    Q_OBJECT
public:
    //Перечисление полей данных
    enum fieldsForConnect{
        hostName = 0,
        dbName = 1,
        login = 2,
        pass = 3,
        port = 4
    };

    DataBase(QObject *parent = nullptr);
    ~DataBase();
    void ConnectToDB();
    void DisconnectFromDB(QString nameDb = "");
    void AddDataBase(QString driver, QString nameDB = "");
    void GetDataArrivals(const QString& airportCode, const QString& date);
    void GetDataDepartures(const QString& airportCode, const QString& date);
    bool status_;

    /*!
     * @brief Метод возвращает последнюю ошибку БД
     */
    QSqlError GetLastError(void);

signals:
    void sig_SendTableFromDB(QSqlTableModel* model);
    void sig_SendStatusConnection(bool);
    void sig_SendQueryFromDB(QSqlQueryModel* model);
    void sig_SendDataToAirports(QSqlQueryModel* model);
    void sig_SendDataToArrivals(QSqlQueryModel* model);
    void sig_SendDataToDepartures(QSqlQueryModel* model);

private:
    QSqlDatabase *dataBase_;
    QSqlTableModel* modelTable_;
    QSqlQueryModel* modelQuery_;

};

#endif // DATABASE_H
