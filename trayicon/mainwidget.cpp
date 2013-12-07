#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    st=new Status();
    abt=new About();
    dblib=DBlib::getInstance();
    createActions();
    createTrayIcon();
    setIcon();

    QString path(QDir::home().path());
   if (path!=NULL){
       path = path +QString("/CloudSync");
       QDir dir(path);
       if (!dir.exists()) {
           dir.mkpath(".");
       }
   }

    trayIcon->show();
}

MainWidget::~MainWidget()
{
    delete ui;
    delete trayIcon;
    delete trayIconMenu;
    delete open;
    delete close;
    delete openstatus;
    delete config;
    delete about;
    delete st;
    delete abt;
    delete dblib;
}


void MainWidget::createActions()
{
    open = new QAction(tr("&Open"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(openDir()));

    config = new QAction(tr("&Config"), this);
    connect(config, SIGNAL(triggered()), this, SLOT(show()));

    openstatus=new QAction(tr("&Recent Changes"), this);
    connect(openstatus, SIGNAL(triggered()), this, SLOT(showStatus()));

    about=new QAction(tr("&About"), this);
    connect(about, SIGNAL(triggered()), this, SLOT(showAbout()));

    close = new QAction(tr("&Quit"), this);
    connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWidget::createTrayIcon()
{
    trayIconMenu = new QMenu(this);


    trayIconMenu->addAction(open);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction(config);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(openstatus);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(about);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(close);


    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);


    connect(
            trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
           );


}

void MainWidget::setIcon()
{
    trayIcon->setIcon(QIcon(":/resource/cloud.png"));
}

void MainWidget::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
       openDir();
}

void MainWidget::closeEvent(QCloseEvent *event){

}

void MainWidget::showMessage(string s){
    trayIcon->showMessage(QString::fromStdString(s),
    tr("This application is still running. To quit please click this icon and select Quit"));
    hide();

}

void MainWidget::showStatus(){
 st->update_view();
 st->show();

}

void MainWidget::showAbout(){
    abt->show();
}
void MainWidget::openDir(){
    /*** get the path of home dir**/
        //char* pPath;
       //  pPath = getenv ("HOME");
          QString path(QDir::home().path());
         if (path!=NULL){
             path = path +QString("/CloudSync");
             QDir dir(path);
             if (!dir.exists()) {
                 dir.mkpath(".");
             }
             QDesktopServices::openUrl(QUrl("file:///" + path));
         }
}

void MainWidget::on_connectbtn_clicked()
{
    QString server=ui->servertxt->text();
    QString username=ui->usernametxt->text();
    QString password=ui->passwordtxt->text();
    if(dblib->open_db()){
        dblib->insert_config(server,username,password);
        //dblib->close_db();
    }    
    initialiser.start();
}


/*
 *#include <iostream>
#include "inotifyhandler.h"
//#include "dblib.h"
//#include "inotify-cxx.h"
//#include "filehandler.h"
using namespace std;

int main()
{

    InotifyHandler ih;
    ih.initialize_inotify();
    cout << "Hello World!" << endl;
    return 0;
}

 */
