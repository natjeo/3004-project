#include "Therapy.h"

Therapy::Therapy(QString session_type, int duration, int intensity){
    this->session_type = session_type;
    this->duration = duration;
    this->intensity = intensity;
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

bool Therapy::readyToStart(){
    return ((session_type != "NAN") && (duration != -1) && (intensity != 0));
}
