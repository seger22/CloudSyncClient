#include "clienthandler.h"

clienthandler::clienthandler()
{
    boost::asio::io_service::work work(io_service_);
    thread_= new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
    DBlib* db=DBlib::getInstance();   
    string ip_addr=db->get_server_address();
    cout<<"Server address:"<<ip_addr<<endl;
    session_client= new Client(io_service_,ip_addr,"9999");

}

void::clienthandler::start_client()
{

}

//
int clienthandler::send_data(string msg)
{

   return session_client->send_header(msg);

}

//
string clienthandler::receive_data()
{
    return session_client->get_data_from_server();
}

void clienthandler::stop_client()
{
    io_service_.stop();
    thread_->join();
}


//
int clienthandler::send_chunks(vector<chunkdat> chunks)
{
   return session_client->send_chunk_data(chunks);
  //  session_client->send_chunks(chunks);
}

int clienthandler::send_chunk(chunkdat chunk,int count){
    return session_client->send_chunk(chunk,count);
}

//
int clienthandler::send_block_data(vector<chunkdat> blocks){
    return session_client->send_block_data(blocks);
}

//
int clienthandler::send_chunk_hashes(vector<u_int64_t> chunk_hashes)
{
    return session_client->send_chunk_hashes(chunk_hashes);
}

//
int clienthandler::send_block_hashes(vector<BlockChecksumSerial > t){
    return session_client->send_block_hashes(t);
}

//
vector<bool> clienthandler::read_unmatched_hashes()
{
    return session_client->read_unmatched_chunks();
}

void clienthandler::send_file(string path, string name)
{
    session_client->send_file(path,name);
}

//
int clienthandler::send_block_offsets(vector<int> block_offsets){
    return session_client->send_block_offsets(block_offsets);
}

//
int clienthandler::send_chunk_offsets(vector<int> chunk_offsets){
    return session_client->send_chunk_offsets(chunk_offsets);
}



