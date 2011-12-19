#include<iostream>
#include<functional>
#include<vector>
#include<unordered_set>
#include<list>
#include<cmath>

#include"helpers.hh"
#include"array.hh"
#include"queue.hh"
#include"pos.hh"
#include"loc.hh"

#ifndef _DIJ_H
#define _DIJ_H
class _dij{
private:
  _array<loc> loc_board;
  _array<loc> loc_board_full;
  _array<int>& obj_board;
  _array<int>& safety_mask;
  _array<int>& not_seen_bfs;
  _queue<loc> loc_queue;
  int usedValue;
  int maxG;
public:
  _dij(_array<int>&, _array<int>&, _array<int>&);
  void findPath(pos, std::function<bool (loc*)>);
  void findPath(int, pos, std::function<bool (loc*)>);
  void findPathFull(pos, std::function<bool (loc*)>);
  std::vector<std::vector<int>>* findObjectsInRange(pos, int);
  void fillUpToRange(pos, int, _array<int>&);
  void fillUpToRangeIfLess(pos, int, _array<int>&);
  void fillEnemySafetyMask(pos, int, _array<int>&);
  std::list<pos>* backtrackPath(loc*);
  std::list<pos>* backtrackPathSafetyExclusion(loc*);
  void setupLocBoard(void);
  void correctForWater(pos);
  void resetG(void);
  void multiSourceBfsFill(std::unordered_set<pos>&, std::function<bool(loc*)>);
  void multiSourceBfsFill(std::list<pos>&, std::function<bool(loc*)>);
  void scanRad2(pos, int, std::function<void(loc*)>);
  void multiSourceSeenArray(_array<int>&);
  void findPathSafetyExclusion(pos, std::function<bool (loc*)>);
  void multiSourceHillDefense(std::list<pos>&, _array<int>&, _array<int>&);
};
std::ostream& operator<<(std::ostream&, std::list<pos>*);
#endif
