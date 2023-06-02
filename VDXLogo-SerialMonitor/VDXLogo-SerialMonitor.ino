#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>

#include "classes.h"

//screen
Adafruit_ILI9341 TFT = Adafruit_ILI9341(5, 21, 4); //CS, DC, T_CS

GFXcanvas1 canvasR(W, H);
GFXcanvas1 canvasG(W, H);
GFXcanvas1 canvasB(W, H);

class canvasRGB {
  public:
    GFXcanvas1& r = canvasR;
    GFXcanvas1& g = canvasG;
    GFXcanvas1& b = canvasB;
};

canvasRGB canvas;
GFXcanvas1 visualTurtleCanvas(W, H);

Turtle dummyTurtle;

//debug
bool debugEnabled = false;
bool fastTurtle = false;

//globalVariables = new stringLinkedList();
stringLinkedList globalVariables;

#include "math_parser.h"

#include "functions.h"
#include "commands.h"

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

  //initialize math parser
  initializeMathParser();

  //initialize global variables
  globalVariables.set("square50", "repeat 4 [fd 50 rt 90]");
  globalVariables.set("triangle50left", "repeat 3 [fd 50 rt 90]");
  globalVariables.set("makeexample", "make \"n [10] make \"d [30] make \"r [3] run makeexample2");
  globalVariables.set("makeexample2", "repeat :n [repeat :r [fd :d lt (360/:r)] fd :d rt (360/:n) make \"r [(:r+1)]]");
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
    
    inputs = execute(turtle, inputs);
  
    //updateScreen(turtle);
  }
}
