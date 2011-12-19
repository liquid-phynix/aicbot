#include<iostream>
#include"pos.hh"

pos::pos(int r, int c):
row(r), col(c){};
pos::pos():
row(0), col(0){};
bool pos::operator==(pos const p)const{
  return p.row==row and p.col==col;
};
bool pos::operator!=(pos const p)const{
  return p.row!=row or p.col!=col;
};
pos pos::operator-(const pos p) const{
  return pos(row-p.row, col-p.col);
};
pos pos::operator+(const pos p) const{
  return pos(row+p.row, col+p.col);
};
std::ostream& operator<<(std::ostream& o, const pos p){
  o << "(" << p.row << " " << p.col << ")";
  return o;
};

std::ostream& operator<<(std::ostream& o, const purpose p){
  switch(p){
    case gatherer: o << "gatherer"; break;
    case attacker: o << "attacker"; break;
    case defender: o << "defender"; break;
    case wanderer: o << "wanderer"; break;
    case prebattle: o << "prebattle"; break;
    case preattacker: o << "preattacker"; break;
    case preattackercautious: o << "preattackercautious"; break;
    case dispatched: o << "dispatched"; break;
    case introuble: o << "introuble"; break;
    //case gathererforever: o << "gathererforever"; break;
    default: o << ".";
  }
  return o;
}


_clusterInfo::_clusterInfo(void):
needed(0),neededOriginal(0),mean_bfs_dist(0),mean_bfs_dist_from_enemy_hill(0){};
