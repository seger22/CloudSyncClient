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


vector<Chunk*> processChunks(DataSource* ds,const char *path)
{
  const u_int64_t POLY = FINGERPRINT_PT;
  window rw(POLY);
  rabinpoly rp(POLY);

  BitwiseChunkBoundaryChecker chunkBoundaryChecker;
  PrintChunkProcessor chunkProcessor;
  vector<Chunk*> chunk;

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

chunk.push_back(new Chunk(hash,offset,chunkProcessor.getSize(),path));
offset+=chunkProcessor.getSize();
  chunkProcessor.completeChunk(hash, val);

        hash = 1;
        rw.reset();
        rw.slide8(1);
    }

  }
chunk.push_back(new Chunk(hash,offset,chunkProcessor.getSize(),path));
  chunkProcessor.completeChunk(hash, val);

return chunk;
}

vector<Chunk*> chunk_file(const char *path)
{
FILE* is = fopen(path, "r");
    DataSource* ds= new RawFileDataSource(is);

    vector<Chunk*> chunk= processChunks(ds,path);
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

vector<chunkdat> unmatchedChunkData(vector<bool> indexes,vector<Chunk*> chunkdetails,string path, int* total_size){
vector<chunkdat> chunkData;
ifstream in_file(path.c_str(),std::ifstream::binary);

for(int y=0;y<indexes.size();y++){
    chunkdat ch_data;
  //  FILE* is = fopen(path.c_str(), "r");
  //      DataSource* ds= new RawFileDataSource(is);

  //  int value = indexes[y];
  //  int off_val=0;
  //  for(int x=0; x< (chunkdetails[value]->getOffset()+chunkdetails[value]->getLength());x++){
  //      if(chunkdetails[value]->getOffset()<=x){
           // chunking +=ds->getByte();
  //          ch_data.data[off_val]=ds->getByte();
  //          off_val++;
  //      }

    //    else
    //        ds->getByte();
    if(!indexes[y]){
    int value = y;
    in_file.seekg(chunkdetails[value]->getOffset(),in_file.beg);
    in_file.read(ch_data.data,chunkdetails[value]->getLength());       

   // }
    ch_data.chunk_size=chunkdetails[value]->getLength();
    total_size+=chunkdetails[value]->getLength();
    cout<<"Read chunk No:"<<value<<endl;
chunkData.push_back(ch_data);
    }
}
in_file.close();
return chunkData;
}



vector<Chunk> combineUnmatchedChunks(vector<bool> unmatched, vector<Chunk*> chunkdetails){
    cout<<"Merging chunks\n";
    vector<Chunk>  combinedChunks;
    for(int i=0;i<unmatched.size();i++){
        if(!unmatched[i]){
            cout<<"Matched\n";
            Chunk* firstChunk=new Chunk(chunkdetails[i]->getHash(),chunkdetails[i]->getOffset(),chunkdetails[i]->getLength(),chunkdetails[i]->getPath());

           while(i+1<unmatched.size() && !unmatched[i+1]){
                firstChunk->setLength(chunkdetails[i+1]->getLength()+firstChunk->getLength());
                i++;
            }
            combinedChunks.push_back(*firstChunk);
        }
        else
            cout<<"unmatched!!!\n";
    }
    return combinedChunks;
}

vector< vector < BlockChecksum> > getBlockChecksum(vector<Chunk> unmatchedChunks, string filepath){
    cout<<"Breaking into blocks and calculating checksums\n";
    vector< vector < BlockChecksum> > blockChecksums;
    int blockSize=700;
    ifstream in_file(filepath.c_str(),std::ifstream::binary);

    for(int i=0; i<unmatchedChunks.size();i++){
        vector < BlockChecksum > chunkDivision;
        in_file.seekg(unmatchedChunks[i].getOffset(),in_file.beg);
        int k;
        char data[blockSize];
        for(k=0;k<unmatchedChunks[i].getLength();k+=blockSize){
            in_file.read(data,blockSize);
            unsigned int weaksum=rs_calc_weak_sum(&data,blockSize);
            BlockChecksum block(weaksum);
            rs_calc_strong_sum(&data,blockSize,&block.strongsum);

            chunkDivision.push_back(block);
        }
        in_file.read(data,unmatchedChunks[i].getLength()+blockSize-k);
        unsigned int weaksum=rs_calc_weak_sum(&data,blockSize);
        BlockChecksum block(weaksum);
        rs_calc_strong_sum(&data,blockSize,&block.strongsum);

        chunkDivision.push_back(block);

    blockChecksums.push_back(chunkDivision);
    }
    in_file.close();
    return blockChecksums;
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
          std::cerr << "Error " << std::endl;
    }

     delete myWrapper;
    return checksum;
}
