class Operator {
  public:
    char op;
    int priority;
};

const int numberOfOperators = 7;
Operator operators[numberOfOperators]; //op for short

void initializeMathParser() {
  int pri = 1;
  
  operators[0].op = '*';
  operators[0].priority = pri;
  operators[1].op = '/';
  operators[1].priority = pri;
  
  pri++;
  operators[2].op = '+';
  operators[2].priority = pri;
  operators[3].op = '-';
  operators[3].priority = pri;

  pri++;
  operators[4].op = '=';
  operators[4].priority = pri;
  operators[5].op = '<';
  operators[5].priority = pri;
  operators[6].op = '>';
  operators[6].priority = pri;
  
  return;
}

bool isOperator(char tempChar) {
  for (int i = 0; i < numberOfOperators; i++) {
    if (tempChar == operators[i].op) {
      return 1;
    }
  }
  return 0;
}

String getSubstring(String tempString, int fromIndex, int toIndex) {
  if (toIndex < 0) {
    toIndex = tempString.length() + toIndex;
  }
  
  String tempSubstring = "";
  for (int i = fromIndex; i <= toIndex; i++) {
    tempSubstring += tempString[i];
  }
  return tempSubstring;
}

int getPriority(char op) {
  for (int i = 0; i < numberOfOperators; i++) {
    if (op == operators[i].op) {
      return operators[i].priority;
    }
  }
  return 0; //shouldn't happen
}

bool isPrioritized(char op1, char op2) {
  int op1Priority = getPriority(op1);
  int op2Priority = getPriority(op2);
  
  if (op1Priority > op2Priority) {
    return true;
  }
  return false;
}

String stripString(String tempString) {
  while (tempString != " " and tempString[0] == ' ') {
    tempString = getSubstring(tempString, 1, -1);
  }
  while (tempString != " " and tempString[tempString.length() - 1] == ' ') {
    tempString = getSubstring(tempString, 0, -2);
  }
  return tempString;
}

int charToInt(char tempChar) {
  return tempChar - '0';
}

int stringToInt(String tempString) {
  return tempString.toInt();
  
  /*int result = 0;
  //for (int i = tempString.size() - 1; i >= 0; i--) {
  for (int i = 0; i < tempString.length(); i++) {
    result = result * 10 + charToInt(tempString[i]);
  }
  return result;*/
}

double stringToDouble(String tempString) {
  return tempString.toDouble();
  
  /*//to do: add scientific notation
  int whole = 0;
  int decimal = 0;
  
  int decimalIndex = tempString.length();
  for (int i = 0; i < tempString.length(); i++) {
    if (tempString[i] == '.') {
      decimalIndex = i;
      break;
    }
  }
  
  whole = stringToInt(getSubstring(tempString, 0, decimalIndex -1));
  decimal = stringToInt(getSubstring(tempString, decimalIndex + 1, -1));
  
  return whole + (decimal / pow(10, (tempString.length() - decimalIndex - 1)));*/
}

double parseMath(String mathInput) {
  /*if (debugEnabled) {
    String debugString = "parseMath(\"" + mathInput + "\")";
    Serial.println(debugString);
  }*/
  
  if (mathInput == "") return 0;
  
  //remove garbage
  mathInput = stripString(mathInput);
  if (mathInput[0] == '(' and mathInput[mathInput.length() - 1] == ')') {
    return parseMath(getSubstring(mathInput, 1, -2));
  }
  
  //find the prioritized operator
  int currentOpIndex = -1;
  int depth = 0;
  for (int i = 0; i < mathInput.length(); i++) {
    char currentChar = mathInput[i];
    if (depth == 0 and isOperator(currentChar)) {
      if (currentOpIndex == -1) {
        currentOpIndex = i;
      }
      else if (isPrioritized(currentChar, mathInput[currentOpIndex])) {
        currentOpIndex = i;
      }
    }
    else if (currentChar == '(') {
      depth++;
    }
    else if (currentChar == ')') {
      depth--;
    }
  }
  
  //if no operator was found
  if (currentOpIndex == -1) {
    if (mathInput[0] == ':') {
      String variableValue = globalVariables.search(getSubstring(mathInput, 1, -1));
      return parseMath(getSubstring(variableValue, 0, -2));
    }
    return stringToDouble(mathInput);
  }
  
  //execute the operation
  double value1 = parseMath(getSubstring(mathInput, 0, currentOpIndex - 1));
  double value2 = parseMath(getSubstring(mathInput, currentOpIndex + 1, -1));
  switch (mathInput[currentOpIndex]) {
    case '*':
      return value1 * value2;
    case '/':
      return value1 / value2;
    case '+':
      return value1 + value2;
    case '-':
      return value1 - value2;
    case '=':
      return value1 == value2;
    case '<':
      return value1 < value2;
    case '>':
      return value1 > value2;
  }
  
  //default return value
  return 0;
}
