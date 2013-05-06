#include "statistics_dialog.h"
#include "ui_statistics_dialog.h"

#include <string.h>
#include <iostream>
using namespace std;

StatisticsDialog::StatisticsDialog(Statistics &stats, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::StatisticsDialog),
  m_stats(stats)
{
  ui->setupUi(this);

  stats.sort();

  // set locale to english, so we get english month names:
  ui->customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  // seconds of current time, we'll use it as starting point in time for data:
  double now = QDateTime::currentDateTime().toTime_t();

  int gi = 0;
  double minTime = now;
  double maxValue = 0;
  for (DatabaseIterator element = stats.begin();
       element != stats.end(); element++){

    Element e = (Element)(*element);

    // new graph per element
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString::fromStdString(e.name()));
    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setPen(pen);
    ui->customPlot->graph()->setBrush(QBrush(QColor(255 / stats.nbElements() * gi,
                                                    255-255/stats.nbElements()*gi,
                                                    0,
                                                    150)));
    gi++;

    // get dates
    QVector<double> time, value;
    int i = 0;
    for (ElementIterator data = e.begin(); data != e.end(); data++){
      QString stringDate = QString::fromStdString((std::string)(*data));
      QDateTime date = QDateTime::fromString(stringDate, "yyyy MM dd hh mm");
      QDateTime date2 = QDateTime(QDate(date.date().year(), date.date().month(),date.date().day()));
      double t = date2.toTime_t();
      ///if (t < now - 24*3600*30*4) continue;
      if (time.size() == 0){
        time.append(0);
        value.append(0);
        time.append(t-1);
        value.append(0);
        time.append(t);
        value.append(1);
      }
      if (time.size() > 0){
        if (time.back() < t){
          time.append(t-1);
          value.append(value.last());
          time.append(t);
          value.append(1);
        }else{
          value.last()++;
        }
      }

      if (minTime > t) minTime = t;
      if (maxValue < value.last()) maxValue = value.last();
      i++;
    }

    ui->customPlot->graph()->setData(time, value);
  }


  // configure bottom axis to show date and time instead of number:
  ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->customPlot->xAxis->setDateTimeFormat("MMMM\nyyyy");

  // set a more compact font size for bottom and left axis tick labels:
  ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

  // set a fixed tick-step to one tick per month:
  ui->customPlot->xAxis->setAutoTickStep(false);
  ui->customPlot->xAxis->setTickStep(2628000); // one month in seconds
  ui->customPlot->xAxis->setSubTickCount(3);

  // apply auto tick and tick label for left axis:
  ui->customPlot->yAxis->setAutoTicks(true);
  ui->customPlot->yAxis->setAutoTickLabels(true);


  // set axis labels:
 // ui->customPlot->xAxis->setLabel("Date");
  //ui->customPlot->yAxis->setLabel("Number of launches");

  // make top and right axes visible but without ticks and labels:
  ui->customPlot->xAxis2->setVisible(true);
  ui->customPlot->yAxis2->setVisible(true);
  ui->customPlot->xAxis2->setTicks(false);
  ui->customPlot->yAxis2->setTicks(false);
  ui->customPlot->xAxis2->setTickLabels(false);
  ui->customPlot->yAxis2->setTickLabels(false);

  // set axis ranges to show all data:
  ui->customPlot->xAxis->setRange(minTime/*now - 24*3600*365*/, now);
  ui->customPlot->yAxis->setRange(0, maxValue);


  // activate legend and position it in top left corner:
  ui->customPlot->legend->setVisible(true);
  ui->customPlot->legend->setPositionStyle(QCPLegend::psTopRight);
  ui->customPlot->legend->setPositionStyle(QCPLegend::psManual);
  ui->customPlot->legend->setAutoSize(false);
  ui->customPlot->legend->setSize(160,ui->customPlot->size().height());
  ui->customPlot->legend->setPosition(QPoint(ui->customPlot->width() - ui->customPlot->legend->size().width(),0));
  ui->customPlot->setAutoMargin(false);
  ui->customPlot->setMargin(40,ui->customPlot->legend->size().width(),0,40);

  QPen pen;
  pen.setColor(QColor(0, 0, 255, 200));
  ui->customPlot->legend->setSelectable(ui->customPlot->legend->spItems);
  ui->customPlot->legend->setSelectedBorderPen(pen);
  ui->customPlot->setInteraction(QCustomPlot::iSelectLegend);
  connect(ui->customPlot, SIGNAL(legendClick(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)),
          this, SLOT(legendClicked(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)));


  // set graph scrollable and zoomable
  ui->customPlot->setInteraction(QCustomPlot::iSelectPlottables);
  ui->customPlot->setRangeZoom(Qt::Horizontal);
  ui->customPlot->setRangeDrag(Qt::Horizontal);

}

StatisticsDialog::~StatisticsDialog()
{
  delete ui;
}

void StatisticsDialog::legendClicked(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event)
{
  cerr << "click!" << endl;

  for (int k = 0; k < ui->customPlot->graphCount(); k++){
    if (legend->itemWithPlottable(ui->customPlot->graph(k)) == (QCPPlottableLegendItem*)item){
      QPen pen;
      pen.setColor(QColor(255, 0, 0, 255));
      pen.setWidth(2);
      ui->customPlot->graph(k)->setPen(pen);
    }else{
      QPen pen;
      pen.setColor(QColor(0, 0, 255, 200));
      pen.setWidth(1);
      ui->customPlot->graph(k)->setPen(pen);

    }
  }

}
