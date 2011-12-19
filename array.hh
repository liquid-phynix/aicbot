#include<iostream>
#include"pos.hh"

#ifndef _ARRAY_H
#define _ARRAY_H
template <class T> class _array{
private:
  int _rows;
  int _cols;
public:
  T* m;
  _array(int r, int c):_rows(r), _cols(c){m = new T[_rows * _cols];}
  _array(int r, int c, T val):_rows(r), _cols(c){m = new T[_rows * _cols];reset(val);}
  ~_array(){if(m) delete[] m;}
  void reset(T val){for(int index = 0; index < _rows * _cols; index++) *(m + index) = val;}
  int wrapR(int row){return (row>=_rows) ? (row-_rows) : ((row<0) ? (row+_rows) : row);}
  int wrapC(int col){return (col>=_cols) ? (col-_cols) : ((col<0) ? (col+_cols) : col);}
  T* operator[](int row){return m + row * _cols;}
  T& operator[](pos p){return m[wrapR(p.row)*_cols+wrapC(p.col)];}
  friend std::ostream& operator<< (std::ostream& o, const _array<T>& ar){
    for(int row=0; row<ar._rows; row++){
      for(int col=0; col<ar._cols; col++){o << *(ar.m+row*ar._cols+col) << ' ';}
      o << '\n';
    }
    return o;
  }
  int rows(void){return _rows;}
  int cols(void){return _cols;}
};
#endif