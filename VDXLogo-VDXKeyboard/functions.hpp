#pragma once

//forward include #include "commands.h"
Inputs execute(Turtle& turtle, Inputs inputs, CanvasRGB& thisCanvas);

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

String getInBrackets(Inputs& inputs, char openBracket = '[', char closedBracket = ']', char endWith = '\\') {
  String tempString = "";
  int depth = 1;
  inputs.iInput++; //assume '['

  while (1) {
    if (inputs.input[inputs.iInput] == openBracket) {
      depth++;
    }
    if (inputs.input[inputs.iInput] == closedBracket) {
      depth--;
    }
    if (depth == 0) {
      break;
    }
    tempString += inputs.input[inputs.iInput];
    
    inputs.iInput++;
  }
  
  tempString += endWith;
  inputs.iInput += 2; //skip '] '
  return tempString;
}

double getValue(Inputs& inputs) {
  if (debugEnabled) Serial.println("value requested");
  double tempValue = 0;
  bool isNegative = 0;

  //is a variable
  if (inputs.input[inputs.iInput] == ':') {
    inputs.iInput++;
    String variableName = getStr(inputs);
    String variableValue = globalVariables.search(variableName);

    if (debugEnabled) Serial.println("Variable: \"" + variableValue + "\"");

    tempValue = parseMath(getSubstring(variableValue, 0, -2));
  
    return tempValue;
  }

  //needs to do math
  if (inputs.input[inputs.iInput] == '(') {
    tempValue = parseMath(getInBrackets(inputs, '(', ')', ' '));
    return tempValue;
  }

  //fallback: get the number
  if (inputs.input[inputs.iInput] == '-') {
    isNegative = true;
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

void make(Inputs& inputs) {
  inputs.iInput++; //assume '"'
  
  String variableName = getStr(inputs);
  String variableValue = getInBrackets(inputs, '[', ']');

  if (variableValue[0] == '(') {
    variableValue = String(parseMath(getSubstring(variableValue, 0, -2)));
  }
  
  globalVariables.set(variableName, variableValue);
};

void repeat(Turtle& turtle, Inputs& inputs, CanvasRGB& thisCanvas = canvas) {
  int repeatCount = int(getValue(inputs));
  String repeatCommands = getInBrackets(inputs, '[', ']');

  for (int repcount = 0; repcount < repeatCount; repcount++) {
    Inputs inputs2;
    inputs2.input = repeatCommands;
    inputs2.repcount = repcount;

    String fakeCommand = "\"repcount " + '[' + String(repcount) + ']';
    Inputs inputs3 = makeInput(fakeCommand);
    make(inputs3);

    while (inputs2.input != "") {
      inputs2 = execute(turtle, inputs2, thisCanvas);
      //updateScreen(turtle);
    }
  }
}

/*double fd2(Turtle& turtle, double val, GFXcanvas1& thisCanvas = visualTurtleCanvas) {
  double newX = turtle.getX() + val * sin(turtle.getH()) * precision;
  double newY = turtle.getY() + val * cos(turtle.getH()) * precision;

  if (turtle.getDrawing()) {
    thisCanvas.drawLine(turtle.getX() / precision + xCenter, turtle.getY() / precision + yCenter, newX / precision + xCenter, newY / precision + yCenter, turtle.getPc());
  }
  turtle.setX(newX);
  turtle.setY(newY);

  return 1;
}*/

int rgb111ToRgb565(int color, int dimmer = 1) {
  return ((color & RED1) * RED565) / dimmer + ((color & GREEN1) * GREEN565) / dimmer + ((color & BLUE1) * BLUE565) / dimmer;
}

void drawVisualTurtle(Turtle& turtle, int tSize) {
  int tSideLenght = sqrt(tSize * tSize / 2);
  turtle.lt(90);
  turtle.fd(tSize / 2);
  turtle.fd(tSize * -1);
  turtle.rt(45);
  turtle.fd(tSideLenght);
  turtle.lt(90);
  turtle.fd(tSideLenght);
}

void updateScreen(Turtle& turtle) {
  canvasT.fillScreen(0);
  //Turtle visualTurtle(visualTurtleCanvas);
  visualTurtle = turtle;
  
  if (visualTurtle.getTurtleShown()) {
    visualTurtle.setCanvas(visualTurtleCanvas);
    visualTurtle.setPc(0b111);
    drawVisualTurtle(visualTurtle, turtleSize);
  }
  
  int tint = rgb111ToRgb565(turtle.getPc(), 1);
  for (int x = 0; x < W; x++) {
    for (int y = 0; y < H; y++) {
      int pixelColor = 0;
      pixelColor += (*canvas.r).getPixel(x, y) * RED565;
      pixelColor += (*canvas.g).getPixel(x, y) * GREEN565;
      pixelColor += (*canvas.b).getPixel(x, y) * BLUE565;
      
      if (canvasT.getPixel(x, y)) { //turtle canvas
        //TFT.drawPixel(x, y, (0xFFFF-pixelColor));
        TFT.drawPixel(x, y, 0xFFFF - pixelColor xor tint); //tint the turtle a bit
      }
      else {
        TFT.drawPixel(x, y, pixelColor);
      }
    }
  }
  
}
