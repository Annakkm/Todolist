#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdb.h"
#include "login.h"
#include "registration.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnclose_clicked()
{
    this->close();
}


void MainWindow::on_btnlogin_2_clicked()
{
    if(!db.OpenDatabase()){
        QMessageBox::information(this, "Помилка !", "Перeвірте підключення бази даних");
    }
    else {
        this->hide();
        log->setWindowFlags(Qt::FramelessWindowHint);
        log->show();
        qDebug()<<"Connected log";
    }
}


void MainWindow::on_btnreg_2_clicked()
{
    if(!db.OpenDatabase()){
        QMessageBox::information(this, "Помилка !", "Перeвірте підключення бази даних");
    }
    else {
        this->hide();
        reg->setWindowFlags(Qt::FramelessWindowHint);
        reg->show();
        qDebug()<<"Connected reg";

    }
}

