#include <iostream>
#include "inotifyhandler.h"
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

