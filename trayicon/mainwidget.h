#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <status.h>
#include <about.h>
#include "initialiser.h"
#include <QDesktopServices> // to open folder
#include <stdlib.h> // to get path of home dir
class QMenu; //Forward declaring the QMenu class
using namespace std;

namespace Ui {
class MainWidget;
}


// I choose to inherit from QWidget but you can inherit from QMainWindow if you prefer
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void showMessage(string s);

private:
    void createActions();
    void createTrayIcon();
    void setIcon();
    void closeEvent(QCloseEvent *); // Overriding the window's close event


    Ui::MainWidget *ui;


    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;


    QAction *open;
    QAction *config;
    QAction *openstatus;
    QAction *close;
    QAction *about;

    Status *st;
    About *abt;
    DBlib *dblib;
    Initialiser initialiser;
private slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void showStatus();
    void showAbout();
    void openDir();


    void on_connectbtn_clicked();
};
#endif // MAINWIDGET_H
