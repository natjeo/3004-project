#include "Therapy.h"

Therapy::Therapy(int id, QString session_type, int duration, int intensity){
    this->id = id;
    this->session_type = session_type;
    this->duration = duration;
    this->intensity = intensity;
}

int Therapy::getId(){
    return(this->id);
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
