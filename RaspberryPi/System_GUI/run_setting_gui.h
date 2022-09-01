#ifndef RUN_SETTING_GUI_H
#define RUN_SETTING_GUI_H

#include <QDialog>

namespace Ui {
class run_setting_gui;
}

class run_setting_gui : public QDialog
{
    Q_OBJECT

public:
    explicit run_setting_gui(QWidget *parent = nullptr);
    ~run_setting_gui();

private slots:
    void on_pbRun_clicked();

private:
    Ui::run_setting_gui *ui;
};

#endif // RUN_SETTING_GUI_H
