#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "run_setting_gui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 사진 첨부
    QPixmap pix("/home/mjspi/Downloads/rice.png");
    int img_width = ui->l_Pic1->width();
    int img_height = ui->l_Pic1->height();
    ui->l_Pic1->setPixmap(pix.scaled(img_width, img_height, Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 시작 버튼 눌렀을 때 동작
void MainWindow::on_pbStart_clicked()
{
    hide();
    run_setting_gui run_gui;
    run_gui.setModal(true);
    run_gui.exec();
}
