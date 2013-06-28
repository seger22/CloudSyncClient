#include "filehandler.h"

Filehandler::Filehandler()
{
    client=new clienthandler();
    dir="/home/satthy/Client";
}

void Filehandler::file_modified(string path, string filename){
    client->send_data("filemodified|"+path+"|"+filename);
    vector<u_int64_t> hashvector;
    int max_hashes=49;

    objects = chunk_file((dir+path+"/"+filename).c_str());
    int count=objects.size();
    int endnum,startnum=0;
    client->send_data(boost::lexical_cast<string>(count));

    cout << "Sending chunk hashes..." << endl;
    while(count >= 0)
    {
        endnum= startnum+((count>=max_hashes) ? max_hashes : count);

    for(int i=startnum;i<endnum;i++){
      hashvector.push_back(objects[i]->getHash());

   }

    client->send_chunk_hashes(hashvector);
    hashvector.clear();
    count-=max_hashes;
    startnum+=max_hashes;
    }
cout<<"Sent : " << objects.size() << " hashes successfully!!"<<endl;
    int max_index=200;
    int size_i=boost::lexical_cast<int>(client->receive_data());
    cout<<"Recieving unmatched chunk indexes..."<<endl;
    while(size_i>0){

    vector<int> v=client->read_unmatched_hashes();
        unmatched_indexes.insert(unmatched_indexes.end(),v.begin(),v.end());
        client->send_data("OK");

        size_i-=max_index;
    }

cout<<"Recieved unmatched chunk indexes successfully!"<<endl;
cout<<"Reading unmatched chunk data..."<<endl;
    vector<chunkdat> unmatchedData= unmatchedChunkData(unmatched_indexes,objects,dir+path+"/"+filename);
    cout<<"Sending unmatched chunk data..."<<endl;

    client->send_chunks(unmatchedData);
     cout<<"Unmatched chunk data sent successfully!"<<endl <<endl;
     unmatched_indexes.clear();

}


vector<int> Filehandler::get_indexes()
{

}





void Filehandler::dir_created(string path, string name)
{
    client->send_data("dircreate|"+path+"|"+name);
}

void Filehandler::file_create(string path, string filename)
{
string checksum = fileChecksum((dir+path+filename).c_str());
string response;
    client->send_data("filecreated|"+path+"|"+filename+"|"+checksum);
response=client->receive_data();

if(strcmp(response.c_str(),"M")==0)
    cout << "File has been sent successfully!" << endl;
else
    this->file_modified(path,filename);

 //   client->send_file(path,filename);
}

void Filehandler::dir_deleted(string path, string name)
{
    client->send_data("dirdelete|"+path+"|"+name);
}

void Filehandler::file_delete(string path, string filename)
{
    //std::string file_del=watch_dir+path+filename;
    client->send_data("filedelete|"+path+"|"+filename);
}

void Filehandler::file_rename(string filepath, string old_name, string new_name)
{
    client->send_data("filerename|"+filepath+"|"+old_name+"|"+new_name);
}

void Filehandler::dir_rename(string path, string oldname, string newname)
{
    client->send_data("dirrename|"+path+"|"+oldname+"|"+newname);
}

void Filehandler::file_moved(string oldpath, string newpath, string name)
{
    client->send_data("filemove|"+oldpath+"|"+newpath+"|"+name);
}

void Filehandler::dir_moved(string oldpath, string newpath)
{
    client->send_data("dirmove|"+oldpath+"|"+newpath);
}

void Filehandler::stop_client(){
    client->stop_client();
}
