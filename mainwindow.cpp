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

    infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("Інформація про програму");
    // Додайте текст чи вміст до вашого міні-вікна, наприклад:

    infoDialog->setGeometry(550, 170, 230, 130);

    QLabel *label = new QLabel("Це інформаційне вікно про програму.", infoDialog);
    label->setAlignment(Qt::AlignCenter);

    // Підключіть слот до події наведення на кнопку
    ui->btninfo->installEventFilter(this);

}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->btninfo) {
        if (event->type() == QEvent::Enter) {
            // При наведенні на кнопку показати міні-вікно
            infoDialog->exec();
        }
    }
    // Продовжити обробку подій
    return QMainWindow::eventFilter(obj, event);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete infoDialog;

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

void MainWindow::on_btninfo_hovered()
{
    infoDialog->exec();
}
