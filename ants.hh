#include<unordered_set>
#include<unordered_map>
#include<list>
#include<vector>
#include<algorithm>
#include<cmath>

#include"pos.hh"
#include"hash.hh"
#include"loc.hh"
#include"array.hh"
#include"dij.hh"

#ifndef _ANT_H
#define _ANT_H
class _ant{
public:
  int vr2;
  _array<_ant*>& ant_expected_array;
  _array<int>& objar;
  pos pCs, eCs, targetPos;
  int lifeTime;
  bool hasTarget;
  int rows, cols;
  purpose antPurpose;
  bool strict;
  int pathLengthToTarget;
  std::list<pos> pathToTarget;
  _dij& dij;
  bool chosenOne;
  bool metTargetCondition;
  bool brave;
  int count_friend;
  int count_foe;
  
  _ant(pos, int, int, _dij&, bool, int, _array<int>&, _array<_ant*>&);
  bool recognizeSelf(std::unordered_set<pos>&);
  void discoverSurroundings(void);
};
#endif


