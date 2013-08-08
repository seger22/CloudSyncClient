#include "filehandler.h"

Filehandler::Filehandler()
{
    client=new clienthandler();
    dir="/home/mayuresan/Project/SyncSource";
}

void Filehandler::file_modified(string path, string filename){
    client->send_data("filemodified|"+path+"|"+filename);
    vector<u_int64_t> hashvector=get_chunk_hashes(path,filename);

    //    objects = chunk_file((dir+path+"/"+filename).c_str());

    cout << "Sending chunk hashes..." << endl;

    //    for(int i=0;i<objects.size();i++)
    //        hashvector.push_back(objects[i]->getHash());
    client->send_chunk_hashes(hashvector);
    //    hashvector.clear();

    cout<<"Sent : " << hashvector.size() << " hashes successfully!!"<<endl;

    vector<bool> v=client->read_unmatched_hashes();

    client->send_data("OK");
    unmatchedHashIndex.insert(unmatchedHashIndex.end(),v.begin(),v.end());
    cout<<"Got boolean vector with size:"<<unmatchedHashIndex.size()<<endl;
    cout<<"Recieved unmatched chunk indexes successfully!"<<endl;
    cout<<"Reading unmatched chunk data..."<<endl;
    int total_size=0;

    //vector< vector<u_int32_t > > weaksums;
//    for(int i=0;i<5;i++){
//        vector<u_int32_t> chunk;
//        for(int k=0;k<5;k++){
//            chunk.push_back(345343400 + 10 * i + k);
//        }
//        weaksums.push_back(chunk);
//    }

  //  client->send_block_hashes(weaksums);
  //  vector<chunkdat> unmatchedData= unmatchedChunkData(unmatchedHashIndex,objects,dir+path+"/"+filename,&total_size);


    cout<<"Calculating block checksums:unmatchedhashindex:"<<unmatchedHashIndex.size()<<"objects:"<<objects.size()<<endl;
    vector<Chunk> combinedchunks = combineUnmatchedChunks(unmatchedHashIndex,objects) ;
    vector< vector < BlockChecksum> > blocksums=getBlockChecksum(combinedchunks,dir+path+"/"+filename);
    cout<<"Got chunks of blocks: "<<blocksums.size()<<endl;
    vector< vector < BlockChecksumSerial> > serialblocksums;

    for(int i=0;i<blocksums.size();i++){
        cout<<"Blocks : "<<blocksums[i].size()<<" from block: "<<i;
        //vector<BlockChecksum> blockchecksum=blocksums[i];
        vector<BlockChecksumSerial> serblocks=blockChecksumToSerial(blocksums[i]);
        serialblocksums.push_back(serblocks);
    }
client->send_block_hashes(serialblocksums);
    cout<<"Sending unmatched chunk data..."<<endl;

  //  client->send_chunks(unmatchedData,total_size);
  //  cout<<"Unmatched chunk data sent successfully!"<<endl <<endl;
    unmatched_indexes.clear();
    unmatchedHashIndex.clear();
}


vector <BlockChecksumSerial> Filehandler::blockChecksumToSerial(vector<BlockChecksum> blocks){
    vector<BlockChecksumSerial> serialBlocks;
    for(int i=0;i<blocks.size();i++){
        BlockChecksumSerial serialBlock(blocks[i].getWeeksum(),blocks[i].strongsum);
       serialBlocks.push_back(serialBlock);
       cout<<"Weaksum: "<<blocks[i].getWeeksum()<<endl;
    }
    return serialBlocks;
}


vector<u_int64_t> Filehandler::get_chunk_hashes(string filepath, string filename){
    vector<u_int64_t> chunk_hashes;
    objects = chunk_file((dir+filepath+"/"+filename).c_str());
    for(int i=0;i<objects.size();i++)
        chunk_hashes.push_back(objects[i]->getHash());
    return chunk_hashes;
}

void Filehandler::file_create(string path, string filename)
{
    string checksum = fileChecksum((dir+path+filename).c_str());
    string response;
    client->send_data("filecreated|"+path+"|"+filename+"|"+checksum);
    response=client->receive_data();

    if(strcmp(response.c_str(),"M")==0)
        cout << "File has been sent successfully!" << endl;
    else{
        this->file_modified(path,filename);

    }

    //   client->send_file(path,filename);
}


vector<int> Filehandler::get_indexes()
{

}





void Filehandler::dir_created(string path, string name)
{
    client->send_data("dircreate|"+path+"|"+name);
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
