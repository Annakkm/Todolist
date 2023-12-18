#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint); // Встановлюємо флаг для видалення тайтлбара та рамки

    w.move(520, 115);

    w.show();
    return a.exec();
}

