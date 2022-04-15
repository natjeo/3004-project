#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy( QString session_type = "NAN", int duration = -1, int intensity = 0, bool therapyInProgress=false);
        ~Therapy();

        QString getSession();
        int getDuration();
        int getIntensity();

        void setIntensity(int intensity);
        void setSession(QString session_type);
        void setDuration(int duration);
        void updateTherapyInProgress(bool);
        void endSession();

        bool readyToStart();
        bool isTherapyInProgress();

    private:
       // QTimer timer;
       // QString intensity;
        QString session_type;
        int duration;
        int intensity;
        bool therapyInProgress;
};

#endif // THERAPY_H
