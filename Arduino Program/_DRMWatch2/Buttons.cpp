/*Global functions related to control buttons.
*/
#ifndef BUTTONCPP
#define BUTTONCPP

#include <Arduino.h>

class Button_{
  private:
  byte pin;
  bool buttonLastValue = false;
  bool holded = false;
  long lastTimePushDownTimeForButton = 0;

  public:
  Button_(byte pin_){
    pin = pin_;
  }

  //Ткнуть мордой в тот факт что кнопка нажата или отпущена
  void refresh(){
    buttonLastValue = readDebounce();
  }

  //Выдаёт true если кнопка сейчас нажата, а во время прошлого вызова не была.
  //Если вызывать снова, начнёт выдавать true если держать кнопку полсекунды не отпуская
  byte isButtonPressed(){
    bool value = readDebounce();
    if(!value) holded = false;
    bool lastValue = buttonLastValue;
    buttonLastValue = value;
    bool isButtonPressDown = value && !lastValue;
    if(isButtonPressDown)
      lastTimePushDownTimeForButton = millis();

  
    if(isButtonPressDown){ //if down
      return true;
    }
    
    //multipress
    if(!holded && value && lastValue && millis()-lastTimePushDownTimeForButton > 500)
      return true;
    return false;
  }

  //Вызывать после того как получен true на pressed. Выдает true после нажатия кнопку не отпускать в течении 5 секунды
  bool waitHold(){
    for (unsigned long started = millis(); readDebounce(); ){
      if(millis() - started > 1200){
        holded = true;
        return true;
      }
    }
    return false;
    
//    if(!readDebounce()) 
//      return false;
//    long started = millis();
//    while(readDebounce() && millis() - started < 1200);
//    buttonLastValue = readDebounce();
//    if(buttonLastValue)
//      holded = true;
//    return buttonLastValue;
  }

  
  
  bool readDebounce(){
    pinMode(pin, INPUT);
    delay(1);
    byte sum = 0;
    for(int i=0; i<10; i++)
      if(digitalRead(pin) == HIGH)
        sum += 1;
    return sum > 5;
  }
};





#endif
