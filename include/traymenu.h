#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QMenu>

class TrayMenu : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenu(QWidget *parent = 0);
    ~TrayMenu();

signals:
    // 自定义信号
    void showSettings();
    void quit();
    void showMainwindow();
    void updateDatabase();

    
    

private:
    void initActions();
    QAction *m_pUpdateAction;
    QAction *m_pQuitAction;
    QAction *m_pShowMainwindowAction;
};

#endif // TRAYMENU_H
