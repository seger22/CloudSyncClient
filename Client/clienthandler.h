#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include<boost/asio.hpp>

#include<sys/stat.h>
#include<iostream>
#include<vector>
#include "client.h"
#include "blockChecksumSerial.h"
#include "chunkdat.h"
#include "dblib.h"

using namespace std;

class clienthandler
{
public:
    clienthandler();
    void start_client();
    //clienthandler get_client_handler();
   /* static clienthandler& getInstance()
           {
               static clienthandler    instance; // Guaranteed to be destroyed.
                                     // Instantiated on first use.
               instance.start_client();
               return instance;
           }*/
    int send_chunk_hashes(vector<u_int64_t> chunk_hashes);
    int send_chunks(vector<chunkdat> chunks);
    int send_chunk(chunkdat chunk, int count);
    int send_data(string msg);
    vector<bool> read_unmatched_hashes();
    string receive_data();
    void stop_client();
    void send_file(string path,string name);
    int send_block_hashes(vector<BlockChecksumSerial> t);

    int send_block_offsets(vector<int> block_offsets);
    int send_chunk_offsets(vector<int> chunk_offsets);
    int send_block_data(vector<chunkdat> blocks);

private:
    boost::asio::io_service io_service_;
    Client *session_client;
    //clienthandler() {};
    //clienthandler(clienthandler const&);              // Don't Implement
    //void operator=(clienthandler const&); // Don't implement
    boost::thread *thread_;
};

#endif // CLIENTHANDLER_H
