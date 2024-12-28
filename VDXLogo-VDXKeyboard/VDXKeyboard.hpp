#pragma once

//forward decleration #include "VDXMenu.h"
void interruptsCheck();

String keypad[4][4] = {{"1.:\",", "4ghi", "7pqrs", "*()"}, {"2abc", "5jkl", "8tuv", "0 []"}, {"3def", "6mno", "9wxyz", "#/?!"}, {"@@", "@", "+-$%~", "=<>_"}};
/*
 * '@' = function overridden later (backspace, delete and enter) to do: add custom sprites
*/

//PS2Keyboard keyboard;

int inputMethod = 2;
/*
 * 0 = reserved for serial
 * 1 = 1 button t9 style keyboard
 * 2 = dpad t9 style
 * 3 = ps2 keyboard (currently disabled)
*/

unsigned long btnBLastPressTime = 0;
bool btnBHeld = false;

void drawKeypad(int selectedX, int selectedY) {
  TFT.fillRect(screenW - 132, screenH - 44, 132, 44, 0b1111111111111111);
  TFT.setTextSize(1);
  for (int ix = 0; ix < 4; ix++) {
    for (int iy = 0; iy < 4; iy++) {
      int buttonPosX = (screenW - 132) + ix * 33;
      int buttonPosY = (screenH - 44) + iy * 11;
      TFT.drawRect(buttonPosX, buttonPosY, 34, 12, 0b1101111101111011);
      TFT.setCursor(buttonPosX + 2, buttonPosY + 2);
      TFT.print(keypad[ix][iy]);
    }
  }
  
  for (int ix = 0; ix < 4; ix++) {
    for (int iy = 0; iy < 4; iy++) {
      bool outlineButton = false;
      if (ix == selectedX or selectedX == -1) {
        if (iy == selectedY or selectedY == -1) {
          int buttonPosX = (screenW - 132) + ix * 33;
          int buttonPosY = (screenH - 44) + iy * 11;
          TFT.drawRect(buttonPosX, buttonPosY, 34, 12, 0b000000000000000);
        }
      }
    }
  }
}

int getTimesClicked(int btn) {
  int counter = 0;
  
  while (true) {
    unsigned long lastClickTime = millis();
    unsigned long currentTime = lastClickTime;
    bool btnNotPressed = digitalRead(btn);
    
    while (btnNotPressed and (currentTime - lastClickTime) < 200 and !(inputMethod == 2 and x_axis) and !(inputMethod == 2 and y_axis)) {
      currentTime = millis();
      btnNotPressed = digitalRead(btn);

      //check for interrupts
      interruptsCheck();
    }
    if (!btnNotPressed) {
      digitalWrite(2, 1);
      counter++;
    }
    else {
      digitalWrite(2, 0);
      return counter;
      /*if (counter != 0 and (millis() - lastClickTime) < 400) {
        return counter;
      }*/
    }
    
    while (!digitalRead(btn)) continue;
  }

  return 0;
}

void drawInputField(String typedString) {
  TFT.fillRect(0, screenH - 44, screenW - 132, 44, 0b1111111111111111);
  TFT.drawRect(0, screenH - 44, screenW - 132, 44, 0b0010000010000100);
  TFT.setCursor(5, screenH - 44 + 5);
  TFT.setTextSize(2);
  //TFT.setTextColor(0b0000000000000000);
  TFT.print(typedString);

  if ((millis()/1000) % 2) TFT.print("_");
}

String clickLetter(String typedString, int selectedX, int selectedY, int selectedZ, bool &keyboardOpened) {  
  char typedLetter = keypad[selectedX][selectedY][selectedZ];
  
  if (typedLetter == '@') {
    if (selectedX == 3 and selectedY == 1 and selectedZ == 0) {
      keyboardOpened = false;
    }
    else if (selectedX == 3 and selectedY == 0 and selectedZ == 0) {
      typedString = getSubstring(typedString, 0, -2);
    }
     else if (selectedX == 3 and selectedY == 0 and selectedZ == 1) {
      typedString = "";
    }
  }
  else {
    typedString += typedLetter;
  }

  return typedString;
}

String backspaceCheck(String typedString) {
  if (btnBHeld) {
    typedString = getSubstring(typedString, 0, -2);
    unsigned long currentTime = millis();
    if ((currentTime - btnBLastPressTime) > 500) {
      btnBLastPressTime = currentTime - 400;
    }
  }
  return typedString;
}

String type() {
  String typedString = "";
  bool keyboardOpened = true;

  //inputMethod = 2
  int selectedX = 0;
  int selectedY = 0;
  int selectedZ = 0;

  if (inputMethod == 0) {
    do {} while (not Serial.available());
    while (Serial.available()) {
      typedString += Serial.readString();
      typedString.trim();
    }
    keyboardOpened = false;
  }
  if (inputMethod == 1) {
    while (keyboardOpened and inputMethod == 1) {
      typedString = backspaceCheck(typedString);
      
      drawKeypad(-1, -1);
      drawInputField(typedString);
  
      
      selectedX = getTimesClicked(btnA) - 1;
      if (selectedX >= 0 and selectedX < 4) {
        drawKeypad(selectedX, -1);
        selectedY = getTimesClicked(btnA) - 1;
        if (selectedY >= 0 and selectedY < 4) {
          drawKeypad(selectedX, selectedY);
          selectedZ = getTimesClicked(btnA);
          if (selectedZ < keypad[selectedX][selectedY].length()) {
            typedString = clickLetter(typedString, selectedX, selectedY, selectedZ, keyboardOpened);
          }
        }
      }
    }
  }
  else if (inputMethod == 2) {
    while (keyboardOpened and inputMethod == 2) {
      typedString = backspaceCheck(typedString);
      
      drawKeypad(selectedX, selectedY);
      drawInputField(typedString);

      selectedZ = getTimesClicked(btnA) - 1;
      if (selectedZ >= 0 and selectedZ < keypad[selectedX][selectedY].length()) {
        typedString = clickLetter(typedString, selectedX, selectedY, selectedZ, keyboardOpened);
      }

      selectedY += y_axis + 4;
      selectedY %= 4;
      selectedX += x_axis + 4;
      selectedX %= 4;

      while (y_axis) continue;
      while (x_axis) continue;
    }
  }
  /*else if (inputMethod == 3) {
    while (keyboardOpened) {
      if (keyboard.available()) {
        char typedLetter = keyboard.read();

        if (typedLetter == PS2_ENTER) {
          keyboardOpened = false;
        }
        else if (typedLetter == PS2_BACKSPACE) {
          typedString = getSubstring(typedString, 0, -2);
        }
        else {
          typedString += typedLetter;
        }
      }
    }
  }*/

  if (debugEnabled) {
    Serial.print("User typed: \"");
    Serial.print(typedString);
    Serial.print("\"\n");
  }
  
  digitalWrite(2, 0);
  return typedString;
}

void IRAM_ATTR isr_btnB() {
  if (digitalRead(btnB)) { //not pressed
    btnBHeld = false;
  }
  else { //pressed
    btnBLastPressTime = millis();
    btnBHeld = true;
  }
}
