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

    void SetAirportText(const QString& airport);
    void closeEvent(QCloseEvent* event) override;
    void SetStatisticsPerYear(const QVector<double> data);

public slots:
    void on_pb_close_clicked();

signals:
    void sig_CloseStatistics();

private:
    Ui::Statistics *ui;
    QVector<QString> months_;
    QVector<int> days_;
    QCPBars *CPBarsForYear_;
    QCPBars *CPBarsForMonth_;
};

#endif // STATISTICS_H
