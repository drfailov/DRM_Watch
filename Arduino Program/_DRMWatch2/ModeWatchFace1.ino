/*Screen with main watchface*/

#define BLOCK_WIDTH  4  // how many blocks contains one symbol
#define BLOCK_HEIGHT 5  // how many blocks contains one symbol

PROGMEM const int32_t font [] {
  
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111001100110011111, //0
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011100100010001000110, //1
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011110001111110001111, //2
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111000111110001111, //3
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000010001000111110011001, //4
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111000111100011111, //5
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111001111100011111, //6
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000010001000100010001111, //7
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111001111110011111, //8
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111000111110011111, //9
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000000000001000000010000, // [10] = :
};


int modeWatchFace1BacklightCounter = 10; //сколько секунд осталось подсветке светить. 0 = выключена

void modeWatchFace1Setup() {
  displayInit();
  byte sleepTime = eepromReadSleepTime();
  if(batteryIsLowPower()) //если разряжен, то макс интервал
    sleepTime = eepromSleepTime8sec;
  if(sleepTime == eepromSleepTime05sec) modeWatchFace1BacklightCounter = 20;
  if(sleepTime == eepromSleepTime1sec) modeWatchFace1BacklightCounter = 10;
  if(sleepTime == eepromSleepTime2sec) modeWatchFace1BacklightCounter = 5;
  if(sleepTime == eepromSleepTime4sec) modeWatchFace1BacklightCounter = 3;
  if(sleepTime == eepromSleepTime8sec) modeWatchFace1BacklightCounter = 2;
    
  attachInterrupt(0, wakeUp, HIGH);
  attachInterrupt(1, wakeUp, HIGH);
}

void modeWatchFace1Loop() {
  if (isButtonUpPressed()) {
    beep();
    byte sleepTime = eepromReadSleepTime();
    if(batteryIsLowPower()) //если разряжен, то макс интервал
      sleepTime = eepromSleepTime8sec;
    if(sleepTime == eepromSleepTime05sec) modeWatchFace1BacklightCounter = 20;
    if(sleepTime == eepromSleepTime1sec) modeWatchFace1BacklightCounter = 10;
    if(sleepTime == eepromSleepTime2sec) modeWatchFace1BacklightCounter = 5;
    if(sleepTime == eepromSleepTime4sec) modeWatchFace1BacklightCounter = 3;
    if(sleepTime == eepromSleepTime8sec) modeWatchFace1BacklightCounter = 2;
  }

  if (isButtonDownPressed()) {
    beep();
    setMode(MODE_MENU_MAIN);
    return;
  }
  displayClear();
  float voltage = batteryVoltage();
  bool isCharging = batteryIsCharging();
  bool isChargeComplete = batteryIsChargeComplete();

  { //time
    int hour = rtcGetHours();
    int minute = rtcGetMinutes();
    int second = rtcGetSeconds();
    int hour1 = hour / 10;
    int hour2 = hour - (hour1 * 10);
    int minute1 = minute / 10;
    int minute2 = minute - (minute1 * 10);
    int second1 = second / 10;
    int second2 = second - (second1 * 10);
    
    drawSymbol(hour1   ,  5, 20, 3, 4);
    drawSymbol(hour2   , 19, 20, 3, 4);
    drawSymbol(10      , 34, 20, 3, 4); // :
    drawSymbol(minute1 , 40, 20, 3, 4);
    drawSymbol(minute2 , 54, 20, 3, 4);
    drawSymbol(10      , 69, 20, 3, 4); // :
    drawSymbol(second1 , 75, 25, 2, 3);
    drawSymbol(second2 , 85, 25, 2, 3);
  }
  
  {//date
    int day = rtcGetDay();
    int month = rtcGetMonth();
    int year = rtcGetYear();

    char chars[11];
    sprintf(chars, "%02d.%02d.%04d", day, month, year);
    displayDrawText(0, 0, 1, chars);
  }
  
  {//Temperature
    float temp = rtcGetTemp();
    char chars[8];
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    dtostrf(temp, 4, 1, chars);
    sprintf(chars, "%sC", chars);
    displayDrawText(0, 61, 1, chars);
  }
  
//  {//voltage
//    byte x = 97;
//    byte y = 60;
//    char chars[10];
//    dtostrf(batteryVoltage(), 4, 2, chars);
//    sprintf(chars, "%sV", chars);
//    displayDrawText(x - (strlen(chars) * 6), y, 1, chars);
//  }
  
  {//battery
    drawBattery(78, 0);
  }

  displayUpdate();
  { //Baclight
    modeWatchFace1BacklightCounter --;
    if (modeWatchFace1BacklightCounter < 0) modeWatchFace1BacklightCounter = 0;
    if (modeWatchFace1BacklightCounter > 0)
      displayBacklightOn();
    else
      displayBacklightOff();
  }

  
  byte sleepTime = eepromReadSleepTime();

  if(batteryIsLowPower()) //если разряжен, то макс интервал
    sleepTime = 8;
  Serial.flush();
  Serial.end();
  delay(5);
  if(sleepTime == eepromSleepTime05sec)
    LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
  else if (sleepTime == eepromSleepTime1sec)
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  else if (sleepTime == eepromSleepTime2sec)
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  else if (sleepTime == eepromSleepTime4sec)
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  else if (sleepTime == eepromSleepTime8sec)
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  else //if some garbage in memory
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  Serial.begin(115200);
}

void modeWatchFace1Finish() {
  detachInterrupt(0);
  detachInterrupt(1);
}

void wakeUp(){ //to react for button
  
}

void drawSymbol(int symbol, int offsetX, int offsetY, int blockSizeX, int blockSizeY){
  for(int bx=0;bx<BLOCK_WIDTH; bx++){
    for(int by=0;by<BLOCK_HEIGHT;by++){
      int bitIndex = bx + by * BLOCK_WIDTH;
      int32_t data = pgm_read_dword(&font[symbol]);
      bool fill = bitRead(data, bitIndex);
      if(fill){
        int x = blockSizeX*bx + offsetX;
        int y = blockSizeY*by + offsetY;
        displayFillRect(/*x*/x, /*y*/y, /*w*/blockSizeX-1, /*h*/blockSizeY-1, /*c*/1);    
      }
    }
  }
}
