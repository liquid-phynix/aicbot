#include"base.hh"

using namespace std;

void _gameData::perTurnReset(void){
  turn++;
  for(uint index=0; index<rows*cols; index++){
    if(obj_array.m[index]!=-1) obj_array.m[index]=-2;
    ant_ptr_array.m[index]=0;
    ant_expected_array.m[index]=0;
    enemy_safety_mask.m[index]=0;
    enemy_safety_mask_plus1.m[index]=0;
    seen_array.m[index]=0;
    if(turn%2==0){navigational_seen_array.m[index]=0;}
    if(turn%5==0){not_seen_bfs.m[index]=-1;}
    
    enemy_array.m[index].f1=12345;
    enemy_array.m[index].f2=12345;
    enemy_array.m[index].f3=-1;
    enemy_array.m[index].f4=-1;
    //food_bfs_array[row][col]=1234567;
    target_array.m[index]=12345;
    target_array_to_target.m[index]=12345;
    
    enemy_array_for_defense.m[index]=-1;
    influence_hill.m[index]=-1;
    //enemy_addittive_array[row][col]=-1;
    //ever_seen_array[row][col]=-1;
    own_ants_array.m[index]=-1;
    
    own_hill_bfs_array.m[index]=-1;
    enemy_hill_bfs_array.m[index]=-1;
  }
  
  sources_for_wanderers.clear();
  //hillDangerLevel=0;
  antsWoPurpose.clear();
  
  myants_received_set.clear();
  received_food_set.clear();
  received_enemy_ants_hmap.clear();
  received_hills_hmap.clear();
  enemy_coverage.clear();
  matched_force_enemies.clear();
};

void _gameData::receiveAnt(pos rc, int owner){
  if(owner==0)
    myants_received_set.insert(rc);
  else{
    haveiseenenemyyet=true;
    received_enemy_ants_hmap[rc]=owner;
    doOnNborsC(rc, [this](pos p1){arMask(ar2, [this, p1](pos p2){enemy_safety_mask[p1+p2]=1;});});
    doOnNborsC(rc, [this](pos p1){arMask((sqrt(ar2)+1)*(sqrt(ar2)+1), [this, p1](pos p2){enemy_safety_mask_plus1[p1+p2]=1;});});
    dij.findPath(rc, [this, &rc](loc* currentLoc){
      int edist=euclideanDist2(rc.row, rc.col, currentLoc->row, currentLoc->col, rows, cols);
      if(edist<enemy_array[currentLoc->row][currentLoc->col].f1){
        enemy_array[currentLoc->row][currentLoc->col].f1=edist;
        enemy_array[currentLoc->row][currentLoc->col].f3=rc.row;
        enemy_array[currentLoc->row][currentLoc->col].f4=rc.col;
      }
      if(currentLoc->g<enemy_array[currentLoc->row][currentLoc->col].f2){
        enemy_array[currentLoc->row][currentLoc->col].f2=currentLoc->g;
      }
      return false;
    });
  }
  obj_array[rc]=owner;
};

void _gameData::receiveFood(pos rc){
  received_food_set.insert(rc);
};

void _gameData::receiveWater(pos rc){
  water_set.insert(rc);
  obj_array[rc] = -1;
  dij.correctForWater(rc);
};

void _gameData::receiveHill(pos rc, int owner){
  received_hills_hmap[rc]=owner;
};
//******************************************************************************************
// end receiving
//******************************************************************************************


//******************************************************************************************
// processing
//******************************************************************************************

void _gameData::processAnts(void){
  auto setToOneFn=[](int& i){i=1;};
  auto addOneFn=[](int& i){i+=1;};
  list<_ant*> myants_list_tmp;
  FOREACH_MYANT(
    if((*antIt)->recognizeSelf(myants_received_set)){
      myants_list_tmp.push_back(*antIt);
    }else delete *antIt;
  )
  myants_list.clear();
  myants_list.swap(myants_list_tmp);
  FOREACH(myants_received_set, myants_list.emplace_back(new _ant(*it, rows, cols, dij, haveiseenenemyyet, vr2, obj_array, ant_expected_array));)
  if(ant_count_water_mark<myants_list.size()){ant_count_water_mark=myants_list.size();}
  FOREACH_MYANT(
    ant_ptr_array[(*antIt)->pCs]=*antIt;
    (*antIt)->pathLengthToTarget=-1;
    ant_visited_array[(*antIt)->pCs]+=1;   
    
    maskedAccess(ever_seen_array, (*antIt)->pCs, vr2, addOneFn);
    
    maskedAccess(seen_array, (*antIt)->pCs, vr2, setToOneFn); //kell a foodhoz, muszaj hogy vr2 legyen
    
    maskedAccess(navigational_seen_array, (*antIt)->pCs, vr2/2, setToOneFn);
  )
  
  if(turn%5==0){dij.multiSourceSeenArray(ever_seen_array);}
  
  
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      if(obj_array[row][col]>0){
        enemy_idle_array[row][col]+=1;
      }else{
        enemy_idle_array[row][col]=-1;
      }
    }
  }
  
  list<pos> sources;
  FOREACH_MYANT(
    sources.emplace_back((*antIt)->pCs);
    (*antIt)->discoverSurroundings();
    //LOG("ant " << (*antIt)->pCs << " sees " << (*antIt)->count_friend << " friends");
  )
  dij.multiSourceBfsFill(sources, [this](loc* l){own_ants_array[l->row][l->col]=l->g; return false;});
};

void _gameData::updateSeen(void){
  auto setToOneFn=[](int& i){i=1;};
  FOREACH_MYANT(
    maskedAccess(seen_array, (*antIt)->pCs, vr2, setToOneFn);
  )
}

void _gameData::processFood(void){
  FOREACH_NI(seen_food_hmap,
             if(seen_array[it->first] and received_food_set.find(it->first)==received_food_set.end())
               seen_food_hmap.erase(it++);
             else it++;
  )
  FOREACH(received_food_set, seen_food_hmap.insert(pair<pos, _ant*>(*it, 0));)
};

void _gameData::processHills(void){
  FOREACH_NI(own_hills_hmap,
             if(seen_array[it->first] and received_hills_hmap.find(it->first)==received_hills_hmap.end()){
               own_hills_hmap.erase(it++);
             }else{it->second=0; it++;}
  )
  FOREACH_NI(enemy_hills_hmap,
             if(seen_array[it->first] and received_hills_hmap.find(it->first)==received_hills_hmap.end()){
               enemy_hills_hmap.erase(it++);
             }else it++;
  )
  FOREACH(received_hills_hmap,
          if(it->second==0)
            own_hills_hmap[it->first]=0;
          else
            enemy_hills_hmap[it->first]=it->second;
  )
  
  FOREACH(enemy_hills_hmap, influence_hill[it->first]=1;)
  
  unordered_set<pos> hillSources;
  FOREACH(enemy_hills_hmap, hillSources.insert(it->first);)
  dij.multiSourceBfsFill(hillSources, [this](loc* currentLoc){
      enemy_hill_bfs_array[currentLoc->row][currentLoc->col]=currentLoc->g;
      return false;
  });
  
  hillSources.clear();
  FOREACH(own_hills_hmap, hillSources.insert(it->first);)
  dij.multiSourceBfsFill(hillSources, [this](loc* l){
      own_hill_bfs_array[l->row][l->col]=l->g;
      return false;
  });
  
  list<pos> safetySources;
  FOREACH(received_enemy_ants_hmap, addEnemyMask(safetySources, it->first);)
  //dij.multiSourceHillDefense(safetySources, enemy_array_for_defense, own_hill_bfs_array);
};


void _gameData::outputCoordinates(void){
  for(int row=0; row<(int)rows; row++)
    for(int col=0; col<(int)cols; col++)
      ant_expected_array[row][col]=0;
  FOREACH_MYANT(if(ant_expected_array[(*antIt)->eCs]){gonnaStay(*antIt);} else{ant_expected_array[(*antIt)->eCs]=*antIt;})
  FOREACH_MYANT(if((*antIt)->pCs!=(*antIt)->eCs) cout << "o " << (*antIt)->pCs.row << " " << (*antIt)->pCs.col << " " << positionToDirection((*antIt)->pCs, (*antIt)->eCs) << endl;)
};
