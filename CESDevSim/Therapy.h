#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy(int id, QString session_type, int duration, int intensity);
        ~Therapy();

        int getId();
        QString getSession();
        int getDuration();
        int getIntensity();

        void setIntensity(int intensity);

    private:
       // QTimer timer;
       // QString intensity;
        int id;
        QString session_type;
        int duration;
        int intensity;
};

#endif // THERAPY_H
