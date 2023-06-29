#ifndef CONTORLOR_H
#define CONTORLOR_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class contorlor; }
QT_END_NAMESPACE

class contorlor : public QMainWindow
{
    Q_OBJECT

public:
    contorlor(QWidget *parent = nullptr);
    ~contorlor();

private slots:
    void on_pushButton_refershSerial_clicked();

    void on_pushButton_connectSerial_clicked();

    void on_pushButton_setTime_clicked();

    void on_radioButton_beepon_clicked();

    void on_radioButton_beepoff_clicked();

    void on_radioButton_ledon_clicked();

    void on_radioButton_ledoff_clicked();

    void refreshSerialFun();

    void StringToHex(QString str, QByteArray &senddata);

    char ConvertHexChar(char ch);

    void getData();

    void DataDisplayAsHex(QByteArray tempArray);

    void analyzeData();

    void serialRcvDataAndAction();

    void mainWindowSetup();

    void sendCmd(QByteArray cmd);

    void actionLogic();

    void processCmd();

private:
    Ui::contorlor *ui;
    QSerialPort *serial;
    QTimer *timer;

    QString mySerialPortName[10];
    int mySerialListNum;
    QByteArray recvArray;
    bool msgVailad;
    bool recvDataIsShow;
    bool sendDataIsShow;
    int DATALENGTH;
    QByteArray tx_buf;

    int hh;
    int mm;
    int ss;

    QString str_ledon = "A5010000000102A5";
    QString str_ledoff = "A5010000000001A5";
    QString str_beepon = "A5020000000103A5";
    QString str_beepoff = "A5020000000002A5";


};
#endif // CONTORLOR_H
