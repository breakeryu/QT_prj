#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdesktopservices.h>
#include <qurl.h>
#include <qstring.h>
#include <QTextEdit>
#include <QDebug>
#include <QComboBox>
#include <qstringlist.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_Search_pButton_clicked();

    void on_Engine_cBox_currentIndexChanged(int index);

    void on_FunSelect_cBox__currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    int funSelect;

    QString url_search;
    QString url_Footprintsearch;
    QString url_Purchasesearch;

    QString ultralibrarian = "https://app.ultralibrarian.com/search?queryText=";
    QString ti = "https://www.ti.com.cn/sitesearch/zh-cn/docs/universalsearch.tsp?langPref=zh-CN&searchTerm=";

    QString lcsc = "https://so.szlcsc.com/global.html?k=";
    QString mouser = "https://www.mouser.cn/c/?q=";
    QString hqchip = "https://www.hqchip.com/search/";

    QComboBox *enginCBox;
    QStringList strList;
    QStringList FootprintList = {
                            "ultralibrarian",
                            "TI",
                            "ADI"
                            };
    QStringList PurchaseList = {
                            "立创商城",
                            "贸泽",
                            "华秋"
                            };

    QStringList FunList = {
                            "购买",
                            "封装"
                            };


};
#endif // MAINWINDOW_H
