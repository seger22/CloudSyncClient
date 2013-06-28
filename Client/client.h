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
#include "chunkdat.h"

using boost::asio::ip::tcp;
using namespace std;


class Client
{
public:
    Client(boost::asio::io_service& io_service_,const string &host, const string &service);
    void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
    void handle_write_request(const boost::system::error_code& err);
    void send_request(string data);
    void send_chunk_header(vector<chunkdat> strings);
    void send_chunk_data(string out_data);
    vector<string> get_block_data();
    void send_chunk_hashes(vector<u_int64_t> chunkHashes);
    void send_chunks(vector<string> chunks);
    vector<int> read_unmatched_chunks();
    bool send_buffer(string buffer);
    //void send_block_data();
    void send_file(string path, string filename);
    void stop_client();
    void start_client();
    void printA(int num);
    void printB();
    void printC();
    void addEvent();
    void send_string_vector(vector<string> strings);
    string get_data_from_server();


private:
    boost::asio::io_service io_service_;
    tcp::resolver resolver_;
     tcp::socket socket_;
     std::string change_request;
   // vector<chunk> chunks_;
     boost::asio::streambuf request_;
     enum { max_length = 1024 };
     char data_[max_length];
     string watch_dir;
     string chunk[65536];
     int chunk_max_length=65536;
//boost::asio::io_service::work client_work(client_io_service);
    //boost::asio::io_service::work client_work;

};

#endif // CLIENT_H
