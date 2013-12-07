#ifndef STATUS_H
#define STATUS_H

#include <QMainWindow>
#include <QMenu>
#include <QCloseEvent>

#include <QSqlTableModel>
#include <dblib.h>
namespace Ui {
class Status;
}

class Status : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Status(QWidget *parent = 0);
    ~Status();
    void update_view();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::Status *ui;
    DBlib *db;
   QSqlTableModel *model;
};

#endif // STATUS_H
