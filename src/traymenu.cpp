#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
{
    initActions();
}

TrayMenu::~TrayMenu()
{
    delete m_pQuitAction;
    delete m_pSettingAction;
}

// 添加菜单项
void TrayMenu::initActions()
{
    m_pSettingAction = new QAction(QIcon(":/icon/setting"), QString::fromLocal8Bit("setting"), this);
    m_pQuitAction = new QAction(QIcon(":/icon/quit"), QString::fromLocal8Bit("exit"), this);
    m_pShowMainwindowAction = new QAction(QIcon(":/icon/show"), QString::fromLocal8Bit("show"), this);

    // 添加菜单项
    addAction(m_pShowMainwindowAction);
    addSeparator();
    addAction(m_pSettingAction);
    addSeparator();
    addAction(m_pQuitAction);

    connect(m_pSettingAction, SIGNAL(triggered(bool)), this, SIGNAL(showSettings()));
    connect(m_pQuitAction, SIGNAL(triggered(bool)), this, SIGNAL(quit()));
    connect(m_pShowMainwindowAction, SIGNAL(triggered(bool)), this, SIGNAL(showMainwindow()));
}
