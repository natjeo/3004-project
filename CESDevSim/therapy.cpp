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
