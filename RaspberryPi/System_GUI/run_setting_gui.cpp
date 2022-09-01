#include "run_setting_gui.h"
#include "ui_run_setting_gui.h"
#include <QPixmap>
#include "i2c.h"

run_setting_gui::run_setting_gui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::run_setting_gui)
{
    ui->setupUi(this);
    // 사진 첨부
    QPixmap pix("/home/mjspi/Downloads/rice.png");
    int img_width = ui->l_Pic2->width();
    int img_height = ui->l_Pic2->height();
    ui->l_Pic2->setPixmap(pix.scaled(img_width, img_height, Qt::KeepAspectRatio));

    // 1~5인분으로 제한
    ui->sb_ManNum->setMinimum(1);
    ui->sb_ManNum->setMaximum(5);

    // 진행바 0%로 초기화
    ui->progressBar->setValue(0);

    // 상태 표시 공백으로 초기화
    ui->l_State->setText("");


}

run_setting_gui::~run_setting_gui()
{
    delete ui;
}

void run_setting_gui::on_pbRun_clicked()
{
    int mans = ui->sb_ManNum->value();  // 입력 받은 인원 저장 변수

    i2c rice_quantity;
    rice_quantity.i2c_network(mans);

    // 진행 표시
    for (int i = 0; i <= 100; ++i) {
        ui->progressBar->setValue(i);
        ui->l_State->setText("진행중");
    }
    ui->l_State->setText("완료");
}
