#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy(int session_number, int duration, int intensity);
        ~Therapy();


        int getSession();
        int getDuration();
        int getIntensity();

    private:
       // QTimer timer;
       // QString intensity;
        int session_number;
        int duration;
        int intensity;
};

#endif // THERAPY_H
