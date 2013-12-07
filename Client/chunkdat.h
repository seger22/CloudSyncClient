#ifndef CHUNKDAT_H
#define CHUNKDAT_H

#include<stdio.h>
#include<iostream>
#include<sys/stat.h>

class chunkdat
{
public:
    enum{max_length=66000};
    char data[max_length];
    int chunk_size;
    //chunkdat();
};

#endif // CHUNKDAT_H
