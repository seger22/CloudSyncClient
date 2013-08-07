#include "filechunkreader.h"

filechunkreader::filechunkreader(string path)
{
    input_file.open(path.c_str(),std::ifstream::binary);
}

chunkdat filechunkreader::get_chunk(Chunk *chunk)
{
    chunkdat ch_data;
    input_file.seekg(chunk->getOffset(),input_file.beg);
    input_file.read(ch_data.data,chunk->getLength());
    ch_data.chunk_size=chunk->getLength();
    return ch_data;
}

void filechunkreader::closefile()
{
    input_file.close();
}
