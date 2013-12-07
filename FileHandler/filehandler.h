#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include<boost/asio.hpp>
#include<sys/stat.h>
#include<string.h>
#include<boost/lexical_cast.hpp>
#include  "chunkhandler.h"
#include "clienthandler.h"
#include "librsync.h"
#include "chunkdat.h"
#include "blockChecksumSerial.h"
#include "blockhandler.h"
#include <boost/thread.hpp>
#include <dblib.h>


using namespace std;

class FileHandler
{
    
public:
    FileHandler();
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
void deleteChunks();

    vector<int> get_indexes();

    void processQueue(vector<bool> matchindex);
    void stop_client();

    //file_rename
    //dir_rename
    //dir_move


private:
    clienthandler* client;
    BlockHandler* blockhandler;
    vector<Chunk> objects;
    vector<int> unmatched_indexes;
    vector<bool> unmatchedHashIndex;
    queue<chunkdat> unmatchedChunks;
    string dir;
    int total_data_sent, total_metadata_sent, total_matched_chunks;
    DBlib* dblib;

};

#endif // FILEHANDLER_H
