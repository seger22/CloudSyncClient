#include "initialiser.h"

Initialiser::Initialiser()
{
}

void Initialiser::run(){
    InotifyHandler ih;
    ih.initialize_inotify();
}
