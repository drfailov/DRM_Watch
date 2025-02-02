#include <Arduino.h>

#ifndef GENERICWATCHFACECPP
#define GENERICWATCHFACECPP

class GenericWatchface{   
public :
  //returns visible name of watchface which will be shown at settings screen.
  //Name should not be too long, othervise it will not fit on screen.
  //Max length is 8 char
  //Example of usage:    return (const char*)F("Matrix");
  virtual const char* name() = 0;

  //Called every time when system needs to redraw screen.
  //Draws one frame of watchface.
  //Argument "animate" is not 0 when watchface drawn first time (after reboot or exiting menu).
  // hour, minute, second, day, month, year, dayOfWeek - current values to draw
  // animate - 0=draw without animation, 1 - animate slowly, 2 - animate more quickly, ...
  //Non-blocking function. Allowed only drawing one frame of watchface.
  //Frequency of calling this is defined by updateMode function.
  virtual void drawWatchface(byte hour, byte minute, byte second, byte day, byte month, int year, byte dayOfWeek, byte animate) = 0;
};

#endif
