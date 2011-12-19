#include<iostream>
#include<cstdlib>
#include<fstream>
#include<list>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<cmath>
#include<functional>
#include<queue>

#include"array.hh"
#include"ants.hh"
#include"helpers.hh"
#include"dij.hh"
#include"pos.hh"
#include"loc.hh"
#include"hungarian.hh"

#ifndef _GAME_DATA_H
#define _GAME_DATA_H
class _gameData{
private:
  bool haveiseenenemyyet;
  uint rows, cols;
  uint vr2, ar2, hillDangerLevel;
  
  //_array<int> water_bfs_array;
  _array<int> own_ants_array;
  _array<int> enemy_idle_array;
  _array<int> enemy_array_for_defense;
  _array<int> not_seen_bfs;
  _array<int> image_array;
  _array<int> target_array;
  _array<int> target_array_to_target;
  _array<quad> enemy_array;
  _array<int> navigational_seen_array; //fontos
  _array<int> ever_seen_array; //fontos
  _array<int> seen_array; //fontos
  _array<int> obj_array;
  _array<int> enemy_hill_bfs_array;
  _array<int> food_bfs_array;
  _array<int> ant_visited_array;
  _array<_ant*> ant_expected_array;
  _array<int> enemy_safety_mask;
  _array<int> enemy_safety_mask_plus1;
  _array<_ant*> ant_ptr_array;
  _array<float> influence_hill;
  _array<float> influence_hill_tmp;
  
  _array<int> own_hill_bfs_array;
  
  _dij dij; //needs obj_array initialized
  
  std::unordered_set<pos> matched_force_enemies;
  std::list<pos> sources_for_wanderers;
  std::list<pos> proper_edge_tiles;
  std::unordered_map<pos, std::list<std::pair<int, int>>> enemy_edge_tiles;
  
  std::unordered_set<pos> patternPoints;
  
  std::unordered_set<pos> myants_received_set;
  std::unordered_set<pos> received_food_set;
  std::unordered_map<pos, int> received_hills_hmap;
  std::unordered_map<pos, int> received_enemy_ants_hmap;
  
  std::unordered_map<pos, std::vector<pos>> enemy_coverage;
  
  std::unordered_set<_ant*> antsWoPurpose;
  
  std::list<_ant*> myants_list;
  std::unordered_set<pos> water_set;
  std::unordered_map<pos, int> own_hills_hmap;
  std::unordered_map<pos, int> enemy_hills_hmap;
  std::unordered_map<pos, _ant*> seen_food_hmap;

  std::list<_clusterInfo> cluster_info_list;
  
public:
  uint turn;
  uint ant_count_water_mark;
  
  _gameData(int, int, int,int);
  
  void receiveAnt(pos, int);
  void receiveHill(pos, int);
  void receiveWater(pos);
  void receiveFood(pos);
  
  void processAnts(void);
  //void processEnemyAnts(void);
  void processFood(void);
  void processHills(void);
  
  void fillHillArray(void);
  //void computeCoordinates(void);
  void outputCoordinates(void);
  void perTurnReset(void);
  void assocFoodWithAnt(void);
  //void wanderAntsAround(void);
  //void wanderAntsAroundPotential(void);
  void wanderAntsAroundSeenArray(void);
  void wanderAntsAroundMinVisited(void);
  void wanderAntsAroundComposite(void);
  void wanderAntsAroundByUnseen(void);
  void wanderAntsAroundSeenArrayMod(void);
  void wanderAntsAroundCompositeMod(void);
  
  void captureEnemyHills(void);
  //void doBattleM(void);
  //void doBattleS(void);
  //void doBattleK(void);
  //void defendHills(void);
  //void diffuseInfluence(void);
  void getMinValDirection(pos&, bool);
  void toPPM(void);
  //void addPosToExpected(pos);
  //bool isExpectedPos(pos);
  bool safeMove(pos);
  void gonnaStay(_ant*);
  //void updateEnemyCoverage(void);
  //bool scoreFn(pos, std::vector<pos>&);
  //double potentialAtPosition(pos&, std::list<_ant*>::iterator);
  void clearHill(void);
  void navigateFormations(void);

  void formationBattle(std::unordered_set<_ant*>&, std::unordered_map<_ant*, pos>&);
  bool evaluate(_array<int>&, std::vector<int>*, std::vector<int>*, int*, int*, int, int&, int&);
  pos chooseMaxNbor(_array<quad>&, pos);
  void doOnNbors(pos, std::function<void(pos&)>);
  void doOnNborsC(pos, std::function<void(pos)>);
  void doOnNborsCN(pos, std::function<void(pos, int)>);
  //void fillPattern(void);
  void reachEnemy(void);
  void followTrail(pos, std::unordered_set<_ant*>&, std::unordered_set<pos>&);
  void addEnemyMaskPlus1(std::unordered_set<pos>&, pos);
  void addEnemyMaskPlus1(std::list<pos>&, pos);
  void addEnemyMask(std::list<pos>&, pos);
  void updateSeen(void);
  void wanderGatherers(void);
  void wanderDefenders(void);
  void debugDraw(void);
  void computeThreat(void);
  void doOnEdgeOfFocus(pos, std::function<void(pos)>);
  
  void keepHillInSightOverride(void);
  void battleConditioner(void);
  void handlePreBattle(void);
  void clusterAnts(void);
  void computeClusterPrioritiesAndDispatch(void);
};
#endif

#define FOREACH_MYANT(BODY) for(auto antIt=myants_list.begin(); antIt!=myants_list.end(); antIt++){BODY}
#define FOREACH_MYANT_NI(BODY) for(auto antIt=myants_list.begin(); antIt!=myants_list.end();){BODY}

#define FOREACH(CONTAINER, BODY) for(auto it=CONTAINER.begin(); it!=CONTAINER.end(); it++){BODY}
#define FOREACH_NI(CONTAINER, BODY) for(auto it=CONTAINER.begin(); it!=CONTAINER.end();){BODY}


#define MAX_TARGET_ARRAY 10
#define MAX_HILL_ARRAY 20
//#define MAX_ADDITTIVE_ARRAY 10

extern bool vdebug;
