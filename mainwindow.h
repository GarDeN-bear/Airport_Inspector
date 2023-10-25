#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>

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


public slots:
    void ScreenTableFromDB(QSqlTableModel *model);
    void ScreenQueryFromDB(QSqlQueryModel *model);
    void ReceiveStatusConnectionToDB(bool status);
    void RunConnectionToDB();

private:
    Ui::MainWindow *ui;
    DataBase* dataBase_;
    QMessageBox *msgBox_;
    Stopwatch *sw_;
    bool isFailConnection_;
};
#endif // MAINWINDOW_H
