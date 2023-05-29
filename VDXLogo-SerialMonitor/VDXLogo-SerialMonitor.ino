#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>

#include "classes.h"

//screen
Adafruit_ILI9341 TFT = Adafruit_ILI9341(5, 21, 4); //CS, DC, T_CS

GFXcanvas8 canvas(W, H);
GFXcanvas1 visualTurtleCanvas(W, H);

Turtle dummyTurtle;

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
