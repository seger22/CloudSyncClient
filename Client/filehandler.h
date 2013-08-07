#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include<boost/asio.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include<sys/stat.h>
#include<string.h>
#include<boost/lexical_cast.hpp>
#include "filechunkreader.h"
#include "clienthandler.h"
#include "librsync.h"
#include "chunkdat.h"


using namespace std;

class BlockChecksumSerial{
public:
      rs_strong_sum_t strongsum;

private :
  friend class boost::serialization::access;
  unsigned int weaksum;
      template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
          ar & weaksum;
          ar & strongsum;
      }
public:
      BlockChecksumSerial(unsigned int wsum, rs_strong_sum_t strsum):weaksum(wsum)
  {
       memcpy(strongsum,strsum,16);
  }

  unsigned int getWeeksum(){
      return weaksum;
  }

};



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
