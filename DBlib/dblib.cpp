#include "dblib.h"



DBlib* DBlib::dblib=0;// for singleton
bool DBlib::open_db(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db"); //home/mayuresan/Project/Qt-Fun/build-CloudTest-Desktop-Debug/DBlib/


    return db.open();
}

bool DBlib::close_db(){
    if(db.isOpen())
        db.close();
}

bool DBlib::insert_config(QString url,QString name,QString pass){
   // if(!db.isOpen()) this->open_db();
    if(db.isOpen()){
        if(empty_config()){// delete the previous record
            QSqlQuery query;
            query.prepare(QString("insert into config values('%1','%2','%3')").arg(url).arg(name).arg(pass));

            if( !query.exec() ){ // insert new record
               // qDebug() << query.lastError();
                return false;}
            else{
               // qDebug( "Inserted!" );
                return true;}
        }
    }
}


bool DBlib::empty_config(){

    if(db.isOpen()){
        QSqlQuery query;
        query.prepare(QString("delete from config"));

        if( !query.exec() ){
            //qDebug() << query.lastError();
            return false;}
        else{
           // qDebug( "Table Empty!" );
            return true;}
    }
}

string DBlib::get_username(){
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare(QString("select name from config"));

        if( !query.exec() ){
            //qDebug() << query.lastError();
            return NULL;}
        else{
            query.next();
            string result=query.value(0).toString().toStdString();
            //qDebug( query.value(0).toString());
            return result;
        }
    }

}
string DBlib::get_password(){
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare(QString("select pass from config"));

        if( !query.exec() ){
            //qDebug() << query.lastError();
            return NULL;
        }
        else{
            query.next();
            string result=query.value(0).toString().toStdString();
            //qDebug( query.value(0).toString());
            return result;
        }
    }
}

string DBlib::get_server_address(){
    if(db.isOpen()){
        QSqlQuery query;
        query.prepare(QString("select url from config"));

        if( !query.exec() ){
            //qDebug() << query.lastError();
            return NULL;}
        else{
            query.next();
            string result=query.value(0).toString().toStdString();
            //qDebug( query.value(0).toString());
            return result;
        }
    }
}

bool DBlib::insert_into_change(string change_message){
    if(!db.isOpen()) this->open_db();
    if(db.isOpen()){
        delete_excess_record_in_changes();
        QString change=change_message.c_str(); // convet to qstring
        QDateTime date_time = QDateTime::currentDateTime();// get the current date time
        QString date = date_time.toString();
        QSqlQuery query;
        query.prepare(QString("insert into changes values('%1','%2')").arg(change).arg(date));

        if( !query.exec() ){ // insert new record
           // qDebug() << query.lastError();
            return false;
        }else{
            //qDebug( "Inserted!" );
            return true;
        }
    }
    return false;

}

bool DBlib::delete_excess_record_in_changes(){
    if(db.isOpen()){
        QSqlQuery query;
      //  query.prepare(QString("delete from changes where rowid >1"));
        query.prepare(QString("select count(*) from changes"));

        if( !query.exec() ){
            //qDebug() << query.lastError();
            return false;
        }else{

            query.next();
            int rows=query.value(0).toInt();
            if(rows>5){
                int val= rows-5;
                // to do - need to check this not working
                query.prepare(QString("delete from changes where rowid < ?"));
                query.addBindValue(val);

                if(!query.exec()){
              //       qDebug() << query.lastError();
                     return false;
                }else{
                    cout<< rows;
                 return true;
                }
            }


        }

    }
}

QSqlTableModel *DBlib::get_model(){

        model->setTable("changes");

        model->select();

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("changes"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("date"));

        return model;
}


