#include <QSettings>
#include <QDebug>
#include <QMessageBox>

#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);

    QSettings *configIniRead = new QSettings("set.ini", QSettings::IniFormat);
    ui->keyEdit->setText(configIniRead->value("key").toString());
    ui->secretEdit->setText(configIniRead->value("secret").toString());
    int translate_flag=configIniRead->value("translate").toInt();
    int precise_flag=configIniRead->value("precise").toInt();
    int copy_flag=configIniRead->value("autoCopy").toInt();
    int detect_flag=configIniRead->value("detect").toInt();
    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->setExclusive(true);
    m_pButtonGroup->addButton(ui->googleradioButton,0);
    m_pButtonGroup->addButton(ui->youdaoRadioButton,1);
    if(translate_flag==0) ui->googleradioButton->setChecked(true);
    else ui->youdaoRadioButton->setChecked(true);

    m_pDetectButtonGroup = new QButtonGroup(this);
    m_pDetectButtonGroup->setExclusive(true);
    m_pDetectButtonGroup->addButton(ui->baiduRadioButton,0);
    m_pDetectButtonGroup->addButton(ui->tencentRadioButton,1);
    if(detect_flag==0) ui->baiduRadioButton->setChecked(true);
    else ui->tencentRadioButton->setChecked(true);

    qDebug()<<precise_flag;
    if(precise_flag==0) ui->preciseCheckBox->setCheckState(Qt::Unchecked);
    else ui->preciseCheckBox->setCheckState(Qt::Checked);
    if(copy_flag==0) ui->autoCopyCheckBox->setCheckState(Qt::Unchecked);
    else ui->autoCopyCheckBox->setCheckState(Qt::Checked);


    delete configIniRead;
//    connect(m_pButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(onSaveButtonClicked()));
    connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(onExitButtonClicked()));
}

//void setting::onButtonClicked(QAbstractButton *button)
//{

//    // 当前点击的按钮
//    qDebug() << QString("Clicked Button : %1").arg(button->text());

//    // 遍历按钮，获取选中状态
//    QList<QAbstractButton*> list = m_pButtonGroup->buttons();
//    foreach (QAbstractButton *pButton, list)
//    {
//        QString strStatus = pButton->isChecked() ? "Checked" : "Unchecked";
//        qDebug() << QString("Button : %1 is %2").arg(button->text()).arg(strStatus);
//    }
//}


setting::~setting()
{
    delete ui;
    delete m_pButtonGroup;
    delete m_pDetectButtonGroup;
}

void setting::onSaveButtonClicked()
{

    QSettings *configIniWrite = new QSettings("set.ini", QSettings::IniFormat);
    configIniWrite->setValue("key",ui->keyEdit->text());
    configIniWrite->setValue("secret",ui->secretEdit->text());

    int copy_flag=ui->autoCopyCheckBox->isChecked();
    configIniWrite->setValue("autoCopy",copy_flag);

    int precise_flag=ui->preciseCheckBox->isChecked();
    configIniWrite->setValue("precise",precise_flag);
    qDebug()<<precise_flag;
    int translate_id=m_pButtonGroup->checkedId();
    configIniWrite->setValue("translate",translate_id);

    int detect_id=m_pDetectButtonGroup->checkedId();
    configIniWrite->setValue("detect",detect_id);

    delete  configIniWrite;

    QMessageBox::information(NULL, "成功", "保存成功");
    Q_EMIT closeSettings();
}

void setting::onExitButtonClicked()
{
    Q_EMIT closeSettings();
}
