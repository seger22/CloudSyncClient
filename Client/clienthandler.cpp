#include "clienthandler.h"

clienthandler::clienthandler()
{
    boost::asio::io_service::work work(io_service_);
    thread_= new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
    session_client= new Client(io_service_,"localhost","9999");
  //  cout<<"started clienthandler"<<endl;
}

void::clienthandler::start_client()
{

}

void clienthandler::send_data(string msg)
{
  //  cout<<"sending data"<<endl;
    session_client->send_request(msg);
   // cout<<"Message sent successfully"<<endl;
}

string clienthandler::receive_data()
{
    return session_client->get_data_from_server();
}

void clienthandler::stop_client()
{
    io_service_.stop();
    thread_->join();
}

void clienthandler::send_chunks(vector<chunkdat> chunks,int total_size)
{
    session_client->send_chunk_header(chunks,total_size);
  //  session_client->send_chunks(chunks);

}

void clienthandler::send_chunk_hashes(vector<u_int64_t> chunk_hashes)
{
    session_client->send_chunk_hashes(chunk_hashes);
}

void clienthandler::send_block_hashes(vector<vector<BlockChecksumSerial> > t){
    session_client->send_block_hashes(t);
}

vector<bool> clienthandler::read_unmatched_hashes()
{
    return session_client->read_unmatched_chunks();
}

void clienthandler::send_file(string path, string name)
{
    session_client->send_file(path,name);
}


