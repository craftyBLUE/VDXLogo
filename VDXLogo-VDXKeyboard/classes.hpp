class CanvasRGB {
  public:
    char colors;
    GFXcanvas1* r;
    GFXcanvas1* g;
    GFXcanvas1* b;

    CanvasRGB(char newColors, GFXcanvas1& rNew, GFXcanvas1& gNew, GFXcanvas1& bNew) {
      colors = newColors;
      r = &rNew;
      g = &gNew;
      b = &bNew;
    }
};

class Turtle {
  private:
    CanvasRGB* thisCanvas;
    bool drawing = true; //pu pd
    int pc = 0b000; //pen color RGB111
    double x = 0; //x pos
    double y = 0; //y pos
    double z = 1; //z pos
    double h = 0; //heading (rotation, z axis)
    double p = 0; //pitch (rotation, x axis)
    double r = 0; //roll (rotation, y axis)
    bool turtleShown = true; //ht, st
  public:
    //Turtle() {}
    Turtle(CanvasRGB& newCanvas) {
      thisCanvas = &newCanvas;
    }
    
    void setCanvas(CanvasRGB& newCanvas) {
      thisCanvas = &newCanvas;
    }
    
    bool getDrawing() {return drawing;}
    void setDrawing(bool new_val) {drawing = new_val;}
    
    int getPc() {return pc;}
    void setPc(int new_val) {pc = new_val;}
    
    double getX() {return x;}
    void setX(double new_val) {x = new_val;}
    
    double getY() {return y;}
    void setY(double new_val) {y = new_val;}

    double getZ() {return z;}
    void setZ(double new_val) {z = new_val;}
    
    double getH() {return h;}
    double getHd() {return h / (PI / 180);}
    void setHd(double new_val) {
      new_val = new_val * (PI / 180);
      while (new_val >= 0) {
        new_val -= 2 * PI;
      }
      while (new_val < 0) {
        new_val += 2 * PI;
      }
      h = new_val;
    }

    double getP() {return p;}
    double getPd() {return p / (PI / 180);}
    void setPd(double new_val) {
      new_val = new_val * (PI / 180);
      while (new_val >= 0) {
        new_val -= 2 * PI;
      }
      while (new_val < 0) {
        new_val += 2 * PI;
      }
      p = new_val;
    }

    double getR() {return r;}
    double getRd() {return r / (PI / 180);}
    void setRd(double new_val) {
      new_val = new_val * (PI / 180);
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

    int printTurtleStatus() {
      return 1;
      if (!debugEnabled) return 1;
      Serial.println("Current turtle status: ");
    
      Serial.print("drawing: ");
      Serial.println(getDrawing());
    
      Serial.print("turtleShown: ");
      Serial.println(getTurtleShown());
    
      Serial.print("pc: ");
      Serial.println(getPc());
      
      Serial.print("x: ");
      Serial.println(getX());
    
      Serial.print("y: ");
      Serial.println(getY());

      Serial.print("z: ");
      Serial.println(getZ());
    
      Serial.print("h: ");
      Serial.println(getH());
    
      Serial.println();
      return 0;
    }

    void setpos(double newX2, double newY2, double newZ2 = 1) {
      double oldX = getX() / precision + xCenter;
      double oldY = getY() / precision + yCenter;
      double oldZ = 1; //getZ() / precision + zCenter;
      double newX = newX2 / precision + xCenter;
      double newY = newY2 / precision + yCenter;
      double newZ = 1; //newZ2 / precision + zCenter;
      if (getDrawing()) {
        (*(*thisCanvas).r).drawLine(oldX / oldZ, oldY / oldZ, newX / newZ, newY / newZ, getPc() & RED1);
        if ((*thisCanvas).colors == '3') { //assume '1' otherwise 
          (*(*thisCanvas).g).drawLine(oldX / oldZ, oldY / oldZ, newX / newZ, newY / newZ, getPc() & GREEN1);
          (*(*thisCanvas).b).drawLine(oldX / oldZ, oldY / oldZ, newX / newZ, newY / newZ, getPc() & BLUE1);
        }
      }
      setX(newX2);
      setY(newY2);
      setZ(newZ2);
      printTurtleStatus();
    }

    double lt(double val) {
      setHd(getHd() + val);
      printTurtleStatus();
      return getHd();
    }
    
    double rt(double val) {
      return lt(val * -1);
    }

    double fd(double val) {
      double moveX = val * sin(getH()) * precision;
      double moveY = val * cos(getH()) * precision;
    
      setpos(getX() + moveX, getY() + moveY);
    
      return sqrt(moveX * moveX + moveY * moveY);
    }
    
    double bk(double val) {
      return fd(val * -1);
    }

    void pu() {
      setDrawing(0);
    }
    
    void pd() {
      setDrawing(1);
    }
    
    void ht() {
      setTurtleShown(0);
    }
    
    void st() {
      setTurtleShown(1);
    }
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

Inputs makeInput(String input) {
  Inputs inputs2;
  inputs2.input = input;
  return inputs2;
}

class stringLinkedList {
  public:
    String id = "";
    String value = "";
    stringLinkedList* next = this;

    
    stringLinkedList* set(String newId, String newValue) {
      stringLinkedList* current;
      current = this;

      while (current -> next != current) {
        if (current -> id == newId) { //replace old value
          //Serial.println("LL.set: replaced " + current -> value);
          current -> value = newValue;
          return current;
        }

        current = current -> next;
        //String debugString = "LL.set: next, " + current -> id + " " + current -> value;
        //Serial.println(debugString);
      }

      //Serial.println("LL.set: appending...");

      stringLinkedList* newNode = new stringLinkedList();
      newNode -> id = newId;
      newNode -> value = newValue;
      newNode -> next = newNode;
      current -> next = newNode; 
      
      return newNode;
    }

    String search(String searchId) {
      stringLinkedList* current;
      current = this;

      while (current -> next != current) {
        current = current -> next;
        if (current -> id == searchId) {
          return current -> value;
        }
      }
      return "";
    }
};
