/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <sstream>




// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyCallbackHandler: public BLECharacteristicCallbacks {
  
  public: int newvalue = 0;
  
  void onRead(BLECharacteristic* pCharacteristic) {
    
    std::string s = pCharacteristic->getValue();
    std::stringstream ss;
    ss << std::hex << s;
    ss >> newvalue;
    
    Serial.printf("Client read newvalue id : %d\n",newvalue);
  }

  void onWrite(BLECharacteristic* pCharacteristic) {
    
    std::string s = pCharacteristic->getValue();
    std::stringstream ss;
    ss << std::hex << s;
    ss >> newvalue;
    Serial.printf("length of element from iphone = %d\n",s.length());
    Serial.printf(" newvalue as string arrived from client write : %s\n",s.c_str());
    Serial.printf(" newvalue id after client write : %d\n",newvalue);
  }
  
};
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("myESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );

  pCharacteristic->setValue("781A");

  Serial.printf("Initial characteristic value : %s\n", pCharacteristic->getValue());
  
  // to send value to clients when they ask with "read" we must instantiate the class MyCallbackHandler defined before
  pCharacteristic->setCallbacks(new MyCallbackHandler());
  
  pService->start();
  
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");


}

void loop() {
  
  // put your main code here, to run repeatedly:
  
  delay(2000);
}
