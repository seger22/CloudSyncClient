/*
 * Copyright (C) 2010 Bobby Martin (bobbymartin2@gmail.com)
 * Copyright (C) 2004 Hyang-Ah Kim (hakim@cs.cmu.edu)
 * Copyright (C) 2000 David Mazieres (dm@uun.org)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 */

#include "chunkhandler.h"

#include <hashlibpp.h>

void debug(const char* msg, ...)
{
    if( DEBUG )
    {
        va_list argp;

        va_start(argp, msg);
        vfprintf(stderr, msg, argp);
        va_end(argp);
    }
}

BOOL fileExists(const char* fname)
{
    return access( fname, F_OK ) != -1;
}

//end utility functions


void printChunkData(
        const char* msgPrefix,
        int size,
        u_int64_t fingerprint,
        u_int64_t hash)
{
    fprintf(stderr, "%s chunk hash: %016llx fingerprint: %016llx length: %d\n",
            msgPrefix,
            hash,
            fingerprint,
            size);
}

void printChunkContents(
        u_int64_t hash,
        const unsigned char* buffer,
        int size)
{
    fprintf(stderr, "Chunk with hash %016llx contains:\n");
    for(int i = 0; i < size; ++i)
    {
        int tmp = buffer[i];
        fprintf(stderr, "%x", tmp);
    }
    fprintf(stderr, "\n");
}
u_int64_t makeBitMask(int maskSize)
{
    u_int64_t retval = 0;

    u_int64_t currBit = 1;
    for(int i = 0; i < maskSize; ++i)
    {
        retval |= currBit;
        currBit <<= 1;
    }

    return retval;
}


vector<Chunk> processChunks(DataSource* ds,const char *path)
{
    const u_int64_t POLY = FINGERPRINT_PT;
    window rw(POLY);
    rabinpoly rp(POLY);

    BitwiseChunkBoundaryChecker chunkBoundaryChecker;
    PrintChunkProcessor chunkProcessor;
    vector<Chunk> chunk;

    int next;
    u_int64_t val = 0;
    //add a leading 1 to avoid the issue with rabin codes & leading 0s
    u_int64_t hash = 1;
    rw.slide8(1);

    unsigned int offset=0;
    while((next = ds->getByte()) != -1)
    {
        chunkProcessor.processByte(next);

        hash = rp.append8(hash, (char)next);
        val = rw.slide8((char)next);

        if( chunkBoundaryChecker.isBoundary(val, chunkProcessor.getSize()) )
        {

           // chunk.push_back(new Chunk(hash,offset,chunkProcessor.getSize(),path));
            Chunk chnk(hash,offset,chunkProcessor.getSize(),path);
            chunk.push_back(chnk);
            offset+=chunkProcessor.getSize();
            chunkProcessor.completeChunk(hash, val);

            hash = 1;
            rw.reset();
            rw.slide8(1);
        }

    }
   // chunk.push_back(new Chunk(hash,offset,chunkProcessor.getSize(),path));
    Chunk chnk(hash,offset,chunkProcessor.getSize(),path);
    chunk.push_back(chnk);
    chunkProcessor.completeChunk(hash, val);

    return chunk;
}

vector<Chunk> chunk_file(const char *path)
{
    FILE* is = fopen(path, "r");
    DataSource* ds= new RawFileDataSource(is);
    vector<Chunk> chunk= processChunks(ds,path);
    fclose(is);
    return chunk;
}


void
chunk_directory(const char *dpath)
{
    DIR *dirp;
    if ((dirp = opendir (dpath)) == 0) return;
    struct dirent *de = NULL;
    while ((de = readdir (dirp))) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        char path[PATH_MAX];
        sprintf(path, "%s/%s", dpath, de->d_name);
        struct stat sb;
        lstat(path, &sb);
        if (S_ISDIR(sb.st_mode))
            chunk_directory(path);
        else if (S_ISREG(sb.st_mode) && !S_ISLNK(sb.st_mode)){
            //fprintf(stderr, "\n\n------------------------------new file starts    %s -------------------------------------\n\n",path);
            chunk_file(path);
        }
    }
    closedir(dirp);
}

vector<chunkdat> unmatchedChunkData(vector<bool> indexes,vector<Chunk> chunkdetails,string path){
    vector<chunkdat> chunkData;
    DeltaHandler deltahandler;
    ifstream in_file(path.c_str(),std::ifstream::binary);
    int beforeEncode=0,afterencode=0;
    for(int y=0;y<indexes.size();y++){
        chunkdat ch_data;
        if(!indexes[y]){
            Bytef *sourceblock=(Bytef*) malloc(1024*1024*sizeof(Byte));
            int value = y;
            in_file.seekg(chunkdetails[value].getOffset(),in_file.beg);
            in_file.read((char *)sourceblock,chunkdetails[value].getLength());
            Bytef* deltachunk=(Bytef*) malloc(1024*1024*sizeof(Byte));
            unsigned long int deltasize;
            deltahandler.getDeltaBlock(sourceblock,chunkdetails[value].getLength(),&deltachunk,&deltasize);
            cout<<"Size before delta encode: "<<chunkdetails[value].getLength()<<" and size after: "<<deltasize<<endl;
            beforeEncode+=chunkdetails[value].getLength();
            afterencode+=deltasize;
            ch_data.chunk_size=deltasize;
            memcpy(ch_data.data,reinterpret_cast<char *>(deltachunk),deltasize);
            cout<<"Reading chunk No: "<<value+1<<endl;
            chunkData.push_back(ch_data);
            free(sourceblock);
            free(deltachunk);
        }
    }
    cout<<"Total chunk size before encode:"<<beforeEncode<<endl;
    cout<<"Total chunk size after encode:"<<afterencode<<endl;
    in_file.close();
    return chunkData;
}

void fillUnmatchedChunkData(vector<bool> indexes, vector<Chunk> chunkHead, string path, queue<chunkdat> *unmatchedChunks){
    ifstream in_file(path.c_str(),std::ifstream::binary);
    DeltaHandler deltahandler;
    int beforeEncode=0,afterencode=0;
    for(int y=0;y<indexes.size();y++){
        chunkdat ch_data;
        if(!indexes[y]){

            Bytef sourceblock[1024*65]={0};
            int value = y;

            in_file.seekg(chunkHead[value].getOffset(),in_file.beg);
            in_file.read((char *)sourceblock,chunkHead[value].getLength());

            Bytef* deltachunk;
            unsigned long int deltasize=0;
            deltahandler.getDeltaBlock(sourceblock,chunkHead[value].getLength(),&deltachunk,&deltasize);            
            beforeEncode+=chunkHead[value].getLength();
            afterencode+=deltasize;
            ch_data.chunk_size=deltasize;
            memcpy(ch_data.data,reinterpret_cast<char *>(deltachunk),deltasize);           
            unmatchedChunks->push(ch_data);            
            deltachunk=NULL;
        }
    }
    in_file.close();
    cout<<"Total size before delta encode:"<<beforeEncode<<endl;
    cout<<"Total size after delta encode:"<<afterencode<<endl;
}


vector<Chunk> combineUnmatchedChunks(vector<bool> chunkmatchstatus, vector<Chunk> chunkdetails){
    vector<Chunk>  combinedChunks;
    int unmatchcount=0;
    for(int i=0;i<chunkmatchstatus.size();i++){
        if(!chunkmatchstatus[i]){
            unmatchcount++;
            Chunk firstChunk(chunkdetails[i].getHash(),chunkdetails[i].getOffset(),chunkdetails[i].getLength(),chunkdetails[i].getPath());

            while(i+1<chunkmatchstatus.size() && !chunkmatchstatus[i+1]){
                firstChunk.setLength(chunkdetails[i+1].getLength()+firstChunk.getLength());
                unmatchcount++;
                i++;
            }
            combinedChunks.push_back(firstChunk);
        }


    }

    return combinedChunks;
}

vector< BlockChecksum>  getBlockChecksum(vector<Chunk> unmatchedChunks, string filepath){
    cout<<"Calculating weak and strong checksums of the blocks...\n";
    vector < BlockChecksum> blockChecksums;
    int blockSize=ConfigurationManager::getBlockSize();
    ifstream in_file(filepath.c_str(),std::ifstream::binary);
    int blockcount=0;
    for(int i=0; i<unmatchedChunks.size();i++){
        in_file.seekg(unmatchedChunks[i].getOffset(),in_file.beg);
        int k=0;
        char data[blockSize];

        for(k=0;k+blockSize<unmatchedChunks[i].getLength();k+=blockSize){
            in_file.read(data,blockSize);
            unsigned int weaksum=rs_calc_weak_sum(&data,blockSize);
            BlockChecksum block(weaksum,unmatchedChunks[i].getOffset()+k,blockSize);

            rs_calc_strong_sum(&data,blockSize,&block.strongsum);
            blockcount++;            
            blockChecksums.push_back(block);
        }
        int kl=unmatchedChunks[i].getLength();
        int kb=k-blockSize;

         if(kl>kb){
            int remainingRBlength=unmatchedChunks[i].getLength()-k;
            in_file.read(data,remainingRBlength);
            char extradata[remainingRBlength];
            unsigned int weaksum=rs_calc_weak_sum(&extradata,blockSize);
            BlockChecksum block(weaksum,unmatchedChunks[i].getOffset()+k,remainingRBlength);
            rs_calc_strong_sum(&data,blockSize,&block.strongsum);
            blockcount++;
            blockChecksums.push_back(block);
        }
    }
    in_file.close();
    return blockChecksums;
}

int getFileSize(string fullpath){
    struct stat st;
    stat(fullpath.c_str(), &st);
    return st.st_size;
}

vector<chunkdat> getUnmatchedBlocks(vector<BlockChecksum> checksumBlocks, vector<bool> unmatchedBlockData,string filepath){
    DeltaHandler deltahandler;
    FILE *targetfile;
    vector<chunkdat> blocks;
    //infile.seekg(0,infile.beg);
    for(int i=0;i<unmatchedBlockData.size();i++){
        if(!unmatchedBlockData[i]){
            chunkdat block;
            int length=checksumBlocks[i].getLength();
            targetfile=fopen(filepath.c_str(),"rb");
            Bytef *sourceblock=(Bytef*) malloc(1024*2*sizeof(Byte));
            fseek(targetfile,checksumBlocks[i].getOffset(),SEEK_SET);
            uLong targetBlocksize=fread((void *)sourceblock,1,length,targetfile);
            // fseek(targetfile,checksumBlocks[i].getOffset(),SEEK_SET);
            // fread(block.data,1,length,targetfile);
            Bytef* deltablock=(Bytef*) malloc(1024*2*sizeof(Byte));
            unsigned long int deltasize=0;
            deltahandler.getDeltaBlock(sourceblock,targetBlocksize,&deltablock,&deltasize);
            memcpy(block.data,reinterpret_cast<char *>(deltablock),deltasize);
            block.chunk_size=deltasize;
            //block.chunk_size=(int)deltasize;
            cout<<"Size before delta: "<<length<<" bytes\n";
            cout<<"Size after delta: "<<deltasize<<" bytes"<<endl;
            blocks.push_back(block);
            fclose(targetfile);
            free(sourceblock);
            sourceblock=NULL;
            free(deltablock);
            deltablock=NULL;
        }
    }
    //infile.close();
    return blocks;
}


vector<chunkdat> getmergedUnmatchedBlocks(vector<BlockChecksum> checksumBlocks, vector<bool> unmatchedBlockData,string filepath, vector<int> *blockOffsets){
    DeltaHandler deltahandler;
    ifstream targetfile(filepath.c_str(), std::ifstream::binary);
    int length=0,beforedelta=0,afterdelta=0;
    vector<chunkdat> blocks;  

    for(int i=0;i<unmatchedBlockData.size();i++){
        if(!unmatchedBlockData[i]){
            chunkdat block;
            length=checksumBlocks[i].getLength();

            Bytef sourceblock[1024*65]={0};
            targetfile.seekg(checksumBlocks[i].getOffset());
            targetfile.read((char *)sourceblock, length);

            blockOffsets->push_back(checksumBlocks[i].getOffset());
            while(i+1<unmatchedBlockData.size()){
                if(!unmatchedBlockData[i+1]){
                    if(checksumBlocks[i+1].getLength()+length<65536){
                        i++;                        
                        targetfile.seekg(checksumBlocks[i].getOffset(), targetfile.beg);

                        targetfile.read((char *)(sourceblock+length),checksumBlocks[i].getLength());

                        length+=checksumBlocks[i].getLength();
                    }
                    else
                        break;
                }
                else{
                    i++;
                }
                blockOffsets->push_back(checksumBlocks[i].getOffset());
            }

            Bytef* deltablock;
            unsigned long int deltasize=0;
            deltahandler.getDeltaBlock(sourceblock,length,&deltablock,&deltasize);
            memcpy(block.data,reinterpret_cast<char *>(deltablock),deltasize);
            block.chunk_size=deltasize;

            beforedelta+=length;
            afterdelta+=deltasize;

            blocks.push_back(block);

        }
        else
            blockOffsets->push_back(checksumBlocks[i].getOffset());
    }
    cout<<"Size before delta: "<<beforedelta<<" bytes\n";
    cout<<"Size after delta: "<<afterdelta<<" bytes\n";
    targetfile.seekg(0, targetfile.end);
    length=targetfile.tellg();
    blockOffsets->push_back(length);
   targetfile.close();    
    return blocks;
}



string fileChecksum(const char *path)
{
    hashwrapper *myWrapper = new sha1wrapper();
    string checksum;
    try
    {
        checksum   = myWrapper->getHashFromFile(path);
    }
    catch(hlException &e)
    {
        std::cerr << "Error: "<<e.error_message() << std::endl;
    }

    delete myWrapper;
    return checksum;
}


chunkdat getSmallFile(string fullpath, int filesize){
     ifstream targetfile(fullpath.c_str(), std::ifstream::binary);
     targetfile.seekg(0,targetfile.beg);
     chunkdat filedat;
     targetfile.read(filedat.data,filesize);
     filedat.chunk_size=filesize;
     targetfile.close();
     return filedat;
}
