#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>
//#include <PS2Keyboard.h>

#include "device_constants.h"

GFXcanvas1 canvasT(W, H); //very dumb workaround to not initialize references to GFXcanvas1 in CanvasRGB
                          //also serves as turtle canvas

#include "classes.hpp"

int btnA = 32;
int btnB = 33;

//screen
Adafruit_ILI9341 TFT = Adafruit_ILI9341(5, 21, 4); //CS, DC, T_CS

GFXcanvas1 canvasR(W, H);
GFXcanvas1 canvasG(W, H);
GFXcanvas1 canvasB(W, H);
CanvasRGB canvas('3', canvasR, canvasG, canvasB);

//GFXcanvas1 canvasT(W, H); ad. "very dumb workaround"
CanvasRGB visualTurtleCanvas('1', canvasT, canvasT, canvasT); //pray to Terry A. Davis that this only copies references

Turtle visualTurtle(visualTurtleCanvas);

//globalVariables = new stringLinkedList();
stringLinkedList globalVariables;

#include "math_parser.hpp"

#include "dpad.hpp"
#include "VDXKeyboard.hpp"
#include "VDXMenu.hpp"

#include "functions.hpp"
#include "commands.hpp"

void setup() {
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  pinMode(btnMenu, INPUT_PULLUP);
  
  //Serial.begin(2000000); debug disabled
  TFT.begin();
  TFT.setRotation(3);
  TFT.setTextColor(0b0000000000000000);
  //W = TFT.width();
  //H = TFT.height();

  /* debug disabled
  Serial.print("\n");
  Serial.println("                   ");
  Serial.println("===   VDXLOGO   ===");
  Serial.println("     by craftyBLUE");
  Serial.println("                   ");
  Serial.println("                   ");
  */
  

  //keyboard.begin(5, 4);

  attachInterrupt(btnMenu, isr_menu, CHANGE);
  attachInterrupt(btnB, isr_btnB, CHANGE);
  
  //initialize math parser
  initializeMathParser();

  //initialize global variables
  globalVariables.set("square50", "repeat 4 [fd 50 rt 90]");
  globalVariables.set("triangle50left", "repeat 3 [fd 50 rt 90]");
  globalVariables.set("makeexample", "make \"n [10] make \"d [30] make \"r [3] makeexample2");
  globalVariables.set("makeexample2", "repeat :n [repeat :r [fd :d lt (360/:r)] fd :d rt (360/:n) make \"r [(:r+1)]]");
}

void loop() { //fake loop
  // "global" variables
  Inputs inputs;

  Turtle turtle(canvas);

  inputs.iInput = 0;
  inputs.input = "cs\\";
  
  while (true) { //loop
    /*if (Serial.available() and inputs.input == "") {
      inputs.input = Serial.readString();
      inputs.input.trim();
  
      inputs.input += "\\";
      
      inputs.iInput = 0;
    }
  
    if (inputs.input != "") {
      Serial.println(inputs.input);
    }*/

    if (inputs.input == "") {
      inputs.input = type() + "\\";
      inputs.iInput = 0;
      //Serial.println(inputs.input);
    }
    
    inputs = execute(turtle, inputs);
  
    //updateScreen(turtle);
  }
}
