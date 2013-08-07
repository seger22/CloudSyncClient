#ifndef FILECHUNKREADER_H
#define FILECHUNKREADER_H

#include<iostream>
#include<fstream>
#include <chunkhandler.h>


using namespace std;

class filechunkreader
{
public:
    filechunkreader(string path);
    chunkdat get_chunk(Chunk *chunk);
    void closefile();

private:
    ifstream input_file;
};

#endif // FILECHUNKREADER_H
