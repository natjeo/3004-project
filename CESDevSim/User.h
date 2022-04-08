#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <string>

class User {
    public:
        User(int id, QString name);

        int getPreferences();
        int getBatteryLvl();
        QString getName();

        void setPreferences(int);
        void setBatteryLvl(int);

    private:
        int id;
        QString name;
        int preferences;
        int batteryLvl;
};

#endif // USER_H
