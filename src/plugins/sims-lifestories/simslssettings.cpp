/*
 * Graphics Rules Maker
 * Copyright (C) 2014-2018 Wouter Haffmans <wouter@simply-life.net>
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
 */

#include "simslssettings.h"
#include "ui_simslssettings.h"

#include <QtCore/QSettings>

#include "graphicsrulesmaker/devicemodel.h"
#include "graphicsrulesmaker/videocarddatabase.h"

SimsLSSettings::SimsLSSettings(DeviceModel *devices, VideoCardDatabase *database, QWidget* parent)
	: QWidget(parent)
{
    ui = new Ui::SimsLSSettings;
    ui->setupUi(this);

    connect(ui->resetDefaults, SIGNAL(clicked(bool)), SLOT(reset()));

    // Load available resolutions
    QStringList resolutions;
    int previousWidth = -1;
    int previousHeight = -1;
    foreach(const GraphicsMode &mode, devices->allModes()) {
        // We don't care about refresh rates - i.e. avoid duplicates from the list
        if (mode.width != previousWidth || mode.height != previousHeight) {
            resolutions << QString::number(mode.width) + "x" + QString::number(mode.height);
            previousWidth = mode.width;
            previousHeight = mode.height;
        }
    }

    ui->defaultResolution->addItems(resolutions);
    ui->maxResolution->addItems(resolutions);

    // Load Settings
    QSettings s;
    s.beginGroup("sims-lifestories");

    ui->forceMem->setValue(s.value("forceMemory", 0).toInt());
    ui->disableSimShadows->setChecked(s.value("disableSimShadows", false).toBool());
    ui->radeonHd7000Fix->setChecked(s.value("radeonHd7000Fix", false).toBool());
    ui->intelHigh->setChecked(s.value("intelHigh", false).toBool());
    ui->intelVsync->setChecked(s.value("intelVsync", false).toBool());
    ui->defaultResolution->setCurrentText(s.value("defaultResolution", "1024x768").toString());
    ui->maxResolution->setCurrentText(s.value("maximumResolution", "1600x1200").toString());

    s.endGroup();
}

SimsLSVariables SimsLSSettings::current() const
{
    QRegExp resolutionString("^(\\d+)x(\\d+)$");
    SimsLSVariables result;
    result.forceMemory = ui->forceMem->value();
    result.disableSimShadows = ui->disableSimShadows->isChecked();
    result.radeonHd7000Fix = ui->radeonHd7000Fix->isChecked();
    result.intelHigh = ui->intelHigh->isChecked();
    result.intelVsync = ui->intelVsync->isChecked();

    if (resolutionString.exactMatch(ui->defaultResolution->currentText())) {
        result.defaultResolution.setWidth(resolutionString.cap(1).toInt());
        result.defaultResolution.setHeight(resolutionString.cap(2).toInt());
    }

    if (resolutionString.exactMatch(ui->maxResolution->currentText())) {
        result.maximumResolution.setWidth(resolutionString.cap(1).toInt());
        result.maximumResolution.setHeight(resolutionString.cap(2).toInt());
    }

    return result;
}

void SimsLSSettings::reset()
{
    // Restore all defaults
    ui->forceMem->setValue(0);
    ui->radeonHd7000Fix->setChecked(false);
    ui->intelHigh->setChecked(false);
    ui->intelVsync->setChecked(false);
    ui->defaultResolution->setCurrentText("1024x768");
    ui->maxResolution->setCurrentText("1600x1200");
}

SimsLSSettings::~SimsLSSettings()
{
    // Write settings
    QSettings s;
    SimsLSVariables vars = current();

    s.beginGroup("sims-lifestories");
    s.setValue("forceMemory", ui->forceMem->value());
    s.setValue("radeonHd7000Fix", ui->radeonHd7000Fix->isChecked());
    s.setValue("intelHigh", ui->intelHigh->isChecked());
    s.setValue("intelVsync", ui->intelVsync->isChecked());
    s.setValue("defaultResolution", ui->defaultResolution->currentText());
    s.setValue("maximumResolution", ui->maxResolution->currentText());
    s.endGroup();

    delete ui;
}

#include "moc_simslssettings.cpp"
