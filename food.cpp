#include"base.hh"

using namespace std;

void _gameData::assocFoodWithAnt(void){
  list<pair<pos, bool>> targets;
  FOREACH(enemy_hills_hmap, targets.push_back(pair<pos, bool>(it->first, false));)
  FOREACH(seen_food_hmap, targets.push_back(pair<pos, bool>(it->first, true));)

  FOREACH(targets, dij.findPathSafetyExclusion(it->first, [this, it](loc* currentLoc){
    _ant* aptr=ant_ptr_array[currentLoc->row][currentLoc->col];
    if(not (aptr and (aptr->antPurpose==wanderer or aptr->antPurpose==gatherer))){return false;}
    if(aptr->antPurpose==gatherer and currentLoc->g-1>=aptr->pathLengthToTarget){return false;}
    auto pathListPtr=dij.backtrackPathSafetyExclusion(currentLoc);
    //cerr << "path to food " << pathListPtr << endl; // from ant to food
    pathListPtr->pop_front();
    if(it->second){pathListPtr->pop_back();}
    pos nextPos;
    if(pathListPtr->size()==0){nextPos=aptr->pCs;}
    else{nextPos=pathListPtr->front();}
    _ant* oldaptr=ant_expected_array[nextPos];
    if(oldaptr){
      //ant_expected_array[oldaptr->eCs]=0;
      ant_expected_array[nextPos]=0;
      oldaptr->antPurpose=wanderer;
      oldaptr->hasTarget=true;
      oldaptr->eCs=oldaptr->pCs;
    }
    if(ant_expected_array[nextPos]){gonnaStay(aptr); delete pathListPtr; return true;}
//     cout << "v setFillColor " << rand()%256 << " " << rand()%256 << " " << rand()%256 << " 0.6\n";
//     for_each(pathListPtr->begin(), pathListPtr->end(), [](pos p){
//       cout << "v tile " << p.row << " " << p.col << endl;
//     });
    aptr->eCs=nextPos;
    ant_expected_array[nextPos]=aptr;
    aptr->pathLengthToTarget=pathListPtr->size();
    aptr->antPurpose=gatherer;
    aptr->targetPos=it->first;
    aptr->hasTarget=true;
   
    for_each(pathListPtr->begin(), pathListPtr->end(), [this](pos p){not_seen_bfs[p]+=2;});
//     for_each(pathListPtr->begin(), pathListPtr->end(), [this](pos p){
//       doOnNbors(p, [this](pos pp){
//         not_seen_bfs[pp]+=2;
//       });
//     });
    
    delete pathListPtr;
    return true;
  });
  )
};