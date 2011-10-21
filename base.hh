#include"array.hh"

#define DEBUG

#ifdef DEBUG
#define LOG(ARG) cerr << ARG << endl
#else
#define LOG(ARG)
#endif


struct inputSnippet{
  char kind;
  int row;
  int col;
  int owner;

  inputSnippet(std::string oKind, int rRow, int cCol, int oOwner):
    kind(oKind[0]), row(rRow), col(cCol), owner(oOwner){
  };
};

struct s_pair{
  int row;
  int col;
  s_pair(int rRow, int cCol):
    row(rRow), col(cCol){};
};


struct s_gameData{
  array<int> objBoard;
  array<float> board;
  array<float> tmpBoard;
  vector<s_pair> water;
  s_gameData(int row, int col):
    objBoard(row, col), board(row, col), tmpBoard(row, col), water(){};
};


typedef std::map<std::string, int> t_paramMap;
typedef std::vector<inputSnippet> t_inputVector;

void go(void);
s_gameData initGame(int, int);
void outputDirections(s_gameData&, t_inputVector&);
char randomDirection(void);
void setInfluence(s_gameData&, t_inputVector&);
void diffuseInfluence(s_gameData&, t_inputVector&);
int wrap(int, int);
char positionToDirection(int, int, int, int);
char getMinValDirection(int, int, s_gameData&);
void processWater(s_gameData&, inputSnippet&);
void floatToRGB(float&, float&, float&, float, float, float, float, float);
void writePPM(const char*, s_gameData&);
