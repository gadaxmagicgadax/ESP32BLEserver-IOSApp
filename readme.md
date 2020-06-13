Abstract

This is an example of creating a BLE Server on ESP32 and an ios app to communicate in read and write with the server.

ESP32BLE_Server.ino is the c++ code taken from the examples present in Arduino IDE for ESP32 with BLE libraries. Logic it's quite simple. A Service and a Characteristic are defined and started advertising its presence. onRead and onWrite methods are defined in the MyCharacteristicCallbackHandler to act on read and write incoming requests. The logic in the loop() is for multi client connect.Quite tricky was to deal with the values coming in as Uint8 but containing the value in hex.

When you start the sketch in ESP32 it is immediately available to any client to connect. I tried with nRF Connect and was able to connect, to read and write values in the characteristic.

<img src="/images/nRF-Connect.png" height="400" width="600">
---
I used different tutorials to get more knowledge around this kind of implementation on ESP32 Arduino IDE. Here is a list:
<ul>
    <li>https://github.com/nkolban/ESP32_BLE_Arduino</li>
<li>https://www.youtube.com/watch?v=osneajf7Xkg&list=PLXUGm2dW8ktSDtlaEwrxcSgk4CjhcvA7R&index=7&t=0s</li>
<li>https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/</li>
</ul>
---
I developed also an ios app with xcode 11 in swift language on my Mac and upload on my iphone. The code is in iosApp folder.

<img src="/images/TestBleClient.png" style="height:80%;">

<p>The App is quite simple. The text box at the bottom shows the characteristic advertised by the ESP32. If you don't see that text that means ESP32 is not working properly. Move the slider to select a value from 1 to 100 and then tap Write. Use nRF Connect on another device (iPad or other smartphone) to check the new value is now in the characteristic value. Tap the Read button and the value of the characteristic will show up over the slider.
<p>I know ! The layout of the app is a disaster ! Please consider this is my first app development and I have to admit xcode 11 constraints on app layout are really a nightmare !
    With xcode 11 you can connect the iphone to the Mac and see also the debug log. I've left some prints to check everything goes fine.
<p>The app uses the swift CoreBluetooth library. <a href="https://developer.apple.com/documentation/corebluetooth">Here</a> you can find the documentation and some tutorials. Other interestng tutorials I used are here:
    <ul>
    <li>https://www.raywenderlich.com/231-core-bluetooth-tutorial-for-ios-heart-rate-monitor#toc-anchor-013</li>
        <li>https://www.novelbits.io/intro-ble-mobile-development-ios/</li>
    </ul>

