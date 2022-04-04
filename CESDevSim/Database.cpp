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




    return true;
}

