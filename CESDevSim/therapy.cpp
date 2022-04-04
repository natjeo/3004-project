#include "therapy.h"

Therapy::Therapy(QTimer* timer, QString intensity) {
    this->timer = timer;
    this->intensity = intensity;
}

// getters
QString Therapy::getIntensity() { return intensity; }
QTimer* Therapy:: getTimer() { return timer; }

// setters
void Therapy::setIntensity(QString i) {intensity = i;}
