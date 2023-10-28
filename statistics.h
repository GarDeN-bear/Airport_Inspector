#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QCloseEvent>
#include <QVector2D>
#include "database.h"
#include "qcustomplot.h"

namespace Ui {
class Statistics;
}

class Statistics : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = nullptr);
    ~Statistics();

    void SetAirport(const QString& airport);
    void closeEvent(QCloseEvent* event) override;
    void SetStatisticsPerYear(DataBase);

public slots:
    void on_pb_close_clicked();

signals:
    void sig_CloseStatistics();

private:
    Ui::Statistics *ui;
    QVector<QString> months_;
    QCPBars *CPBarsForYear_;
};

#endif // STATISTICS_H
