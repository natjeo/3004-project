#include "User.h"

User::User(int id, QString name){
    this->id = id;
    this->name = name;
}

//getters

int User::getPreferences(){
    return this->preferences;
}

int User::getBatteryLvl(){
    return this->batteryLvl;
}

int User::getID(){
    return this->id;
}

QString User::getName(){
    return this->name;
}

// setters
void User::setPreferences(int preference){
    this->preferences = preference;
}

void User::setBatteryLvl(int lvl){
    this->batteryLvl = lvl;
}
