Inputs execute(Turtle& turtle, Inputs inputs, canvasRGB& thisCanvas = canvas) {
  String command = getStr(inputs);

  if (command == "reset") {
    ESP.restart();
  }
  if (command == "cs") {
    setpos(turtle, 0, 0);
    turtle.setH(0);
    lt(turtle, 180);
    thisCanvas.r.fillScreen(screenColor & RED1);
    thisCanvas.g.fillScreen(screenColor & GREEN1);
    thisCanvas.b.fillScreen(screenColor & BLUE1);
  }
  if (command == "pu") {
    pu(turtle);
  }
  if (command == "pd") {
    pd(turtle);
  }
  if (command == "lt") {
    lt(turtle, getValue(inputs));
  }
  if (command == "rt") {
    rt(turtle, getValue(inputs));
  }
  if (command == "fd") {
      fd(turtle, getValue(inputs), thisCanvas);
  }
  if (command == "bk") {
      bk(turtle, getValue(inputs), thisCanvas);
  }
  if (command == "toggledebug") {
    debugEnabled = 1 - debugEnabled;
  }
  if (command == "setpos") {
    setpos(turtle, getValue(inputs) * precision, getValue(inputs) * precision, thisCanvas);
  }
  if (command == "ht") {
    ht(turtle);
  }
  if (command == "st") {
    st(turtle);
  }
  if (command == "repeat") {
    int repeatCount = int(getValue(inputs));
    String repeatCommands = getInBrackets(inputs, '[', ']');

    for (int repcount = 0; repcount < repeatCount; repcount++) {
      Inputs inputs2;
      inputs2.input = repeatCommands;
      inputs2.repcount = repcount;

      while (inputs2.input != "") {
        inputs2 = execute(turtle, inputs2, thisCanvas);
        //updateScreen(turtle);
      }
    } 
  }
  if (command == "setprecision") {
    precision = getValue(inputs);
  }
  if (command == "setturtlesize") {
    turtleSize = getValue(inputs);
  }
  if (command == "update") {
    updateScreen(turtle);
  }
  if (command == "slowturtle") {
    fastTurtle = 0;
  }
  if (command == "fastturtle") {
    fastTurtle = 1;
  }
  if (command == "setpc") {
    turtle.setPc(getValue(inputs));
  }
  if (command == "setsc") {
    screenColor = getValue(inputs);
  }
  if (command == "seth") {
    turtle.setH(getValue(inputs));
  }
  if (command == "home") {
    Inputs inputs2;
    inputs2.input = "setpos 0 0 seth 0 lt 180";
    inputs = execute(turtle, inputs2, thisCanvas);
  }
  
  inputs.input = nextCommand(inputs);
  if (inputs.input != "" and debugEnabled) {
    Serial.print("next commands: ");
    Serial.println(inputs.input);
  }

  if (!fastTurtle) {
    updateScreen(turtle);
  }
  
  return inputs;
}
