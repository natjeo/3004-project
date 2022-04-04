#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

class User {
    public:
        User(int id, const QString& name);

        QString getPreferences();
        QList<QString> getHistoryList();
        QString getName();

        void setPreferences(const QString&);
        void addToHistory(const QString&);

    private:
        int id;
        QString name;
        QString preferences;
        QList<QString> historyList;
};

#endif // USER_H
