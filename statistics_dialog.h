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
**             Date: 05.2013                                              **
****************************************************************************/

#ifndef STATISTICS_DIALOG_H
#define STATISTICS_DIALOG_H

#include <QDialog>
#include "statistics.h"
#include "qcustomplot.h"
#include <QMouseEvent>
#include <QAbstractButton>

namespace Ui {
class StatisticsDialog;
}

/*!
 * \brief Statistics window
 *
 * Displays the statistics in a plot.
 *
 */
class StatisticsDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit StatisticsDialog(Statistics &stats, QWidget *parent = 0);
  ~StatisticsDialog();
  void resizeEvent (QResizeEvent *);

public slots:
  void legendClicked (QCPLegend *legend,
                      QCPAbstractLegendItem *item,
                      QMouseEvent * );

private slots:
  void on_buttonBox_2_clicked(QAbstractButton *);

private:
  Ui::StatisticsDialog *ui;
  Statistics &m_stats;
  QPen pen;
  QPen selectedPen;
};

#endif // STATISTICS_DIALOG_H
