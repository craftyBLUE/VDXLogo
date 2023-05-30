#define RED565 0b1111100000000000
#define GREEN565 0b0000011111100000
#define BLUE565 0b0000000000011111
#define RED1 0b100
#define GREEN1 0b010
#define BLUE1 0b001

const int W = 320;
const int H = 240;

double xCenter = W / 2;
double yCenter = H / 2;

double precision = 2048;
double turtleSize = 12;

int screenColor = 0b111;

class Turtle {
  private:
    bool drawing = true; //pu pd
    int pc = 0b000; //pen color RGB111
    double x = 0; //x pos
    double y = 0; //y pos
    double h = 0; //heading (rotation)
    bool turtleShown = true; //ht, st
  public:
    bool getDrawing() {return drawing;}
    void setDrawing(bool new_val) {drawing = new_val;}
    
    int getPc() {return pc;}
    void setPc(int new_val) {pc = new_val;}
    
    double getX() {return x;}
    void setX(double new_val) {x = new_val;}
    
    double getY() {return y;}
    void setY(double new_val) {y = new_val;}
    
    double getH() {return h;}
    double getHd() {return h / (PI / 180);}
    void setH(double new_val) {
      new_val = new_val * (PI / 180);
      while (new_val >= 0) {
        new_val -= 2 * PI;
      }
      while (new_val < 0) {
        new_val += 2 * PI;
      }
      h = new_val;
    }

    bool getTurtleShown() {return turtleShown;}
    void setTurtleShown(bool new_val) {turtleShown = new_val;}
};

class Inputs {
  public:
    String input = "";
    int iInput = 0;
    int repcount = -1;
    //String command;
    //double value;
    //int iValue;
};
