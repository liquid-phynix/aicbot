//#define DEBUG

//#define VDEBUG

#include<iostream>
#include<fstream>
#include<sstream>
#include<cmath>
#include<list>
#include<algorithm>
#include<functional>

#include"pos.hh"
#include"array.hh"
#include"loc.hh"

#ifdef DEBUG
#define LOG(ARG) std::cerr << ">>> " << ARG << std::endl
#else
#define LOG(ARG)
#endif

void floatToRGB(float&, float&, float&, float, float, float, float, float);
void writePPM(const char*, int, int, int, _array<int>&, std::function<float(int, int, bool&)>);
int wrap(int, int);
int euclideanDist2(pos, pos, int, int);
int euclideanDist2(int, int, int, int, int, int);
void go(void);
void maskedAccess(_array<int>&, pos, int, std::function<void (int&)>);
int mDist(int, int, int, int, int, int);
char positionToDirection(pos, pos);
int properDist(int, int, int);
int cumulativeDiff(std::vector<int>*, std::vector<int>*, int*, int, int, int);

void arMask(int, std::function<void(pos)>);
int response(int);