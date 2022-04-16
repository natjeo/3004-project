#include "DeviceControl.h"
#include "ui_DeviceControl.h"
#include <QDebug>
#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->userInactive = new QTimer(this);

    // setup database for user
    db = new DataBase();
    ui->recordsList->setVisible(false);
    this->sessionTime = 0;

    this->user = db->getUser(0);

        QList<Therapy*> therapyHistory = db->getTherapyRecords();
        qDebug() << therapyHistory.length();
        //qDebug() << therapyHistory.first()->getSession();

        //QString name = user->getName();
        //printf(name.toLatin1());

				//this->battery = new Battery(user->getBatteryLvl());
				this->battery = new Battery;
				this->batteryDisplayTimer = new QTimer(this);
				connect(this->batteryDisplayTimer, &QTimer::timeout, this, &MainWindow::indicateBatteryLevel);

    connect(ui->btn_save, &QPushButton::pressed, this, &MainWindow::recordTherapy);
    connect(ui->btn_home, &QPushButton::pressed, this, &MainWindow::displayHomeScreen);
    connect(ui->btn_up, &QPushButton::pressed, this, &MainWindow::pressUp);
    connect(ui->btn_dn, &QPushButton::pressed, this, &MainWindow::pressDn);
    connect(ui->btn_ok, &QPushButton::pressed, this, &MainWindow::selectPressed);
    connect(ui->btn_ok, &QPushButton::released, this, &MainWindow::selectReleased);
    connect(ui->btn_history, &QPushButton::pressed, this, &MainWindow::displayHistory);
    connect(ui->btn_del, &QPushButton::pressed, this, &MainWindow::stopSession);

    // setup device power status
    powerState = false;
    isAdjustingIntensity = false;
    updatePowerState();
    connect(ui->btn_power, &QPushButton::pressed, this, &MainWindow::powerBtnPressed);
    connect(ui->btn_power, &QPushButton::released, this, &MainWindow::powerBtnReleased);

    graphBars = new QVector<QLabel*> { ui->bar_1, ui->bar_2, ui->bar_3, ui->bar_4, ui->bar_5, ui->bar_6, ui->bar_7, ui->bar_8 };

    QList<QPushButton *> allButtons = this->findChildren<QPushButton *>();
    QPushButton *button;
    foreach (button, allButtons){
        button->installEventFilter(this);
    }

    sessionInProgress=false;
    isRunningTest=false;
    this->intervalTimer = new QTimer(this);
}

void MainWindow::powerBtnPressed()
{
    elapsedTimer.start();
    if (!sessionInProgress && !isRunningTest) {
        qDebug() << "Pressed: " << powerState;
        powerState  = !powerState;
        updatePowerState();

        // group duration button selections
        selDur = new QButtonGroup();
        selDur->addButton(ui->dur_20min);
        selDur->setId(ui->dur_20min, 1);
        selDur->addButton(ui->dur_45min);
        selDur->setId(ui->dur_45min, 2);
        selDur->addButton(ui->dur_3hrs);
        selDur->setId(ui->dur_3hrs, 3);
        selDur->addButton(ui->dur_custom);
        selDur->setId(ui->dur_custom, 4);
        selDur->setExclusive(true);
        connect(selDur, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button){
              button->setChecked(true);
              selectDuration(selDur->checkedId());
        });

        // group session button selections
        selSes = new QButtonGroup();
        selSes->addButton(ui->ses_met);
        selSes->setId(ui->ses_met, 1);
        selSes->addButton(ui->ses_subDelta);
        selSes->setId(ui->ses_subDelta, 2);
        selSes->addButton(ui->ses_delta);
        selSes->setId(ui->ses_delta, 3);
        selSes->addButton(ui->ses_theta);
        selSes->setId(ui->ses_theta, 4);
        selSes->addButton(ui->ses_alpha);
        selSes->setId(ui->ses_alpha, 5);
        selSes->addButton(ui->ses_beta1);
        selSes->setId(ui->ses_beta1, 6);
        selSes->addButton(ui->ses_beta2);
        selSes->setId(ui->ses_beta2, 7);
        selSes->addButton(ui->ses_100Hz);
        selSes->setId(ui->ses_100Hz, 8);
        connect(selSes, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button){
              button->setChecked(true);
              selectSession(selSes->checkedId());
        });

        if (powerState) {
            this->battery->setLevel(this->user->getBatteryLvl());
            this->indicateBatteryLevel();
        } else {
            qDebug() << "no power state";
            this->user->setBatteryLvl(this->battery->getLevel());
        }
    }
}

void MainWindow::selectSession(int btnId)
{
    ui->ses_met->setStyleSheet("border-image: url(:/icons/MET.png);");
    ui->ses_subDelta->setStyleSheet("border-image: url(:/icons/Sub-Delta.png);");
    ui->ses_delta->setStyleSheet("border-image: url(:/icons/Delta.png);");
    ui->ses_theta->setStyleSheet("border-image: url(:/icons/Theta.png);");
    ui->ses_alpha->setStyleSheet("border-image: url(:/icons/Alpha.png);");
    ui->ses_beta1->setStyleSheet("border-image: url(:/icons/Beta 1.png);");
    ui->ses_beta2->setStyleSheet("border-image: url(:/icons/Beta 2.png);");
    ui->ses_100Hz->setStyleSheet("border-image: url(:/icons/100Hz.png);");

    switch (btnId){
        case 1:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/MET_on.png);");
            this->therapy->setSession("MET");
            this->therapy->setIntensity(db->getPreference("MET"));
            break;

        case 2:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Sub-Delta_on.png);");
            this->therapy->setSession("Sub-Delta");
            this->therapy->setIntensity(db->getPreference("Sub-Delta"));
            break;

        case 3:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Delta_on.png);");
            this->therapy->setSession("Delta");
            this->therapy->setIntensity(db->getPreference("Delta"));
            break;

        case 4:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Theta_on.png);");
            this->therapy->setSession("Theta");
            this->therapy->setIntensity(db->getPreference("Theta"));
            break;

        case 5:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Alpha_on.png);");
            this->therapy->setSession("Alpha");
            this->therapy->setIntensity(db->getPreference("Alpha"));
            break;

        case 6:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Beta 1_on.png);");
            this->therapy->setSession("SMR");
            this->therapy->setIntensity(db->getPreference("SMR"));
            break;

        case 7:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Beta 2_on.png);");
            this->therapy->setSession("Beta");
            this->therapy->setIntensity(db->getPreference("Beta"));
            break;

        case 8:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/100Hz_on.png);");
            this->therapy->setSession("100Hz");
            this->therapy->setIntensity(db->getPreference("100Hz"));
            break;

    }
}

void MainWindow::selectDuration(int btnId)
{
    ui->dur_20min->setStyleSheet("border-image: url(:/icons/20min Session.png);");
    ui->dur_45min->setStyleSheet("border-image: url(:/icons/45min Session.png);");
    ui->dur_3hrs->setStyleSheet("border-image: url(:/icons/3hr Session.png);");
    ui->dur_custom->setStyleSheet("border-image: url(:/icons/User Session.png);");

    switch (btnId){
        case 1:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/20min Session_on.png);");
            this->therapy->setDuration(20);
            break;

        case 2:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/45min Session_on.png);");
            this->therapy->setDuration(45);
            break;

        case 3:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/3hr Session_on.png);");
            this->therapy->setDuration(180);
            break;

        case 4:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/Sessions_on.png);");
            break;
    }
}

void MainWindow::recordTherapy(){
    ui->recordsList->setVisible(false);
    qDebug() << this->therapy->getSession();
    if (this->therapy->readyToStart()) {
        if (db->addTherapyRecord(this->therapy)) {
            qInfo("therapy recorded succ");
            displayMessage("therapy saved *SOMEBODY SPELL SUCCESSFULLY CORRECTLY FOR ME*"); //update this!!!!!!!!!!!!!!
        }
    } else {
        displayMessage("Plase select intensity before starting therapy");
    }
}

void MainWindow::displayHistory(){
    ui->btn_history->setVisible(false);
    ui->recordsList->clear();
    QList<Therapy*> therapyList = db->getTherapyRecords();
    for (int i = 0; i < therapyList.length(); i++) {
        QString info = "#:" + QString::number(i+1)+ " Duration " + QString::number(therapyList.at(i)->getDuration()) + "; Session type: " + (therapyList.at(i)->getSession()) + "; Intensity: " + QString::number(therapyList.at(i)->getIntensity());
        ui->recordsList->addItem(info);
    }

    ui->recordsList->setVisible(true);
}

void MainWindow::pressUp()
{
    if (!this->isRunningTest){
        this->isAdjustingIntensity = true;
        int intensity = this->therapy->getIntensity();
        if (intensity < 8){
            intensity++;
            this->therapy->setIntensity(intensity);
        }
        qDebug() << this->therapy->getIntensity();
        flashGraphBar(intensity, 500, intensity);
        qInfo("up");
        this->isAdjustingIntensity = false;
    }
}

void MainWindow::pressDn()
{
    int intensity = this->therapy->getIntensity();
    if (!this->isRunningTest && intensity > 0){
        this->isAdjustingIntensity = true;
        if (intensity > 1){
            intensity--;
            this->therapy->setIntensity(intensity);
        }
        qDebug() << this->therapy->getIntensity();
        flashGraphBar(intensity, 500, intensity);
        qInfo("down");
        this->isAdjustingIntensity = false;
    }
}

void MainWindow::updatePowerState()
{
    ui->dur_20min->setVisible(powerState);
    ui->dur_45min->setVisible(powerState);
    ui->dur_3hrs->setVisible(powerState);
    ui->dur_custom->setVisible(powerState);

    ui->mode_L->setVisible(powerState);
    ui->mode_R->setVisible(powerState);
    ui->connection_CES->setVisible(powerState);
    ui->connection_audio->setVisible(powerState);

    ui->skin_menu->setVisible(powerState);
    ui->table_menu->setVisible(powerState);

    ui->ses_100Hz->setVisible(powerState);
    ui->ses_alpha->setVisible(powerState);
    ui->ses_beta1->setVisible(powerState);
    ui->ses_beta2->setVisible(powerState);
    ui->ses_delta->setVisible(powerState);
    ui->ses_met->setVisible(powerState);
    ui->ses_subDelta->setVisible(powerState);
    ui->ses_theta->setVisible(powerState);

    ui->extra_1->setVisible(powerState);
    ui->extra_2->setVisible(powerState);
    ui->extra_025->setVisible(powerState);
    ui->extra_050->setVisible(powerState);
    ui->extra_075->setVisible(powerState);
    ui->extra_100->setVisible(powerState);
    ui->extra_150->setVisible(powerState);
    ui->extra_200->setVisible(powerState);
    ui->extra_tdcs->setVisible(powerState);

    ui->bar_1->setVisible(powerState);
    ui->bar_2->setVisible(powerState);
    ui->bar_3->setVisible(powerState);
    ui->bar_4->setVisible(powerState);
    ui->bar_5->setVisible(powerState);
    ui->bar_6->setVisible(powerState);
    ui->bar_7->setVisible(powerState);
    ui->bar_8->setVisible(powerState);
    ui->recordsList->setVisible(powerState);
    ui->btn_history->setVisible(powerState);

    if (powerState){
        // device will turn on
        ui->powerLED->setStyleSheet("image: url(:/icons/power_on.png);");
        this->therapy = new Therapy("MET", 20, db->getPreference("MET"));
        displayHomeScreen();
        connect(this->userInactive, &QTimer::timeout, this, &MainWindow::autoPower);
        this->userInactive->start(autoShutOffTime);
    } else {
        // device will turn off
        ui->powerLED->setStyleSheet("image: url(:/icons/power_off.png);");
        if (this->sessionTime > 0) {
            this->sessionTimer->stop();
            this->sessionTime = 0;
        }

    }
}


MainWindow::~MainWindow()
{
		this->db->updateBatteryLvl(this->user->getID(), this->user->getBatteryLvl());
    delete ui;
}

void MainWindow::updatePreferences(){
    qDebug() << db->getPreference("MET");
    if (db->updatePreference(this->therapy->getSession(), this->therapy->getIntensity())) {
        qInfo("preferences updated succ");
        QString message = "Preferences updated";

       qDebug() << db->getPreference("MET");
        displayMessage(message);
    }
}


void MainWindow::displayMessage(QString message){
        QTimer::singleShot(2000, this, &MainWindow::cleanMessage);
        ui->recordsList->setVisible(false);
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addText(message);
        ui->table_menu->setScene(scene);
}

int MainWindow::performConnectionTest() {
    this->isRunningTest = true;
    int n = 1;
		if (this->therapy->getSession() == "Sub-Delta") {
			modeLight = ui->mode_R;
			modeLightOnStyle = "image: url(:/icons/Right_on.png);";
			modeLightOffStyle = "image: url(:/icons/Right.png);";
		} else {
			modeLight = ui->mode_L;
			modeLightOnStyle = "image: url(:/icons/Left_on.png);";
			modeLightOffStyle = "image: url(:/icons/Left.png);";
		}

    intervalTimer = new QTimer(this);
    connect(intervalTimer, &QTimer::timeout, this, [=, &n]() {
        modeLight->setStyleSheet(n % 2 == 0 ? modeLightOffStyle : modeLightOnStyle);
        n++;
    });
    intervalTimer->start(300);
		// No connection
    flashGraphBar(8, 3000, 7);

		bool isOkayConnection = QRandomGenerator::global()->bounded(1);

		// Okay connection
		for (int i = 4; i <= 6; i++) {
				illuminateGraphBar(i);
		}
		delay(2000);
		for (int i = 4; i <= 6; i++) {
				darkenGraphBar(i);
		}

		if (isOkayConnection) {
			intervalTimer->stop();
			modeLight->setStyleSheet(modeLightOffStyle);
			this->isRunningTest = false;
			return OKAY_CONNECTION;
		}

		// Excellent connection
		for (int i = 1; i <= 3; i++) {
				illuminateGraphBar(i);
		}
		delay(2000);
		for (int i = 1; i <= 3; i++) {
				darkenGraphBar(i);
		}
    intervalTimer->stop();
    modeLight->setStyleSheet(modeLightOffStyle);
    this->isRunningTest = false;
		return EXCELLENT_CONNECTION;
}

// start therapy session
void MainWindow::selectPressed(){
    elapsedTimer.start();
    if (this->therapy->readyToStart()) {
            ui->btn_history->setVisible(false);
        ui->table_menu->scene()->clear();
        this->readyToStartSession = true;
        int skinConnectionLevel = this->performConnectionTest();
        if (this->readyToStartSession) {
            this->batteryDisplayTimer->start(BATTERY_DISPLAY_INTERVAL);
            this->battery->startDrain(this->therapy->getIntensity(), skinConnectionLevel);
            sessionInProgress=true;
            this->sessionTime = this->therapy->getDuration()/20 * 10;
            this->sessionTimer = new QTimer(this);
            connect(this->sessionTimer, &QTimer::timeout, this, &MainWindow::updateSessionTimer);
            this->sessionTimer->start(1000);
        }
    } else {
        displayMessage("Plase select intensity before starting therapy");
    }
}

void MainWindow::cleanMessage(){
    ui->table_menu->scene()->clear();
    ui->recordsList->clear();
    qInfo("hereeeee");
    displayHomeScreen();
}

void MainWindow::selectReleased(){
    int timeMilliSecs = elapsedTimer.elapsed();
    if (timeMilliSecs >= 1000) {
        updatePreferences();
    }
}


void MainWindow::indicateBatteryLevel() {
	int batteryStatus = this->battery->curStatus();
	int batteryLevel = this->battery->getLevelForDisplayGraph();

	if (batteryStatus == NORMAL) {
		this->displayBatteryLevel(batteryLevel);	
	} else {
		this->displayBatteryLevel(batteryLevel, true);	

		if (batteryStatus == CRITICALLY_LOW) {
            if (this->sessionTime == 0) {
                displayMessage("Warning: Battery critically low");
            } else {
                displayMessage("Warning: Battery critically low, ending session");
                this->stopSession();
            }
        } else {
            displayMessage("Warning: Battery low");
        }
	}
}

void MainWindow::displayBatteryLevel(int levels, bool flash) {
	// Update battery graph UI with int from 0-8
	qDebug() << "Battery graph: " << levels;
    if (flash && !this->isAdjustingIntensity && !this->isRunningTest) {
        flashGraphBar(levels, 3000);
    } else {
        for (int i = 1; i <= levels; i++) {
            illuminateGraphBar(i);
        }
				delay(1000);
        for (int i = 1; i <= levels; i++) {
            darkenGraphBar(i);
        }
    }
}

void MainWindow::displayHomeScreen(){
    if (this->powerState && this->sessionTime == 0) {
        ui->btn_history->setVisible(true);
        ui->recordsList->setVisible(false);
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addText("WELCOME, EVERYTHING IS FINE!");
    //   QGraphicsTextItem *history = scene->addText("see history");
    //    history->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    //    history->setPos(100, 100);
    //    connect(history, SIGNAL(linkActivated(QString("see history"))), this, SLOT(displayHistory()));
        ui->table_menu->setScene(scene);
    }
}

void MainWindow::updateSessionTimer(){
    if (this->sessionTime > 0) {
        ui->table_menu->scene()->clear();
        QGraphicsTextItem *text = ui->table_menu->scene()->addText("Time before the end of the session:");
        text->setPos(20, 20);
        int hrs = this->sessionTime / 3600;
        int mins = this->sessionTime / 60;
        int secs = this->sessionTime % 60;
        this->sessionTime -= 1;
        QString hrsStr = (hrs == 0) ? "00": QString::number(hrs);
        QString minStr = (mins == 0) ? "00": QString::number(mins);
        QString secStr = (secs == 0) ? "00": QString::number(secs);
        QGraphicsTextItem *timerText = ui->table_menu->scene()->addText(hrsStr + ":" + minStr + ":" + secStr);
        timerText->setPos(85, 50);
    } else {
        this->sessionTimer->stop();
        this->sessionTime = 0;
        stopSession();
    }
}

// end therapy session
void MainWindow::stopSession(){
    if (this->sessionTime >= 0 && sessionInProgress) {
        this->sessionTimer->stop();
        this->sessionTime = 0;
        sessionInProgress=false;
        displayHomeScreen();
    }
		this->batteryDisplayTimer->stop();
		this->battery->getTimer()->stop();
    displayHomeScreen();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if (event->type()==QEvent::MouseButtonPress){
         this->userInactive->start(autoShutOffTime);
        return false;
    }
}

void MainWindow::illuminateGraphBar(int level) {
	QString colour;

	if (level <= 3) {
		colour = QString("green");
	} else if (level <= 6) {
		colour = QString("yellow");
	} else {
		colour = QString("red");
    }

	QString file = QString("image: url(:/icons/%1_%2.png);").arg(QString::number(level), colour);
	graphBars->at(level - 1)->setStyleSheet(file);
    /* qDebug() << graphBars->at(level - 1)->style(); */
}

void MainWindow::darkenGraphBar(int level) {
	QString file = QString("image: url(:/icons/%1.png);").arg(QString::number(level));
	graphBars->at(level - 1)->setStyleSheet(file);
}

void MainWindow::flashGraphBar(int level, int msecDuration, int startLevel) {
    int n = 1;
    QTimer* intervalTimer = new QTimer(this);
    connect(intervalTimer, &QTimer::timeout, this, [=, &n]() {
        if (n % 2 == 0) {
            for (int i = startLevel; i <= level; i++)
                darkenGraphBar(i);
        } else {
            for (int i = startLevel; i <= level; i++)
                illuminateGraphBar(i);
        }

        n++;
    });
    intervalTimer->start(300);
		delay(msecDuration);
    intervalTimer->stop();
    for (int i = startLevel; i <= level; i++)
        darkenGraphBar(i);
}

void MainWindow::delay(int msec) {
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::autoPower(){
    if (!sessionInProgress && !isRunningTest) {
        this->userInactive->stop();
        this->powerState = false;
        updatePowerState();
    } else {
        this->userInactive->start(autoShutOffTime);
    }
}

void MainWindow::powerBtnReleased(){
    if (sessionInProgress || isRunningTest) {
        int timeMilliSecs = elapsedTimer.elapsed();
        if (timeMilliSecs >= 2000) {
           // delay(2000);
            sessionInProgress = false;
            if (isRunningTest) {
                this->intervalTimer->stop();
                this->modeLight->setStyleSheet("image: url(:/icons/Right.png);");
                this->isRunningTest = false;
                this->sessionInProgress = false;
                this->readyToStartSession = false;
            }
            isRunningTest = false;
            sessionInProgress = false;
            powerState = false;
            updatePowerState();
        }
    }
}
