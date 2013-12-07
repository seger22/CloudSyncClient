#include "client.h"

Client::Client(boost::asio::io_service& io_service_, const string &host, const string &service):resolver_(io_service_), socket_(io_service_)
{
   // watch_dir="/home/mayuresan/Project/SyncSource";
    watch_dir=ConfigurationManager::getLocation();
    sending=false;
    tcp::resolver::query query(host, service);
    cout<<"To resolving query\n";
    resolver_.async_resolve(query,
                            boost::bind(&Client::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::iterator));
}

void Client::handle_resolve(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator){
    if (!err)
    {
         cout<<"Resolve success\n";
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
                              boost::bind(&Client::handle_connect, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        cout << "Error: " << err.message() << "\n";
    }
}

void Client::handle_connect(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        boost::system::error_code ec;
        cout<<"Client: connected to Server successfully"<<endl;
        strcpy(data_,"Dummy data");
        boost::asio::write(socket_,boost::asio::buffer(data_,1024),ec);
        //   boost::asio::async_write(socket_,boost::asio::buffer(data_,1024),boost::bind(&Client::handle_write_request, this, boost::asio::placeholders::error));
    }
    else{

    }
}

void Client::handle_write_request(const boost::system::error_code &err)
{
    if(err)
    {
        std::cout<<"Oh no something wnet wrong :( "<<err.message()<<std::endl;
    }
    else
    {
        // cout<<"Message sent successfully!!!"<<endl;
    }
}

//
int Client::send_header(string request)
{
    /*boost::system::error_code err;
    strcpy(data_,request.c_str());
    return boost::asio::write(socket_,boost::asio::buffer(data_,1024),err);*/
    int len=request.size();
    char bytes[2]={0,0};
    bytes[0] = (len >> 8) & 0xFF;
    bytes[1] = len & 0xFF;
    boost::asio::streambuf buf;
    ostream os(&buf);
    os<<bytes[0]<<bytes[1]<<request;
  //  cout<<"Request: "<<request<<endl;
    return boost::asio::write(socket_,buf);
}


//
string Client::get_data_from_server(){
    char ret_data[1024]={0};  
   // socket_.read_some(boost::asio::buffer(ret_data),err);
    int read_size=this->read_integer();
    boost::asio::read(socket_,boost::asio::buffer(ret_data,read_size));
        string res(ret_data);
        return res;
}


unsigned int Client::read_integer(){
    unsigned int num=0;
    unsigned char bytes[4]={0};
    boost::asio::read(socket_, boost::asio::buffer(bytes,4));
    num|=bytes[0] & 0xFF;
    num<<=8;
    num|=bytes[1] & 0xFF;
    num<<=8;
    num|=bytes[2] & 0xFF;
    num<<=8;
    num|=bytes[3] & 0xFF;
    return num;
}



int Client::send_chunk(chunkdat chunk, int count){
    boost::system::error_code err;
    int total_sent=0;
    int len=chunk.chunk_size;
    char bytes[3]={0,0,0};
    bytes[0] = (len >> 16) & 0xFF;
    bytes[1] = (len >> 8) & 0xFF;
    bytes[2] = len & 0xFF;

    boost::asio::streambuf chunk_buf;
    ostream isdata(&chunk_buf);
    isdata<<bytes[0]<<bytes[1]<<bytes[2];

    total_sent+=boost::asio::write(socket_,chunk_buf,boost::asio::transfer_exactly(3));
    //cout<<"Sending chunk: "<<count<<" with size: "<<len<<endl;
    total_sent+=boost::asio::write(socket_,boost::asio::buffer(chunk.data,len),err);
    if(err){
        cout<<"Error:"<<err.message()<<" in chunk:"<<count<<endl;
        return -1;
    }
    return total_sent;
}

//
int Client::send_file_segment(vector<chunkdat> segments, string data){
    int total_sent=0;
    cout<<"Sending "<<segments.size()<<" "<<data<<"s...\n";

    boost::system::error_code err;

    int count=segments.empty() ? 0:segments.size();

    this->send_header(boost::lexical_cast<string>(count));


    for(int j=0;j<count;j++)
    {
        int len=segments[j].chunk_size;
        char bytes[3]={0,0,0};
        bytes[0] = (len >> 16) & 0xFF;
        bytes[1] = (len >> 8) & 0xFF;
        bytes[2] = len & 0xFF;

        boost::asio::streambuf chunk_buf;
        ostream isdata(&chunk_buf);
        isdata<<bytes[0]<<bytes[1]<<bytes[2];



        boost::asio::write(socket_,chunk_buf,boost::asio::transfer_exactly(3));
     //   cout<<"Sending "<<data<<" "<<j+1<<" of "<<count<<" with size "<<len<<endl;

        total_sent+=boost::asio::write(socket_,boost::asio::buffer(segments[j].data,len),err);

        if(err)
            cout<<"Error:"<<err.message()<<" in chunk:"<<j+1<<endl;
    }
    cout<<"Sent unmatched "<<data<<" successfully!!\n\n";
    return total_sent;
}

//
int Client::send_chunk_data(vector<chunkdat> chunks){
    return this->send_file_segment(chunks,"chunk");
}

//
int Client::send_block_data(vector<chunkdat> blocks){
    return this->send_file_segment(blocks,"block");
}

void Client::send_file(string path, string filename)
{
    cout<<"sending file"<<endl;
    boost::array<char, 1024> buf;
    size_t file_size=0;
    std::string filepath=watch_dir+path+filename;
    boost::system::error_code error = boost::asio::error::host_not_found;

    std::ifstream source_file(filepath.c_str(), std::ios_base::binary | std::ios_base::ate);
    if (!source_file.is_open())
    {
        std::cout << "failed to open " << filepath << std::endl;
        return;
    }
    file_size = source_file.tellg();
    std::cout<<"file size :"<<file_size<<std::endl;

    boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream <<file_size << "\n\n";
            boost::asio::write(socket_, request);
            std::cout << "start sending file content.\n";

    source_file.seekg(0);
    for (;;)
    {
        if (source_file.eof()==false)
        {
            source_file.read(buf.c_array(), (std::streamsize)buf.size());
            if (source_file.gcount()<=0)
            {
                std::cout << "read file error " << std::endl;
                break;
            }
            boost::asio::write(socket_, boost::asio::buffer(buf.c_array(),
                                                            source_file.gcount()),
                               boost::asio::transfer_all(), error);
            if (error)
            {
                std::cout << "send error:" << error << std::endl;

            }
        }
        else
            break;
    }
    char response[10]={0};
    boost::asio::read(socket_,boost::asio::buffer(response,2));
    cout<<"Got response: "<<response<<endl;
    source_file.close();

}



//
int Client::send_chunk_hashes(vector<u_int64_t> chunkHashes){

    int size=0;
    size+=this->send_integer(chunkHashes.size());
    for(int j=0;j<chunkHashes.size();j++){
        chunkhash hashbytes;
        u_int64_t hash=chunkHashes[j];
        for(int i=0;i<8;i++){

            hashbytes.data[i]= (hash >> (8-i-1)*8) & 0xFF;

        }

        size+=this->send_buffer(hashbytes.data,8);
    }
    return size;
}


int Client::send_block_offsets(vector<int> block_offsets){

    int size=0;
    size+=this->send_integer(block_offsets.size());
    for(unsigned int i=0; i<block_offsets.size();i++){
        size+=this->send_integer(block_offsets[i]);
    }
    return size;
}

//
int Client::send_chunk_offsets(vector<int> chunk_offsets){

    int size=0;
    size+=this->send_integer(chunk_offsets.size());
    for(unsigned int i=0; i<chunk_offsets.size();i++){
        size+=this->send_integer(chunk_offsets[i]);
    }
    return size;
}

//
int Client::send_block_hashes(vector<BlockChecksumSerial>  block_checksums){

    int sent_size=0;
    sent_size+=this->send_integer(block_checksums.size());
    for(int i=0;i<block_checksums.size();i++){
        BlockChecksumSerial block=block_checksums[i];
        sent_size+=send_integer(block.getWeeksum());
        sent_size+=send_buffer(block.strongsum,sizeof(block.strongsum));
    }
    return sent_size;
}


unsigned int Client::send_integer(unsigned int n){
    unsigned char bytes[4];
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;   
    return boost::asio::write(socket_,boost::asio::buffer(bytes),boost::asio::transfer_exactly(4));
}

int Client::send_buffer(unsigned char *buf, int bufsize){
    return boost::asio::write(socket_, boost::asio::buffer(buf,bufsize),boost::asio::transfer_exactly(bufsize));
}


//
template <class T>
int Client::send_vector(vector<T> v){
    int sent_size=0;
    string outbound_data_;
    ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << v;
    outbound_data_ = archive_stream.str();
    char response[max_length];

    boost::system::error_code err;
    boost::asio::streambuf buf;
    ostream os(&buf);
    os<<outbound_data_.size()<<"\r\n";
    //  cout<<"Outbound data size: "<<outbound_data_.size()<<endl;
    sent_size+=boost::asio::write(socket_,buf);
    sent_size+=boost::asio::write(socket_,boost::asio::buffer(outbound_data_,outbound_data_.size()),err);
    socket_.read_some(boost::asio::buffer(response),err);
    if(err){
        cout<<"Error occured with "<<err.message()<<endl;
        return -1;
    }
    return sent_size;
}


//
vector<bool> Client::read_unmatched_chunks()
{
    cout<<"Reading unmatched hash indexes...\n";
    int arr_size;
    size_t bool_got_size;
    boost::system::error_code err;
    boost::asio::streambuf strbuf;
    vector<bool> t;
    char excessdata[max_length]={0};
    string input="";
    boost::asio::read_until(socket_,strbuf,"\r\n");
    istream is(&strbuf);
    is>>arr_size;
    strbuf.consume(2);
    if(strbuf.size()>0){
        arr_size-=strbuf.size();
        is.read(excessdata,strbuf.size());
        input=excessdata;       
    }
    if(arr_size>0){
        char unmatch_data[arr_size];

        boost::asio::read(socket_,boost::asio::buffer(unmatch_data,arr_size),err);
        string temp=unmatch_data;       
        input+=temp;
    }

    try{
        std::istringstream archive_stream_input(input);
        boost::archive::text_iarchive archive_in(archive_stream_input);
        archive_in >> t;
    }
    catch(boost::archive::archive_exception& ex){
        cout<<"Error occurred while receiving unmatched chunks, with:\n"<<ex.what()<<endl;
    }

    return t;

}


