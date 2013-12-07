#ifndef DBLIB_H
#define DBLIB_H

#include <QtSql>
#include <iostream>
#include <QDateTime>
#include <QMutex>
using namespace std;
class DBlib
{
private:
    DBlib(){};
    QSqlDatabase db;
    static DBlib *dblib;
public:

    static DBlib* getInstance(){
     //   static QMutex mutex;
    //    mutex.lock();
        if(!dblib){
            dblib=new DBlib();
      //      mutex.unlock();
        }
        return dblib;
    }

    bool open_db();// open the database
    bool close_db();// close database
    /*** config table**/
    bool insert_config(QString url,QString name, QString pass);// for insert and update config values
    string get_server_address(); // return server address
    string get_username();// return username
    string get_password();// return password

    /*** changes table**/
    bool insert_into_change(string change_message);
    QSqlTableModel *model=new QSqlTableModel();
    QSqlTableModel* get_model();
private :

    bool empty_config();// delete all records from config table
    bool delete_excess_record_in_changes();


};

#endif // DBLIB_H
