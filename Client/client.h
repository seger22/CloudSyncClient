#ifndef CLIENT_H
#define CLIENT_H

#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/thread.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include<boost/serialization/vector.hpp>
#include<boost/lexical_cast.hpp>
#include<queue>
#include<fstream>
#include<ostream>
#include<vector>
#include<sys/stat.h>
#include<binders.h>
#include <iostream>
#include<sstream>
#include <functional>
#include "blockChecksumSerial.h"
#include "chunkdat.h"
#include "configurationmanager.h"
#include "chunkhash.h"

using boost::asio::ip::tcp;
using namespace std;


class Client
{
public:
    Client(boost::asio::io_service& io_service_,const string &host, const string &service);
    void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
    void handle_write_request(const boost::system::error_code& err);
    int send_header(string data); //

    int send_chunk_data(vector<chunkdat> chunks); //
    int send_block_data(vector<chunkdat> blocks); //
    int send_file_segment(vector<chunkdat> segments, string data); //
    int send_chunk(chunkdat chunk, int count);

    int send_chunk_hashes(vector<u_int64_t> chunkHashes); //

    vector<bool> read_unmatched_chunks(); //

    void send_file(string path, string filename);



    void send_string_vector(vector<string> strings);
    string get_data_from_server(); //
    template <class T>
    int send_vector(vector<T> elements); //
    unsigned int send_integer(unsigned int n);
    int send_buffer(unsigned char *buf, int bufsize);
    int send_block_hashes(vector<BlockChecksumSerial> block_checksums);
    int send_block_offsets(vector<int> block_offsets);
    int send_chunk_offsets(vector<int> chunk_offsets);
    unsigned int read_integer();

private:
    boost::asio::io_service io_service_;
    bool sending;
    tcp::resolver resolver_;
     tcp::socket socket_;
     std::string change_request;   
     boost::asio::streambuf request_;
     enum { max_length = 10240 };
     char data_[max_length];
     string watch_dir;
     string chunk[65536];
     int chunk_max_length=66000;
     queue<chunkdat> chunkq;

};

#endif // CLIENT_H
