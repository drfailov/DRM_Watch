const byte fontWtf3[] PROGMEM = {
  0b11101110, //0 
  0b00100100, //1
  0b10111010, //2
  0b10110110, //3
  0b01110100, //4 
  0b11010110, //5
  0b11011110, //6
  0b10100100, //7
  0b11111110, //8
  0b11110110 //9
};
static const PROGMEM byte pathUp[] = { 6,
// 00,00, 00,00, 00,00, 00,00, 00,00,
   2,0,   12,0,  11,1,  3,1,   4,2,
   10,2
};
static const PROGMEM byte pathUpLeft[] = { 6,
// 00,00, 00,00, 00,00, 00,00, 00,00,
   0,1,   0,12,  1,11,  1,2,   2,3,
   2,10
};
static const PROGMEM byte pathUpRight[] = { 6,
   14,1,   14,12,  13,11,  13,2,  12,3,
   12,10
};
static const PROGMEM byte pathCenter[] = { 6,
   3,12,   11,12,  12,13,  2,13,  3,14,
   11,14
};
static const PROGMEM byte pathBottomLeft[] = { 6,
   0,14,   0,27,   1,26,   1,15,  2,16,
   2,25
};
static const PROGMEM byte pathBottomRight[] = { 6,
   14,14,  14,27,  13,26,  13,15, 12,16,
   12,25
};
static const PROGMEM byte pathBottom[] = { 6,
   2,28,   12,28,  11,27,  3,27,  4,26,
   10,26
};

void WatchfaceNomens(byte hour, byte minute, byte second, byte day, byte month, int year, byte dayOfWeek, byte animate)
{
  displayClear();


  {//треугольничек с полосочкой рисуем
    byte X = 63;
    byte Y = 54;
    for(byte i=1; i<14; i++){
       displayDrawLine(/*X1*/X+i, /*Y1*/Y+i, /*X2*/96, /*Y2*/Y+i, /*C*/1);
    }
    displayDrawLine(/*X1*/0, /*Y1*/Y, /*X2*/96, /*Y2*/Y, /*C*/1);
  }
   
  {//date
    sprintf(buffer, dateFormat, day, month, year);
    displayDrawText(3, 58, 1, buffer);
  }
  
  {//DayOfWeek
#ifdef LANG_EN
    drawDayOfWeek(76, 58, dayOfWeek, 0);
#endif
#ifdef LANG_RU
    drawDayOfWeek(80, 58, dayOfWeek, 0);
#endif
#ifdef LANG_UA
    drawDayOfWeek(80, 58, dayOfWeek, 0);
#endif
  }

  byte X = 96;
  
  {//battery
    X -= 17;
    displayDrawBattery(X, 0);
    if(!batteryIsCharging() && !batteryIsLowPower()) X += 5;
  }

  
  //Alert sign
  if(eepromReadAlertEnabled()){ 
    X-= 13;
    displayDrawAlertSign(X, 0, 1);
  }

  
  //Silent mode sign
  if(eepromReadSilentMode()){ 
    X -= 12;
    displayDrawSilentModeIcon(X, 0, 1);
  }
  


  {//time
    byte hour1 = hour / 10;
    byte hour2 = hour - (hour1 * 10);
    byte minute1 = minute / 10;
    byte minute2 = minute - (minute1 * 10);
    displayDrawNumber(10, 46, 18, 4, 5, animate); // :
//        displayFillRect(/*x*/46, /*y*/24, /*w*/3, /*h*/3, /*c*/1);
//        displayFillRect(/*x*/46, /*y*/35, /*w*/3, /*h*/3, /*c*/1);
    drawNumberNomens(/*x*/8, /*y*/14, /*num*/hour1, /*animate*/ animate);
    drawNumberNomens(/*x*/26, /*y*/14, /*num*/hour2, /*animate*/ animate);
    drawNumberNomens(/*x*/54, /*y*/14, /*num*/minute1, /*animate*/ animate);
    drawNumberNomens(/*x*/72, /*y*/14, /*num*/minute2, /*animate*/ animate);
  }
  
  displayUpdate();
}

void drawNumberNomens(byte x, byte y, byte num, byte animate){
  byte b = pgm_read_byte(&fontWtf3[num]);
  if(bitRead(b, 7)){ 
    displayDrawVector(/*path*/pathUp, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
  if(bitRead(b, 6)){ 
    displayDrawVector(/*path*/pathUpLeft, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
  if(bitRead(b, 5)){ 
    displayDrawVector(/*path*/pathUpRight, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
  if(bitRead(b, 4)){ 
    displayDrawVector(/*path*/pathCenter, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
  if(bitRead(b, 3)){ 
    displayDrawVector(/*path*/pathBottomLeft, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
  if(bitRead(b, 2)){ 
    displayDrawVector(/*path*/pathBottomRight, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
  if(bitRead(b, 1)){ 
    displayDrawVector(/*path*/pathBottom, /*x*/x, /*y*/y, /*animate*/animate, /*color*/1);
  }
}
