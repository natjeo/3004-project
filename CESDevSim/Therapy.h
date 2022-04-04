#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

class Therapy {
    public:
        Therapy(QTimer timer, QString intensity);

    private:
        QTimer timer;
        QString intensity;
};

#endif // THERAPY_H
