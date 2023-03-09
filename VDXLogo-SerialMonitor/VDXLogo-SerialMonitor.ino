#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>

//screen
Adafruit_ILI9341 TFT = Adafruit_ILI9341(5, 21, 4); //CS, DC, T_CS

int W, H;

double xOrigin = 320/2;
double yOrigin = 240/2;

//debug
bool debugEnabled = false;

//input
//Vector vInput; //input queue !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
String input;
int iInput;
String command;
double value;
int iValue;

void setup() {
  Serial.begin(2000000);
  TFT.begin();
  TFT.setRotation(3);
  W = TFT.width();
  H = TFT.height();

  Serial.print("\n");
  Serial.println("                   ");
  Serial.println("===   VDXLOGO   ===");
  Serial.println("     by craftyBLUE");
  Serial.println("                   ");
  Serial.println("                   ");

  iInput = 0;
  input = "cs lt 180\\";
}

class Turtle {
  private:
    bool drawing = true; //pu pd
    int pc = 0x0000; //pen color RGB565
    double x = xOrigin; //x pos
    double y = yOrigin; //y pos
    double r = 0; //rotation
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
};

Turtle turtle;

bool legalChar(char tempChar) {
  if (tempChar >= 'a' and tempChar <= 'z') return 1;
  if (tempChar >= 'A' and tempChar <= 'Z') return 1;
  if (tempChar >= '0' and tempChar <= '9') return 1;
  if (tempChar == '.') return 1;
  return 0;
}

String getStr() {
  String tempString = "";
  for (; legalChar(input[iInput]) and input[iInput] != ' '; iInput++) {
    tempString += input[iInput];
  }
  iInput++;
  return tempString;
}

double getValue() {
  double tempValue = 0;
  bool isNegative = 0;
  if (input[iInput] == '-') {
    isNegative++;
    iInput++;
  }
  
  tempValue = getStr().toDouble();

  if (isNegative) return -1 * tempValue;
  
  return tempValue;
}

String nextCommand() {
  String tempString = "";
  for (; iInput != '\\'; iInput++) {
    tempString += input[iInput];
  }
  iInput = 0;
  return tempString;
}

int printTurtleStatus() {
  if (!debugEnabled) return 1;
  Serial.println("Current turtle status: ");

  Serial.print("drawing: ");
  Serial.println(turtle.getDrawing());

  Serial.print("pc: ");
  Serial.println(turtle.getPc());
  
  Serial.print("x: ");
  Serial.println(turtle.getX());

  Serial.print("y: ");
  Serial.println(turtle.getY());

  Serial.print("r: ");
  Serial.println(turtle.getR());

  Serial.println();
  return 0;
}

void setpos(double newX, double newY) {
  if (turtle.getDrawing()) {
    TFT.drawLine(turtle.getX(), turtle.getY(), newX, newY, turtle.getPc());
  }
  turtle.setX(newX);
  turtle.setY(newY);
  printTurtleStatus();
  return;
}

double lt(double val) {
  turtle.setR(turtle.getR() + val * (PI / 180));
  printTurtleStatus();
  return turtle.getR();
}

double rt(double val) {
  return lt(val * -1);
}

double fd(double val) {
  double moveX = val * sin(turtle.getR());
  double moveY = val * cos(turtle.getR());

  if (turtle.getDrawing()) {
    TFT.drawLine(turtle.getX(), turtle.getY(), turtle.getX() + moveX, turtle.getY() + moveY, turtle.getPc());
  }
  turtle.setX(turtle.getX() + moveX);
  turtle.setY(turtle.getY() + moveY);
  printTurtleStatus();
  return sqrt(moveX * moveX + moveY * moveY);
}

double bk(double val) {
  return fd(val * -1);
}

void pu() {
  turtle.setDrawing(0);
}

void pd() {
  turtle.setDrawing(1);
}

void repeat(double repeatCount, int jInput) {
  jInput++;
  while (repeatCount > 0) {
    while (input[jInput] != ']') {
      Serial.print(input[jInput]);
      jInput++;
    }
  }
  Serial.println(" ");
  jInput += 2;
  iInput = jInput;
  return;
}

void logo() {
  input = "pu bk 100 rt 90 bk 100 lt 90 pd rt 45 fd 120 bk 120 rt 45 fd 120 lt 90 fd 120 logo2\\"; 
  iInput = 0;
}
void logo2() {
  input = "lt 180 rt 45 fd 120 bk 120 rt 45 fd 60 lt 45 fd 40 lt 90 fd 12 bk 24 logo3\\";
  iInput = 0;
}
void logo3() {
  input = "rt 45 fd 16.9706 lt 90 fd 16.9706 lt 180 rt 45 fd 12 lt 90\\";
  iInput = 0;
}

void loop() {  
  if (Serial.available() and input == "") {
    input = Serial.readString();
    input.trim();

    input += "\\";
    
    iInput = 0;
  }

  if (input != "") {
    Serial.println(input);
  }
  
  command = getStr();

  if (command == "reset") {
    ESP.restart();
  }
  if (command == "cs") {
    setpos(xOrigin, yOrigin);
    TFT.fillScreen(0xFFFF);
  }
  if (command == "pu") {
    pu();
  }
  if (command == "pd") {
    pd();
  }
  if (command == "lt") {
    value = getValue();
    lt(value);
  }
  if (command == "rt") {
    value = getValue();
    rt(value);
  }
  if (command == "fd") {
      value = getValue();

      fd(value);
  }
  if (command == "bk") {
      value = getValue();

      bk(value);
  }
  if (command == "repeat") {
    value = getValue();

    repeat(value, iInput);
  }
  if (command == "logo") {
    logo();
    return;
  }
  if (command == "logo2") {
    logo2();
    return;
  }
  if (command == "logo3") {
    logo3();
    return;
  }
  if (command == "toggledebug") {
    debugEnabled = 1 - debugEnabled;
  }
  if (command == "setpos") {
    setpos(getValue(), getValue());
  }
  
  input = nextCommand();
  if (input != "" and debugEnabled) {
    Serial.print("next commands: ");
    Serial.println(input);
  }
}
