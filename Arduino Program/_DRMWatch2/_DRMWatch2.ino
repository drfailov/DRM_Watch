#include "GenericWatchface.cpp"
#include "WatchfaceDrmWatch.cpp"
#include "WatchfaceDrmLite.cpp"
#include "WatchfaceNomens.cpp"
#include "WatchfaceZubat.cpp"
#include "WatchfaceXelibri.cpp"
#include "WatchfaceMatrix.cpp"
#include "WatchfaceLife.cpp"
#include <util/atomic.h>
#include <LowPower.h>
void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0

/* Program contains several screens (menus, watchfaces...).
 * Every screen is a separate mode. Every mode contains of: modeSetup(), modeLoop(), modeFinish().
 * Screens can work independently. Modes divided into different files to ease.
 * Function loop() is routing loop() co correct dcreen according to active mode.
 * Also setMode() function is triggering modeSetup() and modeFinish() for every mode.
 * To change mode call: setMode(MODE_INIT);
 * When adding new mode, it needs to be declared: 
 *    - define constant for it,
 *    - loop() call for modeLoop()
 *    - call its setup() from setMode()
 *    - call its finish() from setMode()
*/
//Modes
//Значения констант режимов
#define MODE_INIT (byte)0
#define MODE_WATCHFACE (byte)1
#define MODE_MENU_MAIN (byte)2
#define MODE_STATUS (byte)3
#define MODE_MENU_MELODIES (byte)4
#define MODE_MENU_SETTINGS (byte)5
#define MODE_MENU_SET_TIME (byte)6
#define MODE_ABOUT (byte)8
#define MODE_MENU_APPS (byte)11
#define MODE_STOPWATCH (byte)12
#define MODE_SET_ALARM (byte)13
//Переменная в которой хранится текущий режим
byte _mode = -1;



//Набор циферблатов следует менять именно здесь. Обязательно обновить количество если оно изменилось.
const byte watchfacesCount = 3;
GenericWatchface *watchfaces[watchfacesCount];

void setup() {
  byte i=0;
  //watchfaces[i++] = new WatchfaceDrmWatch();
  watchfaces[i++] = new WatchfaceDrmLite();
  watchfaces[i++] = new WatchfaceNomens();
  //watchfaces[i++] = new WatchfaceZubat();
  //watchfaces[i++] = new WatchfaceXelibri();
  //watchfaces[i++] = new WatchfaceMatrix();
  watchfaces[i++] = new WatchfaceLife();
  
  
#ifdef LOG
  Serial.begin(115200);
  Serial.println(F("\nDRM Watch V2"));
  Serial.println(version);
  Serial.print(F("- Date "));
  Serial.print(F(__DATE__));
  Serial.print(F(" at "));
  Serial.println(F(__TIME__));
#endif  

  setMode(MODE_INIT);
}

void loop() {
  if (_mode == MODE_INIT) modeInitLoop();
  if (_mode == MODE_WATCHFACE) modeWatchFaceLoop(false);
  if (_mode == MODE_MENU_MAIN ) modeMenuMainLoop();
  if (_mode == MODE_STATUS ) modeStatusLoop();
  if (_mode == MODE_MENU_MELODIES ) modeMenuMelodiesLoop();
  if (_mode == MODE_MENU_SETTINGS ) modeMenuSettingsLoop();
  if (_mode == MODE_MENU_SET_TIME ) modeMenuSetTimeLoop();
  if (_mode == MODE_ABOUT ) modeAboutLoop();
  if (_mode == MODE_MENU_APPS ) modeMenuAppsLoop();
  if (_mode == MODE_STOPWATCH ) modeStopwatchLoop();
  if (_mode == MODE_SET_ALARM ) modeSetAlarmLoop();
}

void setMode(int _modeNew) {
  if (_mode == _modeNew)
    return;
#ifdef LOG
  Serial.print(F("Mode "));
  Serial.print(_mode);
  Serial.print(F(" -> "));
  Serial.println(_modeNew);
  
  Serial.print(F("RAM: "));
  Serial.println(freeRam());
#endif
  //finish old
  if (_mode == MODE_INIT) modeInitFinish();
  if (_mode == MODE_WATCHFACE) modeWatchFaceFinish();
  if (_mode == MODE_MENU_MAIN ) modeMenuMainFinish();
  if (_mode == MODE_STATUS ) modeStatusFinish();
  if (_mode == MODE_MENU_MELODIES ) modeMenuMelodiesFinish();
  if (_mode == MODE_MENU_SETTINGS ) modeMenuSettingsFinish();
  if (_mode == MODE_MENU_SET_TIME ) modeMenuSetTimeFinish();
  if (_mode == MODE_ABOUT ) modeAboutFinish();
  if (_mode == MODE_MENU_APPS ) modeMenuAppsFinish();
  if (_mode == MODE_STOPWATCH ) modeStopwatchFinish();
  if (_mode == MODE_SET_ALARM ) modeSetAlarmFinish();
  
  //init new
  if (_modeNew == MODE_INIT) modeInitSetup();
  if (_modeNew == MODE_WATCHFACE) modeWatchFaceSetup();
  if (_modeNew == MODE_MENU_MAIN ) modeMenuMainSetup();
  if (_modeNew == MODE_STATUS ) modeStatusSetup();
  if (_modeNew == MODE_MENU_MELODIES ) modeMenuMelodiesSetup();
  if (_modeNew == MODE_MENU_SETTINGS ) modeMenuSettingsSetup();
  if (_modeNew == MODE_MENU_SET_TIME ) modeMenuSetTimeSetup();
  if (_modeNew == MODE_ABOUT ) modeAboutSetup();
  if (_modeNew == MODE_MENU_APPS ) modeMenuAppsSetup();
  if (_modeNew == MODE_STOPWATCH ) modeStopwatchSetup();
  if (_modeNew == MODE_SET_ALARM ) modeSetAlarmSetup();  
  
  _mode = _modeNew;
}

//Выплнить всю последовательность действий требуемых для выполнения перезагрузки.
void reboot(){
  Display.displayPowerOff();
  delay(1000);
  resetFunc(); //вызываем reset
}

//переключить на режим выбранного в меню вотчфейса из любого места программы
void goToWatchface(){
  setMode(MODE_WATCHFACE);
}

//объём свободной оперативки нужен для отладки
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//обновляем счётчик миллисекунд во время сна чтобы не ломались счётчики
void setMillis(unsigned long ms){
    extern unsigned long timer0_millis;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        timer0_millis = ms;
    }
}
