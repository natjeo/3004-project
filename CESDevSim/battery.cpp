#include "Battery.h"

Battery::Battery(int initLevel){
	this->timer = new QTimer(this);
	this->level = initLevel;
	connect(timer, &QTimer::timeout, this, &Battery::updateBattery);
	timer->start(100);
}

void Battery::updateBattery() {
	this->drainBattery(1);
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

int Battery::getLevelForDisplayGraph() {
	return qCeil(this->level / 12.5);
}

QTimer* Battery::getTimer() {
	return this->timer;
}
