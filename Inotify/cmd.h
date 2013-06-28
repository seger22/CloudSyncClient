#ifndef CMD_H
#define CMD_H
#include <iostream>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/lexical_cast.hpp>// to cast
using namespace std;
class CMD
{
public:
    CMD();
    static string get_inode_number(string filepath);
    static string get_path_from_inode(string inode);
    static int lock_folder(string abosolute_path);
    static int release_folder(string abosolute_path);
};


#endif // CMD_H
