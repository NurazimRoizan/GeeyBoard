# 1 "C:\\Users\\rnura\\AppData\\Local\\Temp\\tmpikzdaddr"
#include <Arduino.h>
# 1 "C:/coffeez/saiki/GeeyBoard/ProjectThing/ProjectThing.ino"



#include <WiFiMulti.h>
#include <Adafruit_EPD.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

#if __has_include("private.h")

# include "private.h"
#endif
#include "unPhone.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <Arduino.h>
#include <BleKeyboard.h>

const uint16_t kIrLedPin = 12;
IRsend irsend(kIrLedPin);
BleKeyboard bleKeyboard("unPhone9 Geey", "unPhone Co.", 100);

unPhone u = unPhone("everything");

static uint32_t loopIter = 0;
bool useWifi = true;
WiFiMulti wifiMulti;
void wifiSetup();
void wifiConnectTask(void *);
WiFiClient client;
const char* host = "192.168.0.23";
const int port = 3000;
void setup();
void loop();
void wifiConnectTask(void *param);
void ledTest();
#line 35 "C:/coffeez/saiki/GeeyBoard/ProjectThing/ProjectThing.ino"
void setup() {

  Serial.begin(115200);
  Serial.printf("Starting build from: %s\n", u.buildTime);
  u.begin();
  u.store(u.buildTime);


  #if UNPHONE_FACTORY_MODE == 1
    if(u.button1() && u.button2() && u.button3()) {
      u.factoryTestMode(true);
      u.factoryTestSetup();
      return;
    }
  #endif


  u.printWakeupReason();
  u.checkPowerSwitch();
  Serial.printf("battery voltage = %3.3f\n", u.batteryVoltage());
  Serial.printf("enabling expander power\n");
  u.expanderPower(true);


  u.ir(true); u.rgb(0, 0, 0);
  u.rgb(1, 0, 0); delay(300); u.rgb(0, 1, 0); delay(300);
  u.expanderPower(false);
  u.rgb(0, 0, 1); delay(300); u.rgb(1, 0, 0); delay(300);
  u.expanderPower(true);
  u.ir(false);
  u.rgb(0, 1, 0); delay(300); u.rgb(0, 0, 1); delay(300);
  for(uint8_t i = 0; i<4; i++) {
    u.ir(true); delay(300);
    u.expanderPower(false);
    u.ir(false); delay(300);
    u.expanderPower(true);
  }
  u.rgb(0, 0, 0);


  for(int i = 0; i < 3; i++) {
    u.vibe(true); delay(150);
    u.vibe(false); delay(150);
  }
  u.printStore();


  if(useWifi) {
    Serial.println("trying to connect to wifi...");
    wifiSetup();
    xTaskCreate(wifiConnectTask, "wifi connect task", 4096, NULL, 1, NULL);
  }

  u.provisioned();

  irsend.begin();


  bleKeyboard.onConnect([](){ Serial.println("onConnect"); });
  bleKeyboard.onDisconnect([](){ Serial.println("onDisconnect"); });
  bleKeyboard.begin();
  Serial.println("NimBLE Keyboard service started. Ready to pair.");

  Serial.println("done with setup()");
}

void loop() {
#if UNPHONE_FACTORY_MODE == 1
  if(u.factoryTestMode()) { u.factoryTestLoop(); return; }
#endif


  if(loopIter++ == 0)
    u.loraSend("first time: UNPHONE_SPIN=%d MAC=%s", UNPHONE_SPIN, u.getMAC());
  else if(loopIter == 20000000)
    u.loraSend("20000000: UNPHONE_SPIN=%d MAC=%s", UNPHONE_SPIN, u.getMAC());

  if(loopIter % 25000 == 0)
    delay(100);



  if (u.button1()){
    Serial.println("Tringle button is pressed");
    if (bleKeyboard.isConnected()) {


    Serial.println("Button Pressed! Sending launch command (Ctrl+Alt+F12)...");


    bleKeyboard.press(KEY_DOWN_ARROW);
    delay(100);
    bleKeyboard.releaseAll();


    while (u.button1()) {
      delay(50);
    }

    Serial.println("Command sent.");
  }
  }
  if (u.button3()){
    Serial.println("Square button is pressed");
    irsend.sendNEC(0x20DF10EF, 32);
    Serial.println("Done sending infrared burst");
    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

  }
  if (u.button2()){
    irsend.sendPanasonic64(0x40040100BCBD, 48);
    Serial.println("Middle button is pressed");
    bleKeyboard.press(KEY_TAB);
    bleKeyboard.releaseAll();
  }

}

void wifiSetup() {
#ifdef _MULTI_SSID1
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID1);
  wifiMulti.addAP(_MULTI_SSID1, _MULTI_KEY1);
#endif
#ifdef _MULTI_SSID2
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID2);
  wifiMulti.addAP(_MULTI_SSID2, _MULTI_KEY2);
#endif
#ifdef _MULTI_SSID3
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID3);
  wifiMulti.addAP(_MULTI_SSID3, _MULTI_KEY3);
#endif
#ifdef _MULTI_SSID4
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID4);
  wifiMulti.addAP(_MULTI_SSID4, _MULTI_KEY4);
#endif
#ifdef _MULTI_SSID5
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID5);
  wifiMulti.addAP(_MULTI_SSID5, _MULTI_KEY5);
#endif
#ifdef _MULTI_SSID6
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID6);
  wifiMulti.addAP(_MULTI_SSID6, _MULTI_KEY6);
#endif
#ifdef _MULTI_SSID7
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID7);
  wifiMulti.addAP(_MULTI_SSID7, _MULTI_KEY7);
#endif
#ifdef _MULTI_SSID8
  Serial.printf("wifiMulti.addAP 8\n");
  wifiMulti.addAP(_MULTI_SSID8, _MULTI_KEY8);
#endif
}

static bool wifiConnected = false;
void wifiConnectTask(void *param) {
  while(true) {
    bool previousWifiState = wifiConnected;
    if(wifiMulti.run() == WL_CONNECTED)
      wifiConnected = true;
    else
      wifiConnected = false;


    if(previousWifiState != wifiConnected) {
      previousWifiState = wifiConnected;
      u.provisioned();
    }

    delay(1000);
  }
}

void ledTest() {
# 227 "C:/coffeez/saiki/GeeyBoard/ProjectThing/ProjectThing.ino"
  D("\nLED pins:\n")
  D("u.IR_LEDS = %#02X,   %3u\n", u.IR_LEDS, u.IR_LEDS & 0b10111111)
  D("LED_BUILTIN=%#02X,   %3u\n", LED_BUILTIN, LED_BUILTIN & 0b10111111)
  D("u.LED_RED = %#02X,   %2u\n", u.LED_RED, u.LED_RED & 0b10111111)
  D("u.LED_GREEN = %#02X, %2u\n", u.LED_GREEN, u.LED_GREEN & 0b10111111)
  D("u.LED_BLUE = %#02X,  %2u\n", u.LED_BLUE, u.LED_BLUE & 0b10111111)
  delay(4000);
  D("IR...\n")
  D("IR ON\n") u.ir(true); delay(4000);
  D("IR OFF\n") u.ir(false); delay(4000);
  D("RGB...\n")
  delay(4000);
  D("000 off\n") u.rgb(0, 0, 0); delay(4000);
  D("111 all\n") u.rgb(1, 1, 1); delay(4000);
  D("110 r+g\n") u.rgb(1, 1, 0); delay(4000);
  D("101 r+b\n") u.rgb(1, 0, 1); delay(4000);
  D("011 g+b\n") u.rgb(0, 1, 1); delay(4000);
  D("100 red\n") u.rgb(1, 0, 0); delay(4000);
  D("010 grn\n") u.rgb(0, 1, 0); delay(4000);
  D("001 blue\n") u.rgb(0, 0, 1); delay(4000);
  D("000 off\n") u.rgb(0, 0, 0); delay(4000);

  D("\nRGB+IR\n") u.ir(true); delay(4000);
  D("111 all\n") u.rgb(1, 1, 1); delay(4000);
  D("110 r+g\n") u.rgb(1, 1, 0); delay(4000);
  D("101 r+b\n") u.rgb(1, 0, 1); delay(4000);
  D("011 g+b\n") u.rgb(0, 1, 1); delay(4000);
  D("100 red\n") u.rgb(1, 0, 0); delay(4000);
  D("010 grn\n") u.rgb(0, 1, 0); delay(4000);
  D("001 blue\n") u.rgb(0, 0, 1); delay(4000);
  D("000 off\n") u.rgb(0, 0, 0); delay(4000);
  D("IR OFF\n") u.ir(false); delay(4000);
}