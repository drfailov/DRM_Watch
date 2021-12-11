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

static const unsigned char DRMTestLogo[768] PROGMEM =
{ 0xF8, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0xF8,
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0x00, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
0x04, 0x04, 0x04, 0x04, 0x84, 0x44, 0x24, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x94, 0xD4,
0x94, 0x14, 0x94, 0x54, 0x34, 0x04, 0x04, 0xF4, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40,
0x20, 0x20, 0x10, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x80, 0x60, 0x18, 0x06, 0x01, 0x80, 0x60, 0xF0, 0x10, 0x10, 0x10, 0xD0, 0xF0, 0x10, 0x10, 0x11,
0x90, 0x70, 0x1F, 0x00, 0x00, 0x30, 0x1F, 0x10, 0x08, 0x04, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00,
0xFE, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x08, 0xF0, 0x00, 0x00, 0xFE, 0x02, 0x02,
0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0xF8, 0x00, 0x00, 0xFE, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x18, 0x06,
0x01, 0x00, 0xC0, 0x30, 0x3E, 0xC1, 0x00, 0x3F, 0x18, 0x84, 0x63, 0x78, 0x83, 0x3C, 0x98, 0x66,
0x19, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7F, 0x00, 0x00, 0xFF, 0x02, 0x06,
0x0A, 0x12, 0x22, 0x42, 0x82, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0x00, 0x00,
0x1C, 0x13, 0x10, 0x10, 0x10, 0x10, 0x13, 0x1C, 0x16, 0x11, 0x10, 0x10, 0x13, 0x1E, 0x11, 0x10,
0x10, 0x10, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x82, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00, 0x00, 0x80, 0x00, 0x03, 0x00, 0x80,
0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x82, 0x80, 0x80, 0x83, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80,
0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x30, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x03, 0x3C, 0xC0, 0x00, 0x80, 0x60, 0x80, 0x00, 0xC0, 0x3C, 0x03, 0x80, 0x70, 0x4E, 0x41,
0x4E, 0x70, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00,
0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0xFF, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00,
0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x82, 0x42, 0x3E, 0x42, 0x82, 0x02, 0x02, 0x02, 0x82, 0x42, 0x3E, 0x42, 0x82, 0x02, 0x02, 0x02,
0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0x0C, 0x03, 0x00, 0x03, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x00, 0x00,
0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08,
0x08, 0x08, 0x08, 0x04, 0x02, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00};


void displayInit(){
  Serial.print(F("LCD Init..."));
  lcd.Inicialize();  //Инициализация дисплея
  lcd.Clear_LCD();  //Очистка дисплея
  lcd.drawBitmap(0, 0, DRMTestLogo, 96, 64, 1);   
  displayDrawText(3, 6, 1, "   Welcome!   ");
  displayUpdate();
  Serial.println("OK");
  displayBacklightOff();
}

void displayBacklightOn(){
  Serial.println(F("LCD Backlight ON"));
  pinMode(pinLcdBacklight, OUTPUT);
  digitalWrite(pinLcdBacklight, HIGH);
}

void displayBacklightOff(){
  Serial.println(F("LCD Backlight OFF"));
  pinMode(pinLcdBacklight, OUTPUT);
  digitalWrite(pinLcdBacklight, LOW);
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


//-------- Display independent functions

void displayMessage(const char* text){
  displayClear();
  displayDrawText(10, 30, 1, text);
  displayUpdate();
}

void displayDrawCheck(byte x, byte y){
  displayDrawLine(/*X1*/x+0, /*Y1*/y+3, /*X2*/x+1, /*Y2*/y+6, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+3, /*X2*/x+2, /*Y2*/y+5, /*C*/1);
  displayDrawLine(/*X1*/x+1, /*Y1*/y+6, /*X2*/x+5, /*Y2*/y+0, /*C*/1);
  displayDrawLine(/*X1*/x+2, /*Y1*/y+6, /*X2*/x+5, /*Y2*/y+0, /*C*/1);
}

void displayDrawArrowDown(byte x, byte y){
  displayDrawLine(/*X1*/x+0, /*Y1*/y+0, /*X2*/x+6, /*Y2*/y+0, /*C*/1);
  displayDrawLine(/*X1*/x+1, /*Y1*/y+1, /*X2*/x+5, /*Y2*/y+1, /*C*/1);
  displayDrawLine(/*X1*/x+1, /*Y1*/y+2, /*X2*/x+5, /*Y2*/y+2, /*C*/1);
  displayDrawLine(/*X1*/x+2, /*Y1*/y+3, /*X2*/x+4, /*Y2*/y+3, /*C*/1);
  displayDrawLine(/*X1*/x+2, /*Y1*/y+4, /*X2*/x+4, /*Y2*/y+4, /*C*/1);
  displayDrawLine(/*X1*/x+3, /*Y1*/y+5, /*X2*/x+3, /*Y2*/y+5, /*C*/1);
}

void displayDrawArrowRight(byte x, byte y){
  displayDrawLine(/*X1*/x+0, /*Y1*/y+0, /*X2*/x+0, /*Y2*/y+0, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+1, /*X2*/x+2, /*Y2*/y+1, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+2, /*X2*/x+4, /*Y2*/y+2, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+3, /*X2*/x+6, /*Y2*/y+3, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+4, /*X2*/x+4, /*Y2*/y+4, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+5, /*X2*/x+2, /*Y2*/y+5, /*C*/1);
  displayDrawLine(/*X1*/x+0, /*Y1*/y+6, /*X2*/x+0, /*Y2*/y+6, /*C*/1);
}
