#include "Battery.h"

Battery::Battery(int initLevel){
	this->timer = new QTimer(this);
	this->level = initLevel;
	connect(timer, &QTimer::timeout, this, &Battery::updateBattery);
}

void Battery::updateBattery() {
	this->drainBattery(1);
}

void Battery::startDrain() {
    int timeout = (BATTERY_LENGTH_MINS * 60 * 1000) / INIT_LEVEL;
    timer->start(timeout);
}

void Battery::drainBattery(int reduction){
	if (this->level < reduction) {
		this->level = 0;
	} else {
		this->level -= reduction;
	}
	qDebug() << "Battery: " << this->level;
}

int Battery::curStatus() {
	if (this->level <= CRITICALLY_LOW_LEVEL) {
		return CRITICALLY_LOW;
	} else if (this->level <= LOW_LEVEL) {
		return LOW;
	} else {
		return NORMAL;
	}
}

int Battery::getLevel() {
	return this->level;
}

void Battery::setLevel(int level) {
	this->level = level;
}

int Battery::getLevelForDisplayGraph() {
	return qCeil(this->level / 12.5);
}

QTimer* Battery::getTimer() {
	return this->timer;
}
