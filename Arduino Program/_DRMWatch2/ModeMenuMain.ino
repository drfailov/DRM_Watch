#define MAIN_MENU_SELECTED_BACK 0
#define MAIN_MENU_SELECTED_APPS 1
#define MAIN_MENU_SELECTED_SETTINGS 2
#define MAIN_MENU_SELECTED_ABOUT 3

void modeMenuMainSetup() {
  genericMenuSetup();
}

void modeMenuMainLoop() {
  if(genericMenuRoutine(modeMenuMainSelected, 4))return;
  
#ifdef LANG_EN
  const __FlashStringHelper* menuApps = F("Apps");
  const __FlashStringHelper* menuSettings = F("Settings");
  const __FlashStringHelper* menuAbout = F("About");
#endif
#ifdef LANG_RU
  const __FlashStringHelper* menuApps = F("Пpилoжeния");
  const __FlashStringHelper* menuSettings = F("Hacтpoйки");
  const __FlashStringHelper* menuAbout = F("Cпpaвкa");
#endif
#ifdef LANG_UA
  const __FlashStringHelper* menuApps = F("Програми");
  const __FlashStringHelper* menuSettings = F("Налаштування");
  const __FlashStringHelper* menuAbout = F("Дoвiдкa");
#endif

  drawMenuItem(/*index*/MAIN_MENU_SELECTED_BACK, /*icon*/displayDrawBackIcon, /*text*/(__FlashStringHelper*)menuItemBack, /*animate*/animate);
  drawMenuItem(/*index*/MAIN_MENU_SELECTED_APPS, /*icon*/displayDrawAppsIcon, /*text*/menuApps, /*animate*/animate);
  drawMenuItem(/*index*/MAIN_MENU_SELECTED_SETTINGS, /*icon*/displayDrawIconSettings, /*text*/menuSettings, /*animate*/animate);
  drawMenuItem(/*index*/MAIN_MENU_SELECTED_ABOUT, /*icon*/displayDrawIconAbout, /*text*/menuAbout, /*animate*/animate);
  displayUpdate();
  animate = false;
}

void modeMenuMainSelected(){
  //variable is: selected
  if(selected == MAIN_MENU_SELECTED_BACK){
    goToWatchface();
  }
  else if (selected == MAIN_MENU_SELECTED_APPS){
    setMode(MODE_MENU_APPS);
  }
  else if (selected == MAIN_MENU_SELECTED_SETTINGS){
    setMode(MODE_MENU_SETTINGS);
  }
  else if (selected == MAIN_MENU_SELECTED_ABOUT){
    setMode(MODE_ABOUT);
  }
}
