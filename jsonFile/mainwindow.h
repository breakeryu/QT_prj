#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdesktopservices.h>
#include <qurl.h>
#include <qstring.h>
#include <QTextEdit>
#include <QComboBox>
#include <qstringlist.h>
#include "qfile.h"
#include "qdebug.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "QJsonArray"
#include "qdir.h"
#include "QKeyEvent"
#include "qimage.h"
#include "qtimer.h"
#include "qfiledialog.h"
#include "qprocess.h"

#include "qmenu.h"
#include "QSystemTrayIcon"
#include <QAction>//用来生成鼠标点击托盘图标后出现的菜单
#include <QCloseEvent>//用来拦截窗口关闭事件

#include <QSettings>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readJson();

    void searchAction(void);
    bool eventFilter(QObject *obj, QEvent *e);



private slots:
    void on_fun_CBox_currentIndexChanged(int index);

    void on_Engine_cBox_currentIndexChanged(int index);

    void on_Search_pBtn_clicked();

    void showLogo(void);


    void on_config_pBtn_clicked();


    void on_tabWidget_currentChanged(int index);



    void on_logo_pBtn_clicked();

    void trayClicked(QSystemTrayIcon::ActivationReason reason);//此处参数类型不能改动，因为要匹配对应的信号
    void getShow();
    void getExit();
    void getSetting();
    void on_AutoLanch_checkBox_stateChanged(int arg1);

protected:
    void closeEvent(QCloseEvent * e);

private:
    Ui::MainWindow *ui;
    int funSelect;
    QString url_search;
    QString url_logo;
    QString url_config;

    QStringList strList;
    QStringList FootprintList;
    QStringList PurchaseList;
    QStringList DataSheetList;
    QStringList FIMList;

    QStringList FootprintUrl;
    QStringList PurchaseUrl;
    QStringList DataSheetUrl;
    QStringList FIMUrl;

    QStringList FunList = {
                                    "购买",
                                    "封装",
                                    "DataSheet",
                                    "主流IC厂商"
                                    };


    QTimer *timer;

    QSystemTrayIcon * icon;//托盘图标
    QAction * actionShow;//显示窗口操作
    QAction * actionSetting;//显示设置操作
    QAction * actionClose;//退出程序操作
    QMenu * menu;//菜单

};
#endif // MAINWINDOW_H
