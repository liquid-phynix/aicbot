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

  inputSnippet(std::string oKind, int rRow, int cCol, int oOwner) : kind(oKind[0]), row(rRow), col(cCol), owner(oOwner){
  };
};


struct s_gameData{
  array<int> board;

  s_gameData(int row, int col) : board(row, col){};
};


typedef std::map<std::string, int> t_paramMap;
typedef std::vector<inputSnippet> t_inputVector;

void go(void);
s_gameData* initGame(int rows, int cols);
void outputDirections(t_inputVector&);
char randomDirection(void);
