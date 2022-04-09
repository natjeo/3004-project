#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "User.h"
#include "Therapy.h"

class DataBase {
    public:
        DataBase();

        bool initDB();
        User* getUser(int id);

        bool updatePreferences(int, int);
        bool updateBatteryLvl(int, int);
        bool addTherapyRecord(Therapy*);
        QList<Therapy*> getTherapyRecords();
        bool updatePreferences(Therapy* therapy);


    private:
        QSqlDatabase db;

};

#endif // DATABASE_H
