#include "DataBase.h"

DataBase::DataBase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("oasis");

    if (!(db.open())) {
        throw "COULDN'T OPEN THE DATABASE";
    }

    // initialize db
    if (!initDB()) {
        throw "COULDN'T INITIALIZE THE DATABASE";
    }
}

bool DataBase::initDB(){

    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;

    // example create a tables
    if (!query.exec("CREATE TABLE IF NOT EXISTS therapy(therapy_id INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT, session_number INTEGER NOT NULL, duration INTEGER NOT NULL, intensity INTEGER NOT NULL);")){
        return false;
    }
    if (!query.exec("CREATE TABLE IF NOT EXISTS user(user_id INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, battery_lvl INTEGER NOT NULL, pref_intensity INTEGER NOT NULL);")){
       return false;
    }

    // example insert a row
    query.prepare("INSERT OR REPLACE INTO user(user_id, name, battery_lvl, pref_intensity) VALUES (:id, :name, :battery_lvl, :pref_intensity);");
    query.bindValue(":id", 0);
    query.bindValue(":name", "Jane");
    query.bindValue(":battery_lvl", 100);
    query.bindValue(":pref_intensity", 1);

    if (!query.exec()){
        return false;
    }

    return true;
}

User* DataBase::getUser(int id){
    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE user_id=:id");
    query.bindValue(":id", id);
    query.exec();

    if (!db.commit()) {
        throw "COULDN'T EXECUTE THE QUERY";
    }

    int battery_lvl;
    int pref_intensity;
    QString name;

    if (query.first()) {
        battery_lvl = query.value(2).toInt();
        name = query.value(1).toString();
        pref_intensity = query.value(3).toInt();

    } else {
        throw "NO USER FOUND";
    }

    User* user = new User(id, name);
    user->setPreferences(pref_intensity);
    user->setBatteryLvl(battery_lvl);

    return(user);
}


bool DataBase::updatePreferences(int user_id, int pref_intensity){

    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;

    query.prepare("UPDATE user SET pref_intensity=:pref_intensity WHERE user_id=:id;");
    query.bindValue(":id", user_id);
    query.bindValue(":pref_intensity", pref_intensity);

    if (!query.exec()){
        return false;
    }

    return true;
}


bool DataBase::updateBatteryLvl(int user_id, int battery_lvl){
    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;

    query.prepare("UPDATE user SET battery_lvl=:battery_lvl WHERE user_id=:id;");
    query.bindValue(":id", user_id);
    query.bindValue(":battery_lvl", battery_lvl);

    if (!query.exec()){
        return false;
    }

    return true;
}

//bool DataBase::addTherapyRecord(Therapy* therapy){

//    // makes sure the operation is atomic
//    db.transaction();

//    QSqlQuery query;

//    int session = therapy->getSession();
//    int duration = therapy->getDuration();
//    int intensity = therapy->getIntensity();

//    // example insert a row
//    query.prepare("INSERT OR REPLACE INTO therapy(session_number, duration, intensity) VALUES (:session, :duration, :intensity);");
//    query.bindValue(":session", session);
//    query.bindValue(":duration", duration);
//    query.bindValue(":intensity", intensity);

//    if (!query.exec()){
//        return false;
//    }

//    return true;
//}

//QList<Therapy*> DataBase::getTherapyRecords(int){
//    // makes sure the operation is atomic
//    db.transaction();

//    QSqlQuery query;
//    if (!query.exec("SELECT * FROM therapy")) {
//        throw "COULDN'T EXEC THE QUERY";
//    }

//    while(query.next()) {

//    }


//    if (query.first()) {
//        battery_lvl = query.value(2).toInt();
//        name = query.value(1).toString();
//        pref_intensity = query.value(3).toInt();

//    } else {
//        throw "NO USER FOUND";
//    }

//    User* user = new User(id, name);
//    user->setPreferences(pref_intensity);
//    user->setBatteryLvl(battery_lvl);

//    return(user);
//}
