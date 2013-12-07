#ifndef BLOCKHANDLER_H
#define BLOCKHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include "chunkdat.h"
#include "blockchecksum.h"
#include "deltahandler.h"

using namespace std;

#define BUF_SIZE  1024*1024

class BlockHandler
{    
public:
    BlockHandler();
    vector<chunkdat> getUnmatchedBlocks(vector<BlockCheckSum> checksumBlocks, vector<bool> unmatchedBlockData, string filepath);

private:
    DeltaHandler* deltahandler;
};

#endif // BLOCKHANDLER_H
