#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<unordered_map>
#include<ctime>
#include<sys/time.h>


#include"base.hh"
#include"helpers.hh"

std::hash<unsigned long long> std::hash<pos>::ull_hasher;
std::ofstream logStream;
bool vdebug=false;

int elapsedTime(timeval& start, timeval& end){
  return (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
}

int main(int argc, char**){
  srand(time(NULL));
#ifdef VDEBUG
  if(argc>1) vdebug=true;
#endif
  
  timeval startTime, endTime;
  std::cout.sync_with_stdio(0);
  std::unordered_map<std::string, int> gameParams;
  std::istringstream istream;
  std::string oneLine, tmpString;
  
  while(std::getline(std::cin, oneLine)){
    int paramValue;
    istream.clear();
    istream.str(oneLine);
    istream >> tmpString >> paramValue;
    if(tmpString=="ready") break;
    gameParams[tmpString] = paramValue;
  }
  _gameData gameData(gameParams["rows"],
                     gameParams["cols"],
                     gameParams["viewradius2"],
                     gameParams["attackradius2"]);
  go();
  
  while(std::getline(std::cin, oneLine)){
    gettimeofday(&startTime, NULL);
    istream.clear();
    istream.str(oneLine);
    istream >> tmpString;
    LOG("turn: " << gameData.turn);
    if(tmpString=="end"){break;};
    gameData.perTurnReset();
    while(getline(std::cin, oneLine)){
      istream.clear();
      istream.str(oneLine);
      pos f1f2;
      int f3=-1;
      istream >> tmpString >> f1f2.row >> f1f2.col >> f3;
      if(tmpString=="go") break;
      if(tmpString=="a"){gameData.receiveAnt(f1f2, f3); continue;};
      if(tmpString=="w"){gameData.receiveWater(f1f2); continue;};
      if(tmpString=="f"){gameData.receiveFood(f1f2); continue;};
      if(tmpString=="h"){gameData.receiveHill(f1f2, f3); continue;};
    }
    
    gameData.processAnts();
    gameData.processFood();
    gameData.processHills();
    

    
    gameData.assocFoodWithAnt();
    
    if(gameData.ant_count_water_mark>15){
      gameData.captureEnemyHills();
    }
    
    
    gameData.clusterAnts();
    gameData.computeClusterPrioritiesAndDispatch();    
    
    
    
    gameData.computeThreat();
    
    gameData.reachEnemy();
    
    

    
    
    gameData.wanderAntsAroundCompositeMod();
    
    

    if(vdebug){
      gameData.debugDraw();
      gameData.toPPM();
    }
    
    gameData.keepHillInSightOverride();
    
    gameData.outputCoordinates();
    gettimeofday(&endTime, NULL);
    LOG("turn took " << elapsedTime(startTime, endTime) << "ms");
    go();
  }
  return 0;
}
