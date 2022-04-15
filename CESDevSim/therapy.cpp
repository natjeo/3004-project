#include "Therapy.h"

Therapy::Therapy(QString session_type, int duration, int intensity, bool therapyInProgress){
    this->session_type = session_type;
    this->duration = duration;
    this->intensity = intensity;
    this->therapyInProgress = therapyInProgress;
}

QString Therapy::getSession(){
    return(this->session_type);
}

int Therapy::getDuration(){
    return(this->duration);
}

int Therapy::getIntensity(){
    return(this->intensity);
}


void Therapy::setIntensity(int intensity){
    this->intensity = intensity;
}

void Therapy::setSession(QString session_type){
    this->session_type = session_type;
}

void Therapy::setDuration(int duration){
    this->duration = duration;
}

void Therapy::updateTherapyInProgress(bool therapyState){
    this->therapyInProgress = therapyState;
}

bool Therapy::readyToStart(){
    return ((session_type != "NAN") && (duration != -1) && (intensity != 0));
}

bool Therapy::isTherapyInProgress(){
    return (this->therapyInProgress);
}
