#ifndef BLOCKCHECKSUMSERIAL_H
#define BLOCKCHECKSUMSERIAL_H
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "librsync.h"
class BlockChecksumSerial{
public:
      rs_strong_sum_t strongsum;

private :
  friend class boost::serialization::access;
  unsigned int weaksum;
      template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
          ar & weaksum;
          ar & strongsum;
      }
public:
      BlockChecksumSerial(){}
      BlockChecksumSerial(unsigned int wsum, rs_strong_sum_t strsum):weaksum(wsum)
  {
      memcpy(strongsum,strsum,16);
  }

  unsigned int getWeeksum(){
      return weaksum;
  }

};
#endif // BLOCKCHECKSUMSERIAL_H
