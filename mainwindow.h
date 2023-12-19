#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectdb.h"
#include "login.h"
#include "registration.h"
#include <QRegion>
#include <QPainterPath>
#include <QPainter>
#include <QRect>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_btnclose_clicked();

    void on_btnlogin_2_clicked();

    void on_btnreg_2_clicked();

    void on_btninfo_hovered();

    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::MainWindow *ui;
    Registration * reg = new Registration();
    Login * log = new Login();
    ConnectDB db;

    QDialog *infoDialog; // Додайте об'єкт для міні-вікна

};
#endif // MAINWINDOW_H
