#include<iostream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<map>
#include<vector>

#include"array.hh"
#include"base.hh"


using namespace std;


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

  s_gameData* gameData = initGame(parameters["rows"], parameters["cols"]);
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

    outputDirections(turnInput);

    go();

    // for(t_inputVector::iterator ii = turnInput.begin(); ii != turnInput.end(); ++ii){
    //   cerr << (*ii).kind << ": " << (*ii).row << " " << (*ii).col << " " << (*ii).owner << endl;
    // }
    // cerr << endl;
  }
  

  

  // for(t_paramMap::iterator ii = parameters.begin(); ii != parameters.end(); ++ii){
  //   cerr << (*ii).first << ": " << (*ii).second << endl;
  // }


  //  cout << "Hello, World!" <<endl;



  return 0;
}

void outputDirections(t_inputVector& vIn){
  for(t_inputVector::iterator ii = vIn.begin(); ii != vIn.end(); ++ii){
    if((*ii).kind == 'a' and (*ii).owner == 0){
      cout << "o " << (*ii).row << " " << (*ii).col << " " << randomDirection() << endl;
    }
  }
}

char randomDirection(void){
  char choices[] = {'N', 'S', 'E', 'W'};
  return choices[rand()%4];
}

void go(void){
  cout << "go" << endl;
}

s_gameData* initGame(int rows, int cols){
  return new s_gameData(rows, cols);
}

