#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QSqlQueryModel>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QPoint>
#include <QSystemTrayIcon>

#include "csqlquerymodel.h"
#include "ctableview.h"
#include "QHotkey/qhotkey.h"
#include "QHotkey/qhotkey_p.h"

const QString strSelectSQL = "SELECT name, path, type FROM everything";
const QString strWhereGlobWildcard = " WHERE name GLOB '*%1*'"; /* match case */
const QString strWhereLikeWildcard = " WHERE name LIKE '%%1%'";
const QString strWhereGlob = " WHERE name GLOB '%1'"; /* match case */
const QString strWhereLike = " WHERE name LIKE '%1'";
const QString strOrderByNm = " ORDER BY name";
const QString strCountSelectSQL = "SELECT COUNT(*) FROM everything";
const QString strDbName = "/var/lib/everything/everything.db";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void on_keywordEdit_textChanged();
    void reloadModel();
    void on_tableView_doubleClicked(const QModelIndex & index);
    void showContextMenu(const QPoint &pos);
    void openFile();
    void openFilePath();
    void copyFullPath();
    void closeEvent(QCloseEvent *e);  //--关闭事件
    void showMainwindow();
    void updateDatabase();

private:
    void setupUi();
    void retranslateUi();
    bool initTable();
    bool connectDb();
    bool reConnectDb();
    bool loadSettings(bool loadDefault = false);
    void setFilter(const QString& text);
    void setTitle(const QString& text);

private:
    CSqlQueryModel *m_sourceModel;
    QSqlQuery *m_sqlQuery;
    QSqlDatabase m_db;

    /* settings */
    bool m_enableMatchCase;
    bool m_enableRegex;
    double scale_factor;
    QHotkey *file_qk,*close_qk;
    QSystemTrayIcon *pSystemTray;

public:
    /* GUI widgets */
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout,*tabview_horizontalLayout;
    QLineEdit *keywordEdit;
    CTableView *tableView;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

private:
    int m_showContextRow;
};

#endif // MAINWINDOW_H

