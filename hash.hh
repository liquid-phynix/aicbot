#include"pos.hh"

#ifndef _POS_HASH_H
#define _POS_HASH_H
namespace std{
  template<> struct hash<pos>{
    static hash<unsigned long long> ull_hasher;
    size_t operator()(const pos& p)const{
      unsigned long long ull1=p.row, ull2=p.col;
      return (size_t) ull_hasher((ull1<<32)|ull2);
    }
  };
};
#endif