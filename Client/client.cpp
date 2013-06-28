#include "client.h"

Client::Client(boost::asio::io_service& io_service_, const string &host, const string &service):resolver_(io_service_), socket_(io_service_)
{
    watch_dir="/home/satthy/Client";
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
   // boost::asio::async_write(socket_,boost::asio::buffer(data_,1024),boost::bind(&Client::handle_write_request, this, boost::asio::placeholders::error));
}


string Client::get_data_from_server(){
    char ret_data[1024];
    boost::system::error_code err;
    socket_.read_some(boost::asio::buffer(ret_data),err);
    if(!err){
        string res(ret_data);
        //strcpy(res,ret_data);
    return res;
    }
    else{
        cout<<err.message()<<endl;
        return NULL;
    }
}


void Client::send_chunk_header(vector<chunkdat> strings)
{

    char response[max_length];
    int file_wsize=0;

    boost::system::error_code err;

int count=strings.empty() ? 0:strings.size();

this->send_request(boost::lexical_cast<string>(count));

for(int j=0;j<count;j++)
        {

    int len=strings[j].chunk_size;

    file_wsize+=len;

    size_t sent= boost::asio::write(socket_,boost::asio::buffer(strings[j].data,len),err);
            if(!err){

                socket_.read_some(boost::asio::buffer(response),err);
                if(!err){
                }
            }
            else
                cout<<"CHunk sent error:"<<err.message()<<endl;            
        }

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
    std::string outbound_data_;
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
        archive << chunkHashes;
    outbound_data_ = archive_stream.str();
    char response[max_length];

    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(outbound_data_));
    boost::system::error_code err;

    size_t writt= boost::asio::write(socket_,buffers,err);
    socket_.read_some(boost::asio::buffer(response),err);
    if(err){
        cout<<"Error occured with "<<err.message()<<endl;
    }
    else
        ;

}

void Client::send_chunks(vector<string> chunks)
{

}



vector<int> Client::read_unmatched_chunks()
{
    boost::system::error_code err;

    vector<int> t;
           size_t got_size=socket_.read_some(boost::asio::buffer(data_),err);

           string input=data_;
           input=input.substr(0,got_size);
        //   cout<<"Got data :"<<input<<"with size:"<<got_size<<endl;
    std::istringstream archive_stream_input(input);
    boost::archive::text_iarchive archive_in(archive_stream_input);
    archive_in >> t;
    return t;

}


