#include "contorlor.h"
#include "ui_contorlor.h"

contorlor::contorlor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::contorlor)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//set fixed window
    refreshSerialFun();//refresh serial prots
    mainWindowSetup();
    serial = new QSerialPort;//new serial memary
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRcvDataAndAction()));

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

contorlor::~contorlor()
{
    delete ui;
}


void contorlor::on_pushButton_refershSerial_clicked()
{
    refreshSerialFun();
}

void contorlor::on_pushButton_connectSerial_clicked()
{
    if(serial->isOpen()){
        serial->close();
        ui->pushButton_connectSerial->setText("连接串口");
        ui->label_connectStatus->setText("串口未连接！");
        timer->stop();
        return;
    }

    QString portName;
    int index;
    //get portname
    index = ui->comboBox_serialList->currentIndex();
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
    timer->start(300);
    ui->pushButton_connectSerial->setText("断开串口");
    ui->label_connectStatus->setText("串口已连接...");
    if (!serial->open(QIODevice::ReadWrite)){
        qDebug() << "Error! Serial is not open!! ";
    }else{
        qDebug() << "Serial open";
    }
}

void contorlor::on_pushButton_setTime_clicked()
{
    QString temp;
    temp.clear();
    QByteArray timeArray;
    timeArray.resize(8);

    if(serial->isOpen()){
        temp = ui->lineEdit_hh->text();
        hh  = temp.toInt();
        //qDebug()<<"hh = " <<hh;
        temp.clear();
        temp = ui->lineEdit_mm->text();
        mm  = temp.toInt();
        //qDebug()<<"mm = " <<mm;
        temp.clear();
        temp = ui->lineEdit_ss->text();
        ss  = temp.toInt();
        //qDebug()<<"ss = " <<ss;

        if(hh > 23 || mm >59 || ss>59 || hh<0 || mm<0 || ss <0){
            QMessageBox::warning(this,"Error!","无效的输入时间，请再次输入！");
            return;
        }

        for (int i = 0;i < DATALENGTH;i++) {
            switch (i) {
            case 0:
                timeArray[i].operator=(0xA5);
                break;
            case 1:
                timeArray[i].operator = (0x3);
                break;
            case 2:
                timeArray[i].operator = (hh);
                break;
            case 3:
                timeArray[i].operator = (mm);
                break;
            case 4:
                timeArray[i].operator = (ss);
                break;
            case 5:
                timeArray[i].operator = (0x1);
                break;
            case 6:
                timeArray[i].operator = (timeArray[1]+timeArray[2]+timeArray[3]+timeArray[4]+timeArray[5]);
                break;
            case 7:
                timeArray[i].operator = (0xA5);
                break;
            }
        }
        //qDebug()<<"timeArray = "<<timeArray;
        sendCmd(timeArray);
    }
}

void contorlor::on_radioButton_beepon_clicked()
{
    if(serial->isOpen()){
        tx_buf.clear();
        StringToHex(str_beepon,tx_buf);
        sendCmd(tx_buf);
    }
}

void contorlor::on_radioButton_beepoff_clicked()
{
    if(serial->isOpen()){
        tx_buf.clear();
        StringToHex(str_beepoff,tx_buf);
        sendCmd(tx_buf);
    }
}

void contorlor::on_radioButton_ledon_clicked()
{
    if(serial->isOpen()){
        tx_buf.clear();
        StringToHex(str_ledon,tx_buf);
        sendCmd(tx_buf);
    }
}

void contorlor::on_radioButton_ledoff_clicked()
{
    if(serial->isOpen()){
        tx_buf.clear();
        StringToHex(str_ledoff,tx_buf);
        sendCmd(tx_buf);
    }

}


//更新串口函数
void contorlor::refreshSerialFun()
{
    QString portName;
    QString uartName;
    QSerialPortInfo info;

    //refersh listNum
    mySerialListNum = 0;
    //clear comboBox
    ui->comboBox_serialList->clear();
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
        ui->comboBox_serialList->addItem(portName +",(" +uartName+")");
        //qDebug() << tr("SYSTEM: Scan the uart device: ")+uartName + "("+portName+")"+tr(" has been added to the available list! ");
    }
}

void contorlor::StringToHex(QString str, QByteArray &senddata)
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

char contorlor::ConvertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void contorlor::getData()
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
}

void contorlor::DataDisplayAsHex(QByteArray tempArray)
{
    QString displayStr;
    displayStr.clear();
    for (int i = 0; i < DATALENGTH; ++i) {
        displayStr += QString::number(tempArray[i]&0xff,16).toUpper();
        displayStr += " ";
    }
    qDebug() << displayStr;
    if(recvDataIsShow){
        ui->lineEdit_receiveCmd->setText(displayStr);
        recvDataIsShow = false;
    }else if(sendDataIsShow){
        ui->lineEdit_sendCmd->setText(displayStr);
        sendDataIsShow = false;
    }
}

void contorlor::analyzeData()
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

void contorlor::serialRcvDataAndAction()
{
    getData();//todo how to use stack to get data?
    analyzeData();
}


void contorlor::mainWindowSetup()
{
    mySerialListNum = 0;
    DATALENGTH = 8;
    recvDataIsShow = false;
    sendDataIsShow = false;
    msgVailad = false;

    hh = 0;
    mm = 0;
    ss = 0;

}

//串口发送指令，显示发送的命令
void contorlor::sendCmd(QByteArray cmd)
{

    serial->write(cmd);
    serial->waitForBytesWritten(30000);
    sendDataIsShow = true;
    DataDisplayAsHex(cmd);
}

void contorlor::actionLogic()
{
    processCmd();
    msgVailad = false;

}

void contorlor::processCmd()
{
    QByteArray cmdArray = recvArray;
    recvDataIsShow = true;
    DataDisplayAsHex(cmdArray);

    //处理遥测的指令
    if(cmdArray[1].operator ==(0xD1)){
        if(cmdArray[5].operator==(0x1)){
            //遥测显示——灯开
            ui->lineEdit_ledStatus->setText("开");
        }else{
            //遥测显示——灯关
            ui->lineEdit_ledStatus->setText("关");
        }
    }else if(cmdArray[1].operator ==(0xD2)){
        if(cmdArray[5].operator==(0x1)){
            //遥测显示——beep开
            ui->lineEdit_beepStatus->setText("开");
        }else{
            //遥测显示——beep关
            ui->lineEdit_beepStatus->setText("关");
        }
    }else if(cmdArray[1].operator ==(0xD3)){
        if(cmdArray[5].operator==(0x1)){
            //遥测显示——时间设置成功
            ui->lineEdit_timeStatus->setText("成功");
        }else{
            //遥测显示——时间设置失败
            ui->lineEdit_timeStatus->setText("失败");
        }
    }


}





