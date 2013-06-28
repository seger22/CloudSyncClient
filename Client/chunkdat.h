#ifndef CHUNK_H
#define CHUNK_H

#include<stdio.h>
#include<iostream>
#include<sys/stat.h>
#include<boost/asio.hpp>
#include<boost/serialization/access.hpp>

using namespace std;

class chunkdat
{
public:
    enum{max_length=65536};
    char data[max_length];
    int chunk_size;
    //chunkdat();

};


#endif // CHUNK_H
