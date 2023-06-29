#include "golbalvariables.h"


beepState beep;
ledState led;
cmdList serialCmd;
int hh;
int mm;
int ss;
int bandSpeed;
bool SerialisConnect;   //check serial is connect or not
int cmdNum;             //command numbers

int DATALENGTH = 8;

QString str_ledon = "A5D100000001D2A5";
QString str_ledoff = "A5D100000000D1A5";
QString str_beepon = "A5D200000001D3A5";
QString str_beepoff = "A5D200000000D2A5";
QString str_setTimeOk = "A5D300000001D4A5";
QString str_setTimeErr = "A5D300000000D3A5";
QByteArray recvArray;


void variablesSetup(void){
    beep = BEEPOFF;
    led = LEDOFF;
    serialCmd = NO_CMD;
    hh = 0;
    mm = 0;
    ss = 0;
    bandSpeed = 9600;
    SerialisConnect = false;
}
