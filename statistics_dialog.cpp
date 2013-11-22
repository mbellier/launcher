/***************************************************************************
**                                                                        **
**  This file is part of the program 'Launcher'.                          **
**  Copyright (C) 2013 Maxime Bellier                                     **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Maxime Bellier                                       **
**  Website/Contact: http://puissance.m4x.free.fr                         **
**             Date: 05.2013                                           **
****************************************************************************/

/*!
 * \file statistics_dialog.cpp
 * \brief Statistics window
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
 * \copyright This project is released under the GNU Public License.
 */

#include "statistics_dialog.h"
#include "ui_statistics_dialog.h"

#include <string.h>

using namespace std;

StatisticsDialog::StatisticsDialog(Statistics &stats,
                                   QWidget *parent) :
  QDialog(parent, Qt::CustomizeWindowHint),
  ui(new Ui::StatisticsDialog),
  m_stats(stats)
{
  this->setWindowFlags(Qt::WindowCloseButtonHint & Qt::WindowMaximizeButtonHint );
  ui->setupUi(this);

  // sort statistics
  stats.sort();

  // set pens color
  pen.setColor(QColor(0, 0, 255, 100));
  selectedPen.setColor(QColor(255, 0, 0, 255));

  // set locale to english for month names:
  ui->customPlot->setLocale(QLocale(QLocale::English,
                                    QLocale::UnitedKingdom));
  // current time in seconds
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
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setPen(pen);
    ui->customPlot->graph()->setBrush(QBrush(QColor(255 / stats.nbElements() * gi,
                                                    255-255/stats.nbElements()*gi,
                                                    0,
                                                    150)));
    gi++;

    // get values, filtered by days
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
        value.append(0);
      }
      if (time.size() > 0){
        if (time.back() < t){
          time.append(time.last()+ 3600*24-100);
          value.append(value.last());
          time.append(time.last()+1);
          value.append(0);
          time.append(t-1);
          value.append(0);
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
    time.append(time.last()+1);
    value.append(0);
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
  ui->customPlot->xAxis->setSubTickCount(4);

  // apply auto tick and tick label for left axis:
//  ui->customPlot->yAxis->setAutoTicks(true);
  ui->customPlot->yAxis->setAutoTickLabels(true);
  ui->customPlot->yAxis->setAutoTickStep(false);
  ui->customPlot->yAxis->setTickStep(1);
  ui->customPlot->yAxis->setSubTickCount(1);


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


  // activate legend
  ui->customPlot->legend->setVisible(true);
  ui->customPlot->legend->setPositionStyle(QCPLegend::psTopRight);
  ui->customPlot->legend->setPositionStyle(QCPLegend::psManual);
  ui->customPlot->legend->setAutoSize(false);
  ui->customPlot->setAutoMargin(false);
  ui->customPlot->setMargin(40,160,0,40);

  // legend items are selectable
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

void StatisticsDialog::resizeEvent(QResizeEvent *)
{
  // move legend
  ui->customPlot->legend->setSize(160, ui->customPlot->size().height());
  ui->customPlot->legend->setPosition(QPoint(ui->customPlot->width() - 160,0));
  ui->customPlot->replot();
}


void StatisticsDialog::legendClicked(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *)
{
  for (int k = 0; k < ui->customPlot->graphCount(); k++){
    if (legend->itemWithPlottable(ui->customPlot->graph(k)) == (QCPPlottableLegendItem*)item){
      // selected
      ui->customPlot->graph(k)->setPen(selectedPen);
      ui->customPlot->addLayer("top");
      ui->customPlot->graph(k)->setLayer("top");
      QColor color(ui->customPlot->graph(k)->brush().color());
      color.setAlpha(255);
      ui->customPlot->graph(k)->setBrush(QBrush(color));
    }else{
      // unselected
      ui->customPlot->graph(k)->setPen(pen);
      QColor color(ui->customPlot->graph(k)->brush().color());
      color.setAlpha(150);
      ui->customPlot->graph(k)->setBrush(QBrush(color));
    }
  }

}

void StatisticsDialog::on_buttonBox_2_clicked(QAbstractButton *)
{
    this->close();
}
