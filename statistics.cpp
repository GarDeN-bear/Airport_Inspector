#include "statistics.h"
#include "ui_statistics.h"

Statistics::Statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);
    months_ = {"Январь", "Февраль", "Март",
               "Апрель", "Май", "Июнь",
               "Июль", "Август", "Сентябрь",
               "Октябрь", "Ноябрь", "Декабрь"
              };

    for(const auto& el : months_)
    {
        ui->cb_months->addItem(el);
    }
    CPBarsForYear_ = new QCPBars(ui->widget_loadPerYear->xAxis,
                                 ui->widget_loadPerYear->yAxis
                                 );

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->widget_loadPerYear->xAxis->setTicker(textTicker);
    ui->widget_loadPerYear->xAxis->setTickLabelRotation(60);
    ui->widget_loadPerYear->xAxis->setSubTicks(false);
    ui->widget_loadPerYear->xAxis->setTickLength(0, 4);
    ui->widget_loadPerYear->xAxis->setRange(0, 8);
    ui->widget_loadPerYear->xAxis->setBasePen(QPen(Qt::white));
    ui->widget_loadPerYear->xAxis->setTickPen(QPen(Qt::white));
    ui->widget_loadPerYear->xAxis->grid()->setVisible(true);
    ui->widget_loadPerYear->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->widget_loadPerYear->xAxis->setTickLabelColor(Qt::white);
    ui->widget_loadPerYear->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    ui->widget_loadPerYear->yAxis->setRange(0, 12.1);
    ui->widget_loadPerYear->yAxis->setPadding(5); // a bit more space to the left border
    ui->widget_loadPerYear->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
    ui->widget_loadPerYear->yAxis->setBasePen(QPen(Qt::white));
    ui->widget_loadPerYear->yAxis->setTickPen(QPen(Qt::white));
    ui->widget_loadPerYear->yAxis->setSubTickPen(QPen(Qt::white));
    ui->widget_loadPerYear->yAxis->grid()->setSubGridVisible(true);
    ui->widget_loadPerYear->yAxis->setTickLabelColor(Qt::white);
    ui->widget_loadPerYear->yAxis->setLabelColor(Qt::white);
    ui->widget_loadPerYear->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->widget_loadPerYear->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> fossilData, nuclearData, regenData;
    fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
    nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
    regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
    CPBarsForYear_->setData(ticks, fossilData);

}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::SetAirport(const QString &airport)
{
    ui->l_airportsLoad->setText("Загруженность аэропорта " + airport);
}

void Statistics::closeEvent(QCloseEvent *event)
{
    emit sig_CloseStatistics();
}

void Statistics::on_pb_close_clicked()
{
    emit sig_CloseStatistics();
}

