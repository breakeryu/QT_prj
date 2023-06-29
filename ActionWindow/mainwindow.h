#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "golbalvariables.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QString>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:


private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_refreshSerial_clicked();

    void refreshSerialFun();

    void mainWindowSetup();

    void serialRcvDataAndAction();

    void setLedState(ledState state);

    void setBeepState(beepState state);

    void StringToHex(QString str, QByteArray &senddata);

    char ConvertHexChar(char ch);

    void getData();

    void DataDisplayAsHex(QByteArray tempArray);

    void analyzeData();

    void actionLogic();

    void processCmd();

    void runTime();

    void outputMessage();

    void sendCmd(QByteArray cmd);

    void on_actionshow_widnow_triggered();

    void on_actionhide_window_triggered();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *clkTimer;
    QSerialPort *serial;

    QString mySerialPortName[10];
    int mySerialListNum;
    bool msgVailad;
    bool recvDataIsShow;
    bool sendDataIsShow;

};
#endif // MAINWINDOW_H
