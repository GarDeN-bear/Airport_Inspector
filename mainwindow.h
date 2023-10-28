#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QMap>

#include "stopwatch.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_getList_clicked();

    void ScreenTableFromDB(QSqlTableModel *model);
    void ScreenQueryFromDB(QSqlQueryModel *model);
    void ReceiveStatusConnectionToDB(bool status);
    void RunConnectionToDB();
    void FillDataInAirports(QSqlQueryModel *model);

private:
    Ui::MainWindow *ui;
    DataBase* dataBase_;
    QMessageBox *msgBox_;
    Stopwatch *sw_;
    QMap<QString, QString> airports_;

    bool isFailConnection_;
};
#endif // MAINWINDOW_H
