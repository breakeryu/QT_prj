#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readJson();
    ui->search_tEdit->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readJson()
   {
        QString val;
        QFile file(  "://config/config.json");

        //read json file and print
        file.open(QIODevice::ReadOnly);
        val = file.readAll();
        file.close();
        //qDebug() << val;

        //creat json file object
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject sett2 = d.object();

        //read json object "PurchaseUrl"
        QJsonValue valuePur = sett2.value(QString("PurchaseUrl"));
        //qDebug() << valuePur;
        QJsonObject itemPur = valuePur.toObject();
        //qDebug() << tr("QJsonObject of description: ") << itemPur;
        //get json key name
        PurchaseList = itemPur.keys();
        //qDebug() << tr("key name list :") << PurchaseList;

        //read json object "FootprintUrl"
        QJsonValue valueFot = sett2.value(QString("FootprintUrl"));
        //qDebug() << valueFot;
        QJsonObject itemFot = valueFot.toObject();
        //qDebug() << tr("QJsonObject of description: ") << itemFot;
        //get json key name
        FootprintList = itemFot.keys();
        //qDebug() << tr("key name list :") << FootprintList;


        //save url address
        for (int i = 0; i < itemPur.size(); ++i) {
           QString getS_Pur = PurchaseList.value(i);
           QJsonValue sub_Pur = itemPur[getS_Pur];
           PurchaseUrl.append(sub_Pur.toString());
        }
        qDebug() << PurchaseUrl;

        for (int j = 0; j < itemFot.size(); ++j) {
          QString getS_Fot = FootprintList.value(j);
          QJsonValue sub_Fot = itemFot[getS_Fot];
          FootprintUrl.append(sub_Fot.toString());
        }
        qDebug() << FootprintUrl;

        //show welcome window
        ui->fun_CBox->addItems(FunList);
        ui->fun_CBox->setCurrentIndex(0);

}


void MainWindow::searchAction()
{
    QString inputText = ui->search_tEdit->toPlainText();
    qDebug().noquote()<<inputText;
    QDesktopServices::openUrl(QUrl(url_search+inputText));
}


bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->search_tEdit)
    {
        if (e->type() == QEvent::KeyPress) {
             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
             //qDebug("Ate key press %d", keyEvent->key());
             if(keyEvent->key() == Qt::Key_Return){
                 searchAction();
                  return true;
             }else{
                 return QObject::eventFilter(obj, e);
             }
        } else {
             // standard event processing
             return QObject::eventFilter(obj, e);
        }
    }
}

void MainWindow::sleep(int ms)
{
    QDateTime last = QDateTime::currentDateTime();

    QDateTime now;

    while (1)
    {
        now = QDateTime::currentDateTime();

        if (last.msecsTo(now) >= ms)
        {
             break;
        }
    }

}



void MainWindow::on_fun_CBox_currentIndexChanged(int index)
{
    ui->Engine_cBox->clear();
    switch (index) {
        case 0:
            strList = PurchaseList;
            funSelect = 0;
            break;
        case 1:
            funSelect = 1;
            strList = FootprintList;
            break;
        }
        ui->Engine_cBox->addItems(strList);
        ui->Engine_cBox->setCurrentIndex(0);

}


void MainWindow::on_Engine_cBox_currentIndexChanged(int index)
{
    if(funSelect == 0)
       {
            url_search = PurchaseUrl.value(index);

       }else{
            url_search = FootprintUrl.value(index);
       }
}


void MainWindow::on_Search_pBtn_clicked()
{
     searchAction();
}

