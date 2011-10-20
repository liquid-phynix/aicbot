#include<iostream>

using namespace std;

#ifndef _ARRAY_H
#define _ARRAY_H

template <class T> class array{
public:
  T** m;
  int r;
  int c;
  array(void);
  array(int, int);
  array(int, int, bool);
  array(int, int, T);
  ~array(void);
  void show();
  void operator=(array<T>&);
  // array<T> operator+(array<T>&);
  // array<T> operator-(array<T>&);
  // array<T> operator*(array<T>&);
};


template <class T> array<T>::array(void){
  r = 0;
  c = 0;
  m = NULL;
};

template <class T> array<T>::array(int rows, int cols, bool){
  r = rows;
  c = cols;
  m = new T*[rows];
  m[0] = new T[rows * cols];
  for(int row = 1; row < rows; row++){
    m[row] = m[0] + row * cols;
  }
};

template <class T> array<T>::array(int rows, int cols){
  r = rows;
  c = cols;
  m = new T*[rows];
  m[0] = new T[rows * cols];
  for(int row = 1; row < rows; row++){
    m[row] = m[0] + row * cols;
  }
  for(T* tp = m[0]; tp < m[0] + rows * cols; tp++){
    *tp = static_cast<T>(0);
  }
};

template <class T> array<T>::array(int rows, int cols, T def){
  r = rows;
  c = cols;
  m = new T*[rows];
  m[0] = new T[rows * cols];
  for(int row = 1; row < rows; row++){
    m[row] = m[0] + row * cols;
  }
  for(T* tp = m[0]; tp < m[0] + rows * cols; tp++){
    *tp = def;
  } 
};

template <class T> array<T>::~array(){
  delete[] (m[0]);
  delete[] m;
};

template <class T> void array<T>::show(){
  for(int row = 0; row < r; row++){
    for(int col = 0; col < c; col++){
      cout << " " << m[row][col];
    }
    cout << endl;
  }
  cout << endl;
};

template <class T> void array<T>::operator=(array<T>& rightMat){
  r = rightMat.r;
  c = rightMat.c;
  m = rightMat.m;
};

#endif
