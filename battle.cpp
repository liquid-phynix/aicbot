#include"base.hh"

using namespace std;

void _gameData::computeThreat(void){
  unordered_map<pos, list<pair<int, int>>> allTiles;
  int enemyId=0;
  
  /*
  for(auto it1=cluster_info_list.begin(); it1!=cluster_info_list.end(); it1++){
    if(it1->my_ants.size()>0){
      for(auto it2=it1->enemy_ants.begin(); it2!=it1->enemy_ants.end(); it2++){
        enemyId++;
        doOnNborsCN(*it2, [this, &allTiles, enemyId](pos p1, int which){   
          arMask(ar2, [this, &allTiles, enemyId, which, &p1](pos p2){
            pos tmpPos=p1+p2;
            tmpPos.row=wrap(tmpPos.row, rows);
            tmpPos.col=wrap(tmpPos.col, cols);
            auto foundIt=allTiles.find(tmpPos);
            if(foundIt!=allTiles.end()){
              foundIt->second.push_back(pair<int, int>(enemyId, which));
            }else{
              allTiles.insert(pair<pos, list<pair<int, int>>>(tmpPos, list<pair<int, int>>{pair<int, int>(enemyId, which)}));
            }
          });
        });        
      }
    }
  }
  */
  
  
  
  
  for(auto it=received_enemy_ants_hmap.begin(); it!=received_enemy_ants_hmap.end(); it++, enemyId++){
    doOnNborsCN(it->first, [this, &allTiles, enemyId](pos p1, int which){   
      arMask(ar2, [this, &allTiles, enemyId, which, &p1](pos p2){
        auto foundIt=allTiles.find(p1+p2);
        if(foundIt!=allTiles.end()){
          foundIt->second.push_back(pair<int, int>(enemyId, which));
        }else{
          allTiles.insert(pair<pos, list<pair<int, int>>>(p1+p2, list<pair<int, int>>{pair<int, int>(enemyId, which)}));
        }
      });
    });
  }
  
  
  
  
  enemy_edge_tiles.clear();
  for(auto it=allTiles.begin(); it!=allTiles.end(); it++){
    bool addTileToEdge=false;
    doOnNborsC(it->first, [&addTileToEdge, &allTiles](pos p){
      auto foundIt=allTiles.find(p);
      if(foundIt==allTiles.end()) addTileToEdge=true;
    });
    if(addTileToEdge)
      enemy_edge_tiles[it->first]=it->second;
  }
  
  
//   if(vdebug){
//     cout << "v setFillColor 0 0 255 1\n";
//     for(auto it=enemy_edge_tiles.begin(); it!=enemy_edge_tiles.end(); it++){
//       //if(it->second.size()<2)
//         cout << "v circle " << it->first.row << " " << it->first.col << " 0.25 true\n";
//     }
//   }
  ////////////////////////////////////////////////////////////////////////////////////////////
  unordered_map<pos, list<pair<int, int>>> new_enemy_edge_tiles;
  //for(auto it1=received_enemy_ants_hmap.begin(); it1!=received_enemy_ants_hmap.end(); it1++){
  for(auto it1=cluster_info_list.begin(); it1!=cluster_info_list.end(); it1++){
    if(it1->my_ants.size()==0){continue;}
    for(auto it2=it1->enemy_ants.begin(); it2!=it1->enemy_ants.end(); it2++){
      dij.findPath(*it2, [this, it2, &new_enemy_edge_tiles](loc* currentLoc){
        if(currentLoc->g<=2*(1+sqrt(ar2))){
          if(euclideanDist2(it2->row, it2->col, currentLoc->row, currentLoc->col, rows, cols)<=(1+sqrt(ar2))*(1+sqrt(ar2))){
            auto foundIt=enemy_edge_tiles.find(pos(currentLoc->row, currentLoc->col));
            if(foundIt!=enemy_edge_tiles.end()){
              new_enemy_edge_tiles.insert(*foundIt);
            }
          }
          return false;
        }else return true;
      });
    }
  }
  enemy_edge_tiles.swap(new_enemy_edge_tiles);
  
//   if(vdebug){
//     cout << "v setFillColor 0 0 255 1\n";
//     for(auto it=enemy_edge_tiles.begin(); it!=enemy_edge_tiles.end(); it++){
//       if(it->second.size()<2)
//       cout << "v circle " << it->first.row << " " << it->first.col << " 0.25 true\n";
//     }
//   }
  ////////////////////////////////////////////////////////////////////////////////////////////
  
  unordered_map<pos, list<pos>> invertedConnection; // pos.row==enemyId, pos.col==whichSide
  for(auto it=enemy_edge_tiles.begin(); it!=enemy_edge_tiles.end(); it++){
    if(it->second.size()!=1){continue;}
    pos forInserting(it->second.front().first, it->second.front().second);
    auto foundIt=invertedConnection.find(forInserting);
    if(foundIt!=invertedConnection.end()){
      foundIt->second.push_back(it->first);
    }else{
      invertedConnection[forInserting]=list<pos>{it->first};
    }
  }
  
  /*
  for(auto it=invertedConnection.begin(); it!=invertedConnection.end(); it++){
    cerr << "id: " << it->first.row << ", dir: " << it->first.col << ", " << it->second.size() << " count ";
    for_each(it->second.begin(), it->second.end(), [](pos p){cerr << p << " ";}); cerr << endl;
  }
  */
  
  proper_edge_tiles.clear();
  for(auto hmapIt=invertedConnection.begin(); hmapIt!=invertedConnection.end(); hmapIt++){
    int tileCount=hmapIt->second.size();
    if(tileCount<2) continue;
    int tileCountHasAntNbor=0;
    //int antCountSitsOnTop=0;
    /*
    for(auto listIt=hmapIt->second.begin(); listIt!=hmapIt->second.end(); listIt++){
      if(obj_array[*listIt]==0){tileCount++;}
      doOnNbors(*listIt, [this, &tileCountHasAntNbor](pos p){if(obj_array[p]==0) tileCountHasAntNbor++;});
    }
    */
    
    for(auto listIt=hmapIt->second.begin(); listIt!=hmapIt->second.end(); listIt++){
      if(obj_array[*listIt]==0){
        tileCountHasAntNbor++;
        //if(ant_ptr_array[*listIt]->antPurpose==preattacker) antCountSitsOnTop++;
      }else{
        bool hasAntNbor=false;
        doOnNbors(*listIt, [this, &hasAntNbor](pos p){
          if(enemy_edge_tiles.find(p)==enemy_edge_tiles.end() and obj_array[p]==0)
            hasAntNbor=true;
        });
        if(hasAntNbor){tileCountHasAntNbor++;}
      }
    }
    //if(tileCount==2){
      //if(tileCountHasAntNbor==2) proper_edge_tiles.insert(proper_edge_tiles.end(), hmapIt->second.begin(), hmapIt->second.end());
      //}else{
     if(tileCountHasAntNbor>=2)
       proper_edge_tiles.insert(proper_edge_tiles.end(), hmapIt->second.begin(), hmapIt->second.end());
     //if(antCountSitsOnTop==tileCount){}
     //}
  }
  //LOG("proper_edge_tiles: " << proper_edge_tiles.size());
};

void _gameData::reachEnemy(void){
  if(received_enemy_ants_hmap.size()==0){return;}
  unordered_set<pos> tmpPatternPoints;
  unordered_set<_ant*> attackerAnts;
  
  for(auto it=enemy_edge_tiles.begin(); it!=enemy_edge_tiles.end(); it++){
    if(it->second.size()<2){
      tmpPatternPoints.insert(it->first);
    }
  }

  FOREACH_MYANT(if((*antIt)->antPurpose==preattacker) attackerAnts.insert(*antIt);)
    
  //LOG("sourcesforwande*rers size " << sources_for_wanderers.size());
//   dij.multiSourceBfsFill(sources_for_wanderers, [this](loc* l){
//     //if(l->g>40){return true;}
//     target_array_to_target[l->row][l->col]=l->g;
//     return false;
//   });
  
  dij.multiSourceBfsFill(tmpPatternPoints, [this](loc* l){
    if(l->g>20){return true;}
    target_array[l->row][l->col]=l->g;
    return false;
  });
  
  for(auto it=received_enemy_ants_hmap.begin(); it!=received_enemy_ants_hmap.end(); it++){
    doOnNborsC(it->first, [this](pos p1){arMask(ar2, [this, &p1](pos p2){target_array[p1+p2]=123;});});
  }
  
  for(auto it=proper_edge_tiles.begin(); it!=proper_edge_tiles.end(); it++){
    target_array[*it]=0;
  }
  
  
  for(auto it=received_enemy_ants_hmap.begin(); it!=received_enemy_ants_hmap.end(); it++){
    //int reducedDOF=0;
    //doOnNbors(it->first, [this, &reducedDOF](pos p){if(obj_array[p]==-1) reducedDOF++;});
    //if(enemy_idle_array[it->first]>1 or reducedDOF>=2){
      if(enemy_idle_array[it->first]>2){
        dij.findPath(it->first, [this, it](loc* currentLoc){
          int deltaRow=it->first.row-currentLoc->row;
          int deltaCol=it->first.col-currentLoc->col;
          if(deltaRow*deltaRow+deltaCol*deltaCol<=2*(int)ar2){
            if(deltaRow*deltaRow+deltaCol*deltaCol<=(int)ar2){
              target_array[currentLoc->row][currentLoc->col]=-1;
            }
            return false;
          }else return true;
        });
      }
  }
  
  
  
  if(vdebug){cout << "v setLineColor 100 255 255 0.9\n";}
  FOREACH_MYANT(
    //LOG("purpose: " << (*antIt)->antPurpose);
    if((*antIt)->antPurpose==preattacker){
      if(vdebug){cout << "v star " << (*antIt)->pCs.row << " " << (*antIt)->pCs.col << " 0.2 0.8 5 false\n";}
      //doOnNbors((*antIt)->pCs, [this, antIt](pos p){if(target_array[p]==123){target_array[p]=-1;}});
      }
  )
  
  
  
  unordered_set<pos> nextTiles;
  for_each(attackerAnts.begin(), attackerAnts.end(), [&nextTiles, this](_ant* aptr){
    doOnNborsC(aptr->pCs, [&nextTiles, this](pos p){
      if(ant_expected_array[p]==0)
        nextTiles.insert(p);      
    });
  });
  
  //LOG("there are " << attackerAnts.size() << " attacker ants");
  
  //**********************************************************************************************
  for(auto it1=cluster_info_list.begin(); it1!=cluster_info_list.end(); it1++){
    if(it1->my_ants.size()==0 or it1->neededOriginal>0){continue;}
    //LOG("cluster " << it1->my_ants.size() << " of mine against " << it1->enemy_ants.size() << " theirs, needed " << (int)(response(it1->enemy_ants.size())-it1->my_ants.size()) << " but value is " << it1->neededOriginal);
    unordered_set<pos> enemySpan;
    for(auto it2=it1->enemy_ants.begin(); it2!=it1->enemy_ants.end(); it2++){
      doOnNborsC(*it2, [this, &enemySpan](pos p1){
        arMask(ar2, [this, &enemySpan, &p1](pos p2){
          pos tmpPos=p1+p2;
          tmpPos.row=wrap(tmpPos.row, rows);
          tmpPos.col=wrap(tmpPos.col, cols);
          enemySpan.insert(tmpPos);
        });
      });
    }
    unordered_set<pos> enemySpanEdge;
    for(auto it=enemySpan.begin(); it!=enemySpan.end(); it++){
      bool addTileToEdge=false;
      doOnNborsC(*it, [&addTileToEdge, &enemySpan](pos p){
        auto foundIt=enemySpan.find(p);
        if(foundIt==enemySpan.end()){addTileToEdge=true;}
      });
      if(addTileToEdge){enemySpanEdge.insert(*it);}
    }
    
    int antsNearEdge=0;
    for(auto it=enemySpanEdge.begin(); it!=enemySpanEdge.end(); it++){
      doOnNborsC(*it, [this, &antsNearEdge](pos p){
        if(obj_array[p]==0){antsNearEdge++;}
      });
    }
    if((float)enemySpanEdge.size()/(float)antsNearEdge<4.0){
      for(auto it=enemySpanEdge.begin(); it!=enemySpanEdge.end(); it++){
        target_array[*it]=-1;
      }
      if(vdebug){
        cout << "v setFillColor 255 255 0 1\n";
        for(auto it=enemySpanEdge.begin(); it!=enemySpanEdge.end(); it++){
          cout << "v circle " << it->row << " " << it->col << " 0.25 true\n";
        }
      }
    }
    
    //LOG("cluster needs 0 ants, and has " << enemySpanEdge.size() << " edge tiles, " << antsNearEdge << " of them have ants next to");
  }
  //**********************************************************************************************
  
  
  
  _array<int> costMatrix(attackerAnts.size(), nextTiles.size(), 12345);            
  int row, col;
  unordered_set<_ant*>::iterator rowIt;
  unordered_set<pos>::iterator colIt;
  for(row=0, rowIt=attackerAnts.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        costMatrix[row][col]=target_array[*colIt];
      }
    }
  }
  hungarian_problem_t matching;
  hungarian_init(&matching, costMatrix.m, costMatrix.rows(), costMatrix.cols(), HUNGARIAN_MODE_MINIMIZE_COST);      
  hungarian_solve(&matching);
  for(row=0, rowIt=attackerAnts.begin(); row<costMatrix.rows(); row++, rowIt++){
    for(col=0, colIt=nextTiles.begin(); col<costMatrix.cols(); col++, colIt++){
      if(euclideanDist2((*rowIt)->pCs, *colIt, rows, cols)<=1){
        if(matching.assignment[row][col]==1){
          //LOG("ant " << (*rowIt)->pCs << " to " << *colIt);
          (*rowIt)->eCs=*colIt;
          (*rowIt)->hasTarget=true;
          (*rowIt)->antPurpose=attacker;
          ant_expected_array[*colIt]=*rowIt;
        }
      }
    }
  }
  hungarian_free(&matching);
};
