#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>

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
    bool status_;

    /*!
     * @brief Метод возвращает последнюю ошибку БД
     */
    QSqlError GetLastError(void);

signals:
    void sig_SendTableFromDB(QSqlTableModel* model);
    void sig_SendStatusConnection(bool);
private:
    QSqlDatabase *dataBase_;
    QSqlTableModel* modelTable_;

};

#endif // DATABASE_H
