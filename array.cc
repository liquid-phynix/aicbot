#include<iostream>

#include"array.hh"

using namespace std;

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
  

// template <class T> array<T> array<T>::operator+(array<T>& rightMat){
//   array<T>* result = new array<T>(dim);
//   for(int index = 0; index < dim * dim; index++){
//     *(result->m[0] + index) = *(m[0] + index) + *(rightMat.m[0] + index);
//   }
//   return *result;
// }

// template <class T> array<T> array<T>::operator-(array<T>& rightMat){
//   array<T>* result = new array<T>(dim);
//   for(int index = 0; index < dim * dim; index++){
//     *(result->m[0] + index) = *(m[0] + index) - *(rightMat.m[0] + index);
//   }
//   return *result;
// }

// template <class T> array<T> array<T>::operator*(array<T>& rightMat){
//   array<T>* result = new array<T>(dim, true);
//   T rowTimesCol;
//   for(int row = 0; row < dim; row++){
//     for(int col = 0; col < dim; col++){
//       rowTimesCol = static_cast<T>(0);
//       for(int index = 0; index < dim; index++){
//         rowTimesCol += (m[row][index] * rightMat.m[index][col]);
//       }
//       result->m[row][col] = rowTimesCol;
//     }
//   }
//   return *result;
// }
