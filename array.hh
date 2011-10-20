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

//template <class T> array<T>::array(void);

// template class array<int>;
// template class array<double>;
// template class array<float>;
#endif
