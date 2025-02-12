#include "prefs.h"

Preferences stcPrefs;


Preferences* getPrefs(){
    return &stcPrefs;
}

void initPrefs(){
    stcPrefs.begin("STCPrefs"); 
    bool tpInit = stcPrefs.isKey("wasInited"); 
  
    if (tpInit == false) {
      // If tpInit is 'false', the key "nvsInit" does not yet exist therefore this
      //  must be our first-time run. We need to set up our Preferences namespace keys. So..
      stcPrefs.putBool("avasEnabled", false);
  
      stcPrefs.putBool("wasInited", true);          // Create the "already initialized"
                                                  //  key and store a value.
  
      // The "factory defaults" are created and stored so...
  }
}