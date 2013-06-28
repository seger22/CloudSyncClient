#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include<boost/asio.hpp>
#include<sys/stat.h>
#include<boost/lexical_cast.hpp>
#include "chunkhandler.h"
#include "clienthandler.h"
#include "chunkdat.h"

using namespace std;

class Filehandler
{
public:
    Filehandler();
    void file_create(string path, string filename);
    void file_delete(string path, string filename);  //to-do
    void file_modified(string path, string filename);
    void file_moved(string oldpath, string newpath,string name); //to-do
    void file_rename(std::string filepath, std::string old_name, std::string new_name);

    void dir_created(string path, string name);
    void dir_deleted(string path, string name);
    void dir_moved(string oldpath, string newpath);
    void dir_rename(string path, string oldname, string newname);

    vector<int> get_indexes();

    void stop_client();

    //file_rename
    //dir_rename
    //dir_move


private:
    clienthandler* client;
    vector<Chunk*> objects;
    vector<int> unmatched_indexes;
    string dir;
};

#endif // INOTIFYHANDLER_H
