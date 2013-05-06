#ifndef STATISTICS_DIALOG_H
#define STATISTICS_DIALOG_H

#include <QDialog>
#include "statistics.h"
#include "qcustomplot.h"
#include <QMouseEvent>

namespace Ui {
class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit StatisticsDialog(Statistics &stats, QWidget *parent = 0);
  ~StatisticsDialog();
  

public slots:
  void legendClicked (QCPLegend *legend,
                      QCPAbstractLegendItem *item,
                      QMouseEvent *event );

private:
  Ui::StatisticsDialog *ui;
  Statistics &m_stats;

};

#endif // STATISTICS_DIALOG_H
