#include "statistics.h"
#include "ui_statistics.h"

Statistics::Statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);
    CPBarsForYear_ = new QCPBars(ui->widget_loadPerYear->xAxis,
                                 ui->widget_loadPerYear->yAxis
                                 );
    CPBarsForMonth_ = new QCPBars(ui->widget_loadPerMonth->xAxis,
                                  ui->widget_loadPerMonth->yAxis
                                  );

    months_ = {"Январь", "Февраль", "Март",
               "Апрель", "Май", "Июнь",
               "Июль", "Август", "Сентябрь",
               "Октябрь", "Ноябрь", "Декабрь"
              };

    for(const auto& el : months_)
    {
        ui->cb_months->addItem(el);
    }
    ui->tab_perYear->activateWindow();
}

Statistics::~Statistics()
{
    delete CPBarsForYear_;
    delete CPBarsForMonth_;
    delete ui;
}

void Statistics::SetAirportText(const QString &airport)
{
    ui->l_airportsLoad->setText("Загруженность аэропорта " + airport);
}

void Statistics::closeEvent(QCloseEvent *event)
{
    emit sig_CloseStatistics();
}

void Statistics::SetStatisticsPerYear(const QVector<double> data)
{
    ui->widget_loadPerYear->clearGraphs();
    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    for(int i = 0; i < months_.size() + 1; ++i)
    {
        ticks << i + 1;
        if (i < months_.size())
        {
            labels << months_[i];
        }
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

void Statistics::on_pb_close_clicked()
{
    emit sig_CloseStatistics();
}

