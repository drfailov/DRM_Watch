/*Global functions related to display. 
 * Init, draw primitives, draw text...
 * Every call to display has to be done by some call from this file.
 * It needed to make migration to other display easier.
*/


LCD1202 lcd(pinLcdRst, pinLcdCs, pinLcdMosi, pinLcdSck);  // RST, CS, MOSI, SCK

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


static const PROGMEM byte pathZubat[] = { 42,
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

static const PROGMEM byte pathDrmWatch[] = { 57,
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

static const PROGMEM byte pathBubble[] = { 12,
  39, 28,    53, 18,    89, 18,    95, 15,    95,  3,  
  89,  0,    5,   0,     0,  3,     0, 15,     5, 18,
  40, 18,   37,  29
}; 

void displayInit(){
  //displayPowerOff();
  Serial.print(F("LCD Init..."));
  pinMode(pinLcdPower, OUTPUT);
  digitalWrite(pinLcdPower, HIGH);
  delay(100);
  lcd.Inicialize();  //Инициализация дисплея
  delay(10);
  //lcd.Clear_LCD();  //Очистка дисплея  
  //displayUpdate();
  Serial.println("OK");
}

void displayBacklightOn(){
  Serial.println(F("LCD Backlight ON"));
  pinMode(pinLcdBacklight, OUTPUT);
  digitalWrite(pinLcdBacklight, HIGH);
}

void displayBacklightOff(){
  Serial.println(F("LCD Backlight OFF"));
//  pinMode(pinLcdBacklight, OUTPUT);
//  digitalWrite(pinLcdBacklight, LOW);
  pinMode(pinLcdBacklight, INPUT);
}

void displayPowerOff(){
  Serial.println(F("LCD POWER OFF"));


  
  pinMode(pinLcdBacklight, INPUT);
  digitalWrite(pinLcdBacklight, LOW);
  
  pinMode(pinLcdRst, INPUT);
  pinMode(pinLcdCs, INPUT);
  pinMode(pinLcdMosi, INPUT);
  pinMode(pinLcdSck, INPUT);
  pinMode(pinLcdPower, INPUT);
  
  delay(50);
}

void displayClear(){
  lcd.Clear_LCD();
}

void displayUpdate(){
  lcd.Update();
}

void displayDrawLine(byte x0, byte y0, byte x1, byte y1, boolean color){
  lcd.drawLine(/*X1*/x0, /*Y1*/y0, /*X2*/x1, /*Y2*/y1, /*C*/color);
}

void displayDrawRect(byte x, byte y, byte w, byte h, boolean color){
  lcd.drawRect(/*x*/x, /*y*/y, /*w*/w, /*h*/h, /*c*/color);
}

void displayFillRect(byte x, byte y, byte w, byte h, boolean color){
  lcd.fillRect(/*x*/x, /*y*/y, /*w*/w, /*h*/h, /*c*/color);
}

void displayDrawText(int X, int Y, int color, const char* text){
  //1 is black, 0 is white
  lcd.print(X, Y, color, text);
}

void displayDrawText(int X, int Y, int color, char c){
  //1 is black, 0 is white
  lcd.drawChar(X, Y, color, c);
}

//-------- Display independent functions
  
void displayDrawVectorLogo(){
  displayDrawVector(/*path*/pathZubat, /*x*/0, /*y*/20, /*animate*/false, /*color*/1);
  displayDrawVector(/*path*/pathBubble, /*x*/0, /*y*/0, /*animate*/false, /*color*/1);
  displayDrawVector(/*path*/pathDrmWatch, /*x*/48, /*y*/25, /*animate*/true, /*color*/1);
}

//max number of points for one path is 255.
//Array type: const PROGMEM byte path[] = {...);
//Array format: {numberOfPoints, x0, y0, x1, y1, x2, y2, ...}
//255,255 coordinates is skipped, so it can be used for separate paths
void displayDrawVector(const byte* data_array, byte X, byte Y, bool animate, bool color){
  byte numberOfPoints = pgm_read_byte(&data_array[0]);
  byte lx = pgm_read_byte(&data_array[1]);
  byte ly = pgm_read_byte(&data_array[2]);
  int currentIndex = 3;
  for(byte i=1; i < numberOfPoints; i++){
    byte x = pgm_read_byte(&data_array[currentIndex]);
    byte y = pgm_read_byte(&data_array[currentIndex + 1]);
    if(x != 255 && y != 255 && lx != 255 && ly != 255)
      displayDrawLine(/*X1*/lx + X, /*Y1*/ly + Y, /*X2*/x + X, /*Y2*/y + Y, /*C*/color);
    if(animate && i%2==0)
      displayUpdate();
    currentIndex += 2;
    lx = x;
    ly = y;
  }
}


void displayDrawText(int X, int Y, int color, const __FlashStringHelper* str){
  byte len = strlen_P((PGM_P)str);
  if(len > 18) len = 18;
  char buffer[18];
  strcpy_P(buffer, (PGM_P)str);
  for(int i=0; i<len; i++)
    displayDrawText(X + i*6, Y, color, buffer[i]);
}

void displayMessage(const __FlashStringHelper* str){
  displayClear();
  displayDrawVector(/*path*/pathZubat, /*x*/0, /*y*/20, /*animate*/false, /*color*/1);
  displayDrawVector(/*path*/pathBubble, /*x*/0, /*y*/0, /*animate*/false, /*color*/1);
  const byte BUFFER_SIZE = 25;
  char buffer[BUFFER_SIZE];
  strlcpy_P(buffer, (PGM_P)str, BUFFER_SIZE);
  byte pos = 0;
  for(byte i=0; i<BUFFER_SIZE; i++){
    if(buffer[i] == '\0') 
      break;
    Serial.println((byte)buffer[i]);
    if((byte)buffer[i] != 208 && (byte)buffer[i] != 209){
      displayDrawText(10 + pos*6, 6, 1, buffer[i]);
      displayUpdate();
      pos ++;
      delay(6);
    }
  }
  delay(500);
}

void displayMessageFromRam(const char* text){
  displayClear();
  displayDrawVector(/*path*/pathZubat, /*x*/0, /*y*/20, /*animate*/false, /*color*/1);
  displayDrawVector(/*path*/pathBubble, /*x*/0, /*y*/0, /*animate*/false, /*color*/1);
  
  byte len = strlen(text);
  for(int i=0; i<len; i++){
    displayDrawText(10 + i*6, 6, 1, text[i]);
    displayUpdate();
    delay(5);
  }
  delay(500);
}

void displaySimpleMessage(const char* text){
  displayClear();
  displayDrawText(10, 30, 1, text);
  displayUpdate();
}

void displayDrawCheck(byte x, byte y){
  static const char img[6] PROGMEM = { 
      0b00011000,
      0b01110000,
      0b01110000,
      0b00111000,
      0b00001110,
      0b00000011,
    };
  lcd.drawBitmap(x, y, img, 6, 8, 1);
}

void displayDrawArrowDown(byte x, byte y){
  static const char img[7] PROGMEM = { 
      0b00000001,
      0b00000111,
      0b00011111,
      0b01111111,
      0b00011111,
      0b00000111,
      0b00000001,
    };
  lcd.drawBitmap(x, y, img, 7, 8, 1);
}

void displayDrawArrowRight(byte x, byte y){
  static const char img[7] PROGMEM = { 
      0b01111111,
      0b00111110,
      0b00111110,
      0b00011100,
      0b00011100,
      0b00001000,
      0b00001000
    };
  lcd.drawBitmap(x, y, img, 7, 8, 1);
}

void displayDrawSilentModeIcon(byte x, byte y){
      static const char img[11] PROGMEM = { 
      0b00011000,
      0b00011000,
      0b00100100,
      0b01000010,
      0b11111111,
      0b00000000,
      0b00100010,
      0b00010100,
      0b00001000,
      0b00010100,
      0b00100010
    };
    lcd.drawBitmap(x, y, img, 11, 8, 1);
}

void displayDrawBattery(byte x, byte y, byte level, bool isCharging, bool isLowPower){

  //draw battery
  byte xshift = 6;
  displayDrawRect(/*x*/x+xshift+1, /*y*/y, /*w*/11, /*h*/7, /*color*/1);
  displayDrawLine(/*X1*/x+xshift+0, /*Y1*/y+2, /*X2*/x+xshift+0, /*Y2*/y+4, /*C*/1);
  displayDrawLine(/*X1*/x+xshift+1, /*Y1*/y+2, /*X2*/x+xshift+1, /*Y2*/y+4, /*C*/0);
  if(level >= 1)
    displayDrawLine(/*X1*/x+xshift+9, /*Y1*/y+2, /*X2*/x+xshift+9, /*Y2*/y+4, /*C*/1);
  if(level >= 2)
    displayDrawLine(/*X1*/x+xshift+7, /*Y1*/y+2, /*X2*/x+xshift+7, /*Y2*/y+4, /*C*/1);
  if(level >= 3)
    displayDrawLine(/*X1*/x+xshift+5, /*Y1*/y+2, /*X2*/x+xshift+5, /*Y2*/y+4, /*C*/1);
  if(level >= 4)
    displayDrawLine(/*X1*/x+xshift+3, /*Y1*/y+2, /*X2*/x+xshift+3, /*Y2*/y+4, /*C*/1);

  //draw low power symbol
  if(isLowPower)
  {
    static const char img[6] PROGMEM = { 
      0b00100010,
      0b00010100,
      0b00001000,
      0b00010100,
      0b00100010,
      0b00000000
    };
    lcd.drawBitmap(x, y, img, 6, 8, 1);
  }

  //draw charging symbol
  if(isCharging){
    static const char img[6] PROGMEM = { 
      0b00000000,
      0b01001100,
      0b00111110,
      0b00011001,
      0b00000000,
      0b00000000
    };
    lcd.drawBitmap(x, y, img, 6, 8, 1);
  }
}

#define BLOCK_WIDTH  4  // how many blocks contains one symbol
#define BLOCK_HEIGHT 5  // how many blocks contains one symbol
PROGMEM const int32_t watchFaceFont [] {
  
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
void displayDrawNumber(int symbol, int offsetX, int offsetY, int blockSizeX, int blockSizeY){
  for(int bx=0;bx<BLOCK_WIDTH; bx++){
    for(int by=0;by<BLOCK_HEIGHT;by++){
      int bitIndex = bx + by * BLOCK_WIDTH;
      int32_t data = pgm_read_dword(&watchFaceFont[symbol]);
      bool fill = bitRead(data, bitIndex);
      if(fill){
        int x = blockSizeX*bx + offsetX;
        int y = blockSizeY*by + offsetY;
        displayFillRect(/*x*/x, /*y*/y, /*w*/blockSizeX-1, /*h*/blockSizeY-1, /*c*/1);    
      }
    }
  }
}
