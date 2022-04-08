#include "User.h"

User::User(int id, const QString& name){
    this->id = id;
    this->name = name;
}

//getters

QString User::getPreferences(){
    return this->preferences;
}

QList<QString> User::getHistoryList(){
    return this->historyList;
}

QString User::getName(){
    return this->name;
}

// setters
void User::setPreferences(const QString& preference){
    this->preferences = preference;
}

void User::addToHistory(const QString& item){
    this->historyList.push_back(item);
}
