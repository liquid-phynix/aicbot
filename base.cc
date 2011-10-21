#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<map>
#include<vector>
#include<cmath>

#include"array.hh"
#include"base.hh"

using namespace std;

int g_turn = 0;

int main(void){
  t_paramMap  parameters;

  while(!cin.eof()){
    string pName, iLine;
    int pValue = 0;
    getline(cin, iLine);
    stringstream sStream(iLine, stringstream::in);
    sStream >> pName >> pValue;
    if(pName == "ready") break;
    LOG(pName << " : " << pValue);
    parameters[pName] = pValue;
  }

  /*
  cerr << "hashtable:" <<endl;
  for(t_paramMap::iterator ii = parameters.begin(); ii != parameters.end(); ++ii){
    cerr << (*ii).first << ": " << (*ii).second << endl;
  }
  */

  s_gameData gameData = initGame(parameters["rows"], parameters["cols"]);

  go();

  while(true){
    t_inputVector turnInput;

    string kind, iLine;
    int turn = 0;
    getline(cin, iLine);
    stringstream sStream(iLine, stringstream::in);
    sStream >> kind >> turn;
    if(kind == "turn") {cerr << kind << " " << turn << endl;}

    while(!cin.eof()){
      string kind, iLine;
      int f1 = 0, f2 = 0, f3 = -1;
      getline(cin, iLine);
      stringstream sStream(iLine, stringstream::in);
      sStream >> kind >> f1 >> f2 >> f3;
      if(kind == "go") break;
      turnInput.push_back(inputSnippet(kind, f1, f2, f3));
    }
    
    for(t_inputVector::iterator ii = turnInput.begin(); ii != turnInput.end(); ++ii){
      if((*ii).kind == 'w'){
        processWater(gameData, *ii);
      }
    }
    
    writePPM("board", gameData);

    outputDirections(gameData, turnInput);
    go();

    diffuseInfluence(gameData, turnInput);

    // for(t_inputVector::iterator ii = turnInput.begin(); ii != turnInput.end(); ++ii){
    //   cerr << (*ii).kind << ": " << (*ii).row << " " << (*ii).col << " " << (*ii).owner << endl;
    // }
    // cerr << endl;
    g_turn++;
  }
  

  

  // for(t_paramMap::iterator ii = parameters.begin(); ii != parameters.end(); ++ii){
  //   cerr << (*ii).first << ": " << (*ii).second << endl;
  // }


  //  cout << "Hello, World!" <<endl;



  return 0;
}

void outputDirections(s_gameData& gData, t_inputVector& vIn){
  for(t_inputVector::iterator ii = vIn.begin(); ii != vIn.end(); ++ii){
    if((*ii).kind == 'a' and (*ii).owner == 0){
      char dir = getMinValDirection((*ii).row, (*ii).col, gData);
      if(dir != 'C'){
        cout << "o " << (*ii).row << " " << (*ii).col << " " << dir << endl;
        cerr << "o " << (*ii).row << " " << (*ii).col << " " << dir << endl;
      }
    }
  }
}

// void outputDirections(t_inputVector& vIn){
//   for(t_inputVector::iterator ii = vIn.begin(); ii != vIn.end(); ++ii){
//     if((*ii).kind == 'a' and (*ii).owner == 0){
//       cout << "o " << (*ii).row << " " << (*ii).col << " " << randomDirection() << endl;
//     }
//   }
// }

char randomDirection(void){
  char choices[] = {'N', 'S', 'E', 'W'};
  return choices[rand()%4];
}

void go(void){
  cout << "go" << endl;
}

s_gameData initGame(int rows, int cols){
  return * new s_gameData(rows, cols);
}

void processWater(s_gameData& gData, inputSnippet& water){
  gData.water.push_back(s_pair(water.row, water.col));
  gData.objBoard.m[water.row][water.col] = 1;
}

inline int wrap(int val, int len){
  if(val < 0){
    return val + len;
  }
  if(val >= len){
    return val - len;
  }
  return val;
}

char positionToDirection(int row, int col, int toRow, int toCol){
  int diffRow = toRow - row;
  int diffCol = toCol - col;
  if(diffRow > 0 && diffCol == 0) return 'S';
  if(diffRow < 0 && diffCol == 0) return 'N';
  if(diffRow == 0 && diffCol > 0) return 'E';
  if(diffRow == 0 && diffCol < 0) return 'W';
  return 'C';
}

char getMinValDirection(int row, int col, s_gameData& gData){
  int& rows = gData.board.r;
  int& cols = gData.board.c;
  int nbors[4][2] = {{wrap(row + 1, rows), col},
                     {wrap(row - 1, rows), col},
                     {row, wrap(col + 1, cols)},
                     {row, wrap(col - 1, cols)}};
  int minIndex = 0;
  float minVal = 10.0;
  for(int index = 0; index < 4; index++){
    float val = gData.board.m[nbors[index][0]][nbors[index][1]];
    cerr << "rc: "<< nbors[index][0] << " " << nbors[index][1] << " : " << val <<endl;
    if(minVal > val and val > 0.0){
      minVal = val;
      minIndex = index;
    }
  }
  return positionToDirection(row, col, nbors[minIndex][0], nbors[minIndex][1]);
}
      

void setInfluence(s_gameData& gData, t_inputVector& iVec){
  for(t_inputVector::iterator ii = iVec.begin(); ii != iVec.end(); ++ii){
    if((*ii).kind == 'a'){
      if((*ii).owner == 0){
        gData.board.m[(*ii).row][(*ii).col] = 1.0;
      }
      // else{
      //   gData.board.m[(*ii).row][(*ii).col] = -1.0;
      // }
    }
  }
}

void diffuseInfluence(s_gameData& gData, t_inputVector& iVec){
  int& rows = gData.board.r;
  int& cols = gData.board.c;
  int iterations = 3;
  array<float>* arP;
  array<float>* arS;
  array<float>* arT;
  arP = &(gData.board);
  arS = &(gData.tmpBoard);

  setInfluence(gData, iVec);
  while(iterations--){
    for(int row = 0; row < rows; row++){
      for(int col = 0; col < cols; col++){
        arS->m[row][col] = (arP->m[wrap(row + 1, rows)][col] +
                            arP->m[wrap(row - 1, rows)][col] +
                            arP->m[row][wrap(col + 1, cols)] +
                            arP->m[row][wrap(col - 1, cols)] +

                            arP->m[wrap(row + 1, rows)][wrap(col + 1, cols)] +
                            arP->m[wrap(row + 1, rows)][wrap(col - 1, cols)] +
                            arP->m[wrap(row - 1, rows)][wrap(col + 1, cols)] +
                            arP->m[wrap(row - 1, rows)][wrap(col - 1, cols)] +

                            arP->m[row][col]) / 9.0;
      }
    }
    for(vector<s_pair>::iterator ii = gData.water.begin(); ii != gData.water.end(); ++ii){
      arS->m[(*ii).row][(*ii).col] = 0.0;
    }
    setInfluence(gData, iVec);

    arT = arP;
    arP = arS;
    arS = arT;
  }
}

void floatToRGB(float& r, float& g, float& b, float h, float min, float max, float s = 1.0, float v = 1.0){
  h-=min;
  h*=360.0;
  h/=max;

  float x = s * v * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
  float m = (1.0 - s) * v;
  switch((int)(h / 60.0)){
  case 0:
    r = s * v;
    g = x;
    b = 0.0;
    break;
  case 1:
    r = x;
    g = s * v;
    b = 0.0;
    break;
  case 2:
    r = 0.0;
    g = s * v;
    b = x;
    break;
  case 3:
    r = 0.0;
    g = x;
    b = s * v;
    break;
  case 4:
    r = x;
    g = 0.0;
    b = s * v;
    break;
  default:
    r = s * v;
    g = 0.0;
    b = x;
  } 
}

void writePPM(const char* prefix, s_gameData& gData){
  ofstream ppm;
  ostringstream oss;
  oss << prefix << g_turn << ".ppm";
  string fname = oss.str();
  cerr << "filename: " << fname <<endl;
  ppm.open(fname.c_str());
  if(!ppm.is_open()) return;

  // rows <-> cols
  ppm << "P3" << endl << gData.board.c << " " << gData.board.r << endl << 255 << endl;

  float min = 0.0;
  float max = 0.0;
  float val = 0.0;
  for(int row = 0; row < gData.board.r; row++){
    for(int col = 0; col < gData.board.c; col++){
      val = gData.board.m[row][col];
      if(val < min) min = val;
      if(val > max) max = val;
    }
  }

  float r, g, b;
  for(int row = 0; row < gData.board.r; row++){
    for(int col = 0; col < gData.board.c; col++){
      val = gData.board.m[row][col];
      floatToRGB(r, g, b, val, min, max);
      if(gData.objBoard.m[row][col] == 1)
        ppm << "0 0 " << (int)(255) << "    ";
      else
        ppm << (int)(255 * r) << " " << (int)(255 * g) << " " << (int)(255 * b) << "    ";
    }
    ppm << endl;
  }
  ppm.close();
}
