#include"base.hh"

using namespace std;

/*
void _gameData::wanderAntsAroundComposite(void){
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=wanderer){continue;}
    int friendCount=(*antIt)->count_friend;
//     dij.scanRad2((*antIt)->pCs, vr2, [this, &friendCount](loc* currentLoc){
//       if(obj_array[currentLoc->row][currentLoc->col]==0)
//         friendCount++;
//     });
    if(friendCount<3 or friendCount>8){
      pos tmpPos=(*antIt)->pCs;
      getMinValDirection(tmpPos, false);
      if(safeMove(tmpPos)){
        (*antIt)->eCs=tmpPos;
        (*antIt)->hasTarget=true;
        ant_expected_array[tmpPos]=*antIt;
      }else gonnaStay(*antIt);
    }else{
      dij.findPathSafetyExclusion((*antIt)->pCs, [this, antIt](loc* currentLoc){
        if(seen_array[currentLoc->row][currentLoc->col]==0){
          auto pathListPtr=dij.backtrackPathSafetyExclusion(currentLoc);
          pathListPtr->pop_back();
          pos nextPos=pathListPtr->back();
          if(!safeMove(nextPos)){delete pathListPtr; gonnaStay(*antIt); return true;}
          (*antIt)->eCs=nextPos;
          ant_expected_array[(*antIt)->eCs]=*antIt;
          (*antIt)->hasTarget=true;
          delete pathListPtr;
          return true;
        }else return false;
      });
    }
  )
}
*/

// ha nem teljesul a seen_array[currentLoc->row][currentLoc->col]==0 feltetel adott g-belul, akkor kene a matchinget hasznalni
void _gameData::wanderAntsAroundCompositeMod(void){
  unordered_set<_ant*> antsWandering;
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=wanderer){continue;}
    //if(false){
    //if((not_seen_bfs[(*antIt)->pCs]>30 or not_seen_bfs[(*antIt)->pCs]==-1) and (enemy_array[(*antIt)->pCs].f1>=25 or enemy_array[(*antIt)->pCs].f1==12345)){
    if(not_seen_bfs[(*antIt)->pCs]>30 or not_seen_bfs[(*antIt)->pCs]==-1){
      //or enemy_safety_mask[(*antIt)->pCs]>0){
      //bool foundZero=false;
      dij.findPathSafetyExclusion((*antIt)->pCs, [this, antIt/*, &foundZero*/](loc* currentLoc){
        if(navigational_seen_array[currentLoc->row][currentLoc->col]==0){
          auto pathListPtr=dij.backtrackPathSafetyExclusion(currentLoc);
//           cout << "v setFillColor " << rand()%256 << " " << rand()%256 << " " << rand()%256 << " 0.6\n";
//           for_each(pathListPtr->begin(), pathListPtr->end(), [](pos p){
//             cout << "v tile " << p.row << " " << p.col << endl;
//           });
          pathListPtr->pop_back();
          pos nextPos=pathListPtr->back();
          if(!safeMove(nextPos)){delete pathListPtr; gonnaStay(*antIt); return true;}
          (*antIt)->eCs=nextPos;
          (*antIt)->hasTarget=true;
          ant_expected_array[nextPos]=*antIt;
          delete pathListPtr;
          //foundZero=true;
          return true;
        }else return false;
      });
      //if(not foundZero){antsWandering.insert(*antIt);}
    }else antsWandering.insert(*antIt);
  )
  if(antsWandering.size()==0){return;}
  unordered_set<pos> nextTiles;
  FOREACH(antsWandering,
          doOnNborsC((*it)->pCs, [&nextTiles, this](pos p){if(enemy_safety_mask_plus1[p]==0) nextTiles.insert(p);});
  )
  
  _array<int> costMatrix(antsWandering.size(), nextTiles.size(), 12345);
  int row, col;
  unordered_set<_ant*>::iterator rowIt;
  unordered_set<pos>::iterator colIt;
  
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(enemy_safety_mask[*colIt]==0 and ant_expected_array[*colIt]==0){
          //costMatrix[row][col]=not_seen_bfs[*colIt];
          if(enemy_array[*colIt].f1<=25){
            costMatrix[row][col]=not_seen_bfs[*colIt]-enemy_array[*colIt].f2;
          }else{
            costMatrix[row][col]=not_seen_bfs[*colIt];
          }
        }
      }
    }
  }
  
  hungarian_problem_t matching;
  hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
  hungarian_solve(&matching);   
  
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(matching.assignment[row][col]==1){
          (*rowIt)->eCs=*colIt;
          (*rowIt)->hasTarget=true;
          ant_expected_array[*colIt]=*rowIt;
        }
      }
    }
  }
  hungarian_free(&matching);
}

/*
void _gameData::wanderAntsAroundMinVisited(void){
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=wanderer){continue;}
    pos tmpPos=(*antIt)->pCs;
    getMinValDirection(tmpPos, false);
    if(safeMove(tmpPos)){
      (*antIt)->eCs=tmpPos;
      (*antIt)->hasTarget=true;
      ant_visited_array[(*antIt)->pCs]+=5;
      ant_expected_array[tmpPos]=*antIt;
      doOnNbors((*antIt)->pCs,[this](pos p){
        ant_visited_array[p]+=1;
      });
    }else gonnaStay(*antIt);
  )
};
*/

/*
void _gameData::wanderAntsAroundSeenArray(void){
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=wanderer){continue;}
    int minVal=12345;
    pos minPos;
    dij.findPath((*antIt)->pCs, [this, &minVal, &minPos](loc* currentLoc){
      if(currentLoc->g>20) return true;
      if(ever_seen_array[currentLoc->row][currentLoc->col]<minVal){
        minVal=ever_seen_array[currentLoc->row][currentLoc->col];
        minPos.row=currentLoc->row;
        minPos.col=currentLoc->col;
      }
      return false;
    });
    dij.findPath((*antIt)->pCs, [this, antIt, minVal](loc* currentLoc){
      if(ever_seen_array[currentLoc->row][currentLoc->col]==minVal){
        auto pathListPtr=dij.backtrackPath(currentLoc);
        pathListPtr->pop_back();
        pos nextPos=pathListPtr->back();
        if(ant_expected_array[nextPos]){gonnaStay(*antIt); delete pathListPtr; return true;}
        (*antIt)->eCs=nextPos;
        (*antIt)->hasTarget=true;
        ant_expected_array[(*antIt)->eCs]=*antIt;

        int add=6;
        for_each(pathListPtr->begin(), pathListPtr->end(), [this, &add](pos p){
          //doOnNbors(p, [this, add](pos pp){ever_seen_array[pp]+=add;});
          ever_seen_array[p]+=add; add=((add>0)?add-1:0);
        });
        
//         int add=1;
//         for_each(pathListPtr->begin(), pathListPtr->end(), [this, &add](pos p){
//           //doOnNbors(p, [this, add](pos pp){ever_seen_array[pp]+=add;});
//           ever_seen_array[p]+=add; add=add<3?add+1:0;
//         });
        
        
//        for_each(pathListPtr->begin(), pathListPtr->end(), [this](pos p){ever_seen_array[p]+=1;});     
//         int add=5;
//         for_each(pathListPtr->begin(), pathListPtr->end(), [this, &add](pos p){
//           doOnNbors(p, [this, add](pos pp){ever_seen_array[pp]+=add;});
//           ever_seen_array[p]+=add; add=add>0?add-1:0;
//         });
        
        delete pathListPtr;
        return true;
      }else return false;
    });
  )
};
*/

/*
void _gameData::wanderAntsAroundSeenArrayMod(void){
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=wanderer){continue;}
    int edgeOfTheUniverse=not_seen_bfs[(*antIt)->pCs];
    dij.findPath((*antIt)->pCs, [this, antIt, edgeOfTheUniverse](loc* currentLoc){
      if(not_seen_bfs[currentLoc->row][currentLoc->col]<edgeOfTheUniverse){
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

/*
void _gameData::wanderGatherers(void){
  unordered_set<_ant*> antsWandering;
  unordered_set<pos> nextTiles;
  FOREACH_MYANT(
    if((*antIt)->antPurpose==gathererforever and (*antIt)->hasTarget==false){
      antsWandering.insert(*antIt);
      if(enemy_safety_mask_plus1[(*antIt)->pCs]==0)
        nextTiles.insert((*antIt)->pCs);
      doOnNbors((*antIt)->pCs, [&nextTiles, this](pos p){
        if(enemy_safety_mask_plus1[p]==0)
          nextTiles.insert(p);
      });
    }
  )
  
  _array<int> costMatrix(antsWandering.size(), nextTiles.size(), 12345);
  int row, col;
  unordered_set<_ant*>::iterator rowIt;
  unordered_set<pos>::iterator colIt;
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(enemy_safety_mask_plus1[*colIt]==0)
          costMatrix[row][col]=not_seen_bfs[*colIt];
      }
    }
  }
  hungarian_problem_t matching;
  hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
  hungarian_solve(&matching);
  //hungarian_print_assignment(&matching);      
  
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(matching.assignment[row][col]==1){
          (*rowIt)->eCs=*colIt;
          (*rowIt)->hasTarget=true;
        }
      }
    }
  }
  hungarian_free(&matching);
  //     int edgeOfTheUniverse=not_seen_bfs[(*antIt)->pCs];
  //     dij.findPath((*antIt)->pCs, [this, antIt, edgeOfTheUniverse](loc* currentLoc){
    //       if(not_seen_bfs[currentLoc->row][currentLoc->col]<edgeOfTheUniverse){
      //         auto pathListPtr=dij.backtrackPath(currentLoc);
  //         pathListPtr->pop_back();
  //         pos nextPos=pathListPtr->back();
  //         if(!safeMove(nextPos)){gonnaStay(*antIt); delete pathListPtr; return false;}
  //         (*antIt)->eCs=nextPos;
  //         (*antIt)->hasTarget=true;
  //         ant_expected_array[(*antIt)->eCs]=*antIt;
  //         delete pathListPtr;
  //         return true;
  //       }else return false;
  //     });
  };
  */
  
/*
void _gameData::wanderAntsAroundByUnseen(void){
  unordered_set<_ant*> antsWandering;
  unordered_set<pos> nextTiles;
  FOREACH_MYANT(
    if((*antIt)->antPurpose==wanderer){
      antsWandering.insert(*antIt);
      nextTiles.insert((*antIt)->pCs);
      doOnNbors((*antIt)->pCs, [&nextTiles](pos p){nextTiles.insert(p);});
    }
  )
  
  _array<int> costMatrix(antsWandering.size(), nextTiles.size(), 12345);
  int row, col;
  unordered_set<_ant*>::iterator rowIt;
  unordered_set<pos>::iterator colIt;
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(enemy_safety_mask[*colIt]==0)
          costMatrix[row][col]=not_seen_bfs[*colIt];
      }
    }
  }
  hungarian_problem_t matching;
  hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
  hungarian_solve(&matching);
  //hungarian_print_assignment(&matching);      
  
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(matching.assignment[row][col]==1){
          (*rowIt)->eCs=*colIt;
          (*rowIt)->hasTarget=true;
        }
      }
    }
  }
  hungarian_free(&matching);
}
*/

/*
void _gameData::wanderDefenders(void){
  //if(received_enemy_ants_hmap.size()<1){return;}
  unordered_set<_ant*> antsWandering;
  unordered_set<pos> nextTiles;
  FOREACH_MYANT(
    if((*antIt)->antPurpose!=gatherer or (*antIt)->antPurpose!=attacker){
      if(enemy_array_for_defense[(*antIt)->pCs]==-1){continue;}
      antsWandering.insert(*antIt);
      doOnNborsC((*antIt)->pCs, [&nextTiles, this](pos p){
        if(enemy_safety_mask[p]==0)
          nextTiles.insert(p);
      });
    }
  )
  
  _array<int> costMatrix(antsWandering.size(), nextTiles.size(), 12345);
  int row, col;
  unordered_set<_ant*>::iterator rowIt;
  unordered_set<pos>::iterator colIt;
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(enemy_array_for_defense[*colIt]!=-1){
          costMatrix[row][col]=enemy_array_for_defense[*colIt];
        }
      }
    }
  }
  hungarian_problem_t matching;
  hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
  hungarian_solve(&matching);
  //hungarian_print_assignment(&matching);      
  
  for(row=0, rowIt=antsWandering.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(matching.assignment[row][col]==1){
          (*rowIt)->eCs=*colIt;
          (*rowIt)->hasTarget=true;
          (*rowIt)->antPurpose=defender;
        }
      }
    }
  }
  hungarian_free(&matching);
};
*/

// double _gameData::potentialAtPosition(pos& rc, list<_ant*>::iterator posAntIt){
  //   double res=0;
//   for(auto antIt=myants_list.begin(), endIt=myants_list.end(); antIt!=endIt; antIt++){
  //     if(antIt==posAntIt) continue;
//     res+=50.0/(1000.0+euclideanDist2(rc, (*antIt)->pCs, rows, cols));
//   }
//   return res;
// }
// 
// void _gameData::wanderAntsAroundPotential(void){
  //   pos o[]={pos(0,0), pos(1,0), pos(-1,0), pos(0,1), pos(0,-1)};
//   for(auto antIt=myants_list.begin(), endIt=myants_list.end(); antIt!=endIt; antIt++){
  //     if((*antIt)->hasTarget) continue;
//     
//     vector<pos> nextMoves;
//     vector<double> potValues;
//     pos tmpPos;
//     for(int i=0; i<5; i++){
  //       tmpPos=(*antIt)->pCs+o[i];
//       if(safeMove(tmpPos)){
  //         nextMoves.push_back(tmpPos);
//         potValues.push_back(potentialAtPosition(tmpPos, antIt));
//       }
//     }
//     pos minMove;
//     double minPot=1234567;
//     for(int i=0; i<nextMoves.size(); i++){
  //       if(potValues[i]<minPot){
    //         minPot=potValues[i];
//         minMove=nextMoves[i];
//       }
//     }
//     
//     if(safeMove(minMove)){
  //       (*antIt)->eCs=minMove;
//       ant_expected_array[minMove]=*antIt;
//       (*antIt)->hasTarget=true;
//     }else gonnaStay(*antIt);
//   }
// };


void _gameData::keepHillInSightOverride(void){
  if(myants_list.size()<3){return;}
  for(auto it=own_hills_hmap.begin(); it!=own_hills_hmap.end(); it++){
    _ant* closestOne=0;
    dij.findPathSafetyExclusion(it->first, [this, &closestOne](loc* currentLoc){
      if((currentLoc->g*currentLoc->g)<=2*vr2){
        _ant* antPtr=ant_expected_array[currentLoc->row][currentLoc->col];
        if(antPtr){
          if((currentLoc->g*currentLoc->g)>vr2/2){
            closestOne=antPtr;
            ant_expected_array[currentLoc->row][currentLoc->col]=0;
            closestOne->eCs=closestOne->pCs;
            closestOne->hasTarget=false;
            return true;
          }else return true;
        }else return false;
      }else return true;
    });
    if(closestOne){
      //LOG("ant " << closestOne->pCs << " overriden for hill " << it->first);
      dij.findPathSafetyExclusion(closestOne->pCs, [this, it, closestOne](loc* currentLoc){
        if(currentLoc->row==it->first.row and currentLoc->col==it->first.col){
          //LOG("path found");
          auto pathListPtr=dij.backtrackPathSafetyExclusion(currentLoc);
//           cout << "v setFillColor " << rand()%256 << " " << rand()%256 << " " << rand()%256 << " 0.6\n";
//           for_each(pathListPtr->begin(), pathListPtr->end(), [](pos p){
//             cout << "v tile " << p.row << " " << p.col << endl;
//           });
          pathListPtr->pop_front();
          pathListPtr->pop_back();
          pos nextPos;
          nextPos=pathListPtr->back();
          if(ant_expected_array[nextPos]){gonnaStay(closestOne); delete pathListPtr; return true;}
          closestOne->eCs=nextPos;
          ant_expected_array[nextPos]=closestOne;
          closestOne->antPurpose=defender;
          closestOne->hasTarget=true;    
          delete pathListPtr;
          return true;
        }else return false;
      });
    }
  }
};