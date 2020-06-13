/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara on this : https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_server_multiconnect/BLE_server_multiconnect.ino
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <sstream>




// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

class MyServerCallbacks: public BLEServerCallbacks {
  
  void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    }

  void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCharacteristicCallbackHandler: public BLECharacteristicCallbacks {
  
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
  
  // create the BLE Device
  BLEDevice::init("myESP32");

  // create the BLE server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // create the BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // create the BLE characteristic
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // define callback for the Characteristic
  pCharacteristic->setCallbacks(new MyCharacteristicCallbackHandler());
  
  // start the service
  pService->start();

  
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x12);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();

  Serial.println("Waiting a client connection to notify...");

}

void loop() {

  // notify changed value
    if (deviceConnected) {
      
        Serial.println("start advertising : device connected = 1");
        // pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        // value++;
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        Serial.println("start advertising : device connected = 0 and oldDeviceConnected = 1");
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        Serial.println("start advertising : device connected = 1 and oldDeviceConnected = 0");
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

    delay(1000);
}
