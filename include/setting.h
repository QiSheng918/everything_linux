#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QAbstractButton>
#include <QButtonGroup>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = nullptr);
    ~setting();

private slots:
    void onSaveButtonClicked();
    void onExitButtonClicked();

signals:
    void closeSettings();

private:
    Ui::setting *ui;
    QButtonGroup* m_pButtonGroup;
    QButtonGroup* m_pDetectButtonGroup;
};

#endif // SETTING_H
