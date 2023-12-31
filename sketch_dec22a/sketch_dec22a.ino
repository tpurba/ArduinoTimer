#include <BTAddress.h>
#include <BTAdvertisedDevice.h>
#include <BTScan.h>
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
long interval = 5000;       // interval at which to print Hello (milliseconds)
unsigned long currentMillis = 0;
unsigned long wakeUpTime = 0;
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
  Serial.begin(9600);
  as = IDLE;
  SerialBT.begin("SunRise"); // Bluetooth device name
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
        Serial.println("currentMillis:" + String(currentMillis));
        Serial.println("wakeUpTime:" + String(wakeUpTime));
        as = ALARM;
      }
    break;
    case ALARM:
      //Serial.println("in alarm state");
      Serial.println("Wake Up!");
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
