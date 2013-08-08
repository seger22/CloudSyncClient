#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include<boost/asio.hpp>
#include<sys/stat.h>
#include<string.h>
#include<boost/lexical_cast.hpp>
#include "chunkhandler.h"
#include "clienthandler.h"
#include "librsync.h"
#include "chunkdat.h"
#include "blockChecksumSerial.h"


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

    vector<u_int64_t> get_chunk_hashes(string filepath, string filename);
    vector<BlockChecksumSerial> blockChecksumToSerial(vector<BlockChecksum> blocks);


    vector<int> get_indexes();

    void stop_client();

    //file_rename
    //dir_rename
    //dir_move


private:
    clienthandler* client;
    vector<Chunk*> objects;
    vector<int> unmatched_indexes;
    vector<bool> unmatchedHashIndex;
    string dir;
};

#endif // INOTIFYHANDLER_H
