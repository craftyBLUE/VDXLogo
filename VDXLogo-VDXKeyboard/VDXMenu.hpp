#pragma once

#include "VDXKeyboard.hpp"

int btnMenu = 13;
//int btnMenuReleased = 0;
unsigned long btnMenuLastPressTime = 0;
bool btnMenuHeld = false;

bool menuFlag = false;

void drawItem(int itemSize[4], int positionInList, int selectedItem, String itemText) {
  int itemPostionX = itemSize[0];
  int itemPostionY = itemSize[1] + itemSize[3] * positionInList;
  

  if (positionInList == selectedItem) {
    TFT.fillRect(itemPostionX, itemPostionY, itemSize[2], itemSize[3], 0b1011111011110111);
    TFT.drawRect(itemPostionX, itemPostionY, itemSize[2], itemSize[3], 0b0000000000000000);
  }
  else {
    TFT.fillRect(itemPostionX, itemPostionY, itemSize[2], itemSize[3], 0b1111111111111111);
    TFT.drawRect(itemPostionX, itemPostionY, itemSize[2], itemSize[3], 0b1101111101111011);
  }
  
  TFT.setTextSize(2);
  TFT.setCursor(itemPostionX + itemSize[0] + 4, itemPostionY + (itemSize[1] + itemSize[3]) / 2 - 4 * 2);
  TFT.print(itemText);
}

void drawList(int page, int selectedItem) {
  int itemSize[4] = {0, 0, W, 30};
  
  switch (page) {
    case 0:
      drawItem(itemSize, 0, selectedItem, "Input method");
      drawItem(itemSize, 1, selectedItem, "something else");
      break;
    case 1:
      drawItem(itemSize, 0, selectedItem, "1 button input");
      drawItem(itemSize, 1, selectedItem, "Dpad input");
      drawItem(itemSize, 2, selectedItem, "Serial input"); //move to index 0
      break;
  }
}

void menu() {
  TFT.fillScreen(0b1111111111111111);

  int page = 0;
  int selectedItem = 0;
  while (true) { //while (menuOpen)
    
    drawList(page, selectedItem);

    if (!digitalRead(btnA)) {
      switch (page) {
        case 0:
          switch (selectedItem) {
            case 0:
              page = 1;
              break;
          }
          break;
        case 1:
          switch (selectedItem) {
            case 0:
              inputMethod = 1;
              break;
            case 1:
              inputMethod = 2;
              break;
            case 2:
              inputMethod = 0;
              break;
          }
      }
    }

    selectedItem += y_axis;

    if (btnMenuHeld and (millis() - btnMenuLastPressTime) > 500) {
      break;
    }
    //digitalWrite(2, 1);
    /*if (btnMenuReleased == 3) {
      break;
    }*/
    //digitalWrite(2, 0);

    while (y_axis) continue;
    while (x_axis) continue;
  }
}

void interruptsCheck() {
  if (menuFlag) {
    menu();
    menuFlag = false;
  }
}

void IRAM_ATTR isr_menu() {
  if (digitalRead(btnMenu)) { //not pressed
    btnMenuHeld = false;
    //digitalWrite(2, 0);
    
    /*if (btnMenuReleased == 1) {
       btnMenuReleased = 2;
    }*/
  }
  else { //pressed
    btnMenuLastPressTime = millis();
    menuFlag = true;
    btnMenuHeld = true;
    //digitalWrite(2, 1);
    /*if (!menuFlag) {
      menuFlag = true;
      //btnMenuReleased = 1;
    }*/
    
    /*if (btnMenuReleased == 2) {
      btnMenuReleased = 3;
    }*/
  }
}
