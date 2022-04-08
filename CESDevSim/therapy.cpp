#include "Therapy.h"

Therapy::Therapy(int session_number, int duration, int intensity){
    this->session_number = session_number;
    this->duration = duration;
    this->intensity = intensity;
}

int Therapy::getSession(){
    return(this->session_number);
}

int Therapy::getDuration(){
    return(this->duration);
}

int Therapy::getIntensity(){
    return(this->intensity);
}
