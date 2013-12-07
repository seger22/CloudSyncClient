#include "blockhandler.h"


BlockHandler::BlockHandler()
{
}

//vector<chunkdat> getUnmatchedBlocks(vector<BlockChecksum> checksumBlocks, vector<bool> unmatchedBlockData,string filepath)
vector<chunkdat> BlockHandler::getUnmatchedBlocks(vector<BlockCheckSum> checksumBlocks, vector<bool> unmatchedBlockData,string filepath){
  //  ifstream infile(filepath.c_str(),ifstream::binary);
    FILE *targetfile;
    vector<chunkdat> blocks;
    //infile.seekg(0,infile.beg);
    for(int i=0;i<unmatchedBlockData.size();i++){
        if(!unmatchedBlockData[i]){           
            chunkdat block;
            int length=checksumBlocks[i].getLength();
            targetfile=fopen(filepath.c_str(),"rb");
            Bytef *sourceblock=(Bytef*) malloc(BUF_SIZE*sizeof(Byte));           
            fseek(targetfile,checksumBlocks[i].getOffset(),SEEK_SET);
            uLong targetBlocksize=fread((void *)sourceblock,1,length,targetfile);
           // fseek(targetfile,checksumBlocks[i].getOffset(),SEEK_SET);
           // fread(block.data,1,length,targetfile);
            Bytef* deltablock=(Bytef*) malloc(BUF_SIZE*sizeof(Byte));
            unsigned long int deltasize=0;
            deltahandler->getDeltaBlock(sourceblock,targetBlocksize,&deltablock,&deltasize);
            memcpy(block.data,reinterpret_cast<char *>(deltablock),deltasize);
            block.chunk_size=deltasize;
            //block.chunk_size=(int)deltasize;
            cout<<"Size before delta: "<<length<<" and size after: "<<deltasize<<endl;
            blocks.push_back(block);
            free(sourceblock);
            free(deltablock);
        }
    }
    fclose(targetfile);
    //infile.close();
    return blocks;
}
