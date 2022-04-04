#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

class User {
    public:
        User(int id);

    private:
        int id;
        QString preferences;
        QList<QString> historyList;
};

#endif // USER_H
