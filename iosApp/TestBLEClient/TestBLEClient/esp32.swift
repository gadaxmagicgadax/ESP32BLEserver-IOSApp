//
//  esp32.swift
//  TestBLEClient
//
//  Created by giovanni gadaleta on 31/05/2020.
//  Copyright © 2020 nextrace. All rights reserved.
//

import UIKit
import CoreBluetooth

class ESP32Peripheral: NSObject {

    /// MARK: - Particle LED services and charcteristics Identifiers

    public static let countServiceUUID          = CBUUID.init(string: "4fafc201-1fb5-459e-8fcc-c5c9c331914b")
    public static let countCharacteristicUUID   = CBUUID.init(string: "beb5483e-36e1-4688-b7f5-ea07361b26a8")
    
}

