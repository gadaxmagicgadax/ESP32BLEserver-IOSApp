//
//  ViewController.swift
//  TestBLEClient
//
//  Created by giovanni gadaleta on 31/05/2020.
//  Copyright © 2020 nextrace. All rights reserved.
//

import UIKit
import CoreBluetooth

class ViewController: UIViewController, CBPeripheralDelegate, CBCentralManagerDelegate {

    private var centralManager: CBCentralManager!
    private var peripheral: CBPeripheral!
    // Characteristics
    private var countChar: CBCharacteristic?
    
    @IBAction func Send2ESP(_ sender: Any) {
        
        
        let slider = UInt8.init(CountLabel.text!)
        // let dtc = CountLabel.text?.data(using: String.Encoding.utf8)
        
        writeCountToChar(withCharacteristic: countChar!, withValue: Data([slider!]))
        
        
    }
    
    @IBAction func ReadESP(_ sender: Any) {
        let valueRead = readCountToChar(withCharacteristic: countChar!)
        print("ReadESP valueRead = \(valueRead)")
  
        
    }
    
    @IBOutlet weak var CountLabel: UILabel!
    
    @IBOutlet weak var CSlider: UISlider!
    
    @IBAction func CountSlider(_ sender: UISlider) {
        
        CountLabel.text = String(Int(sender.value))
        let slider:UInt8 = UInt8(Int(sender.value))
    }
    @IBOutlet weak var TextViewESP32: UITextView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        centralManager = CBCentralManager(delegate: self, queue: nil)

        
    }
    
    // If we're powered on, start scanning
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        print("Central state update")
        if central.state != .poweredOn {
            print("Central is not powered on")
        } else {
            print("Central scanning for", ESP32Peripheral.countServiceUUID);
            TextViewESP32.text = "Central Scanning for \(ESP32Peripheral.countServiceUUID)\n";
            centralManager.scanForPeripherals(withServices: [ESP32Peripheral.countServiceUUID],
                                              options: [CBCentralManagerScanOptionAllowDuplicatesKey : true])
        }
    }
    
    // Handles the result of the scan
        func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {

            // We've found it so stop scan
            self.centralManager.stopScan()

            // Copy the peripheral instance
            self.peripheral = peripheral
            self.peripheral.delegate = self

            // Connect!
            self.centralManager.connect(self.peripheral, options: nil)
            print("Connected to your peripheral")

        }

    // The handler if we do connect succesfully
        func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
            if peripheral == self.peripheral {
                print("Connected to your Count Board")
                peripheral.discoverServices([ESP32Peripheral.countServiceUUID])
            }
        }

    // Handles discovery event
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let services = peripheral.services {
            for service in services {
                print("service in for : " + String(describing: service))
                if service.uuid == ESP32Peripheral.countServiceUUID {
                    print("count service found")
                    //Now kick off discovery of characteristics
                    peripheral.discoverCharacteristics([ESP32Peripheral.countCharacteristicUUID], for: service)
                    TextViewESP32.text += "Found characteristic \(ESP32Peripheral.countCharacteristicUUID)\n"
                    return
                }
            }
        }
    }
    
    // Handling discovery of characteristics
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                if characteristic.uuid == ESP32Peripheral.countCharacteristicUUID {
                    print("count characteristic found")
                    countChar = characteristic
                    TextViewESP32.text = "Characteristic value is \(String(describing: countChar))\n"
                    
                    if characteristic.properties.contains(.notify)  {
                        TextViewESP32.text += " - NOTIFY"
                    }
                    if characteristic.properties.contains(.write)  {
                        TextViewESP32.text += " - WRITE"
                    }
                    if characteristic.properties.contains(.writeWithoutResponse)  {
                        TextViewESP32.text += " - WRITE_WR"
                    }
                    if characteristic.properties.contains(.read)  {
                        TextViewESP32.text += " - READ"
                    }
                    if characteristic.properties.contains(.indicate)  {
                        TextViewESP32.text += " - INDICATE"
                    }
                    CSlider.isEnabled = true
                   
                }
            }
        }
    }
    
    private func writeCountToChar( withCharacteristic characteristic: CBCharacteristic, withValue value: Data) {

        // Check if it has the write property
        //if characteristic.properties.contains(.writeWithoutResponse) && peripheral != nil {
        if characteristic.properties.contains(.write) && peripheral != nil {
            
            print("writing the characteristic value : \(value[0])")
            print("lenght of value = \(value.count)")
            // peripheral.writeValue(value, for: characteristic, type: .withoutResponse)
            peripheral.writeValue(value, for: characteristic, type: .withResponse)
            
            
            
        }

    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor descriptor: CBDescriptor, error: Error?) {
        print("didWriteValue for characteristic")
        if let error = error {
            print("didWriteValueFailed… error: \(error)")
            return
        }
    }
    

    private func readCountToChar( withCharacteristic characteristic: CBCharacteristic) -> Int {
        
        // Check if it has the write property
        //if characteristic.properties.contains(.writeWithoutResponse) && peripheral != nil {
        if characteristic.properties.contains(.read) && peripheral != nil {
            
            peripheral.readValue(for: characteristic)
            
            guard let csv = characteristic.value else { return -1}
            let bA = [UInt8](csv)
            print("reading the characteristic value : ",bA[0])
            CountLabel.text = String(Int(bA[0]))
            
        }
        if characteristic.value == nil {
            return(0)
        } else {
            return(1)
        }
        

    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        print("didupdatevalue for characteristic")
        if let error = error {
            print("didupdatevalueFailed… error: \(error)")
            return
        }

        print("characteristic uuid: \(characteristic.uuid), value: \(characteristic.value)")
        
    }
}

