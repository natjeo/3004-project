#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class DataBase {
    public:
        DataBase();

        void addTherapyRecord();
        void addProfile(int id);

        bool initDB();

    private:
        QSqlDatabase db;

};

#endif // DATABASE_H
