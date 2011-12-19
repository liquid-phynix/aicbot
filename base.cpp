#include"base.hh"

using namespace std;

_gameData::_gameData(int row, int col, int vrad2, int arad2):
haveiseenenemyyet(false),
rows(row),
cols(col),
vr2(vrad2),
ar2(arad2),
hillDangerLevel(0),
own_ants_array(row, col, -1),
enemy_idle_array(row, col, -1),
enemy_array_for_defense(row, col, -1),
not_seen_bfs(row, col, -1),
image_array(row, col, 0),
target_array(row, col, MAX_TARGET_ARRAY),
target_array_to_target(row, col, -1),
enemy_array(row, col, quad(12345, 0, 0, 0)),
navigational_seen_array(row, col, 0),
ever_seen_array(row, col, -1),
seen_array(row, col, 0),
obj_array(row, col, -2),
enemy_hill_bfs_array(row, col, MAX_HILL_ARRAY),
food_bfs_array(row, col),
ant_visited_array(row, col, -1),
ant_expected_array(row, col, 0),
enemy_safety_mask(row, col, 0),
enemy_safety_mask_plus1(row, col, 0),
ant_ptr_array(row, col, 0),
influence_hill(row, col),
influence_hill_tmp(row, col),
own_hill_bfs_array(row, col),
dij(obj_array, enemy_safety_mask_plus1, not_seen_bfs),
//dij(obj_array, enemy_safety_mask, not_seen_bfs),
turn(1),
ant_count_water_mark(0){};

void _gameData::addEnemyMask(list<pos>& poses, pos rc){
  float sqrtfar2=sqrt(ar2);
  for(int row=-4; row<=4; row++){
    for(int col=-4; col<=4; col++){
      float oedist=sqrt(euclideanDist2(0,0,row,col,rows,cols));
      //if(sqrtfar2+1<oedist and oedist<=sqrtfar2+2){
        if(oedist<=sqrtfar2+1)
          poses.emplace_back(pos(wrap(rc.row+row, rows), wrap(rc.col+col,cols)));
    }
  }
};


void _gameData::addEnemyMaskPlus1(list<pos>& poses, pos rc){
  float sqrtfar2=sqrt(ar2);
  for(int row=-4; row<=4; row++){
    for(int col=-4; col<=4; col++){
      float oedist=sqrt(euclideanDist2(0,0,row,col,rows,cols));
      //if(sqrtfar2+1<oedist and oedist<=sqrtfar2+2){
        if(oedist<=sqrtfar2+2)
          poses.emplace_back(pos(wrap(rc.row+row, rows), wrap(rc.col+col,cols)));
    }
  }
};

void _gameData::addEnemyMaskPlus1(unordered_set<pos>& poses, pos rc){
  float sqrtfar2=sqrt(ar2);
  int maskcnt=0;
  for(int row=-4; row<=4; row++){
    for(int col=-4; col<=4; col++){
      float oedist=sqrt(euclideanDist2(0,0,row,col,rows,cols));
      //if(sqrtfar2+1<oedist and oedist<=sqrtfar2+2){
      if(oedist<=sqrtfar2+2){
        poses.insert(pos(wrap(rc.row+row, rows), wrap(rc.col+col,cols)));
        maskcnt++;
      }
    }
  }
  //LOG("added " << maskcnt << " mask tiles");
};

void _gameData::doOnEdgeOfFocus(pos p, function<void(pos)> fun){
  pos edgeTiles[]={
    pos(-2,0),pos(-2,-1),pos(-2,1),
    pos(2,0),pos(2,-1),pos(2,1),
    pos(-1,-2),pos(0,-2),pos(1,-2),
    pos(-1,2),pos(0,2),pos(1,2)
  };
  for(int i=0; i<12; i++){
    pos tmpPos=p+edgeTiles[i];
    tmpPos.row=wrap(tmpPos.row, rows);
    tmpPos.col=wrap(tmpPos.col, cols);
    if(obj_array[tmpPos]!=-1){
      fun(tmpPos);
    }
  }
}





// void _gameData::computeThreat(void){
//   unordered_set<pos> allTiles;
//   for(auto it=received_enemy_ants_hmap.begin(); it!=received_enemy_ants_hmap.end(); it++){
//     doOnNborsC(it->first, [this, &allTiles](pos p1){
//       allTiles.insert(p1);
//       doOnEdgeOfFocus(p1, [&allTiles](pos p2){
//         allTiles.insert(p2);
//       });
//     });
//   }
//   unordered_set<pos> edgeTiles;
//   for(auto it=allTiles.begin(); it!=allTiles.end(); it++){
//     bool addTileToEdge=false;
//     doOnNborsC(*it, [&addTileToEdge, &allTiles](pos p){
//       auto foundIt=allTiles.find(p);
//       if(foundIt==allTiles.end()) addTileToEdge=true;
//     });
//     if(addTileToEdge)
//       edgeTiles.insert(*it);
//   }
//   
//   cout << "v setFillColor 255 0 0 0.3\n";
//   for(auto it=edgeTiles.begin(); it!=edgeTiles.end(); it++){
  //     cout << "v tile " << (*it).row << " " << (*it).col << "\n";  /*
//   }
// }




// void _gameData::battleConditioner(void){
//   list<_ant*> notGatherers;
//   for_each(myants_list.begin(), myants_list.end(), [&notGatherers](_ant* aptr){if(aptr->antPurpose!=gatherer and (not aptr->strict)) notGatherers.push_back(aptr);});
//   unordered_set<_ant*> remainderMine;
//   unordered_set<pos> remainderEnemy;
//   
//   list<pos> newEnemyList;
//   for_each(received_enemy_ants_hmap.begin(), received_enemy_ants_hmap.end(), [&newEnemyList](pair<pos, int> p){
//     newEnemyList.push_back(p.first);
//   });
//   for_each(received_enemy_ants_hmap.begin(), received_enemy_ants_hmap.end(), [&newEnemyList](pair<pos, int> p){
//     newEnemyList.push_back(p.first);
//   });
//   
//   {
//     //_array<int> costMatrix(notGatherers.size(), received_enemy_ants_hmap.size(), 12345);
//     _array<int> costMatrix(notGatherers.size(), newEnemyList.size(), 12345);
//     int row, col;
//     auto rowIt=notGatherers.begin();
//     //auto colIt=received_enemy_ants_hmap.begin();
//     auto colIt=newEnemyList.begin();
//     for(row=0, rowIt=notGatherers.begin(); row<costMatrix.rows(); row++, rowIt++){
//       for(col=0, colIt=newEnemyList.begin(); col<costMatrix.cols(); col++, colIt++){
//       //for(col=0, colIt=received_enemy_ants_hmap.begin(); col<costMatrix.cols(); col++, colIt++){
//         int edist2=euclideanDist2((*rowIt)->pCs, /*colIt->first*/ *colIt, rows, cols);
//         costMatrix[row][col]=(edist2<50?edist2:12345);
//         //mDist((*rowIt)->pCs.row, (*rowIt)->pCs.col, colIt->first.row, colIt->first.col, rows, cols);
//       }
//     }
//     hungarian_problem_t matching;
//     hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
//     hungarian_solve(&matching);
// 
//     if(vdebug){cout << "v setLineColor 0 0 0 0.8\n";}
//     for(row=0, rowIt=notGatherers.begin(); row<costMatrix.rows(); row++, rowIt++){
//       for(col=0, colIt=newEnemyList.begin(); col<costMatrix.cols(); col++, colIt++){
//       //for(col=0, colIt=received_enemy_ants_hmap.begin(); col<costMatrix.cols(); col++, colIt++){
//         if(matching.assignment[row][col]==1 and costMatrix[row][col]<50){
//           (*rowIt)->antPurpose=preattacker;
//           if(vdebug){cout << "v line " << (*rowIt)->pCs.row << " " << (*rowIt)->pCs.col << " " << /*colIt->first.row*/ colIt->row << " " << /*colIt->first.col*/ colIt->col << "\n";}
//         }
//       }
//     }
//     for(row=0, rowIt=notGatherers.begin(); row<costMatrix.rows(); row++, rowIt++){
//       bool addTo=true;
//       for(col=0, colIt=newEnemyList.begin(); col<costMatrix.cols(); col++, colIt++){
//       //for(col=0, colIt=received_enemy_ants_hmap.begin(); col<costMatrix.cols(); col++, colIt++){
//         if(matching.assignment[row][col]==1 and costMatrix[row][col]<50){
//           addTo=false;
//           break;
//         }
//       }
//       if(addTo){remainderMine.insert(*rowIt);}
//     }
//     for(col=0, colIt=newEnemyList.begin(); col<costMatrix.cols(); col++, colIt++){
//     //for(col=0, colIt=received_enemy_ants_hmap.begin(); col<costMatrix.cols(); col++, colIt++){
//       bool addTo=true;
//       for(row=0, rowIt=notGatherers.begin(); row<costMatrix.rows(); row++, rowIt++){  
//         if(matching.assignment[row][col]==1 and costMatrix[row][col]<50){
//           addTo=false;
//           break;
//         }
//       }
//       if(addTo){remainderEnemy.insert(/*colIt->first*/ *colIt);}
//     }
//     hungarian_free(&matching);
//   }
//   
//   
// 
//   
//   if(remainderMine.size()>0 and remainderEnemy.size()>0){
//     _array<int> costMatrix(remainderMine.size(), remainderEnemy.size(), 12345);
//     int row, col;
//     auto rowIt=remainderMine.begin();
//     auto colIt=remainderEnemy.begin();
//     for(row=0, rowIt=remainderMine.begin(); row<costMatrix.rows(); row++, rowIt++){
//       for(col=0, colIt=remainderEnemy.begin(); col<costMatrix.cols(); col++, colIt++){
//         costMatrix[row][col]=euclideanDist2((*rowIt)->pCs, *colIt, rows, cols);
//       }
//     }
//     hungarian_problem_t matching;
//     hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
//     hungarian_solve(&matching);
//     
//     if(vdebug){cout << "v setLineColor 255 255 255 0.8\n";}
//     for(row=0, rowIt=remainderMine.begin(); row<costMatrix.rows(); row++, rowIt++){
//       for(col=0, colIt=remainderEnemy.begin(); col<costMatrix.cols(); col++, colIt++){
//         if(matching.assignment[row][col]==1){
//           (*rowIt)->antPurpose=prebattle;
//           (*rowIt)->targetPos=*colIt;
//           if(vdebug){cout << "v line " << (*rowIt)->pCs.row << " " << (*rowIt)->pCs.col << " " << colIt->row << " " << colIt->col << "\n";}
//         }
//       }
//     }
//     hungarian_free(&matching);
//   }
// };

/*
void _gameData::handlePreBattle(void){
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=prebattle){continue;}
    dij.findPath(200, (*antIt)->pCs, [this, antIt](loc* currentLoc){
      if(currentLoc->row==(*antIt)->targetPos.row and currentLoc->col==(*antIt)->targetPos.col){
        auto pathListPtr=dij.backtrackPath(currentLoc);
        pathListPtr->pop_back();
        pos nextPos=pathListPtr->back();
        if(!safeMove(nextPos)){return false;}
        (*antIt)->eCs=nextPos;
        (*antIt)->hasTarget=true;
        ant_expected_array[(*antIt)->eCs]=*antIt;
        delete pathListPtr;
        return true;
      }else return false;
    });
  )
};
*/





void _gameData::captureEnemyHills(void){
  if(enemy_hills_hmap.size()==0) return;
  LOG("in captureHill, with hills");
  for(auto antIt=myants_list.begin(); antIt!=myants_list.end(); antIt++){
    //if(enemy_hill_bfs_array[(*antIt)->pCs]<10) continue;
    //if((*antIt)->antPurpose!=wanderer) continue;
    dij.findPath((*antIt)->pCs, [this, antIt](loc* currentLoc){
      if(currentLoc->g>30) return true;
      if(enemy_hill_bfs_array[currentLoc->row][currentLoc->col]==0){
        auto pathListPtr=dij.backtrackPath(currentLoc);
        pathListPtr->pop_back();
        pos tmpPos=pathListPtr->back();
        if(safeMove(tmpPos)){
          //LOG("ant " << (*antIt)->pCs << " has hill as target");
          (*antIt)->eCs=tmpPos;
          (*antIt)->hasTarget=true;
          ant_expected_array[tmpPos]=*antIt;
          (*antIt)->antPurpose=attacker;
        }
        delete pathListPtr;
        return true;
      }
      else return false;
    });
  }
};



void _gameData::getMinValDirection(pos& rc, bool /*hill*/){
  float vals[5]={0,0,0,0,0};
//   float tmp=0;
  float valsPlus[4]={0,0,0,0};
  enum {north, south, east, west};
  
  auto addFn=[this, &rc](int r, int c){
    pos coords=rc+pos(r,c);
    if(!safeMove(coords))
      return 9999999.0f;
       //return (r>1 or r<-1 or c>1 or c< -1) ? 1.0f : 100000.0f;
    else
      return (float)ant_visited_array[coords];
    //+0.5f*this->influence_hill[wrap(rc.row+r, this->rows)][wrap(rc.col+c, this->cols)];
  };
  
  vals[north]+=addFn(-1, 0);
  vals[south]+=addFn(1, 0);
  vals[east]+=addFn(0, 1);
  vals[west]+=addFn(0, -1);
  
  int nbors[4][2]=
  {{wrap(rc.row-1, rows), rc.col},
  {wrap(rc.row+1, rows), rc.col},
  {rc.row, wrap(rc.col+1, cols)},
  {rc.row, wrap(rc.col-1, cols)}};
  
  int minIndex=0;
  float minVal=1234567;
  float minValPlus=100000;
  for(int index=0; index<4; index++){
    //cerr << vals[index] << "\t";
     if(minVal>=vals[index]){
      if(minVal>vals[index]){
        minVal=vals[index];
        minValPlus=valsPlus[index];
        minIndex=index;
      }
      else if(valsPlus[index]<minValPlus){
        minVal=vals[index];
        minValPlus=valsPlus[index];
        minIndex=index;
      }
  }
  }
  //cerr << "\n";
  if(minVal!=1234567){
    rc.row = nbors[minIndex][0];
    rc.col = nbors[minIndex][1];
  }
};

pos _gameData::chooseMaxNbor(_array<quad>& ar, pos p){
  pos o[]={pos(0,0), pos(1,0), pos(-1,0), pos(0,1), pos(0,-1)};
  int minIndex=0;
  int minVal=0;
  for(int i=1; i<5; i++){
    pos tmpPos=p+o[i];
    if(obj_array[tmpPos]==-1) continue;
    int tmp=ar[tmpPos].f1;
    if(tmp>minVal){
      minVal=tmp;
      minIndex=i;
    }
  }
  return p+o[minIndex];
};


void _gameData::doOnNbors(pos p, function<void(pos&)> fun){
  pos o[]={pos(1,0), pos(-1,0), pos(0,1), pos(0,-1)};
  for(int i=0; i<4; i++){
    pos tmpPos=p+o[i];
    if(obj_array[tmpPos]==-1) continue;
    tmpPos.row=wrap(tmpPos.row, rows);
    tmpPos.col=wrap(tmpPos.col, cols);
    fun(tmpPos);
  }
};

void _gameData::doOnNborsC(pos p, function<void(pos)> fun){
  pos o[]={pos(1,0), pos(-1,0), pos(0,1), pos(0,-1)};
  fun(p);
  for(int i=0; i<4; i++){
    pos tmpPos=p+o[i];
    if(obj_array[tmpPos]==-1) continue;
    tmpPos.row=wrap(tmpPos.row, rows);
    tmpPos.col=wrap(tmpPos.col, cols);
    fun(tmpPos);
  }
};

void _gameData::doOnNborsCN(pos p, function<void(pos, int)> fun){
  pos o[]={pos(1,0), pos(-1,0), pos(0,1), pos(0,-1)};
  fun(p, 0);
  for(int i=0; i<4; i++){
    pos tmpPos=p+o[i];
    if(obj_array[tmpPos]==-1) continue;
    tmpPos.row=wrap(tmpPos.row, rows);
    tmpPos.col=wrap(tmpPos.col, cols);
    fun(tmpPos, i+1);
  }
};

bool _gameData::safeMove(pos nextPos){
  return ant_expected_array[nextPos]==0
  and obj_array[nextPos]!=-1
  //and obj_array[nextPos]!=100
  and enemy_safety_mask[nextPos]==0;
};

void _gameData::gonnaStay(_ant* antPtr){
  _ant* antThatWantsHere=ant_expected_array[antPtr->pCs];
  if(antThatWantsHere and antThatWantsHere!=antPtr)
    gonnaStay(antThatWantsHere);
  ant_expected_array[antPtr->pCs]=antPtr;
  antPtr->eCs=antPtr->pCs;
};