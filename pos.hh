#include<iostream>
#include<unordered_map>
#include<functional>
#include<list>

#ifndef _POS_H
#define _POS_H
struct pos{
  int row, col;
  pos(int, int);
  pos();
  bool operator==(const pos) const;
  bool operator!=(const pos) const;
  pos operator-(const pos) const;
  pos operator+(const pos) const;
  friend std::ostream& operator<<(std::ostream&, const pos);
};
#endif

#ifndef _QUAD_H
#define _QUAD_H
struct quad{
  int f1, f2, f3, f4;
  quad(int _f1, int _f2, int _f3, int _f4):f1(_f1), f2(_f2), f3(_f3), f4(_f4){};
  quad(){};
};
#endif

#ifndef _PURPOSE_H
#define _PURPOSE_H
enum purpose {prebattle, gatherer, preattacker, attacker, defender, wanderer, preattackercautious, dispatched, introuble};
std::ostream& operator<<(std::ostream&, const purpose);
#endif


#ifndef _CLUSTERINFO
#define _CLUSTERINFO
class _clusterInfo{
public:
  _clusterInfo(void);
  std::list<pos> my_ants;
  std::list<pos> enemy_ants;
  int needed;
  int neededOriginal;
  int mean_bfs_dist;
  int mean_bfs_dist_from_enemy_hill;
};
#endif