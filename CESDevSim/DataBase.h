#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "User.h"

class DataBase {
    public:
        DataBase();

        void addTherapyRecord();
        void addProfile(int id);

        bool initDB();
        User* getUser(int id);

    private:
        QSqlDatabase db;

};

#endif // DATABASE_H
