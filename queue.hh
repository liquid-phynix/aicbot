#include<iostream>

#ifndef _QUEUE_H
#define _QUEUE_H
template <class T> class _queue{
private:
  int length;
  int readFromPos;
  int writeToPos;
  T** vec;
public:
  int size;
  _queue(int _length=1024):length(_length), readFromPos(0), writeToPos(0), size(0){vec = new T*[length];}
  ~_queue(void){delete[] vec;}
  void reset(void){readFromPos=0; writeToPos=0; size=0;}
  T* pop(void){
    T* ret = vec[readFromPos];
    if(readFromPos == length - 1) readFromPos = 0;
    else readFromPos++;
    size--;
    if(size < 0){std::cerr << "queue size lower than 0, exiting" << std::endl;  exit(-1);}
    return ret;
  }
  void push(T* element){
    vec[writeToPos] = element;
    if(writeToPos == length - 1) writeToPos = 0;
    else writeToPos++;
    size++;
    if(size > length){std::cerr << "queue size greater than " << length << ", exiting" << std::endl; exit(-1);}    
  }
};
#endif
