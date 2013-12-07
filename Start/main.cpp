#include <iostream>
#include "inotifyhandler.h"
//#include "dblib.h"
//#include "inotify-cxx.h"
//#include "filehandler.h"
using namespace std;

int main()
{

    InotifyHandler ih;
    ih.initialize_inotify();    
    return 0;
}

