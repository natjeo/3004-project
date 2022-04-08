#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy(QString session_type, int duration, int intensity);
        ~Therapy();

        QString getSession();
        int getDuration();
        int getIntensity();

    private:
       // QTimer timer;
       // QString intensity;
        QString session_type;
        int duration;
        int intensity;
};

#endif // THERAPY_H
