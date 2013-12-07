#ifndef GUI_H
#define GUI_H
#include <mainwidget.h>
class GUI
{
private:
    GUI(){};
    static GUI* gui;
    MainWidget mw;
public:
    static GUI* getInstance(){

        if(!gui){
            gui=new GUI();
        }
        return gui;
    }

    MainWidget* getMainwidget(){
        return &mw;
    };
};

#endif // GUI_H
