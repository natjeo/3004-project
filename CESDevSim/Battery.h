#ifndef BATTERY_H
#define BATTERY_H

#include <QString>
#include <QTimer>

class Battery {
    public:
        Battery(const QString&);

        void updateTimer();
        void navigateMenu();

    private:
        QTimer* timer;
        QString intensity;
};

#endif // BATTERY_H
