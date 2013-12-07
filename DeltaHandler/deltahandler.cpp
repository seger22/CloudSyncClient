#include "deltahandler.h"


DeltaHandler::DeltaHandler()
{
}

void DeltaHandler::getDeltaBlock(const Bytef sourceBlock[], int block_size, Bytef** deltaBlock, unsigned long int *deltasize){
    //Bytef* deltaBlock;
    //long unsigned int deltasize;
    zd_compress1(NULL,0,sourceBlock,block_size,deltaBlock,deltasize);
    //zd_compress(NULL,0,sourceBlock,block_size,deltaBlock,deltasize);

}

void DeltaHandler::getSourceBlock(Bytef** sourceBlock, unsigned long int *sourcesize, Bytef deltaBlock[], int deltablock_size){
   // Bytef* sourceBlock;
   // long unsigned int sourcesize;
    unsigned long int deltasize=static_cast<unsigned long int>(deltablock_size);
    zd_uncompress1(NULL,0,sourceBlock,sourcesize,deltaBlock,deltasize);
}

size_t zd_alloc(zd_mem_buffer *m, size_t n){
  if(n>0){
    if((m->buffer= (Bytef *) malloc(n * sizeof(Byte))) == NULL){
      perror("zd_alloc: could not allocate");
      return 0;
    }

    m->pos= m->buffer;
    m->size= n;
    return n;
  }

  m->size = 0;
  return 0;
}

size_t zd_realloc(zd_mem_buffer *m, size_t n){

  Bytef *p;
  int tmp_pos= (m->pos - m->buffer);

  if(n >0){
    p= (Bytef *) realloc(m->buffer, (m->size+n));
    if(p == NULL){
      perror("zd_realloc: could not realloc");
      return 0;
    }
    /* reset newly allocated memory*/
    if (m->buffer != p)
    {
       m->pos = p + tmp_pos;
       m->buffer= p;
    }

    m->size += n;
    return n;
  }

  return 0;
}

size_t zd_free(zd_mem_buffer *m){

  int n=0;

  if(m->size >0){
    free(m->buffer);
    m->pos= NULL;
    n= m->size; m->size=0;
    /*m->buffer= NULL;*/
  }

  return n;
}
