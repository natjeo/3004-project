#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy( QString session_type = "NAN", int duration = -1, int intensity = 0);
        ~Therapy();

        QString getSession();
        int getDuration();
        int getIntensity();

        void setIntensity(int intensity);
        void setSession(QString session_type);
        void setDuration(int duration);

        bool readyToStart();
    private:
       // QTimer timer;
       // QString intensity;
        QString session_type;
        int duration;
        int intensity;
};

#endif // THERAPY_H
