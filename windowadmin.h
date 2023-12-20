#ifndef WINDOWADMIN_H
#define WINDOWADMIN_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QString>
#include <QDebug>
#include <QShowEvent>
#include <QPushButton>
#include <QRadioButton>
#include <QSqlError>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSignalMapper>
#include <QVariant>
#include <QLabel>
#include <QScrollArea>
#include <unordered_set>
#include <QList>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QDebug>
#include "QInputDialog"
#include <QLineEdit>
#include <QDateEdit>
#include <QList>
#include <QCheckBox>
#include <QStringList>
#include <QPalette>
#include <QGridLayout>
#include <QDate>
#include "connectdb.h"

class Login;



namespace Ui {
class WindowAdmin;
}

class WindowAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit WindowAdmin(QWidget *parent = nullptr);
    ~WindowAdmin();

public slots:
    void loginWithCredentials(const QString &email, const QString &password);
    void displayEmployeesForAdmin(int adminIdCompany, const QString &seachText);
    void onLabelClicked(const QString &link);
    void handleButtonClick(QPushButton *clickedButton);
    void onEmployeeButtonClicked(QString employeeName, QString email); // метод натиснення на кнопку
    void clearDateEdits();
    void setDateTime(QDateEdit* dateEdit, const QDate& deadlineDat);


    QList<QDate> getDateForEmployee(int employeeId);//-+

    void updateSearchResults(const QString &text);


    void loadDeadlines(int employeeId);

    void clearLineEdits();
    void createLineEdits(int number);
    void clearLayout(QLayout* layout);
    int getEmployeeId(const QString& employeeName, const QString& email);
    int getAdminId(const QString& adminName, const QString& email);
    QStringList getTasksForEmployee(int employeeId);
    bool isTaskAlreadyExists(const QString& taskDescription);
    int getTasksId(const int& employeeID, const int& adminId);
    QList<QDate> getDeadlines(int employeeId);
    QStringList getStatusesForEmployee(int employeeId);




private slots:
    void on_btnclose_clicked();

    void on_btn_save_clicked();

    void on_btn_update_clicked();

    void onRadioButtonClicked();


private:
    Ui::WindowAdmin *ui;
    int employeeCount;
    QList<QList<QString>> taskList;


    QHash<QRadioButton*, QLineEdit*> radioButtonLineEditMap;


    QGridLayout * gridlayout_;
    QGridLayout * gridLayout_main;
    QVBoxLayout * layout_2;
    QPushButton * currentSelectedButton;
    QScrollArea * scrollArea_;
    QScrollArea * scrollArea_main;
    ConnectDB db;

    QList<QLineEdit*> lineEdits;
    QList<QDateEdit*> dateEdits;
    QList<QCheckBox*> checkBoxes;

    QString selectedEmployeeName;
    QString selectedEmployeeEmail;
    int selectedEmployeeId;
    int selectedAdminId;
    int idCompany;



    bool buttonClicked = false;
    bool isTrue = true;
    bool isChecked = false;

    QLineEdit * l = new QLineEdit();
    QString currentTask;
    QDateEdit *currentDateEdit;

    QList<QDate> deadlines;

  //  QListWidget *searchResultsList;


};

#endif // WINDOWADMIN_H
