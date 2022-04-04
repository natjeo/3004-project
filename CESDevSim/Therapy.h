#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy(QTimer* timer, QString intensity);

        // getters
        QString getIntensity();
        QTimer* getTimer();

        // setters
        void setIntensity(QString intensity);

    private:
        QTimer* timer;
        QString intensity;
};

#endif // THERAPY_H
