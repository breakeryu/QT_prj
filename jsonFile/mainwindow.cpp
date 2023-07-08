#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readJson();
    ui->search_tEdit->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showLogo()));

    ui->main_tWidget->setCurrentIndex(0);

    timer->start(100);


    icon = new QSystemTrayIcon(this);//获取托盘图标对象
    QIcon thisIcon(":/HAL.ico");//从资源文件获取托盘图标的图标文件
    icon->setIcon(thisIcon);//设置托盘图标的图标文件
    connect(icon, &QSystemTrayIcon::activated, this, &MainWindow::trayClicked);//点击托盘图标发生的事件
    icon->show();//在托盘显示此图标
    actionShow = new QAction(tr("主界面(&M)"), this);//显示窗口操作
    actionClose = new QAction(tr("退出(&E)"), this);//退出程序操作
    actionSetting = new QAction(tr("设置(&S)"), this);//设置操作
    menu = new QMenu(this);
    menu->addAction(actionShow);//添加至菜单
    menu->addAction(actionSetting);
    menu->addAction(actionClose);
    connect(actionShow, &QAction::triggered, this, &MainWindow::getShow);
    connect(actionClose, &QAction::triggered, this, &MainWindow::getExit);
    connect(actionSetting, &QAction::triggered, this, &MainWindow::getSetting);

}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::readJson()
   {

        QString appPath = QApplication::applicationDirPath();

        QString val;
        QFile file(appPath+"/config/config.json");

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

        //read json object "DataSheetUrl"
        QJsonValue valueDataSheet = sett2.value(QString("DataSheetUrl"));
        QJsonObject itemDataSheet = valueDataSheet.toObject();
        DataSheetList = itemDataSheet.keys();
        //qDebug() << tr("key name list :") << DataSheetList;

        //read json object "FIMUrl"
        QJsonValue valueFIM = sett2.value(QString("FIMUrl"));
        QJsonObject itemFIM = valueFIM.toObject();
        FIMList = itemFIM.keys();
        //qDebug() << tr("key name list :") << FIMList;

        //read json object "pathUrl"
        QJsonValue valuePath = sett2.value(QString("pathUrl"));
        url_logo = valuePath.toString()+"/";

        //qDebug() << url_logo;

        //read json object "configUrl"
        QJsonValue valueConfig = sett2.value(QString("configUrl"));
        url_config = valueConfig.toString()+"/";

        //qDebug() << url_config;


        //save url address
        for (int i = 0; i < itemPur.size(); ++i) {
           QString getS_Pur = PurchaseList.value(i);
           QJsonValue sub_Pur = itemPur[getS_Pur];
           PurchaseUrl.append(sub_Pur.toString());
        }
        //qDebug() << PurchaseUrl;

        for (int j = 0; j < itemFot.size(); ++j) {
          QString getS_Fot = FootprintList.value(j);
          QJsonValue sub_Fot = itemFot[getS_Fot];
          FootprintUrl.append(sub_Fot.toString());
        }
        //qDebug() << FootprintUrl;


        for (int j = 0; j < itemDataSheet.size(); ++j) {
          QString getS_DataSheet = DataSheetList.value(j);
          QJsonValue sub_DataSheet = itemDataSheet[getS_DataSheet];
          DataSheetUrl.append(sub_DataSheet.toString());
        }
        //qDebug() << DataSheetUrl;

        for (int j = 0; j < itemFIM.size(); ++j) {
          QString getS_FIM = FIMList.value(j);
          QJsonValue sub_FIM = itemFIM[getS_FIM];
          FIMUrl.append(sub_FIM.toString());
        }
        //qDebug() << FIMUrl;

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
    }else {
        return QObject::eventFilter(obj, e);
    }
}



void MainWindow::showLogo()
{
    QString logoName;

    switch (funSelect) {
    case 0:
        logoName = PurchaseList[ui->Engine_cBox->currentIndex()];

        break;
    case 1:
        logoName = FootprintList[ui->Engine_cBox->currentIndex()];

        break;
    case 2:
        logoName = DataSheetList[ui->Engine_cBox->currentIndex()];
        break;
    case 3:
        logoName = FIMList[ui->Engine_cBox->currentIndex()];
        break;
    }

    //qDebug()<<tr("logoName:")<<logoName;

    QPixmap photo = QPixmap(url_logo+logoName);

    QPixmap pix = photo.scaled(ui->logo_label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);

    ui->logo_label->setPixmap(pix);
    ui->logo_label->show();

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
    case 2:
        strList = DataSheetList;
        funSelect = 2;
        break;
    case 3:
        funSelect = 3;
        strList = FIMList;
        break;
    }
    ui->Engine_cBox->addItems(strList);
    ui->Engine_cBox->setCurrentIndex(0);

}


void MainWindow::on_Engine_cBox_currentIndexChanged(int index)
{

    switch (funSelect) {
    case 0:
        url_search = PurchaseUrl.value(index);

        break;
    case 1:
        url_search = FootprintUrl.value(index);

        break;
    case 2:
        url_search = DataSheetUrl.value(index);
        break;
    case 3:
        url_search = FIMUrl.value(index);
        break;
    }

}


void MainWindow::on_Search_pBtn_clicked()
{
     searchAction();
}




void MainWindow::on_config_pBtn_clicked()
{

    QString path = url_config;
    QFileInfo info(path);
   #if defined(Q_OS_WIN)
       QStringList args;
       if (!info.isDir())
           args << "/select,";
       args << QDir::toNativeSeparators(path);
       if (QProcess::startDetached("explorer", args))
           return;
   #elif defined(Q_OS_MAC)
       QStringList args;
       args << "-e";
       args << "tell application \"Finder\"";
       args << "-e";
       args << "activate";
       args << "-e";
       args << "select POSIX file \"" + path + "\"";
       args << "-e";
       args << "end tell";
       args << "-e";
       args << "return";
       if (!QProcess::execute("/usr/bin/osascript", args))
           return;
   #endif
       QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir()? path : info.path()));

}



void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        timer->start(100);
    }else{
        timer->stop();
    }
}





void MainWindow::on_logo_pBtn_clicked()
{
    QString path = url_logo;
    QFileInfo info(path);
   #if defined(Q_OS_WIN)
       QStringList args;
       if (!info.isDir())
           args << "/select,";
       args << QDir::toNativeSeparators(path);
       if (QProcess::startDetached("explorer", args))
           return;
   #elif defined(Q_OS_MAC)
       QStringList args;
       args << "-e";
       args << "tell application \"Finder\"";
       args << "-e";
       args << "activate";
       args << "-e";
       args << "select POSIX file \"" + path + "\"";
       args << "-e";
       args << "end tell";
       args << "-e";
       args << "return";
       if (!QProcess::execute("/usr/bin/osascript", args))
           return;
   #endif
       QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir()? path : info.path()));
}

void MainWindow::trayClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){//reason为托盘图标被触发的形式
    case QSystemTrayIcon::Context://鼠标右键点击
        menu->exec(this->cursor().pos());//显示菜单 注意不是show()
        break;
    case QSystemTrayIcon::Trigger://鼠标左键点击
        show();//显示主窗口
        break;
    }
}

void MainWindow::getShow()
{
    ui->main_tWidget->setCurrentIndex(0);
    show();//显示主窗口
}

void MainWindow::getExit()
{
    qApp->quit();//退出程序
}

void MainWindow::getSetting()
{
    ui->main_tWidget->setCurrentIndex(1);
    show();//显示主窗口
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    e->ignore();//忽略原本的关闭操作
    hide();//仅仅隐藏主窗口
}

void MainWindow::on_AutoLanch_checkBox_stateChanged(int arg1)
{

    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    QString appName = QApplication::applicationName();

    if (arg1 != 0){
        QString strAppPath=QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        reg.setValue(appName,strAppPath);
    } else {
        reg.remove(appName);
    }
}




