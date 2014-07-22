/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  W. Haffmans <wouter@simply-life.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SIMS2SETTINGS_H
#define SIMS2SETTINGS_H

#include <qt/QtWidgets/QWidget>

namespace Ui
{
class Sims2Settings;
}

class Sims2Settings : public QWidget
{
    Q_OBJECT
public:
    Sims2Settings(QWidget* parent);
    ~Sims2Settings();

private:
    Ui::Sims2Settings* ui;
};

#endif // SIMS2SETTINGS_H