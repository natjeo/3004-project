#ifndef BATTERY_H
#define BATTERY_H

#include <QString>
#include <QTimer>
#include <QDebug>
#include <QtMath>

#define NORMAL 0
#define LOW 1
#define CRITICALLY_LOW 2

#define LOW_LEVEL 25
#define CRITICALLY_LOW_LEVEL 12

#define BATTERY_LENGTH_MINS 20
#define INIT_LEVEL 100

class Battery: public QObject {
	public:
        Battery(int = INIT_LEVEL);

		int curStatus();
		int getLevel();
		void setLevel(int);
		int getLevelForDisplayGraph();
		void drainBattery(int);
        void startDrain(int, int);
		QTimer* getTimer();

	private:
		void updateBattery();
		QTimer* timer;
		int level;
};

#endif // BATTERY_H
