Abstract

This is an example of creating a BLE Server on ESP32 and an ios app to communicate in read and write with the server.

ESP32BLE_Server.ino is the c++ code taken from the examples present in Arduino IDE for ESP32 with BLE libraries. Logic it's quite simple. A Service and a Characteristic are defined and started advertising its presence. onRead and onwrite methods are defined in the MyCallbackHandler to act on read and write incoming requests. Quite tricky was to deal with the values coming in as Uint8 but containing the value in hex.

When you start the sketch in ESP32 it is immediately available to any client to connect. I tried with nRF Connect and was able to connect, to read and write values in the characteristic.

I developed also an ios app with xcode 11 in swift language on my Mac and upload on my iphone. The code is in iosApp folder.


