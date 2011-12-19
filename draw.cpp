#include"base.hh"

using namespace std;


void _gameData::toPPM(void){
//   writePPM("hillenemy", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
//     ow=false; return (float)abs(enemy_array_for_defense[row][col]-hill_array[row][col]);
//   });
      writePPM("not_seen_bfs", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
            ow=false; return (float)navigational_seen_array[row][col];
        });
  //   writePPM("board", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
    //     ow=true; return (float)image_array[row][col];
  //   });
  
//     writePPM("targetbfs", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
//         ow=false; return (float)target_array_to_target[row][col];
//     });

  //   writePPM("enemyeuclidean", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
    //     ow=false; return (float)enemy_array[row][col].f1;
  //   });
//     writePPM("safety", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
//         ow=false; return (float)enemy_safety_mask[row][col];
    //});
  //   writePPM("safety_plus1", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
    //     ow=false; return (float)enemy_safety_mask_plus1[row][col];
  //   });
  //   writePPM("hillarray", turn, rows, cols, obj_array, [this](int row, int col, bool& ow){
    //     ow=false; return (float)hill_array[row][col];
  //   });
};

void _gameData::debugDraw(void){
  /*
  float min=0;
  float max=0;
  float val=0;
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      val=target_array[row][col];
      if(val<min) min=val;
      if(val>max) max=val;
    }
  }
  
  if((max-min)<0.01){
    max=1;
    min=0;  
  }
  max+=(max-min)*0.4;
  
  float r, g, b;
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      if(target_array[row][col]==0){
        floatToRGB(r, g, b, target_array[row][col], min, max, 1, 1);
        cout << "v setFillColor " << (int)(255 * r) << " " << (int)(255 * g) << " " << (int)(255 * b) << " 0.35\n";
        cout << "v tile " << row << " " << col << "\n";
      }
    }
  }
  */
              
              
  /*
  enemy safety mask
  unordered_set<pos> tmpPatternPoints;
  for(auto it=received_enemy_ants_hmap.begin(); it!=received_enemy_ants_hmap.end(); it++){
    addEnemyMaskPlus1(tmpPatternPoints, it->first);
  }
  dij.multiSourceBfsFill(tmpPatternPoints, [this](loc* currentLoc){
    target_array[currentLoc->row][currentLoc->col]=currentLoc->g;
  });
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      if(target_array[row][col]==0){
        cout << "v setFillColor 255 0 0 0.35\n";
        cout << "v tile " << row << " " << col << "\n";
      }
    }
  }
  */
  
                        
  //   defense vis.
  
  float min=0;
  float max=0;
  float val=0;
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      //val=enemy_addittive_array[row][col]; if(val==-1) continue;
      //val=enemy_array_for_defense[row][col]; if(val==-1) continue;
      //val=enemy_safety_mask[row][col]; if(val==0) continue;
      val=target_array[row][col]; if(val==123 or val>15) continue;
      //val=target_array_to_target[row][col]; if(val>5) continue;
      //val=target_array[row][col]; if(val==MAX_TARGET_ARRAY) continue;
      //if(val==-1 or val==123) continue;
      if(val<min) min=val;
      if(val>max) max=val;
    }
  } 
  if((max-min)<0.01){
    max=1;
    min=0;  
  }
  max+=(max-min)*0.4;
  
  //LOG("max/min " << max << "/" << min);
  
  float r, g, b;
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      //val=enemy_addittive_array[row][col]; if(val==-1) continue;
      //val=enemy_array_for_defense[row][col]; if(val==-1) continue;
      //val=enemy_safety_mask[row][col]; if(val==0) continue;
      val=target_array[row][col]; if(val==123 or val>15) continue;
      //val=target_array_to_target[row][col]; if(val>5) continue;
      //val=target_array[row][col]; if(val==MAX_TARGET_ARRAY) continue;
      //if(val==-1 or val==123) continue;
      floatToRGB(r, g, b, val, min, max, 1, 1);
      cout << "v setFillColor " << (int)(255 * r) << " " << (int)(255 * g) << " " << (int)(255 * b) << " 0.35\n";
      cout << "v tile " << row << " " << col << "\n";
    }
  }
  
  
  

  cout << "v setFillColor 0 255 0 1\n";
  for_each(proper_edge_tiles.begin(), proper_edge_tiles.end(), [](pos p){
    cout << "v circle " << p.row << " " << p.col << " 0.25 true\n";
  });

  /*
  for(int row=0; row<(int)rows; row++){
    for(int col=0; col<(int)cols; col++){
      //if(enemy_idle_array[row][col]!=-1){
        //cout << "i "<< row << " " << col << " " << enemy_idle_array[row][col] << "\n";
      //}
      if(influence_hill[row][col]==0){
        cout << "v setFillColor 0 255 0 0.6\n";
        cout << "v tile "<< row << " " << col << "\n";
      }else if(influence_hill[row][col]>0){
        cout << "v setFillColor 0 255 255 0.6\n";
        cout << "v tile "<< row << " " << col << "\n";
      }
    }
  }
  */
  
//   for_each(myants_list.begin(), myants_list.end(), [](_ant* aptr){
//     cout << "i "<< aptr->pCs.row << " " << aptr->pCs.col << " " << aptr->antPurpose << "(" << aptr->strict << ")\n";
//   });
  
  /*
  cout << "v setFillColor 0 0 0 1\n";
  for_each(matched_force_enemies.begin(), matched_force_enemies.end(), [](pos p){
    cout << "v tile "<< p.row << " " << p.col << " \n";
  });
  */
  
                        
  //lathato ellensegek:
  /*
  cout << "v setFillColor 255 255 255 0.3\n";
  for_each(received_enemy_ants_hmap.begin(), received_enemy_ants_hmap.end(), [](pair<pos, int> p){
    cout << "v tile " << p.first.row << " " << p.first.col << "\n";
  });
  */

  /*
  FOREACH(formation_set,
          float r; float g; float b;
          floatToRGB(r,g,b,rand()/(float)RAND_MAX,0,1,1,1);
          cout << "v setFillColor " << (int)(255 * r) << " " << (int)(255 * g) << " " << (int)(255 * b) << " 0.5\n";
          for(auto antptrit=(*it)->members.begin(); antptrit!=(*it)->members.end(); antptrit++){
            cout << "v tile " << (*antptrit)->pCs.row << " " << (*antptrit)->pCs.col << "\n";
          }
  )
  */
};