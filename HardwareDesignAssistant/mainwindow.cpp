#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    funSelect = 0;

    ui->Engine_cBox->addItems(PurchaseList);
    ui->Engine_cBox->setCurrentIndex(0);

    ui->FunSelect_cBox_->addItems(FunList);
    ui->FunSelect_cBox_->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Search_pButton_clicked()
{

    QString inputText = ui->Search_tEdit->toPlainText();
    qDebug().noquote()<<inputText;

    QDesktopServices::openUrl(QUrl(url_search+inputText));

}






void MainWindow::on_Engine_cBox_currentIndexChanged(int index)
{
    if(funSelect == 0)
    {
        switch (index) {
        case 0:
            url_search = lcsc;
            break;
        case 1:
            url_search = mouser;
            break;
        case 2:
            url_search = hqchip;
            break;
        }
    }else{
        switch (index) {
        case 0:
            url_search = ultralibrarian;
            break;
        case 1:
            url_search = ti;
            break;
        case 2:

            break;
        }
    }

}

void MainWindow::on_FunSelect_cBox__currentIndexChanged(int index)
{

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
    ui->Engine_cBox->clear();
    ui->Engine_cBox->addItems(strList);
    ui->Engine_cBox->setCurrentIndex(0);

}
