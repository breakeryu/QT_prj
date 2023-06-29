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
#include "qdatetime.h"

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
    void sleep(int ms);
private slots:
    void on_fun_CBox_currentIndexChanged(int index);

    void on_Engine_cBox_currentIndexChanged(int index);

    void on_Search_pBtn_clicked();

private:
    Ui::MainWindow *ui;
    int funSelect;
    QString url_search;
    QStringList strList;
    QStringList FootprintList;
    QStringList PurchaseList;
    QStringList FunList = {
                                    "购买",
                                    "封装"
                                    };

    QStringList FootprintUrl;
    QStringList PurchaseUrl;


};
#endif // MAINWINDOW_H
