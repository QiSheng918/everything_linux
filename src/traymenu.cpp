#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
{
    initActions();
}

TrayMenu::~TrayMenu()
{
    delete m_pQuitAction;
    delete m_pShowMainwindowAction;
    delete m_pUpdateAction;
}

// 添加菜单项
void TrayMenu::initActions()
{
    m_pUpdateAction = new QAction(QIcon(":/icon/setting"), QString::fromLocal8Bit("update"), this);
    m_pQuitAction = new QAction(QIcon(":/icon/quit"), QString::fromLocal8Bit("exit"), this);
    m_pShowMainwindowAction = new QAction(QIcon(":/icon/show"), QString::fromLocal8Bit("show"), this);

    // 添加菜单项
    addAction(m_pShowMainwindowAction);
    addSeparator();
    addAction(m_pUpdateAction);
    addSeparator();
    addAction(m_pQuitAction);

    connect(m_pUpdateAction, SIGNAL(triggered(bool)), this, SLOT(updateDatabase()));
    connect(m_pQuitAction, SIGNAL(triggered(bool)), this, SIGNAL(quit()));
    connect(m_pShowMainwindowAction, SIGNAL(triggered(bool)), this, SIGNAL(showMainwindow()));
    
}


void TrayMenu::updateDatabase(){
    // QString cmd = "sh ../shell_scripts/update_everything_database.sh";
    QString cmd = "echo \"ss\" | sudo  -S  ./everything_updatedb";
    system(cmd.toLocal8Bit().data());
}
