#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QCloseEvent>
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
    void SetDataPerYear(const QVector<double> data);
    void SetDataPerMonth(const QMultiMap<int, double> data);
    void SetStatisticsPerMonth(const int month);
    int findMonth(const QString& arg1);

public slots:
    void on_pb_close_clicked();

signals:
    void sig_CloseStatistics();

private slots:
    void on_cb_months_currentTextChanged(const QString &arg1);

private:
    Ui::Statistics *ui;
    QMap<int, QString> months_;
    QVector<int> days_;
    QCPBars *CPBarsForYear_;
    QCPBars *CPBarsForMonth_;
    QMultiMap<int, double> dataMonth_;
    bool isDataMonthReady_;
};

#endif // STATISTICS_H
