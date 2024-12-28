#pragma once

#include "functions.hpp"

Inputs execute(Turtle& turtle, Inputs inputs, CanvasRGB& thisCanvas = canvas) {
  //for better performance maybe put this check somewhere where it isn't checked so often
  interruptsCheck();
  
  String command = getStr(inputs);

  if (command == "reset") {
    ESP.restart();
  }
  else if (command == "cs") {
    turtle.setpos(0, 0);
    turtle.setHd(0);
    turtle.lt(180);
    (*thisCanvas.r).fillScreen(screenColor & RED1);
    (*thisCanvas.g).fillScreen(screenColor & GREEN1);
    (*thisCanvas.b).fillScreen(screenColor & BLUE1);
  }
  else if (command == "pu") {
    turtle.pu();
  }
  else if (command == "pd") {
    turtle.pd();
  }
  else if (command == "lt") {
    turtle.lt(getValue(inputs));
  }
  else if (command == "rt") {
    turtle.rt(getValue(inputs));
  }
  else if (command == "fd") {
    turtle.fd(getValue(inputs));
  }
  else if (command == "bk") {
    turtle.bk(getValue(inputs));
  }
  else if (command == "toggledebug") {
    debugEnabled = 1 - debugEnabled;
  }
  else if (command == "setpos") {
    turtle.setpos(getValue(inputs) * precision, getValue(inputs) * precision);
  }
  else if (command == "ht") {
    turtle.ht();
  }
  else if (command == "st") {
    turtle.st();
  }
  else if (command == "repeat") {
    repeat(turtle, inputs, thisCanvas);
  }
  else if (command == "setprecision") {
    precision = getValue(inputs);
  }
  else if (command == "setturtlesize") {
    turtleSize = getValue(inputs);
  }
  else if (command == "update") {
    updateScreen(turtle);
  }
  else if (command == "slowturtle" or command == "slow") {
    fastTurtle = 0;
  }
  else if (command == "fastturtle" or command == "fast") {
    fastTurtle = 1;
  }
  else if (command == "setpc") {
    turtle.setPc(getValue(inputs));
  }
  else if (command == "setsc") {
    screenColor = getValue(inputs);
  }
  else if (command == "seth") {
    turtle.setHd(getValue(inputs));
  }
  else if (command == "home") {
    inputs = execute(turtle, makeInput("setpos 0 0 seth 0 lt 180"), thisCanvas);
  }
  else if (command == "run") { //nice to have
    inputs = execute(turtle, makeInput(globalVariables.search(getStr(inputs))), thisCanvas);
  }
  else if (command == "make") {
    make(inputs);
  }
  else if (command == "if") {
    String ifConditional = getInBrackets(inputs, '(', ')');
    String ifTrue = getInBrackets(inputs, '[', ']');

    //if (debugEnabled) Serial.println("If: " + String(parseMath(ifConditional)));
    if (parseMath(ifConditional)) {
      execute(turtle, makeInput(ifTrue), thisCanvas);
    }
  }
  else {
    String customCommand = globalVariables.search(command);
    if (customCommand) {
      inputs = execute(turtle, makeInput(customCommand), thisCanvas);
    }
    /*else {
      command not found
    }*/
  }
  
  inputs.input = nextCommand(inputs);
  /*if (inputs.input != "" and debugEnabled) {
    Serial.print("next commands: ");
    Serial.println(inputs.input);
  }*/

  if (!fastTurtle) {
    updateScreen(turtle);
  }
  
  return inputs;
}
