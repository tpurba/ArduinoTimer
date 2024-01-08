#include <BluetoothSerial.h>
//library for spi and gif for lcd round display 
#include <SPI.h>
#include <TFT_eSPI.h>     // Install this library with the Arduino IDE Library Manager
#include <AnimatedGIF.h>  // Install this library with the Arduino IDE Library Manager
AnimatedGIF gif;
//include images gif 
#include "images/sunriseFinal.h"

TFT_eSPI tft = TFT_eSPI();
BluetoothSerial SerialBT;
long interval;
unsigned long currentMillis = 0;
unsigned long wakeUpTime = 0;
//define image 
#define GIF_IMAGE sunriseFinal

//enumerate
enum AlarmState{
  IDLE, 
  INIT, 
  ACTIVE,
  ALARM,
  ABORT
};
AlarmState as;
int inputValue;
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(2);     // Adjust Rotation of your screen (0-3)
  tft.fillScreen(TFT_BLACK);
  as = IDLE;
  SerialBT.begin("SunRise"); // Bluetooth device name
  gif.begin(BIG_ENDIAN_PIXELS);
}

void loop() {
  switch (as){
    case IDLE:
      //Serial.println("in idle state");
      if(SerialBT.available()){
        as = INIT;
      }
    break;
    case INIT:
      //Serial.println("in init state");
      setAlarm();
      as = ACTIVE;
    break;
    case ACTIVE:
      //Serial.println("in active state");
      currentMillis = millis();
      if(currentMillis >= wakeUpTime){
        as = ALARM;
      }
    break;
    case ALARM:
      //Serial.println("in alarm state");
      Serial.println("Wake Up!");
      playImage();
      tft.fillScreen(TFT_BLACK);
      as = IDLE;
    break;
    case ABORT:
      Serial.println("in abort state");
    break;
  }
}
void setAlarm(){
  //take out and set the time for alarm and the display color on the alarm
  String message = SerialBT.readStringUntil('\n');
  Serial.println("Received: " + message);
  interval = message.toInt();
  Serial.println("interval: " + String(interval));
  // Get the current time
  currentMillis = millis();
  //set the wake up time
  wakeUpTime = currentMillis + interval;
}
void playImage(){
  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    tft.startWrite();
    while (gif.playFrame(true, NULL))
    {
      yield();
    }
    gif.close();
    tft.endWrite(); 
    Serial.println("Successfully finished gif");
  }
}
