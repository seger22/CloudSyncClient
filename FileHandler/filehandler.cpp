#include "filehandler.h"


FileHandler::FileHandler()
{
    dblib=DBlib::getInstance();

    client=new clienthandler();
    dir=ConfigurationManager::getLocation();
    blockhandler=new BlockHandler();
}

/**
 * @brief FileHandler::file_modified
 * @param path
 * @param filename
 */
void FileHandler::file_modified(string path, string filename){
    total_data_sent=0;
    total_metadata_sent=0;
    string checksum = fileChecksum((dir+path+filename).c_str());

    int isSmall=0;
    int filesize=getFileSize(dir+path+filename);
    if(filesize<ConfigurationManager::getBlockSize()){
        isSmall=1;
        cout<<"File size is small, so directly sending to the server!\n";
        total_metadata_sent+=client->send_data("sf|"+path+"|"+filename);
        chunkdat smfile=getSmallFile(dir+path+filename,filesize);
        total_data_sent=client->send_chunk(smfile,1);
        cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes and metadata: "<<total_metadata_sent<<" bytes\n"<<endl;
        return;
    }

    total_metadata_sent+=client->send_data("fm|"+path+"|"+filename + "|" + checksum + "|"+ boost::lexical_cast<string>(isSmall));

    string response=client->receive_data();
    if(strcmp(response.c_str(),"M")==0){
        total_matched_chunks=0;
        cout << "File already exists in the server!\n";
        cout<<"Total data transferred for this change: "<<total_metadata_sent<<" bytes\n"<<endl;
        ofstream outfile;
        outfile.open("/home/mayuresan/Project/transferlog.txt",ofstream::out|ofstream::app);
        outfile<<total_metadata_sent<<"\t"<<total_data_sent<<"\t"<<total_metadata_sent+total_data_sent<<"\t"<<total_matched_chunks<<endl;
        outfile.close();
    }
    else{
        cout<<"\n--------------------Chunk matching phase-------------------------\n";
        vector<u_int64_t> hashvector=get_chunk_hashes(path,filename);
        cout << "Sending chunk hashes..." << endl;
        int hash_size=client->send_chunk_hashes(hashvector);
        total_metadata_sent+=hash_size;

        cout<<"Total number of chunk hashes sent: " << hashvector.size() <<endl;

        cout<<"Recieving unmatched chunk indexes...\n";
        vector<bool> v=client->read_unmatched_hashes();

        total_metadata_sent+=client->send_data("OK");

        vector<int> chunkOffsets;
        vector<int> blockOffsets;
        int match_count=0;
        for(int k=0;k<v.size();k++){
            if(v[k]){
                chunkOffsets.push_back(objects[k].getOffset());
                match_count++;
            }
        }
        total_matched_chunks=match_count;
        if(match_count==v.size()){
            cout<<"Same file in the destination!!!\n\n";
            return;
        }

        unmatchedHashIndex.insert(unmatchedHashIndex.end(),v.begin(),v.end());
        cout<<"\n--------------------Block matching phase-------------------------\n";
        cout<<"Dividing unmatched chunks into blocks...\n";
        vector<Chunk> combinedchunks = combineUnmatchedChunks(unmatchedHashIndex,objects);
        if(combinedchunks.size()==0)
            return;

        vector< BlockChecksum > blocksums=getBlockChecksum(combinedchunks,dir+path+"/"+filename);

        cout<<"Total number of block checksums to be sent:"<<blocksums.size()<<endl;

        vector<BlockChecksumSerial> serblocks=blockChecksumToSerial(blocksums);
        cout<<"Sending block checksums...\n";

        hash_size= client->send_block_hashes(serblocks);

        total_metadata_sent+=hash_size;
        cout<<"Receiving block match status...\n";
        vector<bool> unmatchedBlockIndexes=client->read_unmatched_hashes();        

        total_metadata_sent+= client->send_data("OK");

        cout<<"Sending unmatched block data...\n";

        total_metadata_sent+= client->send_chunk_offsets(chunkOffsets);
        //vector<chunkdat> unmatchedBlocks= getUnmatchedBlocks(blocksums,unmatchedBlockIndexes,(dir+path+"/"+filename).c_str());
        vector<chunkdat> unmatchedBlocks=getmergedUnmatchedBlocks(blocksums,unmatchedBlockIndexes,(dir+path+"/"+filename).c_str(),&blockOffsets);
        total_metadata_sent+=client->send_block_offsets(blockOffsets);
        total_data_sent+=client->send_block_data(unmatchedBlocks);

        unmatched_indexes.clear();
        unmatchedHashIndex.clear();
        cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes and metadata: "<<total_metadata_sent<<" bytes\n"<<endl;
        ofstream outfile;
        outfile.open("/home/mayuresan/Project/transferlog.txt",ofstream::out|ofstream::app);
        outfile<<total_metadata_sent<<"\t"<<total_data_sent<<"\t"<<total_metadata_sent+total_data_sent<<"\t"<<total_matched_chunks<<endl;     
        outfile.close();        
        dblib->insert_into_change("File \""+filename+"\" in \""+path+"\" modified");

    }
}


vector <BlockChecksumSerial> FileHandler::blockChecksumToSerial(vector<BlockChecksum> blocks){
    vector<BlockChecksumSerial> serialBlocks;
    for(int i=0;i<blocks.size();i++){
        BlockChecksumSerial serialBlock(blocks[i].getWeeksum(),blocks[i].strongsum);
        serialBlocks.push_back(serialBlock);
        //cout<<"Weaksum: "<<blocks[i].getWeeksum()<<endl;
    }
    return serialBlocks;
}


vector<u_int64_t> FileHandler::get_chunk_hashes(string filepath, string filename){
    vector<u_int64_t> chunk_hashes;
    objects = chunk_file((dir+filepath+"/"+filename).c_str());
    for(int i=0;i<objects.size();i++)
        chunk_hashes.push_back(objects[i].getHash());
    return chunk_hashes;
}

void FileHandler::file_create(string path, string filename)
{
    string checksum = fileChecksum((dir+path+filename).c_str());
    string response;
    total_data_sent=0;
    total_metadata_sent=0;

    int isSmall=0;
    int filesize=getFileSize(dir+path+filename);
    if(filesize<ConfigurationManager::getBlockSize()){
        isSmall=1;
        cout<<"File size is small, so directly sending to the server!\n";
        total_metadata_sent+=client->send_data("sf|"+path+"|"+filename);
        chunkdat smfile=getSmallFile(dir+path+filename,filesize);
        total_data_sent=client->send_chunk(smfile,1);
        cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes and metadata: "<<total_metadata_sent<<" bytes\n"<<endl;
        return;
    }

    total_metadata_sent+=client->send_data("fc|"+path+"|"+filename+"|"+checksum+"|"+boost::lexical_cast<string>(isSmall));
    response=client->receive_data();

    if(strcmp(response.c_str(),"M")==0){
        total_matched_chunks=0;
        cout << "File already exists in the server, so copied to the destination folder!\n";
        cout<<"Total data transferred for this change: "<<total_metadata_sent<<" bytes\n"<<endl;
        ofstream outfile;
        outfile.open("/home/mayuresan/Project/transferlog.txt",ofstream::out|ofstream::app);
        outfile<<total_metadata_sent<<"\t"<<total_data_sent<<"\t"<<total_metadata_sent+total_data_sent<<"\t"<<total_matched_chunks<<endl;
        outfile.close();
    }
    else if(strcmp(response.c_str(),"E")==0){
        cout<<"File with same name already exists in the destination\n";
        this->file_modified(path, filename);
    }
    else{
        vector<u_int64_t> hashvector;
        objects = chunk_file((dir+path+"/"+filename).c_str());
        for(int i=0;i<objects.size();i++)
            hashvector.push_back(objects[i].getHash());

        cout << "Sending chunk hashes..." << endl;
        int hash_size=client->send_chunk_hashes(hashvector);
        total_metadata_sent+=hash_size;

        cout<<"Total number of chunk hashes sent: " << hashvector.size() <<endl;

        cout<<"Recieving unmatched chunk indexes...\n";

        vector<bool> matchindex=client->read_unmatched_hashes();
        total_metadata_sent+=client->send_data("OK");
        cout<<"Reading unmatched chunk data from the file..."<<endl;
        fillUnmatchedChunkData(matchindex,objects,dir+path+"/"+filename,&unmatchedChunks);
        processQueue(matchindex);
        unmatched_indexes.clear();

        cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes and metadata: "<<total_metadata_sent<<" bytes\n"<<endl;
        ofstream outfile;
        outfile.open("/home/mayuresan/Project/transferlog.txt",ofstream::out|ofstream::app);
        outfile<<total_metadata_sent<<"\t"<<total_data_sent<<"\t"<<total_metadata_sent+total_data_sent<<"\t"<<total_matched_chunks<<endl;

        outfile.close();

    }
      dblib->insert_into_change("File \""+filename+"\" created in \""+path+"\"");

}


void FileHandler::processQueue(vector<bool> matchindex){
    cout<<"Sending unmatched chunks..."<<endl;
    int count=1;
    for(int i=0;i<matchindex.size();i++){
        if(!matchindex[i]){
            if(unmatchedChunks.empty()){
                while(unmatchedChunks.empty()){}
            }
            chunkdat chunk=unmatchedChunks.front();
            total_data_sent+=client->send_chunk(chunk,count);
            unmatchedChunks.pop();
            count++;
        }
    }
    total_matched_chunks=matchindex.size()-count+1;
}


void FileHandler::dir_created(string path, string name)
{
    total_data_sent=0;
    total_data_sent+=client->send_data("dc|"+path+"|"+name);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("Directory \""+name+"\" created in \""+path+"\"");

}

void FileHandler::dir_deleted(string path, string name)
{
    total_data_sent=0;
    total_data_sent+=client->send_data("dd|"+path+"|"+name);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("Directory \"" +name+"\" deleted from \""+path+"\"");

}

void FileHandler::file_delete(string path, string filename)
{
    //std::string file_del=watch_dir+path+filename;
    total_data_sent=0;
    total_data_sent+=client->send_data("fd|"+path+"|"+filename);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("File \"" +filename+"\" deleted from \""+path+"\"");

}

void FileHandler::file_rename(string filepath, string old_name, string new_name)
{
    total_data_sent=0;
    total_data_sent+=client->send_data("fr|"+filepath+"|"+old_name+"|"+new_name);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("File \"" +old_name+"\" in "+filepath+"\" is renamed to \""+new_name+"\"");

}

void FileHandler::dir_rename(string path, string oldname, string newname)
{
    total_data_sent=0;
    total_data_sent+=client->send_data("dr|"+path+"|"+oldname+"|"+newname);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("Directory \"" +oldname+"\" in \""+path+"\" is renamed to \""+newname+"\"");

}

void FileHandler::file_moved(string oldpath, string newpath, string name)
{
    total_data_sent=0;
    total_data_sent+=client->send_data("fv|"+oldpath+"|"+newpath+"|"+name);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("File \"" +name+"\" in \""+oldpath+"\" is moved to \""+newpath+"\"");

}

void FileHandler::dir_moved(string oldpath, string newpath)
{
    total_data_sent=0;
    total_data_sent+=client->send_data("dm|"+oldpath+"|"+newpath);
    cout<<"Total data transferred for this change: "<<total_data_sent<<" bytes"<<endl;

        dblib->insert_into_change("Directory in \""+oldpath+"\" is moved to \""+newpath+"\"");

}

void FileHandler::stop_client(){
    client->stop_client();
}

