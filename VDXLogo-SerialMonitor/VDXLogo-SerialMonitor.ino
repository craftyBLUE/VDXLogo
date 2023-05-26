#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>

#include "classes.h"

//screen
Adafruit_ILI9341 TFT = Adafruit_ILI9341(5, 21, 4); //CS, DC, T_CS

GFXcanvas1 canvas(W, H);
GFXcanvas1 visualTurtleCanvas(W, H);

Turtle dummyTurtle;

//int W, H;

//double xOrigin = 320/2;
//double yOrigin = 240/2;

//debug
bool debugEnabled = false;

void setup() {
  Serial.begin(2000000);
  TFT.begin();
  TFT.setRotation(3);
  //W = TFT.width();
  //H = TFT.height();

  Serial.print("\n");
  Serial.println("                   ");
  Serial.println("===   VDXLOGO   ===");
  Serial.println("     by craftyBLUE");
  Serial.println("                   ");
  Serial.println("                   ");
}

bool legalChar(char tempChar) {
  if (tempChar >= 'a' and tempChar <= 'z') return 1;
  if (tempChar >= 'A' and tempChar <= 'Z') return 1;
  if (tempChar >= '0' and tempChar <= '9') return 1;
  if (tempChar == '.') return 1;
  return 0;
}

String getStr(Inputs& inputs) {
  String tempString = "";
  for (; legalChar(inputs.input[inputs.iInput]) and inputs.input[inputs.iInput] != ' '; inputs.iInput++) {
    tempString += inputs.input[inputs.iInput];
  }
  inputs.iInput++;
  return tempString;
}

double getValue(Inputs& inputs) {
  double tempValue = 0;
  bool isNegative = 0;
  if (inputs.input[inputs.iInput] == '-') {
    isNegative++;
    inputs.iInput++;
  }
  
  tempValue = getStr(inputs).toDouble();

  if (isNegative) return -1 * tempValue;
  
  return tempValue;
}

String nextCommand(Inputs& inputs) {
  String tempString = "";
  for (; inputs.iInput != '\\'; inputs.iInput++) {
    tempString += inputs.input[inputs.iInput];
  }
  inputs.iInput = 0;
  return tempString;
}

int printTurtleStatus(Turtle& turtle) {
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

void setpos(Turtle& turtle, double newX, double newY) {
  if (turtle.getDrawing()) {
    canvas.drawLine(turtle.getX(), turtle.getY(), newX, newY, turtle.getPc());
  }
  turtle.setX(newX);
  turtle.setY(newY);
  printTurtleStatus(turtle);
  return;
}

double lt(Turtle& turtle, double val) {
  turtle.setR(turtle.getR() + val * (PI / 180));
  printTurtleStatus(turtle);
  return turtle.getR();
}

double rt(Turtle& turtle, double val) {
  return lt(turtle, val * -1);
}

double fd(Turtle& turtle, double val, GFXcanvas1& thisCanvas = canvas) {
  double moveX = val * sin(turtle.getR());
  double moveY = val * cos(turtle.getR());

  if (turtle.getDrawing()) {
    thisCanvas.drawLine(turtle.getX(), turtle.getY(), turtle.getX() + moveX, turtle.getY() + moveY, turtle.getPc());
  }
  turtle.setX(turtle.getX() + moveX);
  turtle.setY(turtle.getY() + moveY);
  printTurtleStatus(turtle);
  return sqrt(moveX * moveX + moveY * moveY);
}

double bk(Turtle& turtle, double val, GFXcanvas1& thisCanvas = canvas) {
  return fd(turtle, val * -1, thisCanvas);
}

void pu(Turtle& turtle) {
  turtle.setDrawing(0);
}

void pd(Turtle& turtle) {
  turtle.setDrawing(1);
}

void ht(Turtle& turtle) {
  turtle.setTurtleShown(0);
}

void st(Turtle& turtle) {
  turtle.setTurtleShown(1);
}

void logo(Inputs& inputs) {
  inputs.input = "pu bk 100 rt 90 bk 100 lt 90 pd rt 45 fd 120 bk 120 rt 45 fd 120 lt 90 fd 120 logo2\\"; 
  inputs.iInput = 0;
}
void logo2(Inputs& inputs) {
  inputs.input = "lt 180 rt 45 fd 120 bk 120 rt 45 fd 60 lt 45 fd 40 lt 90 fd 12 bk 24 logo3\\";
  inputs.iInput = 0;
}
void logo3(Inputs& inputs) {
  inputs.input = "rt 45 fd 16.9706 lt 90 fd 16.9706 lt 180 rt 45 fd 12 lt 90\\";
  inputs.iInput = 0;
}

void drawVisualTurtle(Turtle& turtle, int tSize) {
  int tSideLenght = sqrt(tSize * tSize / 2);
  lt(turtle, 90);
  fd(turtle, tSize / 2, visualTurtleCanvas);
  bk(turtle, tSize, visualTurtleCanvas);
  rt(turtle, 45);
  fd(turtle, tSideLenght, visualTurtleCanvas);
  lt(turtle, 90);
  fd(turtle, tSideLenght, visualTurtleCanvas);
}

void updateScreen(Turtle& turtle) {
  visualTurtleCanvas.fillScreen(0x0000);
  Turtle visualTurtle;
  visualTurtle = turtle;
  visualTurtle.setPc(0xFFFF);

  if (visualTurtle.getTurtleShown()) {
    drawVisualTurtle(visualTurtle, 12);
  }

  for (int x = 0; x < W; x++) {
    for (int y = 0; y < H; y++) {
      if (visualTurtleCanvas.getPixel(x, y)) {
        TFT.drawPixel(x, y, (1-canvas.getPixel(x, y)) * 0xFFFF);
      }
      else {
        TFT.drawPixel(x, y, canvas.getPixel(x, y) * 0xFFFF);
      }
    }
  }
  
}

void loop() { //fake loop
  // "global" variables
  Inputs inputs;

  Turtle turtle;

  inputs.iInput = 0;
  inputs.input = "cs\\";
  
  while (true) { //loop
  if (Serial.available() and inputs.input == "") {
    inputs.input = Serial.readString();
    inputs.input.trim();

    inputs.input += "\\";
    
    inputs.iInput = 0;
  }

  if (inputs.input != "") {
    Serial.println(inputs.input);
  }
  
  inputs.command = getStr(inputs);

  if (inputs.command == "reset") {
    ESP.restart();
  }
  if (inputs.command == "cs") {
    setpos(turtle, xOrigin, yOrigin);
    turtle.setR(0);
    lt(turtle, 180);
    canvas.fillScreen(0xFFFF);
  }
  if (inputs.command == "pu") {
    pu(turtle);
  }
  if (inputs.command == "pd") {
    pd(turtle);
  }
  if (inputs.command == "lt") {
    inputs.value = getValue(inputs);
    lt(turtle, inputs.value);
  }
  if (inputs.command == "rt") {
    inputs.value = getValue(inputs);
    rt(turtle, inputs.value);
  }
  if (inputs.command == "fd") {
      inputs.value = getValue(inputs);

      fd(turtle, inputs.value);
  }
  if (inputs.command == "bk") {
      inputs.value = getValue(inputs);

      bk(turtle, inputs.value);
  }
  if (inputs.command == "logo") {
    logo(inputs);
    return;
  }
  if (inputs.command == "logo2") {
    logo2(inputs);
    return;
  }
  if (inputs.command == "logo3") {
    logo3(inputs);
    return;
  }
  if (inputs.command == "toggledebug") {
    debugEnabled = 1 - debugEnabled;
  }
  if (inputs.command == "setpos") {
    setpos(turtle, getValue(inputs), getValue(inputs));
  }
  if (inputs.command == "ht") {
    ht(turtle);
  }
  if (inputs.command == "st") {
    st(turtle);
  }
  
  inputs.input = nextCommand(inputs);
  if (inputs.input != "" and debugEnabled) {
    Serial.print("next commands: ");
    Serial.println(inputs.input);
  }

  updateScreen(turtle);
  }
}
