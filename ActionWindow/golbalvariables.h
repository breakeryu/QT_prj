#ifndef GOLBALVARIABLES_H
#define GOLBALVARIABLES_H
#include <QByteArray>
#include <QString>



enum cmdList{
    TURN_ON_LED = 0,
    TURN_OFF_LED,
    TURN_ON_BEEP,
    TURN_OFF_BEEP,
    SET_TIME,
    READ_TIME,
    NO_CMD
};

enum ledState {
    LEDON = 0,
    LEDOFF
};

enum beepState {
    BEEPON = 0,
    BEEPOFF
};


extern beepState beep;
extern ledState led;
extern cmdList serialCmd;
extern int hh;
extern int mm;
extern int ss;
extern int bandSpeed;
extern bool SerialisConnect;
extern int DATALENGTH;
extern QByteArray recvArray;
extern void variablesSetup(void);
extern QString str_ledon;
extern QString str_ledoff;
extern QString str_beepon;
extern QString str_beepoff;
extern QString str_setTimeOk;
extern QString str_setTimeErr;
#endif // GOLBALVARIABLES_H
