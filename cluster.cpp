#include"base.hh"

using namespace std;

void _gameData::clusterAnts(void){
  cluster_info_list.clear();
  typedef pair<pos, bool> pbpair;
  if(received_enemy_ants_hmap.size()==0) return;
  /*
  auto sameGroup=[this](pbpair& p1, pbpair& p2){
    bool ret=false;
    int maxedist2=(p1.second==p2.second)?2:50;
    dij.findPath(p1.first, [this, &p1, &p2, &ret, maxedist2](loc* l){
      if((l->g)*(l->g)<=2*maxedist2){
        if(euclideanDist2(l->row, l->col, p1.first.row, p1.first.col, rows, cols)<=maxedist2){
          if(l->row==p2.first.row and l->col==p2.first.col){
            ret=true;
            return true;
          }  
        }
        return false;
      }else return true;
    });
    return ret;
  };
  */
 
  
  
  auto sameGroup=[this](pbpair& p1, pbpair& p2){
    bool ret=false;
    int maxg=(p1.second==p2.second)?3:7;
    dij.findPath(p1.first, [this, maxg, &p2, &ret](loc* l){
      if(l->g<=maxg){
        if(l->row==p2.first.row and l->col==p2.first.col){
            ret=true;
            return true;
        }
        return false;
      }else return true;
    });
    return ret;
  };
  
  
  
  vector<pbpair> availableAnts;
  for_each(myants_list.begin(), myants_list.end(), [&availableAnts](_ant* a){
    if(a->antPurpose==wanderer /*and (not a->strict)*/) availableAnts.push_back(pbpair(a->pCs, true));
  });
  for_each(received_enemy_ants_hmap.begin(), received_enemy_ants_hmap.end(), [&availableAnts](pair<pos, int> p){
    availableAnts.push_back(pbpair(p.first, false));
  });
  
  vector<bool> checkedRow(availableAnts.size(), false);
  _array<bool> connMat(availableAnts.size(), availableAnts.size(), true);
  for(int row=0; row<connMat.rows(); row++){
    for(int col=row+1; col<connMat.cols(); col++){
      connMat[row][col]=connMat[col][row]=sameGroup(availableAnts[row], availableAnts[col]);
    }
  }
  
  function<void(int, list<pbpair>*)> recur=[&connMat, &checkedRow, &recur, &availableAnts](int startRow, list<pbpair>* lp){
    for(int col=0; col<connMat.cols(); col++){
      if(connMat[startRow][col]){
        if(not checkedRow[col]){
          checkedRow[col]=true;
          lp->push_back(availableAnts[col]);
          recur(col, lp);
        }
      }
    }
  };
  
  list<list<pbpair>*> clusters;
  int row=0;
  while(row<connMat.rows()){
    if(not checkedRow[row]){
      clusters.push_back(new list<pbpair>);
      checkedRow[row]=true;
      clusters.back()->push_back(availableAnts[row]);
      recur(row, clusters.back());
    }
    row++;
  }
  
  clusters.remove_if([](list<pbpair>* l){
    bool onlyMine=true;
    for(auto it=l->begin(); it!=l->end(); it++){if(it->second==false){onlyMine=false; break;}}
    return onlyMine;
  });
  
  /*
  list<pos> myAnts;
  list<pos> enemyAnts;
  */
  
  for(auto clusterIt=clusters.begin(); clusterIt!=clusters.end(); clusterIt++){
    cluster_info_list.push_back(_clusterInfo());
    _clusterInfo& cref=cluster_info_list.back();
    for(auto listIt=(*clusterIt)->begin(); listIt!=(*clusterIt)->end(); listIt++){
      if(listIt->second){
        cref.my_ants.push_back(listIt->first);
        ant_ptr_array[listIt->first]->antPurpose=preattacker;
      }else{
        cref.enemy_ants.push_back(listIt->first);
      }
    }
  }
    
    /*
    int mineCount=0;
    int enemyCount=0;
    for(auto listIt=(*clusterIt)->begin(); listIt!=(*clusterIt)->end(); listIt++){
      if(listIt->second) mineCount++;
      else enemyCount++;
    }
    if(response(enemyCount)>mineCount){
      // enemy cluster not matched in force
      for(auto listIt=(*clusterIt)->begin(); listIt!=(*clusterIt)->end(); listIt++){
        if(listIt->second==false){
          sources_for_wanderers.push_back(listIt->first);
        }else{
          ant_ptr_array[listIt->first]->antPurpose=preattackercautious;
        }
      }
    }else{
      // force matched
      for(auto listIt=(*clusterIt)->begin(); listIt!=(*clusterIt)->end(); listIt++){
        if(listIt->second==false){
          //enemyAnts.push_back(listIt->first);
          matched_force_enemies.insert(listIt->first);
        }else{
          //myAnts.push_back(listIt->first);
          ant_ptr_array[listIt->first]->antPurpose=preattacker;
        }
      }
    }
    */

  /*
  for_each(cluster_info_list.begin(), cluster_info_list.end(), [](_clusterInfo& ci){
    LOG("cluster has " << ci.my_ants.size() << " mine and " << ci.enemy_ants.size() << " enemies");
  });
  */
  
  //****************************************************************************************
  /*
  if(myAnts.size()>0 and enemyAnts.size()>0){
    list<pos> nextTiles;
    for_each(myAnts.begin(), myAnts.end(), [&nextTiles, this](pos p){
      doOnNborsC(p, [&nextTiles](pos p){nextTiles.push_back(p);});
    });
    
    _array<int> costMatrix(myAnts.size(), nextTiles.size(), 12345);
    int row, col;
    list<pos>::iterator rowIt;
    list<pos>::iterator colIt;
    for(row=0, rowIt=myAnts.begin(); row<costMatrix.rows(); row++, rowIt++){
      for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
        if(euclideanDist2(*rowIt, *colIt, rows, cols)<=1){
          int minDist=12345;
          for_each(enemyAnts.begin(), enemyAnts.end(), [this, &minDist, colIt](pos p){
            int edist2=euclideanDist2(p, *colIt, rows, cols);
            if(edist2<minDist){minDist=edist2;}
          });
          costMatrix[row][col]=minDist;
        }
      }
    }
    hungarian_problem_t matching;
    hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
    hungarian_solve(&matching);
    for(row=0, rowIt=myAnts.begin(); row<costMatrix.rows(); row++, rowIt++){
      for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
        if(euclideanDist2(*rowIt, *colIt, rows, cols)<=1){
          if(matching.assignment[row][col]==1){
            _ant* aptr=ant_ptr_array[*rowIt]; if(aptr==NULL) continue;
            aptr->eCs=*colIt;
            aptr->hasTarget=true;
            aptr->antPurpose=attacker;
            ant_expected_array[*colIt]=aptr;
          }
        }
      }
    }
    hungarian_free(&matching);
  }
  */
  //****************************************************************************************
  
  //LOG("cluster has " << mineCount << " of mine and " << enemyCount << " of enemies");
  
  
  /*
   *  LOG("availableAnts.size=" << availableAnts.size());
   *  LOG("enemies.size=" << received_enemy_ants_hmap.size());
   *  
   *  cerr << connMat << "\n";
   *  LOG("there are " << clusters.size() << " clusters");
   */
  //cout << "v setLayer 0\n";
  
  
  //int clusterCount=0;
  if(vdebug){
    cout << "v setLayer 0\n";
    cout << "v setLineColor 50 50 50 0.8\n";
    for(auto it1=clusters.begin(); it1!=clusters.end(); it1++/*, clusterCount++*/){
      for(auto it2=(*it1)->begin(); it2!=(*it1)->end(); it2++){
        cout << "v rect "<< it2->first.row-0.5 << " " << it2->first.col-0.5 << " 1 1 true\n";
        auto it3=(*it1)->begin();
        for(it3++; it3!=(*it1)->end(); it3++){
          cout << "v line " << it2->first.row << " " << it2->first.col << " " << it3->first.row << " " << it3->first.col << "\n";
        }
      }
    }
    cout << "v setLineColor 255 0 0 1\n";
    for_each(matched_force_enemies.begin(), matched_force_enemies.end(), [](pos p){
      cout << "v rect "<< p.row-0.5 << " " << p.col-0.5 << " 1 1 false\n";
    });
    cout << "v setLayer 1\n";
  }

  for_each(clusters.begin(), clusters.end(), [](list<pbpair>* l){delete l;});
};

// float weight(int enemy, int own){
//   //return own;
//   if(enemy==-1){
//     return own;
//   }else{
//     return (1.0-exp(-0.2*enemy))*own;
// //     //return abs(enemy-own);
// //     if(enemy<10 and own>10){
// //       return enemy;
// //     }else return own;
//   }
// }

class compareClusterInfo{
public:
  compareClusterInfo(){};
  bool operator()(const _clusterInfo* lhs, const _clusterInfo* rhs)const{
    //return weight(lhs->mean_bfs_dist_from_enemy_hill, lhs->mean_bfs_dist)>weight(rhs->mean_bfs_dist_from_enemy_hill, rhs->mean_bfs_dist);
    return lhs->mean_bfs_dist>rhs->mean_bfs_dist;  
  }
};

class compareClusterInfoHill{
public:
  compareClusterInfoHill(){};
  bool operator()(const _clusterInfo* lhs, const _clusterInfo* rhs)const{
    return lhs->mean_bfs_dist_from_enemy_hill>rhs->mean_bfs_dist_from_enemy_hill;
    //return lhs->mean_bfs_dist>rhs->mean_bfs_dist;  
  }
};

void _gameData::computeClusterPrioritiesAndDispatch(void){
  /*
  for(auto it=cluster_info_list.begin(); it!=cluster_info_list.end(); it++){
    cerr << "cluster has ";
    for_each(it->my_ants.begin(), it->my_ants.end(), [](pos p){cerr << p << " ";});
    cerr << "friends and ";
    for_each(it->enemy_ants.begin(), it->enemy_ants.end(), [](pos p){cerr << p << " ";});
    cerr << "foes\n";
  }
  */
 
  
  priority_queue<_clusterInfo*, vector<_clusterInfo*>, compareClusterInfo> clusterPique;
  priority_queue<_clusterInfo*, vector<_clusterInfo*>, compareClusterInfoHill> clusterPiqueHill;
  
  for(auto it=cluster_info_list.begin(); it!=cluster_info_list.end(); it++){
    it->needed=(response(it->enemy_ants.size())-(it->my_ants.size()));
    if(it->needed<0) it->needed=0; it->neededOriginal=it->needed;
    int meanDistanceFromHill=0;
    int meanDistanceFromEnemyHill=0;
    for_each(it->enemy_ants.begin(), it->enemy_ants.end(), [this, &meanDistanceFromHill, &meanDistanceFromEnemyHill](pos p){
      meanDistanceFromHill+=own_hill_bfs_array[p];
      meanDistanceFromEnemyHill+=enemy_hill_bfs_array[p];
    });
    meanDistanceFromHill/=(it->enemy_ants.size());
    meanDistanceFromEnemyHill/=(it->enemy_ants.size());
    it->mean_bfs_dist=meanDistanceFromHill;
    it->mean_bfs_dist_from_enemy_hill=(meanDistanceFromEnemyHill<0?-1:meanDistanceFromEnemyHill);
    
    if(meanDistanceFromEnemyHill>0){
      clusterPiqueHill.push(&*it);
    }
    clusterPique.push(&*it);
  }
  LOG("#" << clusterPique.size() << " clusters in prique");
  /*
  while(not clusterPique.empty()){
    cerr << "cluster meanDistanceFromHill: " << clusterPique.top()->mean_bfs_dist << " needs " << clusterPique.top()->needed << " of mine\n";
    clusterPique.pop();
  }
  */
  
  int countWanderers=0;
  for_each(myants_list.begin(), myants_list.end(), [&countWanderers](_ant* aptr){if(aptr->antPurpose==wanderer) countWanderers++;});
  int originalCountWanderers=countWanderers;
  
  //if(clusterPique.top()->mean_bfs_dist>5 and received_enemy_ants_hmap.size()>0){return;}
  
  while(clusterPiqueHill.size()>0){
    _clusterInfo* currentCluster=clusterPiqueHill.top();
    if(currentCluster->needed==0){
      clusterPiqueHill.pop();
      continue;
    }
    pos targetPos=currentCluster->enemy_ants.front();
    dij.findPath(15, targetPos, [this](loc* l){
      //if(l->g>60) return true;
      _ant* aptr=ant_ptr_array[l->row][l->col];
      if(aptr==0){return false;}
      if(aptr->antPurpose!=wanderer){return false;}
      auto pathListPtr=dij.backtrackPath(l);
      pathListPtr->pop_front();
      pathListPtr->pop_back();
      pos nextPos;
      nextPos=pathListPtr->front();
      if(ant_expected_array[nextPos]){gonnaStay(aptr); delete pathListPtr; return true;}
      aptr->eCs=nextPos;
      ant_expected_array[nextPos]=aptr;
      aptr->antPurpose=dispatched;
      aptr->hasTarget=true;
      delete pathListPtr;
      return true;
    });
    currentCluster->needed--;
    if(currentCluster->needed==0){break;}
  }
  
  //while((not clusterPique.empty()) and countWanderers>ceil((float)originalCountWanderers/4.0)){
  while((not clusterPique.empty()) and countWanderers>0){
    _clusterInfo* currentCluster=clusterPique.top();
    //LOG("cluster needs " << currentCluster->needed);
    if(currentCluster->needed==0){// or currentCluster->mean_bfs_dist>20){
      clusterPique.pop();
      continue;
    }
    
     pos targetPos=(currentCluster->my_ants.size()==0)?currentCluster->enemy_ants.front():currentCluster->my_ants.front();
    //pos targetPos=currentCluster->enemy_ants.front();
    //LOG("targetPos " << targetPos);
    dij.findPath(100, targetPos, [this](loc* l){
      //if(l->g>60) return true;
      _ant* aptr=ant_ptr_array[l->row][l->col];
      if(aptr==0) return false;
      if(aptr->strict) return false;
      if(aptr->antPurpose!=wanderer){return false;}
      
      //LOG("ant found " << aptr);
      auto pathListPtr=dij.backtrackPath(l);
      pathListPtr->pop_front();
      pathListPtr->pop_back();
      pos nextPos;
      nextPos=pathListPtr->front();
      if(ant_expected_array[nextPos]){gonnaStay(aptr); delete pathListPtr; return true;}
      aptr->eCs=nextPos;
      ant_expected_array[nextPos]=aptr;
      aptr->antPurpose=dispatched;
      aptr->hasTarget=true;
      delete pathListPtr;
      return true;
    });
    currentCluster->needed--;
    countWanderers--;
  }
};