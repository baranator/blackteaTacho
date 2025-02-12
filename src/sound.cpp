#include "sound.h"
#include "assets/avassound.h"

#include <ESP_I2S.h>
//#include <DebugLog.h>


I2SClass I2S;
boolean sound_on=true;
unsigned long laststep;
unsigned long i=0;
unsigned int rate=100;
uint8_t volume_perc=70;
int8_t fac=1;

const uint16_t PLAY_BUF_SIZE=100; //should be a multiple of 100

unsigned char play_buffer[PLAY_BUF_SIZE]; 



/** sanitize index */
uint16_t sind(uint16_t i){
  i=i%sizeof(avasbytes);
  return i;
}

void soundLoop() {
  //cut off header and plopp at the end of file


  if(millis()-laststep>10000){
    if(rate>=300){
      fac=-1;
    }else if(rate<=100){
      fac=1;
    }
    rate=rate+fac*20;
    laststep=millis();
  }
  

  //für rate=1000 , 10x beschleunigt, rate=50, speed halbiert
  if(rate>=100 && sound_on){
    //combine multiple samples to their average -> speed rises as effectivly more samples are played
    //in the same time-interval, if the sampling rate is steady

    for(int j=0;j<PLAY_BUF_SIZE;j++){

      unsigned int index=j*rate/100;
      unsigned int rest=j*rate%100;
      uint16_t si=sind(i+index);
      
      if(rest == 0){
        //index ist ganzzahlig
        play_buffer[j]=(u8_t)(avasbytes[si]*(u16_t)volume_perc/100);
      }else{
        
        play_buffer[j]=(u8_t)((avasbytes[si]+(avasbytes[sind(i+index+1)]-avasbytes[si])*((u16_t)rest)/100)*(u16_t)volume_perc/(100));
      }

    }
    unsigned long written=I2S.write(play_buffer, PLAY_BUF_SIZE);
    i+=written*rate/100;
    delay(1);
  }else{
    delay(3); //needed to give cpu some time if sound ist off
  }
}

void soundT(void *pvParameters){

  I2S.setPins(0,18, 17); //SCK, WS, SDOUT, SDIN, MCLK
  I2S.begin(I2S_MODE_STD, 22000, I2S_DATA_BIT_WIDTH_8BIT, I2S_SLOT_MODE_MONO);
  while(1){
    soundLoop();
  }
}

void soundSetup(){
  laststep=millis();

  xTaskCreate(
    soundT,       // Function name of the task
    "Blink 1",    // Name of the task (e.g. for debugging)
    2048,         // Stack size (bytes)
    NULL,         // Parameter to pass
    1,            // Task priority
    NULL // Task handle
);

 // I2S.setPins(0,18, 17); //SCK, WS, SDOUT, SDIN, MCLK
 // I2S.begin(I2S_MODE_STD, 22000, I2S_DATA_BIT_WIDTH_8BIT, I2S_SLOT_MODE_MONO);

}
void soundEnable(){

  sound_on=true;
}
void soundDisable(){
  sound_on=false;
}