#include "DataBase.h"

DataBase::DataBase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("oasis");

    if (!(db.open())) {
        /* throw "COULDN'T OPEN THE DATABASE"; */
    }

    // initialize db
    if (!initDB()) {
        /* throw "COULDN'T INITIALIZE THE DATABASE"; */
    }
}

bool DataBase::initDB(){

    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;
//    if (!query.exec("DROP TABLE therapy")) {
//        throw "COULDN'T EXEC THE QUERY";
//    }

    // example create a tables
    if (!query.exec("CREATE TABLE IF NOT EXISTS therapy(therapy_id INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT, session_type TEXT NOT NULL, duration INTEGER NOT NULL, intensity INTEGER NOT NULL);")){
        return false;
    }
    if (!query.exec("CREATE TABLE IF NOT EXISTS user(user_id INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, battery_lvl INTEGER NOT NULL, pref_intensity INTEGER NOT NULL);")){
       return false;
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS session_preference(session_type TEXT NOT NULL UNIQUE PRIMARY KEY, pref_intensity INTEGER NOT NULL);")){
       return false;
    }

    if (!query.exec("INSERT OR REPLACE INTO session_preference VALUES('MET', 0), ('Sub-Delta', 0), ('Delta', 0), ('Theta', 0), ('Alpha', 0), ('SMR', 0), ('Beta', 0), ('100Hz', 0)")) {
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

bool DataBase::addTherapyRecord(Therapy* therapy){

    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;

    QString session = therapy->getSession();
    int duration = therapy->getDuration();
    int intensity = therapy->getIntensity();

    // example insert a row
    query.prepare("INSERT OR REPLACE INTO therapy(session_type, duration, intensity) VALUES (:session, :duration, :intensity);");
    query.bindValue(":session", session);
    query.bindValue(":duration", duration);
    query.bindValue(":intensity", intensity);

    if (!query.exec()){
        return false;
        throw "COULDN'T EXEC THE QUERY";
    }

    return true;
}

QList<Therapy*> DataBase::getTherapyRecords(){
    // makes sure the operation is atomic
    db.transaction();

    QList<Therapy*> therapyList = {};


    QSqlQuery query;
    if (!query.exec("SELECT * FROM therapy")) {
        qInfo("what");
        //throw "COULDN'T EXEC THE QUERY";
    }

    while(query.next()) {
        therapyList.append(new Therapy(query.value(1).toString(), query.value(2).toInt(), query.value(3).toInt()));
    }

    return(therapyList);
}


bool DataBase::updatePreference(QString session_type, int pref_intensity){
    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;

    query.prepare("UPDATE session_preference SET pref_intensity=:pref_intensity WHERE session_type=:session_type;");
    query.bindValue(":session_type", session_type);
    query.bindValue(":pref_intensity", pref_intensity);

    if (!query.exec()){
        return false;
    }

    return true;
}

int DataBase::getPreference(QString session_type){
    // makes sure the operation is atomic
    db.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM session_preference WHERE session_type=:session_type");
    query.bindValue(":session_type", session_type);
    query.exec();

    if (!db.commit()) {
        throw "COULDN'T EXECUTE THE QUERY";
    }

    int pref_intensity = 0;

    if (query.first()) {
        pref_intensity = query.value(1).toInt();

    } else {
        throw "NO USER FOUND";
    }

    return pref_intensity;
}
