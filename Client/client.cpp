#include "client.h"

Client::Client(boost::asio::io_service& io_service_, const string &host, const string &service):resolver_(io_service_), socket_(io_service_)
{
    watch_dir="/home/mayuresan/Project/SyncSource";
    sending=false;
    tcp::resolver::query query(host, service);
    resolver_.async_resolve(query,
                            boost::bind(&Client::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::iterator));
}

void Client::handle_resolve(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator){
    if (!err)
    {
        // cout<<"Gone into resolve"<<endl;
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
                              boost::bind(&Client::handle_connect, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
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


void Client::send_request(string request)
{
    boost::system::error_code err;
    strcpy(data_,request.c_str());

    boost::asio::write(socket_,boost::asio::buffer(data_,1024),err);   
}


string Client::get_data_from_server(){
    char ret_data[1024];
    boost::system::error_code err;
    socket_.read_some(boost::asio::buffer(ret_data),err);
    if(!err){
        string res(ret_data);        
        return res;
    }
    else{
        cout<<err.message()<<endl;
        return NULL;
    }
}


void Client::send_chunk_header(vector<chunkdat> strings, int total_size)
{    
    int file_wsize=0;

    boost::system::error_code err;

    int count=strings.empty() ? 0:strings.size();

    this->send_request(boost::lexical_cast<string>(count));

    for(int j=0;j<count;j++)
    {
        int len=strings[j].chunk_size;
        char bytes[3]={0,0,0};
        bytes[0] = (len >> 16) & 0xFF;
        bytes[1] = (len >> 8) & 0xFF;
        bytes[2] = len & 0xFF;

        file_wsize+=len;
        boost::asio::streambuf chunk_buf;
        ostream isdata(&chunk_buf);
        isdata<<"A"<<bytes[0]<<bytes[1]<<bytes[2]<<"d";

        //cout<<"Sending chunk with size:"<<len<<endl;
       // cout<<"Sending chunks in bytes: "<<(int)bytes[1]<<"And :"<<(int)bytes[0]<<endl;

        boost::asio::write(socket_,chunk_buf,boost::asio::transfer_exactly(5));
        cout<<"Sending chunk:"<<j+1<<" of "<<count<<" with size "<<len<<endl;

        boost::asio::write(socket_,boost::asio::buffer(strings[j].data,len),err);

        if(err)
            cout<<"Error:"<<err.message()<<" in chunk:"<<j+1<<endl;
    }
    cout<<"Send file size:"<<file_wsize<<endl;
}


void Client::send_string_vector(vector<string> strings)
{
    std::string outbound_data_;
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << strings;
    outbound_data_ = archive_stream.str();
    //   cout<<"Out size:"<<outbound_data_.size()<<endl;
    //char response[max_length];
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(outbound_data_));
    boost::system::error_code err;
    size_t send=boost::asio::write(socket_,buffers,err);
    //  cout<<"Sent amount: "<<send<<endl;
}




void Client::send_chunk_data(string out_data){
    string buf_data;
    int str_size=out_data.size();
    size_t sentsize=0;
    boost::system::error_code ec;
    int count=0;
    while(count+max_length-1<str_size){
        buf_data=out_data.substr(count,max_length) ;
        cout<<"Size of buf_data:"<<buf_data.length()<<endl;

        //cout<<"Sent data is : "<<buf_data<<endl;

        sentsize=boost::asio::write(socket_,boost::asio::buffer(buf_data,buf_data.length()),ec);
        if(!ec){
            cout<<"chunks sent successfully"<<endl;
            cout<<"Sent size: "<<sentsize<<endl;
        }
        else{
            cout<<"Chunk sending Wrong with :"<<ec.message()<<endl;
            cout<<"Sent size: "<<sentsize<<endl;
            break;
        }
        count+=max_length;
    }
    buf_data=out_data.substr(count,str_size-count);

    vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(buf_data));

    sentsize=boost::asio::write(socket_,buffers,ec);
    if(!ec){
        cout<<"chunk sent successfully"<<endl;
        cout<<"Sent size: "<<sentsize<<endl;
    }
    else{
        cout<<"Chunk sending Wrong with :"<<ec.message()<<endl;
        cout<<"Sent size: "<<sentsize<<endl;
        //break;
    }
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

}


void Client::stop_client()
{
    //client_io_service.stop();
    //threads.join_all();
}

void printnum(int c){
    cout<<c<<endl;
}

bool Client::send_buffer(string buffer)
{
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(buffer));
    boost::system::error_code err;
    cout<<"Writing data "<<buffer<<endl;
    boost::asio::write(socket_,boost::asio::buffer(buffer,max_length),err);
    if(err){
        cout<<"Error occured with "<<err.message()<<endl;
        return false;
    }
    else{
        cout<<"Chunk hashed Sent Successfully"<<endl;
        return true;
    }
}


void Client::send_chunk_hashes(vector<u_int64_t> chunkHashes){
 /*   std::string outbound_data_;
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << chunkHashes;
    outbound_data_ = archive_stream.str();
    char response[max_length];

    boost::system::error_code err;    
    boost::asio::streambuf buf;
    ostream os(&buf);
    os<<outbound_data_.size()<<"\r\n";
    //cout<<"Size is:"<<outbound_data_.size()<<endl;
    boost::asio::write(socket_,buf);
    boost::asio::write(socket_,boost::asio::buffer(outbound_data_,outbound_data_.size()),err);
    socket_.read_some(boost::asio::buffer(response),err);
    */
    bool err=this->send_vector<u_int64_t>(chunkHashes);
    if(err){
       // cout<<"Error occured with "<<err.message()<<endl;
    }
    else
        cout<<"Sent chunk hashes successfully!!\n"  ;

}


void Client::send_block_hashes(vector<vector<u_int32_t> > t){
    this->send_vector< vector <u_int32_t > > (t);
}

template <class T>
bool Client::send_vector(vector<T> v){
    std::string outbound_data_;
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << v;
    outbound_data_ = archive_stream.str();
    char response[max_length];

    boost::system::error_code err;    
    boost::asio::streambuf buf;
    ostream os(&buf);
    os<<outbound_data_.size()<<"\r\n";    
    boost::asio::write(socket_,buf);
    boost::asio::write(socket_,boost::asio::buffer(outbound_data_,outbound_data_.size()),err);
    socket_.read_some(boost::asio::buffer(response),err);
    if(err){
        cout<<"Error occured with "<<err.message()<<endl;
        return false;
    }
    return true;
}





vector<bool> Client::read_unmatched_chunks()
{
    cout<<"Reading unmatched hash indexes...\n";
    int arr_size;
    boost::system::error_code err;
    boost::asio::streambuf strbuf;
    vector<bool> t;
    char excessdata[max_length]={0};
    string input="";
    size_t got_size= boost::asio::read_until(socket_,strbuf,"\r\n");
    istream is(&strbuf);
    is>>arr_size;
    cout<<"Got size as:"<<arr_size<<endl;
    strbuf.consume(2);
    if(strbuf.size()>0){
        arr_size-=strbuf.size();
        is.read(excessdata,strbuf.size());
        input=excessdata;
        //cout<<"Got excess data as:"<<input<<endl;
    }
    if(arr_size>0){
char unmatch_data[arr_size];

   got_size=boost::asio::read(socket_,boost::asio::buffer(unmatch_data,arr_size),err);
string temp=unmatch_data;
temp=temp.substr(0,got_size);
    input+=temp;
    }
    //cout<<"Got data :"<<input<<"with size:"<<got_size<<endl;

    std::istringstream archive_stream_input(input);
    boost::archive::text_iarchive archive_in(archive_stream_input);
    archive_in >> t;
    return t;
}


