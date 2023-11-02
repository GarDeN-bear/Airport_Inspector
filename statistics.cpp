#include "statistics.h"
#include "ui_statistics.h"

Statistics::Statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);
    isDataMonthReady_ = false;
    ui->tabWidget->setCurrentWidget(ui->tab_perYear);
    months_ = { {1, "Январь"}, {2, "Февраль"}, {3, "Март"},
                {4, "Апрель"}, {5, "Май"}, {6, "Июнь"},
                {7, "Июль"}, {8, "Август"}, {9, "Сентябрь"},
                {10, "Октябрь"}, {11, "Ноябрь"}, {12, "Декабрь"}
              };
//    ui>setWindowTitle("Статистика");

    for(const auto& [key, value] : months_.asKeyValueRange())
    {
        ui->cb_months->addItem(value);
    }

    CPBarsForYear_ = new QCPBars(ui->widget_loadPerYear->xAxis,
                                 ui->widget_loadPerYear->yAxis
                                 );

    CPBarsForMonth_ = new QCPBars(ui->widget_loadPerMonth->xAxis,
                                  ui->widget_loadPerMonth->yAxis
                                  );

}

Statistics::~Statistics()
{
    delete CPBarsForYear_;
    delete CPBarsForMonth_;
    delete ui;
}

void Statistics::on_cb_months_currentTextChanged(const QString &arg1)
{
    if (isDataMonthReady_)
    {
        setStatisticsPerMonth(findMonth(arg1));
    }
}

void Statistics::setAirportText(const QString &airport)
{
    ui->l_airportsLoad->setText("Загруженность аэропорта " + airport);
}

void Statistics::setDataPerYear(const QVector<double> data)
{
    ui->widget_loadPerYear->clearGraphs();
    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    for(const auto& [key, value] : months_.asKeyValueRange())
    {
        ticks << key;
        labels << value;
    }
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->widget_loadPerYear->xAxis->setTicker(textTicker);
    ui->widget_loadPerYear->xAxis->setTickLabelRotation(60);
    ui->widget_loadPerYear->xAxis->setSubTicks(false);
    ui->widget_loadPerYear->xAxis->setTickLength(0, 4);
    ui->widget_loadPerYear->xAxis->setLabel("Месяцы");
    ui->widget_loadPerYear->xAxis->setRange(0, months_.size() + 1);
    ui->widget_loadPerYear->xAxis->grid()->setVisible(true);

    // prepare y axis:
    ui->widget_loadPerYear->yAxis->setRange(0, *std::max_element(data.begin(), data.end()) + 1);
    ui->widget_loadPerYear->yAxis->setPadding(5); // a bit more space to the left border
    ui->widget_loadPerYear->yAxis->setLabel("Загруженность аэропорта за год");
    ui->widget_loadPerYear->yAxis->grid()->setSubGridVisible(true);
    ui->widget_loadPerYear->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->widget_loadPerYear->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    CPBarsForYear_->setData(ticks, data);
    ui->widget_loadPerYear->replot();
}

void Statistics::setDataPerMonth(const QMultiMap<int, double> data)
{
    dataMonth_ = data;
    isDataMonthReady_ = !isDataMonthReady_;
    setStatisticsPerMonth(1);
}

void Statistics::setStatisticsPerMonth(const int month)
{

    ui->widget_loadPerMonth->clearGraphs();
    QVector<double> data;
    for (const auto& [key, value] : dataMonth_.asKeyValueRange())
    {
        if (key == month)
        {
            data.append(value);
        }
    }
    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    for(int i = 0; i < data.size(); ++i)
    {
        ticks << i + 1;
        labels << QString::number(i + 1);
    }
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->widget_loadPerMonth->xAxis->setTicker(textTicker);
    ui->widget_loadPerMonth->xAxis->setTickLabelRotation(60);
    ui->widget_loadPerMonth->xAxis->setSubTicks(false);
    ui->widget_loadPerMonth->xAxis->setTickLength(0, 4);
    ui->widget_loadPerMonth->xAxis->setLabel("Дни");
    ui->widget_loadPerMonth->xAxis->setRange(0, data.size() + 1);
    ui->widget_loadPerMonth->xAxis->grid()->setVisible(true);

    // prepare y axis:
    ui->widget_loadPerMonth->yAxis->setRange(0, *std::max_element(data.begin(), data.end()) + 1);
    ui->widget_loadPerMonth->yAxis->setPadding(5); // a bit more space to the left border
    ui->widget_loadPerMonth->yAxis->setLabel("Загруженность аэропорта за месяц");
    ui->widget_loadPerMonth->yAxis->grid()->setSubGridVisible(true);
    ui->widget_loadPerMonth->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->widget_loadPerMonth->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    CPBarsForMonth_->setData(ticks, data);
    ui->widget_loadPerMonth->replot();
}

int Statistics::findMonth(const QString &arg1)
{
    for (const auto& [key, value] : months_.asKeyValueRange())
    {
        if (arg1 == value)
        {
            return key;
        }
    }
    return -1;
}

void Statistics::on_pb_close_clicked()
{
    isDataMonthReady_ = !isDataMonthReady_;
    ui->cb_months->setCurrentText(months_[1]);
    emit sig_CloseStatistics();
}

void Statistics::closeEvent(QCloseEvent *event)
{
    isDataMonthReady_ = !isDataMonthReady_;
    ui->cb_months->setCurrentText(months_[1]);
    emit sig_CloseStatistics();
}

