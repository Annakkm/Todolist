#ifndef CONNECTDB_H
#define CONNECTDB_H
#include <QSqlDatabase>
#include <QSql>
#include <QMessageBox>

class ConnectDB
{
private:
    QSqlDatabase db;
public:
    ConnectDB();
    bool OpenDatabase();
    void CloseDatabase();
};

#endif // CONNECTDB_H
