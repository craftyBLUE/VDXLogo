//debug
bool debugEnabled = false;
bool fastTurtle = false;

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

String getInBrackets(Inputs& inputs, char openBracket = '[', char closedBracket = ']') {
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
  
  tempString += '\\';
  inputs.iInput += 2; //skip '] '
  return tempString;
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

  Serial.print("turtleShown: ");
  Serial.println(turtle.getTurtleShown());

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

void setpos(Turtle& turtle, double newX, double newY, GFXcanvas8& thisCanvas = canvas) {
  if (turtle.getDrawing()) {
    thisCanvas.drawLine(turtle.getX() / precision + xCenter, turtle.getY() / precision + yCenter, newX / precision + xCenter, newY / precision + yCenter, turtle.getPc());
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

double fd(Turtle& turtle, double val, GFXcanvas8& thisCanvas = canvas) {
  double moveX = val * sin(turtle.getR()) * precision;
  double moveY = val * cos(turtle.getR()) * precision;

  setpos(turtle, turtle.getX() + moveX, turtle.getY() + moveY, thisCanvas);

  return sqrt(moveX * moveX + moveY * moveY);
}

double bk(Turtle& turtle, double val, GFXcanvas8& thisCanvas = canvas) {
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

double fd2(Turtle& turtle, double val, GFXcanvas1& thisCanvas = visualTurtleCanvas) {
  double newX = turtle.getX() + val * sin(turtle.getR()) * precision;
  double newY = turtle.getY() + val * cos(turtle.getR()) * precision;

  if (turtle.getDrawing()) {
    thisCanvas.drawLine(turtle.getX() / precision + xCenter, turtle.getY() / precision + yCenter, newX / precision + xCenter, newY / precision + yCenter, turtle.getPc());
  }
  turtle.setX(newX);
  turtle.setY(newY);

  return 1;
}

void drawVisualTurtle(Turtle& turtle, int tSize) {
  int tSideLenght = sqrt(tSize * tSize / 2);
  lt(turtle, 90);
  fd2(turtle, tSize / 2, visualTurtleCanvas);
  fd2(turtle, tSize * -1, visualTurtleCanvas);
  rt(turtle, 45);
  fd2(turtle, tSideLenght, visualTurtleCanvas);
  lt(turtle, 90);
  fd2(turtle, tSideLenght, visualTurtleCanvas);
}

int rgba2222torgb565(int pixelColor) {
  int red = pixelColor & 0b11000000;
  int green = pixelColor & 0b00110000;
  int blue = pixelColor & 0b00001100;
  int alpha = pixelColor & 0b00000011;

  red = red / 0b01000000 * RED565 / 3;
  green = green / 0b00010000 * GREEN565 / 3;
  blue = blue / 0b00000100 * BLUE565 / 3;

  return red + green + blue;
}

void updateScreen(Turtle& turtle) {
  visualTurtleCanvas.fillScreen(0);
  Turtle visualTurtle;
  visualTurtle = turtle;
  visualTurtle.setPc(0xFFFF);

  if (visualTurtle.getTurtleShown()) {
    drawVisualTurtle(visualTurtle, turtleSize);
  }

  for (int x = 0; x < W; x++) {
    for (int y = 0; y < H; y++) {
      int pixelColor = rgba2222torgb565(canvas.getPixel(x, y));
      if (visualTurtleCanvas.getPixel(x, y)) {
        TFT.drawPixel(x, y, (0xFFFF-pixelColor));
      }
      else {
        TFT.drawPixel(x, y, pixelColor);
      }
    }
  }
  
}
