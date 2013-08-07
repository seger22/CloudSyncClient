#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include<boost/asio.hpp>

#include<sys/stat.h>
#include<iostream>
#include<vector>
#include "client.h"
#include "chunkdat.h"

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
    void send_chunk_hashes(vector<u_int64_t> chunk_hashes);
    void send_chunks(vector<chunkdat> chunks, int total_size);
    void send_data(string msg);
    vector<bool> read_unmatched_hashes();
    string receive_data();
    void stop_client();
    void send_file(string path,string name);
    void send_block_hashes(vector< vector <u_int32_t > > t);


private:
    boost::asio::io_service io_service_;
    Client *session_client;
    //clienthandler() {};
    //clienthandler(clienthandler const&);              // Don't Implement
    //void operator=(clienthandler const&); // Don't implement
    boost::thread *thread_;
};

#endif // CLIENTHANDLER_H
