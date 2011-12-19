#include<vector>
#include<cstdlib>

#ifndef _LOC_H
#define _LOC_H
struct _superCell;
struct loc{
  loc();
  int g, row, col, used;
  loc* n; loc* s; loc* e; loc* w;
  _superCell* parent;
};
#endif