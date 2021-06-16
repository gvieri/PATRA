#include "sys/time.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"
#include "BLEBeacon.h"
#include "esp_sleep.h"
#include "esp_bt_device.h"


#define GPIO_DEEP_SLEEP_DURATION     10  // sleep x seconds and then wake up
#define ADV_INTERVAL                 100 // ~= 0.1 sec. 
#define PATRA_ID                     0   // patraIDentifier ... 
#define ONBOARD_LED  2

RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory
RTC_DATA_ATTR static uint8_t  ledstat; 
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
BLEAdvertising *pAdvertising;   // BLE Advertisement type
struct timeval now;

const uint8_t* baddress; 

#define BEACON_UUID "4ab62e76-b724-11eb-8529-0242ac130003" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

void setBeacon() {

  

  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C 
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor((bootcount & 0xFFFF0000) >> 16); // using boot count can be useful as time counter
  oBeacon.setMinor(bootcount & 0xFFFF);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData(); 
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

  std::string strServiceData = "";

  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData();
  oAdvertisementData.addData(strServiceData);

  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

void setup() {

  char buffer[40]; 
  char *ptbuffer; 
  
  pinMode(ONBOARD_LED,OUTPUT);
  Serial.begin(115200);
  
  ptbuffer=buffer; 
  
  ledstat++ ; 
  Serial.printf("ledstat  %d time\n", ledstat);
  if (ledstat==10) { 
    ledstat=0; 
    digitalWrite(ONBOARD_LED,LOW);  
    Serial.printf("LED off\n");    
  } 
  if (ledstat==5) {
    digitalWrite(ONBOARD_LED,HIGH);
    Serial.printf("LED on\n"); 
  }
  
  gettimeofday(&now, NULL);
  Serial.printf("start ESP32 for the  %d time\n", bootcount++);
  Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n", now.tv_sec, now.tv_sec - last);
  last = now.tv_sec;

  // Create the BLE Device
  sprintf(buffer,"PATRA%02d as iBeacon", PATRA_ID); 
  BLEDevice::init(buffer);
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer(); 
  pAdvertising = BLEDevice::getAdvertising();
  BLEDevice::startAdvertising();
  setBeacon();
  Serial.println("Beacon has been lit!"); 
 
  baddress = esp_bt_dev_get_address();
  for (int i=0; i<6 ; i++) {
    sprintf(ptbuffer,"%02X:",baddress[i]); 
    ptbuffer+=3; 
  }
  *(ptbuffer-1)=0; 

  Serial.printf("ble address: %s\n", buffer);  
  // Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started...");
  delay(100);
  pAdvertising->stop();
  Serial.printf("enter deep sleep\n");
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
 
}

void loop() {
  // useless 
}
