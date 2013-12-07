#ifndef BLOCKCHECKSUM_H
#define BLOCKCHECKSUM_H

#include "librsync.h"

class BlockCheckSum
{
private :
    unsigned int offset;
    unsigned int length;
  unsigned int weeksum;

public:
  rs_strong_sum_t strongsum;
  BlockCheckSum(unsigned int wsum,unsigned int o, unsigned l){
      weeksum=wsum;
      offset=o;
      length=l;
     // strongsum = stsum;
  }

  unsigned int getWeeksum(){
      return weeksum;
  }
  unsigned int getOffset(){
      return offset;
  }
  unsigned int getLength(){
      return length;
  }
};

#endif // BLOCKCHECKSUM_H
