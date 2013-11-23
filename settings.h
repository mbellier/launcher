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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QString>
#include <QAbstractButton>

namespace Ui {
class Settings;
}

/*!
 * \brief Settings window
 *
 * Interface to modify the application parameters.
 *
 */
class Settings : public QDialog
{
  Q_OBJECT
  
public:
  Settings(const QString &settingsFile, QWidget *parent = 0);
  ~Settings();
  void         loadSettings();
  void         saveSettings();
  bool         settingSorted() const;
  unsigned int settingMaxColumnSize() const;
  unsigned int settingButtonWidth() const;
  unsigned int settingButtonHeight() const;
  bool         settingflatButtons() const;

private slots:
  void on_checkFlat_toggled(bool checked);
  void on_optionSortedTrue_toggled(bool checked);
  void on_sliderWidth_valueChanged(int value);
  void on_sliderButtonWidth_valueChanged(int value);
  void on_sliderButtonHeight_valueChanged(int value);

private:
  Ui::Settings *ui;
  QString       m_settingsFile;
  bool          m_settingSorted;
  unsigned int  m_settingMaxColumnSize;
  unsigned int  m_settingButtonWidth;
  unsigned int  m_settingButtonHeight;
  bool          m_settingFlatButtons;
};

#endif // SETTINGS_H
