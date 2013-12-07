#include "status.h"
#include "ui_status.h"

Status::Status(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);

    db=DBlib::getInstance();
   update_view();


}

Status::~Status()
{
    delete ui;

}

void Status::update_view(){
    //if(db->open_db()){
        model=new QSqlTableModel();
        model->setTable("changes");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("change"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("date"));


        ui->tableView->setModel(model);
        int val=this->width();
        ui->tableView->setColumnWidth(0,val/2);
        ui->tableView->setColumnWidth(1,val/2);
      //  db->close_db();
    //}
}

void Status::on_pushButton_clicked()
{
    update_view();
}
