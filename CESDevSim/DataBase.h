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

        bool updatePreference(QString, int);
        int getPreference(QString);
        bool updateBatteryLvl(int, int);
        bool addTherapyRecord(Therapy*);
        QList<Therapy*> getTherapyRecords();


    private:
        QSqlDatabase db;

};

#endif // DATABASE_H
