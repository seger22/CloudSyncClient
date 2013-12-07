#include "mainwidget.h"
#include <QApplication>
#include "status.h"
#include <QtSql>

#include <gui.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MainWidget w;

    GUI *g=GUI::getInstance();
    MainWidget *mw=g->getMainwidget();
    mw->showMessage("hello");

    // using SQLITE driver
   // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   // db.setDatabaseName("cloudsyncDB.db");

   // if (db.open()) {
   // w.showMessage("");
//}

    return a.exec();
}
