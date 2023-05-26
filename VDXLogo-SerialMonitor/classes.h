const int W = 320;
const int H = 240;

double xOrigin = W / 2;
double yOrigin = H / 2;

class Turtle {
  private:
    bool drawing = true; //pu pd
    int pc = 0x0000; //pen color RGB565, note: black or white in the screen buffer
    double x = xOrigin; //x pos
    double y = yOrigin; //y pos
    double r = 0; //rotation
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
    
    double getR() {return r;}
    void setR(double new_val) {
      while (new_val >= 0) {
        new_val -= 2 * PI;
      }
      while (new_val < 0) {
        new_val += 2 * PI;
      }
      r = new_val;
    }

    bool getTurtleShown() {return turtleShown;}
    void setTurtleShown(bool new_val) {turtleShown = new_val;}
};

class Inputs {
  public:
    String input;
    int iInput;
    String command;
    double value;
    int iValue;
};
