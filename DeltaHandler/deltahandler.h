#ifndef DELTAHANDLER_H
#define DELTAHANDLER_H

#include <iostream>
#include "zd_mem.h"
#include "zdlib.h"

using namespace std;

class DeltaHandler
{
    
public:
    DeltaHandler();
    void getDeltaBlock(const Bytef sourceBlock[], int block_size, Bytef** deltablock, unsigned long *deltasize);
    void getSourceBlock(Bytef **sourceBlock, unsigned long *block_size, Bytef deltaBlock[], int deltablock_size);
};

#endif // DELTAHANDLER_H
