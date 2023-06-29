#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    variablesSetup();
    mainWindowSetup();
    refreshSerialFun();
    serial = new QSerialPort;
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRcvDataAndAction()));

    clkTimer = new QTimer(this);
    connect(clkTimer,SIGNAL(timeout()),this,SLOT(runTime()));
    clkTimer->start(1000);

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,
            [=]()
    {
        if(msgVailad)
        {
            actionLogic();
        }
    }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

//"串口连接"按键的回调函数
void MainWindow::on_pushButton_connect_clicked()
{
    if(serial->isOpen()){
        serial->close();
        ui->pushButton_connect->setText("连接串口");
        ui->label_serialStatus->setText("串口未连接！");
        timer->stop();
        return;
    }

    QString portName;
    int index;
    //get portname
    index = ui->comboBox_serialPort->currentIndex();
    portName = mySerialPortName[index];
    //qDebug() << "portName: "+portName;

    //close serial port before setting
    serial->close();
    //begin setting
    serial->setBaudRate(QSerialPort::Baud9600);
    //qDebug() << "Baud Rate: 9600; ";
    serial->setDataBits(QSerialPort::Data8);
    //qDebug() << "Data bits: 8 bits; ";
    serial->setParity(QSerialPort::NoParity);
    //qDebug() << "partiy set: noParity.";
    serial->setStopBits(QSerialPort::OneStop);
    //qDebug() << "stop bits: 1 bit; ";
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //qDebug() << "flow ctrl: no flow ctrl; ";

    //begin serial port
    serial->setPortName(portName);
    timer->start(100);
    ui->pushButton_connect->setText("断开串口");
    ui->label_serialStatus->setText("串口已连接...");
    if (!serial->open(QIODevice::ReadWrite)){
        qDebug() << "Error! Serial is not open!! ";
    }else{
        qDebug() << "Serial open";
    }

}

//“更新串口”按键的回调函数
void MainWindow::on_pushButton_refreshSerial_clicked()
{
    refreshSerialFun();
}


//更新串口函数
void MainWindow::refreshSerialFun()
{
    QString portName;
    QString uartName;
    QSerialPortInfo info;

    //refersh listNum
    mySerialListNum = 0;
    //clear comboBox
    ui->comboBox_serialPort->clear();
    //qDebug() << "Debug: Refresh the Serial list...";
    //refresh list
    foreach(info, QSerialPortInfo::availablePorts()){
        if(mySerialListNum < 10){
            mySerialPortName[mySerialListNum++] = info.portName();
        }else{
            qDebug() << "ERROR!!too many Serial port!!";
        }
        portName = info.portName();
        uartName = info.description();
        ui->comboBox_serialPort->addItem(portName +",(" +uartName+")");
        //qDebug() << tr("SYSTEM: Scan the uart device: ")+uartName + "("+portName+")"+tr(" has been added to the available list! ");
    }
}

//应用初始化动作
void MainWindow::mainWindowSetup()
{
    mySerialListNum = 0;
    hh = 10;
    mm = 55;
    ss = 30;
    recvDataIsShow = false;
    sendDataIsShow = false;
    msgVailad = false;
    setLedState(LEDOFF);
    setBeepState(BEEPOFF);

    ui->radioButton_ledon->setAttribute(Qt::WA_TransparentForMouseEvents,QIODevice::ReadOnly);
    ui->radioButton_ledon->setFocusPolicy(QIODevice::ReadOnly ? Qt::NoFocus : Qt::StrongFocus);

    ui->radioButton_ledoff->setAttribute(Qt::WA_TransparentForMouseEvents,QIODevice::ReadOnly);
    ui->radioButton_ledoff->setFocusPolicy(QIODevice::ReadOnly ? Qt::NoFocus : Qt::StrongFocus);

    ui->radioButton_beepon->setAttribute(Qt::WA_TransparentForMouseEvents,QIODevice::ReadOnly);
    ui->radioButton_beepon->setFocusPolicy(QIODevice::ReadOnly ? Qt::NoFocus : Qt::StrongFocus);

    ui->radioButton_beepoff->setAttribute(Qt::WA_TransparentForMouseEvents,QIODevice::ReadOnly);
    ui->radioButton_beepoff->setFocusPolicy(QIODevice::ReadOnly ? Qt::NoFocus : Qt::StrongFocus);


}

//串口接收之后的动作
void MainWindow::serialRcvDataAndAction()
{
    /*
    getData();
    analyzeData();
    actionLogic();
    outputMessage();
    waitForRev();
    */
    getData();//todo how to use stack to get data?
    analyzeData();
}

//灯小插件
void MainWindow::setLedState(ledState state)
{
    //load img
    QImage img;
    if(state == LEDON){
        img.load(":/rsource/img/ledon.png");
    }else{
        img.load(":/rsource/img/ledoff.png");
    }
    //show
    QPixmap pixmap = QPixmap::fromImage(img);
    int width = ui->label_led->width();
    int height = ui->label_led->height();
    QPixmap fitpixmap = pixmap.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->label_led->setPixmap(fitpixmap);
}

//蜂鸣器小插件
void MainWindow::setBeepState(beepState state)
{
    //load img
    QImage img;
    if(state == BEEPON){
        img.load(":/rsource/img/beepon.jpg");
    }else{
        img.load(":/rsource/img/beepoff.png");
    }
    //show
    QPixmap pixmap = QPixmap::fromImage(img);
    int width = ui->label_beep->width();
    int height = ui->label_beep->height();
    QPixmap fitpixmap = pixmap.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->label_beep->setPixmap(fitpixmap);

}

// 转换字符串到16进制数
void MainWindow::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len;) {
        //char lstr,
        hstr = str[i].toLatin1();
        if (hstr == ' ') {
            i++;
            continue;
        }
        i++ ;
        if (i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if ((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}


// 转换字符串到char
char MainWindow::ConvertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}


//数据接收函数
void MainWindow::getData()
{
    bool ok = false;
    QByteArray tempArray;
    tempArray.clear();
    tempArray = serial->readAll().toHex();
    //qDebug() << "TEMParray" <<" :" <<tempArray;
    recvArray.clear();
    for (int i = 0; i < tempArray.length(); i+=2) {
        recvArray += tempArray.mid(i,2).toInt(&ok,16);

    }
    //qDebug() << "array" <<" :" <<recvArray;
}


//进行数据显示
void MainWindow::DataDisplayAsHex(QByteArray tempArray)
{
    QString displayStr;
    displayStr.clear();
    for (int i = 0; i < DATALENGTH; ++i) {
        displayStr += QString::number(tempArray[i]&0xff,16).toUpper();
        displayStr += " ";
    }
    qDebug() << displayStr;
    if(recvDataIsShow){
        ui->lineEdit_recvData->setText(displayStr);
        recvDataIsShow = false;
    }else if(sendDataIsShow){
        ui->lineEdit_returnData->setText(displayStr);
        sendDataIsShow = false;
    }

}

//分析串口消息是否有效
void MainWindow::analyzeData()
{

    QByteArray analyzeArray = recvArray;
    uint8_t sumCheck = 0;
    if(analyzeArray[0].operator == (0xA5) && analyzeArray[DATALENGTH-1].operator == (0xA5)){
        for (int i = 1; i < DATALENGTH-2; ++i) {
            sumCheck += analyzeArray[i];
        }
        if(analyzeArray[DATALENGTH-2].operator == (sumCheck)){
            msgVailad = true;
        }else {
            msgVailad = false;
        }
    }else{
        msgVailad = false;
    }
}


//槽函数，接收到串口信息之后，来执行逻辑功能函数
void MainWindow::actionLogic()
{
    //qDebug() << "recv array!!";
    processCmd();
    msgVailad = false;
}

//解析串口指令
void MainWindow::processCmd()
{

    QByteArray cmdArray = recvArray;
    recvDataIsShow = true;
    DataDisplayAsHex(cmdArray);

    int16_t cmdMark = cmdArray[1];

    switch (cmdMark) {
        case 0x1:
            if(cmdArray[5].operator == (0x1)){
                ui->radioButton_ledon->setChecked(true);
                led = LEDON;
                serialCmd = TURN_ON_LED;
            }else{
                ui->radioButton_ledoff->setChecked(true);
                led = LEDOFF;
                serialCmd = TURN_OFF_LED;
            }
            setLedState(led);
            break;
        case 0x2:
            if(cmdArray[5].operator == (0x1)){
                beep = BEEPON;
                serialCmd = TURN_ON_BEEP;
                ui->radioButton_beepon->setChecked(true);
            }else{
                beep = BEEPOFF;
                serialCmd = TURN_OFF_BEEP;
                ui->radioButton_beepoff->setChecked(true);
            }
            setBeepState(beep);
            break;
        case 0x3:
            if(cmdArray[5].operator == (0x1)){
                hh = cmdArray[2];
                mm = cmdArray[3];
                ss = cmdArray[4];
                QString timeText = QString::number(hh) + ":" + QString::number(mm) + ":"+ QString::number(ss);
                ui->lineEdit_setTime->setText(timeText);
                serialCmd = SET_TIME;
            }
            break;
    }
    outputMessage();
}

//编写的时钟小插件
void MainWindow::runTime()
{
    if(ss == 59){
        ss = 0;
        if(mm == 59){
            mm = 0;
            if(hh == 23){
                hh =0;
            }else{
                hh++;
            }
        }else{
            mm++;
        }
    }else{
        ss++;
    }
    QString timeText = QString::number(hh) + ":" + QString::number(mm) + ":"+ QString::number(ss);
    ui->label_time->setText(timeText);
}


//根据收到的命令，选择遥测返回指令，写入到tx_buf
void MainWindow::outputMessage()
{
    QByteArray tx_buf;
    tx_buf.clear();

    switch (serialCmd) {
        case (TURN_ON_LED):
            StringToHex(str_ledon,tx_buf);
            break;
        case(TURN_OFF_LED):
            StringToHex(str_ledoff,tx_buf);
            break;
        case(TURN_ON_BEEP):
            StringToHex(str_beepon,tx_buf);
            break;
        case(TURN_OFF_BEEP):
            StringToHex(str_beepoff,tx_buf);
            break;
        case(SET_TIME):
            StringToHex(str_setTimeOk,tx_buf);
            break;
        case(READ_TIME):

            break;

        case(NO_CMD):

            break;

    }
    sendCmd(tx_buf);
}


//串口发送指令，显示发送的命令
void MainWindow::sendCmd(QByteArray cmd)
{

    serial->write(cmd);
    serial->waitForBytesWritten(30000);
    sendDataIsShow = true;
    DataDisplayAsHex(cmd);
}


//显示辅助遥测窗口
void MainWindow::on_actionshow_widnow_triggered()
{
    ui->frame_tool->show();
}


//隐藏辅助遥测窗口
void MainWindow::on_actionhide_window_triggered()
{
    ui->frame_tool->hide();
}




