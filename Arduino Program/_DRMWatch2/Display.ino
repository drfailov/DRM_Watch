  /*Global functions related to display. 
   * Init, draw primitives, draw text...
   * Every call to display has to be done by some call from this file.
   * It needed to make migration to other display easier.
  */
#include "lcd1202.h"

#define LCD_X        96
#define LCD_Y        68


const int32_t  PROGMEM watchFaceFont [] { 
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
  0b00000000000001000100010010001111, //7
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111001111110011111, //8
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000011111000111110011111, //9
  //  Lines:    |5 ||4 ||3 ||2 ||1 |  <<<<
  0b00000000000000000001000000010000 // [10] = :
};
const PROGMEM byte pathDrmWatch[] = { 57,
   0,  0,     7,  0,    10,  3,    10, 13,     7, 16,  //D
   0, 16,     0,  0,   255,255,    13, 16,    13,  0,  //R
  20,  0,    22,  2,    22,  6,    20,  8,    14,  8,
  22, 16,   255,255,    25, 16,    25,  0,    33,  8,  //M
  41,  0,    41, 16,   255,255,     1, 23,     4, 34,  //W
   6, 28,     8, 34,    11, 22,   255,255,    11, 34,  //A
  15, 22,    17, 29,    14, 29,    17, 29,    19, 34,
 255,255,    20, 22,    26, 22,    23, 22,    23, 34,  //T
 255,255,    36, 24,    34, 22,    31, 22,    29, 24,  //C
  29, 32,    31, 34,    34, 34,    36, 32,   255,255,
  39, 22,    39, 34,    39, 30,    41, 28,    43, 28,  //H
  45, 30,    45, 34
}; 

const PROGMEM byte pathBubble[] = { 12,
  39, 28,    53, 18,    89, 18,    95, 15,    95,  3,  
  89,  0,    5,   0,     0,  3,     0, 15,     5, 18,
  40, 18,   37,  29
}; 

const PROGMEM byte pathZubat[] = { 42,
   0, 36,     4, 35,    20,  3,    23,  0,    36,  0, //контур
  34,  3,    34,  8,    23,  8,    16, 24,    35, 24,
  35, 35,    33, 37,     0, 37,   255,255,    16, 43,//лапы
  18, 41,    20, 43,    18, 41,    18, 37,    26, 37,
  26, 41,    24, 43,    26, 41,    28, 43,   255,255,
  33,  9,    29, 17,    28,  9,    23, 17,    23,  8, //зубы
  20, 16,    23, 23,    27, 15,    29, 23,    33, 15,
  34, 23,   255,255,    31,  2,    31,  4,    31,  3, //глаз
  32,  3,    30,  3
};

  //LCD1202 lcd;
  LCD1202 lcd(pinLcdRst, pinLcdCs, pinLcdMosi, pinLcdSck);
  /*
   * Display workflow:
   * 
   * > displayInit();
   *       V
   * > displayClear();
   *       V
   * > displayDrawLine(...)
   * > displayFillRect(...)
   * > displayDrawText(...)
   *   ...
   *       V
   * > displayUpdate() 
   *       V
   * > displayClear();
   *   ...
   *
  */
  
  

  static const byte* const getPathZubat(){
    return pathZubat;
  }

   
  static const byte* const getPathDrmWatch(){ 
    return pathDrmWatch;
  }

  static const byte* const getPathBubble(){
    return pathBubble;
  } 
  
  
  //Инициализация дисплея. Подаём питание, ждём немного и инициализируем программно
  static void displayInit(){
    #ifdef LOG
      Serial.print(F("LCD Init..."));
    #endif
    pinMode(pinLcdPower, OUTPUT);
    digitalWrite(pinLcdPower, HIGH);
    delay(100);
    lcd.Inicialize();  //Инициализация дисплея
    #ifdef LOG
      Serial.println("OK");
    #endif
  }
  
  //Врубаем подсветку
  static void displayBacklightOn(){
    if(batteryIsLowPower()) return; //если батарея разряжена, подсветку не включать
    pinMode(pinLcdBacklight, OUTPUT);
    digitalWrite(pinLcdBacklight, HIGH);
  }
  
  //Отсоединяем пин подсветки полностью
  static void displayBacklightOff(){
    pinMode(pinLcdBacklight, INPUT);
  }
  
  //Отсоединяем все пины дисплея от ардуины, в том числе и питание
  static void displayPowerOff(){
  #ifdef LOG
    Serial.println(F("LCD PWR OFF"));
  #endif
    pinMode(pinLcdBacklight, INPUT);
    //digitalWrite(pinLcdBacklight, LOW);
    
    pinMode(pinLcdRst, INPUT);
    pinMode(pinLcdCs, INPUT);
    pinMode(pinLcdMosi, INPUT);
    pinMode(pinLcdSck, INPUT);
    pinMode(pinLcdPower, INPUT);
  }
  
  //Функция очистки дисплея. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayClear(){
    lcd.Clear_LCD();
  }
  
  //Функция оновления дисплея из буфера. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  //Если в параметрах указано перевернктый дисплей, эта функция сама это сделает. Она обращается к MyEEPROM
  static void displayUpdate(){
    //true is flip 180, false is no flip
    lcd.Update(/*flip*/eepromReadFlipScreen()); 
  }
  
  //Функция рисования линии. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayDrawLine(byte x0, byte y0, byte x1, byte y1, boolean color){
    lcd.drawLine(/*X1*/x0, /*Y1*/y0, /*X2*/x1, /*Y2*/y1, /*C*/color);
  }
  
  //Функция рисования периметра прямоугольника. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayDrawRect(byte x, byte y, byte w, byte h, boolean color){
    lcd.drawRect(/*x*/x, /*y*/y, /*w*/w, /*h*/h, /*c*/color);
  }
  
  //Функция рисования залитого прямоугольника. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayFillRect(byte x, byte y, byte w, byte h, boolean color){
    lcd.fillRect(/*x*/x, /*y*/y, /*w*/w, /*h*/h, /*c*/color);
  }
  
  //Функция рисования залитого круга. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayFillCircle(byte x, byte y, byte r, boolean color){
    lcd.fillCircle(x, y, r, color);
  }
  
  
  //Функция рисования текста. Эта конкретная функция рисует из оперативки, т.е. из буфера. 
  //Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  //Для рисования PROGMEM строк или строк с макросом F() есть другие функции пониже.
  static void displayDrawText(int X, int Y, int color, const char* text){
    //1 is black, 0 is white
    lcd.print(X, Y, color, text);
  }
  
  //Рисование одного символа из оперативки. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayDrawText(int X, int Y, int color, char c){
    //1 is black, 0 is white
    lcd.drawChar(X, Y, color, c);
  }
  
  //Рисование растровой картинки. Она здесь указана для простоты перехода на другой дисплей - всё взаимодействие с дисплее происходит через эти "обёртки".
  static void displayDrawBitmap(byte x, byte y, const unsigned char *img, byte w, byte h, byte color){
    lcd.drawBitmap(x, y, img, w, h, color);
  }
  
  static void displaySetPixel(byte x, byte y, bool color){
    lcd.drawPixel(x, y, color);
  }
  
  static bool displayGetPixel(byte x, byte y){
    return lcd.getPixel(x, y);
  }
  
  
  //-------- Display independent functions
  
  //Реконструкция изначального растра из тестовой программы. Эта функция нигде не вызывается, поэтому памяти не занимает. Пусть валяется.
  static void displayDrawVectorLogo(){
    displayDrawVector(/*path*/getPathZubat(), /*x*/0, /*y*/20, /*animate*/false, /*color*/1);
    displayDrawVector(/*path*/getPathBubble(), /*x*/0, /*y*/0, /*animate*/false, /*color*/1);
    displayDrawVector(/*path*/getPathDrmWatch(), /*x*/48, /*y*/25, /*animate*/true, /*color*/1);
  }
  
  //Рисование векторной картинки. Формат вектора описан в комментарии ниже.
  //max number of points for one path is 255.
  //Array type: const PROGMEM byte path[] = {...);
  //Array format: {numberOfPoints, x0, y0, x1, y1, x2, y2, ...}
  //255,255 coordinates is skipped, so it can be used for separate paths
  //animate: 0-noAnimation, 1...-speed,low is slow.
  static void displayDrawVector(const byte* data_array, byte X, byte Y, byte animate, bool color){
    byte numberOfPoints = pgm_read_byte(&data_array[0]);
    byte lx = pgm_read_byte(&data_array[1]);
    byte ly = pgm_read_byte(&data_array[2]);
    int currentIndex = 3;
    for(byte i=1; i < numberOfPoints; i++){
      byte x = pgm_read_byte(&data_array[currentIndex]);
      byte y = pgm_read_byte(&data_array[currentIndex + 1]);
      if(x != 255 && y != 255 && lx != 255 && ly != 255)
        displayDrawLine(/*X1*/lx + X, /*Y1*/ly + Y, /*X2*/x + X, /*Y2*/y + Y, /*C*/color);
      if(animate!=0 && i%animate==0){
        displayUpdate();
        if (isButtonUpPressed()) 
          animate = false;
      }
      currentIndex += 2;
      lx = x;
      ly = y;
    }
  }
  
  //Рисование на экране текста из макроса F().
  static void displayDrawText(int X, int Y, int color, const __FlashStringHelper* str){
    strcpy_P(buffer, (PGM_P)str);
    byte pos = 0;
    for(int i=0; i<BUFFER_SIZE; i++){
      if(buffer[i] == '\0') 
        break;
      if((byte)buffer[i] != 208 && (byte)buffer[i] != 209){
        displayDrawText(X + pos*6, Y, color, buffer[i]);
        pos++;
      }
    }
  }
  
  //Отображение красивого уведомления с Зубатом и анимацией.
  //Анимацию можно перемотать кнопкой вверх
  static void displayMessage(const __FlashStringHelper* str){
    displayTransition();
    displayDrawVector(/*path*/getPathZubat(), /*x*/0, /*y*/20, /*animate*/false, /*color*/1);
    displayDrawVector(/*path*/getPathBubble(), /*x*/0, /*y*/0, /*animate*/false, /*color*/1);
    strcpy_P(buffer, (PGM_P)str);
    byte pos = 0;
    bool animateMessage = true;
    for(byte i=0; i<BUFFER_SIZE; i++){
      if(buffer[i] == '\0') 
        break;
      if((byte)buffer[i] != 208 && (byte)buffer[i] != 209){
        displayDrawText(10 + pos*6, 6, 1, buffer[i]);
        pos ++;
        if(animateMessage){
          displayUpdate();
          delay(6);
        }
      }
      if (isButtonUpPressed()) 
        animateMessage = false;
    }
    displayUpdate();
    delay(animateMessage?500:200);
    displayTransition();
  }
  
  //Вывод простого сообщения из оперативки.
  static void displaySimpleMessage(const char* text){
    displayTransition();
    displayDrawText(10, 30, 1, text);
    displayUpdate();
  }
  
  //Рисование иконки плэй. 7х7. Используется на экране секундомера.
  static void displayDrawPlaySign(byte x, byte y, bool color){
    static const unsigned char img[7] PROGMEM = { 
        0b01111111,
        0b00111110,
        0b00111110,
        0b00011100,
        0b00011100,
        0b00001000,
        0b00001000
      };
    displayDrawBitmap(x, y, img, 7, 8, color);
  }
  
  //Рисование иконки паузы. 7х7. Используется на экране секундомера.
  static void displayDrawPauseSign(byte x, byte y, bool color){
    static const unsigned char img[7] PROGMEM = { 
        0b01111111,
        0b01111111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01111111,
        0b01111111
      };
    displayDrawBitmap(x, y, img, 7, 8, color);
  }
  
  //Рисование иконки стоп. 7х7. Используется на экране воспроизведения музыки.
  //Рисунок находится в битовом массиве.
  //Начало массива - левая часть рисунка. Один бит - один пиксель.
  //Т.е. смотреть на массив следует повернув его на 90 градусов против часовой стрелки.
  static void displayDrawStopSign(byte x, byte y, bool color){
    static const unsigned char img[7] PROGMEM = { 
        0b01111111,
        0b01111111,
        0b01111111,
        0b01111111,
        0b01111111,
        0b01111111,
        0b01111111
      };
    displayDrawBitmap(x, y, img, 7, 8, color);
  }
  

  
  //Рисование прямоугольника с цифрами. Используется на экранах настройки будильника и времени
  static void displayDraw2DigitNumberWithFrame(byte index, byte x, byte y, byte number){
    sprintf(buffer, "%02d", number);
    (selected==index?displayFillRect:displayDrawRect)(/*x*/x, /*y*/y, /*w*/19, /*h*/15, /*c*/1);
    displayDrawText(x + 4, y+4, index!=selected, buffer);
  }
  //Рисование рамки с картинкой
  
  //Рисование прямоугольника с иконкой 9x7. Используется на экранах главного меню, секундомера
  static void displayDrawIconWithFrame(byte index, byte x, byte y, void (*drawIcon)(byte x,byte y,bool color)){
    (selected==index?displayFillRect:displayDrawRect)(/*x*/x, /*y*/y, /*w*/21, /*h*/15, /*c*/1);
    drawIcon(x+6, y+4, selected!=index);
  }
  
  /*draws battery*/
  static void displayDrawBattery(byte x, byte y, byte level, bool isCharging, bool isLowPower){
    byte xshift = 5;

    static const unsigned char img[12] PROGMEM = { 
      0b00011100,
      0b01100011,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01000001,
      0b01111111,
    };
    displayDrawBitmap(x+xshift, y, img, 12, 8, 1);
    
    for(byte i=0; i < level; i++)
      displayDrawLine(/*X1*/x+xshift+9-i*2, /*Y1*/y+2, /*X2*/x+xshift+9-i*2, /*Y2*/y+4, /*C*/1);

    if(isCharging){ //draw charging symbol
      static const unsigned char img[6] PROGMEM = { 
        0b00000000,
        0b01001100,
        0b00111110,
        0b00011001,
        0b00000000,
        0b00000000
      };
      displayDrawBitmap(x, y, img, 6, 8, 1);
    }
    else if(isLowPower) //draw low power symbol
    {
      static const unsigned char img[4] PROGMEM = { 
        0b00000000,
        0b00000000,
        0b01101111,
        0b01101111
      };
      displayDrawBitmap(x, y, img, 4, 8, 1);
    }
  }

  //Это основная функция которая вызывается с циферлатов
  void displayDrawBattery(byte x, byte y) {
    bool isCharging = batteryIsCharging();
    bool isLowPower = batteryIsLowPower();
    byte level = batteryBars();  
    //isLowPower = true; //for test
    //isCharging = false; //for test
    displayDrawBattery(x, y, level, isCharging, isLowPower);
  }

  //нарисовать красивую цифру шривтом DRM
  #define BLOCK_WIDTH  4  // how many blocks contains one symbol
  #define BLOCK_HEIGHT 5  // how many blocks contains one symbol      
  void displayDrawNumber(byte symbol, byte offsetX, byte offsetY, byte blockSizeX, byte blockSizeY, byte animate){
    for(byte bx=0;bx<BLOCK_WIDTH; bx++){
      for(byte by=0;by<BLOCK_HEIGHT;by++){
        byte bitIndex = bx + by * BLOCK_WIDTH;
        int32_t data = pgm_read_dword(&watchFaceFont[symbol]);
        bool fill = bitRead(data, bitIndex);
        if(fill){
          byte x = blockSizeX*bx + offsetX;
          byte y = blockSizeY*by + offsetY;
          displayFillRect(/*x*/x, /*y*/y, /*w*/blockSizeX-1, /*h*/blockSizeY-1, /*c*/1);
          if(animate && bitIndex%animate == 0){
            displayUpdate();
          }
        }
      }
    }
  }

  void drawDayOfWeek(byte x, byte y, byte dayOfWeek, bool color) { 
    //Да, это говно. Но оно занимает меньше места в памяти, да, реально! Я проверил и сам офигел.
    const __FlashStringHelper* txt;
  #ifdef LANG_RU
    if (dayOfWeek == 1) txt = F("Пн");
    if (dayOfWeek == 2) txt = F("Вт");
    if (dayOfWeek == 3) txt = F("Ср");
    if (dayOfWeek == 4) txt = F("Чт");
    if (dayOfWeek == 5) txt = F("Пт");
    if (dayOfWeek == 6) txt = F("Сб");
    if (dayOfWeek == 0) txt = F("Вс");
  #endif
  #ifdef LANG_UA
    if (dayOfWeek == 1) txt = F("Пн");
    if (dayOfWeek == 2) txt = F("Bт");
    if (dayOfWeek == 3) txt = F("Cp");
    if (dayOfWeek == 4) txt = F("Чт");
    if (dayOfWeek == 5) txt = F("Пт");
    if (dayOfWeek == 6) txt = F("Cб");
    if (dayOfWeek == 0) txt = F("Hд");
  #endif
  #ifdef LANG_EN
    if (dayOfWeek == 1) txt = F("Mon");
    if (dayOfWeek == 2) txt = F("Tue");
    if (dayOfWeek == 3) txt = F("Wed");
    if (dayOfWeek == 4) txt = F("Thu");
    if (dayOfWeek == 5) txt = F("Fri");
    if (dayOfWeek == 6) txt = F("Sat");
    if (dayOfWeek == 0) txt = F("Sun");
  #endif
    displayDrawText(x, y, color, txt);
  }

  void displayTransition(){
    for(byte x = LCD_X-1; x < LCD_X; x--){ //byte is cycling to 255
      for(byte y = 0; y < LCD_Y; y++){
          displaySetPixel(x,y,0);
      }
      if(x%30==0)
        displayUpdate();
    }
    animate = true;
  }

  void displayTransitionFade(){
    for(byte i = 0; i < 2; i++){ 
      for(byte y = LCD_Y-1; y < LCD_Y; y--){ //byte is cycling to 255
        for(byte x = 0; x < LCD_X; x++){
          if(((i+y)%2)+((i+x)%2)==0)
            displaySetPixel(x,y,0);
        }
      }
      displayUpdate();
    }
  }
  void displayTransitionSwipeDownAndFade(){
    byte amountOfShift = 5;
    byte mesh=5;
    for(byte i = 0; i < 4; i++){ 
      for(byte y = LCD_Y-1; y < LCD_Y; y--){ //byte is cycling to 255
        for(byte x = 0; x < LCD_X; x++){
          if(y - amountOfShift >= 0 && (x+y)%mesh!=0){
            displaySetPixel(x,y, displayGetPixel(x,y-amountOfShift));
          }
          else{
            displaySetPixel(x,y,0);
          }
        }
      }
      mesh--;
      amountOfShift += 10;
      displayUpdate();
    }
  }
  void displayTransitionWipeDown(){
    byte amountOfShift = 15;
    for(byte y = 0; y < LCD_Y; y++){
      for(byte x = 0; x < LCD_X; x++)
         displaySetPixel(x,y,0);
      if(y%amountOfShift ==0)
        displayUpdate();
    }
  }
  void displayTransitionSwipeLeft(){
    byte amountOfShift = 23;
    for(byte i = 0; i < 5; i++){  
      for(byte y = 0; y < LCD_Y; y++){
        for(byte x = 0; x < LCD_X; x++){
          if(x + amountOfShift < LCD_X){
            displaySetPixel(x,y, displayGetPixel(x+amountOfShift,y));
          }
          else{
            displaySetPixel(x,y,0);
          }
        }
      }
      amountOfShift += 10;
      displayUpdate();
    }
  }
