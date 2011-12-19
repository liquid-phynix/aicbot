#include"dij.hh"

_dij::_dij(_array<int>& objBoard, _array<int>& safetyMask, _array<int>& notSeenBFS):
loc_board(objBoard.rows(), objBoard.cols()),
loc_board_full(objBoard.rows(), objBoard.cols()),
obj_board(objBoard),
safety_mask(safetyMask),
not_seen_bfs(notSeenBFS),
loc_queue(objBoard.rows()*objBoard.cols()),
usedValue(0),
maxG(80){
  setupLocBoard();
};

std::ostream& operator<<(std::ostream& o, std::list<pos>* l){
  std::for_each(l->begin(), l->end(), [&o](pos p){o << p << " ";});
  return o;
};

std::list<pos>* _dij::backtrackPath(loc* currentLoc){
  std::list<pos>* path=new std::list<pos>();
  loc* tmpLoc; int cLG;
  while(true){
    path->push_back(pos(currentLoc->row, currentLoc->col));
    cLG = currentLoc->g;
    if(cLG == 0) break;
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
  }
  return path;  
};

/*
std::list<pos>* _dij::backtrackPathSafetyExclusion(loc* currentLoc){
  std::list<pos>* path=new std::list<pos>();
  loc* tmpLoc; int cLG;
  while(true){
    path->push_back(pos(currentLoc->row, currentLoc->col));
    cLG = currentLoc->g;
    //std::cerr << currentLoc << " : " << pos(currentLoc->row, currentLoc->col) << std::endl;
    if(cLG == 0) break;
    tmpLoc = currentLoc->n;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->s;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->e;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->w;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
  }
  return path;
};
*/


std::list<pos>* _dij::backtrackPathSafetyExclusion(loc* currentLoc){
  std::list<pos>* path=new std::list<pos>();
  loc* tmpLoc; int cLG;
  while(true){    
    path->push_back(pos(currentLoc->row, currentLoc->col));
    cLG = currentLoc->g;
    if(cLG == 0) break;
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->used==usedValue and tmpLoc->g < cLG){currentLoc = tmpLoc; continue;};
  }
  return path;
};

void _dij::findPath(pos startPos, std::function<bool (loc*)> endCondition){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc=&loc_board[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g=0;
  currentLoc->used=usedValue;
  while(currentLoc->g<maxG and loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    if(endCondition(currentLoc)){break;}
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};

void _dij::findPath(int maximumG, pos startPos, std::function<bool (loc*)> endCondition){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc=&loc_board[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g=0;
  currentLoc->used=usedValue;
  while(currentLoc->g<maximumG and loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    if(endCondition(currentLoc)){break;}
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};


void _dij::findPathSafetyExclusion(pos startPos, std::function<bool (loc*)> endCondition){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc = &loc_board[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g = 0;
  currentLoc->used=usedValue;
  while(currentLoc->g<maxG and loc_queue.size>0){
    loc* tmpLoc;
    currentLoc = loc_queue.pop();
    if(endCondition(currentLoc)){break;}
    tmpLoc = currentLoc->n;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->used<usedValue){
      tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);
    }
    tmpLoc = currentLoc->s;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->used<usedValue){
      tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);
    }
    tmpLoc = currentLoc->e;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->used<usedValue){
      tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);
    }
    tmpLoc = currentLoc->w;
    if(tmpLoc and safety_mask[tmpLoc->row][tmpLoc->col]==0 and tmpLoc->used<usedValue){
      tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);
    }
  }
};

void _dij::findPathFull(pos startPos, std::function<bool (loc*)> endCondition){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc=&loc_board_full[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g=0;
  currentLoc->used=usedValue;
  while(currentLoc->g<maxG and loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    if(endCondition(currentLoc)){break;}
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};

void _dij::multiSourceBfsFill(std::unordered_set<pos>& sources, std::function<bool(loc*)> coreFn){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc;
  for(auto posIt=sources.begin(), endIt=sources.end(); posIt!=endIt; posIt++){
    currentLoc=&loc_board[*posIt];
    currentLoc->g=0;
    currentLoc->used=usedValue;
    loc_queue.push(currentLoc);
  }
  
  while(loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    if(coreFn(currentLoc)) break;
    
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};


void _dij::multiSourceBfsFill(std::list<pos>& sources, std::function<bool(loc*)> coreFn){
  if(sources.size()<1) return;
  usedValue++;
  loc_queue.reset();
  loc* currentLoc;
  for(auto posIt=sources.begin(), endIt=sources.end(); posIt!=endIt; posIt++){
    currentLoc=&loc_board[*posIt];
    currentLoc->g=0;
    currentLoc->used=usedValue;
    loc_queue.push(currentLoc);
  }  
  while(loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    if(coreFn(currentLoc)) break;
    
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};


void _dij::multiSourceHillDefense(std::list<pos>& sources, _array<int>& targetArray, _array<int>& hillBFS){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc;
  int maxHillG=0;
  for(auto it=sources.begin(); it!=sources.end(); it++){
    if(hillBFS[*it]>16) continue;
    if(hillBFS[*it]>maxHillG)
      maxHillG=hillBFS[*it];
    currentLoc=&loc_board[*it];
    currentLoc->g=0;
    currentLoc->used=usedValue;
    loc_queue.push(currentLoc);
  }
  while(loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    if(currentLoc->g>0) targetArray[currentLoc->row][currentLoc->col]=(currentLoc->g==0?123:currentLoc->g);
    if(currentLoc->g>20) continue;
    
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue and hillBFS[tmpLoc->row][tmpLoc->col]<maxHillG){
      tmpLoc->used=usedValue;
      tmpLoc->g=currentLoc->g+1;
      loc_queue.push(tmpLoc);
    }
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue and hillBFS[tmpLoc->row][tmpLoc->col]<maxHillG){
      tmpLoc->used=usedValue;
      tmpLoc->g=currentLoc->g+1;
      loc_queue.push(tmpLoc);
    }
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue and hillBFS[tmpLoc->row][tmpLoc->col]<maxHillG){
      tmpLoc->used=usedValue;
      tmpLoc->g=currentLoc->g+1;
      loc_queue.push(tmpLoc);
    }
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue and hillBFS[tmpLoc->row][tmpLoc->col]<maxHillG){
      tmpLoc->used=usedValue;
      tmpLoc->g=currentLoc->g+1;
      loc_queue.push(tmpLoc);
    }
  }
};

void _dij::multiSourceSeenArray(_array<int>& seen_array){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc;
  int dim=seen_array.rows()*seen_array.cols();
  for(int index=0; index<dim; index++){
    if(seen_array.m[index]==-1){
      loc_board.m[index].g=0;
      loc_board.m[index].used=usedValue;
      loc_queue.push(&loc_board.m[index]);
    }
  }
  
  while(loc_queue.size>0){
    loc* tmpLoc;
    currentLoc=loc_queue.pop();
    not_seen_bfs[currentLoc->row][currentLoc->col]=currentLoc->g;
    
    tmpLoc=currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc=currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};

/*
std::vector<std::vector<int>>* _dij::findObjectsInRange(pos startPos, int rad){
  auto result=new std::vector<std::vector<int>>();
  loc_queue.reset();
  resetG();
  loc* currentLoc = &loc_board_full[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g = 0;
  int objType=-2;
  //  while(currentLoc->g <= rad and loc_queue.size > 0){
    while((currentLoc->g)*(currentLoc->g)<=2*rad*rad and loc_queue.size > 0){
      //        std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\t";
    loc* tmpLoc;
    currentLoc = loc_queue.pop();
    
    objType=obj_board[currentLoc->row][currentLoc->col];
    if(euclideanDist2(startPos, pos(currentLoc->row, currentLoc->col),
      loc_board_full.rows(), loc_board_full.cols()) <= rad*rad and objType >= 0 and objType < 100){
      result->push_back(std::vector<int>{currentLoc->row, currentLoc->col, objType});
      //std::cerr << "t: " << objType << "\t";
    }
    
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
  }
      //    std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\n";
      return result;
};
*/


void _dij::scanRad2(pos startPos, int rad2, std::function<void(loc*)> fun){
  usedValue++;
  loc_queue.reset();
  loc* currentLoc=&loc_board_full[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g=0;
  currentLoc->used=usedValue;
  while((currentLoc->g)*(currentLoc->g)<=2*rad2 and loc_queue.size>0){
    loc* tmpLoc;
    currentLoc = loc_queue.pop();
    if(euclideanDist2(startPos.row, startPos.col, currentLoc->row, currentLoc->col, loc_board_full.rows(), loc_board_full.cols()) <= rad2){
      fun(currentLoc);
    }
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->used<usedValue){tmpLoc->used=usedValue; tmpLoc->g=currentLoc->g+1; loc_queue.push(tmpLoc);}
  }
};


/*
void _dij::fillUpToRange(pos startPos, int range, _array<int>& fillAr){
  loc_queue.reset();
  resetG();
  loc* currentLoc = &loc_board[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g = 0;

  //  while(currentLoc->g <= range and loc_queue.size > 0){
    while(euclideanDist2(startPos, pos(currentLoc->row, currentLoc->col),
      loc_board.rows(), loc_board.cols()) <= range and loc_queue.size > 0){
      //        std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\t";
    loc* tmpLoc;
    currentLoc = loc_queue.pop();
    fillAr[currentLoc->row][currentLoc->col]=currentLoc->g;
    
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
      }
      //    std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\n";
};
*/

//rossz
/*
void _dij::fillEnemySafetyMask(pos startPos, int rad2, _array<int>& fillAr){
  loc_queue.reset();
  loc* currentLoc = &loc_board_full[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g = 0;
  
  //  while(currentLoc->g <= range and loc_queue.size > 0){
    while((currentLoc->g)*(currentLoc->g)<=2*rad2 and loc_queue.size > 0){
      //        std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\t";
    loc* tmpLoc;
    currentLoc = loc_queue.pop();
    //fillAr[currentLoc->row][currentLoc->col]=currentLoc->g;
    if(euclideanDist2(startPos, pos(currentLoc->row, currentLoc->col),loc_board.rows(), loc_board.cols()) <= rad2){
      fillAr[currentLoc->row][currentLoc->col]+=1;
    }
    
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
      }
      //    std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\n";
};
*/

/*
void _dij::fillUpToRangeIfLess(pos startPos, int range, _array<int>& fillAr){
  loc_queue.reset();
  resetG();
  loc* currentLoc = &loc_board[startPos];
  loc_queue.push(currentLoc);
  currentLoc->g = 0;
  
  //  while(currentLoc->g <= range and loc_queue.size > 0){
    while(euclideanDist2(startPos, pos(currentLoc->row, currentLoc->col),
      loc_board.rows(), loc_board.cols()) <= range and loc_queue.size > 0){
      //        std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\t";
    loc* tmpLoc;
    currentLoc = loc_queue.pop();
    if(currentLoc->g < fillAr[currentLoc->row][currentLoc->col] or fillAr[currentLoc->row][currentLoc->col] == -1)
      fillAr[currentLoc->row][currentLoc->col]=currentLoc->g;
    
    tmpLoc = currentLoc->n;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->s;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->e;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
    tmpLoc = currentLoc->w;
    if(tmpLoc and tmpLoc->g == -1){tmpLoc->g = currentLoc->g + 1; loc_queue.push(tmpLoc);}
      }
      //    std::cerr << "(" << currentLoc->row << " " << currentLoc->col << ") " << obj_board[currentLoc->row][currentLoc->col] << "\n";
};
*/


void _dij::setupLocBoard(void){
  int down, up, right, left;
  int rows = obj_board.rows();
  int cols = obj_board.cols();
  for(int row = 0; row < rows; row++){
    for(int col = 0; col < cols; col++){
      down  = wrap(row + 1, rows);
      up    = wrap(row - 1, rows);
      right = wrap(col + 1, cols);
      left  = wrap(col - 1, cols);
      loc_board[row][col].s = &(loc_board[down][col]);
      loc_board[row][col].n = &(loc_board[up][col]);
      loc_board[row][col].e = &(loc_board[row][right]);
      loc_board[row][col].w = &(loc_board[row][left]);
      loc_board[row][col].row = row;
      loc_board[row][col].col = col;
      
      loc_board_full[row][col].s = &(loc_board_full[down][col]);
      loc_board_full[row][col].n = &(loc_board_full[up][col]);
      loc_board_full[row][col].e = &(loc_board_full[row][right]);
      loc_board_full[row][col].w = &(loc_board_full[row][left]);
      loc_board_full[row][col].row = row;
      loc_board_full[row][col].col = col;
      
    }
  }
};

void _dij::correctForWater(pos rc){
  int down, up, right, left;
  int rows = obj_board.rows();
  int cols = obj_board.cols();
  down  = wrap(rc.row + 1, rows);
  up    = wrap(rc.row - 1, rows);
  right = wrap(rc.col + 1, cols);
  left  = wrap(rc.col - 1, cols);
  loc_board[rc].n = NULL;
  loc_board[rc].s = NULL;
  loc_board[rc].e = NULL;
  loc_board[rc].w = NULL;
  loc_board[down][rc.col].n  = NULL;
  loc_board[up][rc.col].s    = NULL;
  loc_board[rc.row][right].w = NULL;
  loc_board[rc.row][left].e  = NULL;
};

/*
void _dij::resetG(void){
  int rows = obj_board.rows();
  int cols = obj_board.cols();
  for(int row = 0; row < rows; row++){
    for(int col= 0; col< cols; col++){
      loc_board[row][col].g = -1;
      loc_board_full[row][col].g = -1;
    }
  }
};
*/