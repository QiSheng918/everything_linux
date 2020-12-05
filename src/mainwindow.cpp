#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <QSqlRecord>
#include <QScreen>
#include <QTime>
#include <QHeaderView>
#include <QMessageBox>


#define Debug

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_sourceModel()
{
    setupUi();

    setWindowIcon(QIcon(":/everything_linux"));
    m_sourceModel = new CSqlQueryModel;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(strDbName);

    file_qk = new QHotkey(QKeySequence("ctrl+j"), true);
    connect(file_qk, SIGNAL(activated()), this, SLOT(showMainwindow()));

    close_qk=new QShortcut(this);
    close_qk->setKey(QKeySequence("ctrl+w"));
    connect(close_qk, SIGNAL(activated()), this, SLOT(hide()));

    pTrayMenu = new TrayMenu(this);
    connect(pTrayMenu, SIGNAL(updateDatabase()), this, SLOT(updateDatabase()));
    connect(pTrayMenu, SIGNAL(quit()), qApp, SLOT(quit()));
    connect(pTrayMenu, SIGNAL(showMainwindow()), this, SLOT(showMainwindow()));

    pSystemTray = new QSystemTrayIcon(this);
    pSystemTray->setContextMenu(pTrayMenu);
    pSystemTray->setToolTip("Everything_linux");
    pSystemTray->setIcon(QIcon(":/everything_linux"));
    // connect(pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showMainwindow()));
    pSystemTray->show();

    this->updateDatabase();
    if (connectDb())
    {
        loadSettings(1);
        initTable();
    }
    else {
        QMessageBox::warning(this, "Open database failed", "Can't open the everything database");
        qApp->quit();
    }
}

MainWindow::~MainWindow()
{
    if(centralWidget!=nullptr){
        delete centralWidget;
        centralWidget=nullptr;
    }
    if(verticalLayout!=nullptr){
        delete verticalLayout;
        verticalLayout=nullptr;
    }
    if(horizontalLayout!=nullptr){
        delete horizontalLayout;
        horizontalLayout=nullptr;
    }
    if(keywordEdit!=nullptr){
        delete keywordEdit;
        keywordEdit=nullptr;
    }
    if(tabview_horizontalLayout!=nullptr){
        delete tabview_horizontalLayout;
        tabview_horizontalLayout=nullptr;
    }
    if(verticalLayout!=nullptr){
        delete verticalLayout;
        verticalLayout=nullptr;
    }
    
    if(m_sourceModel!=nullptr){
        delete m_sourceModel;
        m_sourceModel=nullptr;
    }
    if(file_qk!=nullptr){
        delete file_qk;
        file_qk=nullptr;
    }
    if(close_qk!=nullptr){
        delete close_qk;
        close_qk=nullptr;
    }
    if(pSystemTray!=nullptr){
        delete pSystemTray;
        pSystemTray=nullptr;
    }
    if(pTrayMenu!=nullptr){
        delete pTrayMenu;
        pTrayMenu=nullptr;
    }


}

void MainWindow::setupUi()
{
    scale_factor = QGuiApplication::primaryScreen()->geometry().width() / 1920.0;

    this->resize(int(1000 * scale_factor), int(750 * scale_factor));
    init_tabview_column_size[0]=int(200*scale_factor);
    init_tabview_column_size[1]=int(500*scale_factor);
    init_tabview_column_size[2]=int(100*scale_factor);
    init_tabview_column_size[3]=int(200*scale_factor);

    this->centralWidget = new QWidget(this);

    this->verticalLayout = new QVBoxLayout(centralWidget);
    this->verticalLayout->setSpacing(3);
    this->verticalLayout->setContentsMargins(1, 1, 1, 1);

    this->horizontalLayout = new QHBoxLayout();
    this->horizontalLayout->setSpacing(3);

    this->keywordEdit = new QLineEdit(centralWidget);
    this->keywordEdit->setObjectName(QString::fromUtf8("keywordEdit"));

    this->horizontalLayout->addWidget(keywordEdit);

    this->verticalLayout->addLayout(horizontalLayout);

    this->tabview_horizontalLayout = new QHBoxLayout();
    this->tabview_horizontalLayout->setSpacing(3);
    this->tableView = new CTableView(centralWidget);

    this->tableView->setObjectName(QString::fromUtf8("tableView"));
    // this->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tabview_horizontalLayout->addWidget(tableView);
    this->verticalLayout->addLayout(tabview_horizontalLayout);

    this->setCentralWidget(centralWidget);

    this->setWindowTitle(QApplication::translate("MainWindow", "Everything", 0));

    /* connect slots by OBJECT name */
    QMetaObject::connectSlotsByName(this);
}

void MainWindow::showMainwindow()
{
    this->show();
    if (windowState() != Qt::WindowActive)
    {
        setWindowState(Qt::WindowActive); //正常化父窗体
    }
    //延时等待父窗体正常化 延时250毫秒
    QTime _Timer = QTime::currentTime().addMSecs(250);
    while (QTime::currentTime() < _Timer)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

bool MainWindow::reConnectDb()
{
    if (!m_db.isOpen())
    {
        m_db.close();
    }
    connectDb();

    return true;
}

bool MainWindow::connectDb()
{
    if (!m_db.open())
    {
        QMessageBox::warning(0, "Connect Database Error", m_db.lastError().text());
        return false;
    }
    return true;
}

bool MainWindow::initTable()
{
    m_sourceModel->setQuery(strSelectSQL + strOrderByNm);
    m_sourceModel->setHeaderData(0, Qt::Horizontal, "Name");
    m_sourceModel->setHeaderData(1, Qt::Horizontal, "Path");
    m_sourceModel->setHeaderData(2, Qt::Horizontal, "Size");
    m_sourceModel->setHeaderData(3, Qt::Horizontal, "Data Modified");

    tableView->setModel(m_sourceModel);
    for(int i=0;i<4;i++){
        tableView->setColumnWidth(i, init_tabview_column_size[i]);
    }
    tableView->setStyleSheet("QTableView{border-style: none}"
                             "QTableView::item:selected{background: rgb(51,153,255)}");

    connect(tableView, SIGNAL(hoverRowChanged(int)), m_sourceModel, SLOT(setHoverRow(int)));

    /* create right button menu */
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    return true;
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    m_showContextRow = index.row();
    openFile();
}

void MainWindow::openFile()
{
    QString cmd = "xdg-open " + tableView->model()->index(m_showContextRow, 1).data().toString() + "/" + tableView->model()->index(m_showContextRow, 0).data().toString();
    if (system(cmd.toLocal8Bit().data()) != 0)
    {
        QMessageBox::warning(this, "Error opening", "No application is registered as handling this file");
    }
}

void MainWindow::openFilePath()
{
    QString cmd = "xdg-open " + tableView->model()->index(m_showContextRow, 1).data().toString();
    system(cmd.toLocal8Bit().data());
}

void MainWindow::copyFullPath()
{
    QClipboard *board = QApplication::clipboard();
    QString path = tableView->model()->index(m_showContextRow, 1).data().toString();
    QString name = tableView->model()->index(m_showContextRow, 0).data().toString();
    if (path == "/")
    {
        board->setText(path + name);
    }
    else
    {
        board->setText(path + "/" + name);
    }
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    if (tableView->indexAt(pos).row() == -1)
    {
        return;
    }

    m_showContextRow = tableView->indexAt(pos).row();
    QMenu menu(this);
    QAction *open = menu.addAction("Open");
    QAction *openPath = menu.addAction("Open Path");
    QAction *copyFullPath = menu.addAction("Copy Full Name to Clipboard");
    connect(open, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(openPath, SIGNAL(triggered()), this, SLOT(openFilePath()));
    connect(copyFullPath, SIGNAL(triggered()), this, SLOT(copyFullPath()));
    menu.exec(QCursor::pos());
}

bool MainWindow::loadSettings(bool loadDefault)
{
    if (loadDefault)
    {
        m_enableMatchCase = false;
        m_enableRegex = false;
    }
    else
    {
        //load settings from ini file
    }
}

void MainWindow::setTitle(const QString &text)
{
    if (text.isEmpty())
    {
        setWindowTitle("Everything");
    }
    else
    {
        setWindowTitle(text + " - Everything");
    }
}

void MainWindow::setFilter(const QString &text)
{
    //    m_sourceModel->setQuery(strSelectSQL+" WHERE name LIKE '%" + text + "%' ORDER BY name");
    //    m_proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive));
    if (text.contains('*'))
    {
        if (m_enableMatchCase)
        {
            m_sourceModel->setQuery(strSelectSQL + strWhereGlob.arg(text) + strOrderByNm);
        }
        else
        {
            QString keyword = text;
            m_sourceModel->setQuery(strSelectSQL + strWhereLike.arg(keyword.replace('*', '%')) + strOrderByNm);
        }
    }
    else
    {
        if (m_enableMatchCase)
        {
            m_sourceModel->setQuery(strSelectSQL + strWhereGlobWildcard.arg(text) + strOrderByNm);
        }
        else
        {
            m_sourceModel->setQuery(strSelectSQL + strWhereLikeWildcard.arg(text) + strOrderByNm);
        }
    }
}

void MainWindow::on_keywordEdit_textChanged()
{
#ifdef Debug
    qDebug() << keywordEdit->text();
#endif

    setTitle(keywordEdit->text());
    setFilter(keywordEdit->text());
}

void MainWindow::reloadModel()
{
#ifdef Debug
    qDebug() << "reloadModel";
#endif
    reConnectDb();
    loadSettings(1);
    initTable();
    m_sourceModel->setQuery(strSelectSQL + strOrderByNm, m_db);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (windowState() != Qt::WindowMinimized)
    {
        setWindowState(Qt::WindowMinimized); //最小化父窗体
    }
    //延时等待父窗体最小化 延时250毫秒
    QTime _Timer = QTime::currentTime().addMSecs(250);
    while (QTime::currentTime() < _Timer)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    this->hide();
    event->ignore();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    int column_width_total=0;
    for(int i=0;i<4;i++){
        column_width_total+=tableView->columnWidth(i);
    }
    #ifdef Debug
        qDebug()<<"resize";
        qDebug()<<this->size();
        qDebug()<<column_width_total;
        qDebug()<<event->oldSize().width();
    #endif
    
    double factor=this->size().width()/(1.0*column_width_total);
    for(int i=0;i<4;i++){
        tableView->setColumnWidth(i, int(tableView->columnWidth(i)*factor));
    }
}

void MainWindow::updateDatabase(){
    QString cmd = "echo \"ss\" | sudo  -S  ./everything_updatedb";
    if (system(cmd.toLocal8Bit().data()) != 0)
    {
        QMessageBox::warning(this, "Error update database", "Can't update the everything database");
    }
}
