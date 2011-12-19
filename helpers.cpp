#include"helpers.hh"

int wrap(int val, int len){
  if(val < 0)
    return val + len;
  else if(val >= len)
    return val - len;
  else return val;
}

void go(void){std::cout << "go" << std::endl;}

void floatToRGB(float& r, float& g, float& b, float h, float min, float max, float s = 1.0, float v = 1.0){
  h-=min; h*=360.0; h/=max;
  float x = s * v * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
  //float m = (1.0 - s) * v;
  switch((int)(h / 60.0)){
  case 0: r = s * v; g = x; b = 0.0; break;
  case 1: r = x; g = s * v; b = 0.0; break;
  case 2: r = 0.0; g = s * v; b = x; break;
  case 3: r = 0.0; g = x; b = s * v; break;
  case 4: r = x; g = 0.0; b = s * v; break;
  default: r = s * v; g = 0.0; b = x;
  }
}

void writePPM(const char* prefix, int turn, int rows, int cols, _array<int>& objar, std::function<float(int, int, bool&)> retrieve){
  std::ofstream ppm;
  std::ostringstream oss;
  oss << prefix << turn << ".ppm";
  std::string fname = oss.str();
  ppm.open(fname.c_str());
  if(!ppm.is_open()) return;
  // rows <-> cols
  ppm << "P3" << "\n" << cols << " " << rows << "\n" << 255 << "\n";
  
  float min=0;
  float max=0;
  float val=0;
  bool overwrite;
  for(int row=0; row<rows; row++)
    for(int col=0; col<cols; col++){
      val=retrieve(row, col, overwrite);
      if(val<min) min=val;
      if(val>max) max=val;
    }
    
    if((max-min)<0.01){max=1;min=0;}
    max+=(max-min)*0.4;
  
  float r, g, b;
  for(int row=0; row<rows; row++){
    for(int col=0; col<cols; col++){
      val=retrieve(row, col, overwrite);
      floatToRGB(r, g, b, val, min, max);
        if(overwrite and objar[row][col]==0)
          ppm << "0 0 0    ";
        else
          ppm << (int)(255 * r) << " " << (int)(255 * g) << " " << (int)(255 * b) << "    ";
    }
    ppm << "\n";
  }
  ppm.close();
}

int euclideanDist2(pos p1, pos p2, int rows, int cols){
  int r1=std::min(std::min(std::abs(p1.row-p2.row),std::abs(p1.row-p2.row+rows)),
                  std::abs(p1.row-p2.row-rows));
  int r2=std::min(std::min(std::abs(p1.col-p2.col), std::abs(p1.col-p2.col+cols)),
                  std::abs(p1.col-p2.col-cols));
  return r1*r1+r2*r2;
}

int euclideanDist2(int row1, int col1, int row2, int col2, int rows, int cols){
  int r1=std::min(std::min(std::abs(row1-row2),std::abs(row1-row2+rows)),
                  std::abs(row1-row2-rows));
  int r2=std::min(std::min(std::abs(col1-col2), std::abs(col1-col2+cols)),
                  std::abs(col1-col2-cols));
  return r1*r1+r2*r2;
}

void maskedAccess(_array<int>& ar, pos origo, int rad2, std::function<void (int&)> appliedFun){
  pos o;
  for(o.row=-sqrt(rad2); o.row<sqrt(rad2); o.row++)
    for(o.col=-sqrt(rad2); o.col<sqrt(rad2); o.col++)
      if(o.row*o.row+o.col*o.col<=rad2)
        appliedFun(ar[origo+o]);
};

int mDist(int r1, int c1, int r2, int c2, int rows, int cols){
  return std::min(std::min(abs(r1-r2), std::abs(r1-r2+rows)), std::abs(r1-r2-rows))+
  std::min(std::min(abs(c1-c2), std::abs(c1-c2+cols)), std::abs(c1-c2-cols));
};


char positionToDirection(pos fromPos, pos toPos){
  pos diffPos=toPos-fromPos;
  if(diffPos.row!=0){
    if(diffPos.row>1) diffPos.row=-1;
    else if(diffPos.row<-1) diffPos.row=1;
    if(diffPos.row>0) return 's';
    else if(diffPos.row<0) return 'n';
  }else if(diffPos.col!=0){
    if(diffPos.col>1) diffPos.col=-1;
    else if(diffPos.col<-1) diffPos.col=1;
    if(diffPos.col>0) return 'e';
    else if(diffPos.col<0) return 'w';
  }
  return 'c';
};

int properDist(int c1, int c2, int maxlen){
  return std::min(std::min(std::abs(c1-c2), std::abs(c1-c2+maxlen)), std::abs(c1-c2-maxlen));
}

int cumulativeDiff(std::vector<int>* rowVec, std::vector<int>* colVec, int* indices, int len, int rows, int cols){
  int result=0;
  for(int index=0; index<len; index++){
    //if(kV1[index]!=0){
      result+=properDist(rowVec[index][0], rowVec[index][indices[index]], rows);
      result+=properDist(colVec[index][0], colVec[index][indices[index]], cols);
      //}
  }
  return result;
};

void arMask(int rad2, std::function<void(pos)> fun){
  for(int row=-sqrt(rad2); row<=sqrt(rad2); row++)
    for(int col=-sqrt(rad2); col<=sqrt(rad2); col++)
      if(row*row+col*col<=rad2)
        fun(pos(row, col));
}

int response(int enemies){
  switch(enemies){
    case 1:
      return 3;
    case 2:
      return 5;
    case 3:
      return 6;
    case 4:
      return 7;
    default:
      return ceil(1.75*(float)enemies);
  }
}

// int response(int enemies){
//   if(enemies<4)
//     return 2*enemies;
//   else
//     return 3.0*(float)enemies/2.0;
// }

// int response(int enemies){
//   switch(enemies){
//     case 1:
//       return 3;
//     case 2:
//       return 4;
//     case 3:
//       return 5;
//     default:
//       return 2*enemies;
//   }
// }
