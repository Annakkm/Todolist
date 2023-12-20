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

    QTextBrowser *textBrowser = new QTextBrowser(infoDialog);
    textBrowser->setPlainText("To Do List - це програма, яка допомагає організовувати робочий день, планувати завдання, відстежувати їх виконання.      Пошта для зв'язку: mykhailenko.anna18@gmail.com");
    textBrowser->setAlignment(Qt::AlignCenter);
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setGeometry(10, 5, 210, 130);

    infoDialog->setGeometry(550, 170, 230, 130);
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
