#ifndef INITIALISER_H
#define INITIALISER_H
#include "inotifyhandler.h"
#include <QThread>


class Initialiser : public QThread
{
public:
    Initialiser();

protected:
    void run();
};

#endif // INITIALISER_H
