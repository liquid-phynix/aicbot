#include"ants.hh"
#include"helpers.hh"


_ant::_ant(pos p, int _rows, int _cols, _dij& _d, bool seenEnemy, int _vr2, _array<int>& _objar, _array<_ant*>& _aptr):
vr2(_vr2),
ant_expected_array(_aptr),
objar(_objar),
pCs(p),
eCs(p),
targetPos(),
lifeTime(0),
hasTarget(false),
rows(_rows),
cols(_cols),
antPurpose(wanderer),
//strict(false),
//strict(true),
strict(seenEnemy?(rand()%7==0):true),
//strict(rand()%5==0),
//strict(not seenEnemy),
pathLengthToTarget(-1),
dij(_d),
chosenOne(false),
metTargetCondition(false),
brave(false),
count_friend(0),
count_foe(0)
{};

bool _ant::recognizeSelf(std::unordered_set<pos>& recAnts){
  auto it=recAnts.find(eCs);
  if(it!=recAnts.end()){
    //LOG("recognized, purpose: " << antPurpose);
    antPurpose=wanderer;
    //LOG("recognized, purpose changed to: " << antPurpose);
    //if(lifeTime>40) antPurpose=defender;
    //antPurpose=wanderer;
    brave=false;
    targetPos.row=0; targetPos.col=0;
    pathLengthToTarget=-1;
    hasTarget=false;
    lifeTime++;
    pCs=eCs;
    recAnts.erase(it);
    return true;
  }
  return false;
};

void _ant::discoverSurroundings(void){
  count_friend=0; count_foe=0;
  dij.findPath(pCs, [this](loc* l){
    if((l->g)*(l->g)<=2*vr2){
      if(euclideanDist2(l->row, l->col, pCs.row, pCs.col, rows, cols)<=vr2){
        int objarval=objar[l->row][l->col];
        if(objarval==0){
          count_friend++;
        }
        else if(objarval>0)
          count_foe++;
      }
      return false;
    }else return true;
  });
  count_friend--;
  
#ifdef VDEBUG
    std::cout << "i " << pCs.row << " " << pCs.col << " friend:" << count_friend << "/foe:" << count_foe << "\n";
#endif
  
  if((count_foe>0 and count_friend==0) or (count_foe>2 and count_friend<2)){
    //LOG("ant " << pCs << " in trouble");
    dij.findPathSafetyExclusion(pCs, [this](loc* l){
      if(objar[l->row][l->col]==0){
        auto pathListPtr=dij.backtrackPathSafetyExclusion(l);
        //LOG("path length: " << pathListPtr->size());
        pathListPtr->pop_back();
        if(pathListPtr->size()==0){delete pathListPtr; return true;}
        pos nextPos=pathListPtr->back();
        if(ant_expected_array[nextPos]){delete pathListPtr; return true;}
        eCs=nextPos;
        ant_expected_array[eCs]=this;
        antPurpose=introuble;
        hasTarget=true;
        delete pathListPtr;
        return true;
      }else return false;
    });
  }
  
  
  
//   if(count_foe==0){
//     strict=true;
//   }else if(count_foe<3 and count_friend<2){
//     strict=true;
//   }
  
//   else{
//     strict=false;
//   }
 
}
