//nRF Toolbox by Nordic Semiconductor
//https://www.youtube.com/watch?v=lUi2jhFIz80


#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define MOT_1F 23
#define MOT_1R 22
#define MOT_2F 18
#define MOT_2R 19


BLECharacteristic *pCharacteristic;
BLEDescriptor *pDescriptor;
bool deviceConnected = false;
bool deviceNotifying = false;
uint8_t txValue = 0;


#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

void mot_up() {
   digitalWrite(MOT_1F, 1); // turn on 
   digitalWrite(MOT_2F, 1); // turn on 
   delay(500);              // wait 
   digitalWrite(MOT_1F, 0); // turn off
   digitalWrite(MOT_2F, 0); // turn off
  
}

void mot_down() {
   digitalWrite(MOT_1R, 1); // turn on 
   digitalWrite(MOT_2R, 1); // turn on 
   delay(500);              // wait 
   digitalWrite(MOT_1R, 0); // turn off
   digitalWrite(MOT_2R, 0); // turn off
  
}
void mot_left() {
   digitalWrite(MOT_1R, 1); // turn on 
   digitalWrite(MOT_2F, 1); // turn on 
   delay(500);              // wait 
   digitalWrite(MOT_1R, 0); // turn off
   digitalWrite(MOT_2F, 0); // turn off
  
}
void mot_right() {
   digitalWrite(MOT_1F, 1); // turn on 
   digitalWrite(MOT_2R, 1); // turn on 
   delay(500);              // wait 
   digitalWrite(MOT_1F, 0); // turn off
   digitalWrite(MOT_2R, 0); // turn off
  
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        if (rxValue=="up") {
          mot_up();
        }
        if (rxValue=="down") {
          mot_down();
        }
        if (rxValue=="right") {
          mot_right();
        }
        if (rxValue=="left") {
          mot_left();
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};


class MyDisCallbacks: public BLEDescriptorCallbacks {
    void onWrite(BLEDescriptor *pDescriptor) {
      uint8_t* rxValue = pDescriptor->getValue();

      if (pDescriptor->getLength() > 0) {
        if (rxValue[0]==1) {
          //deviceNotifying=true;
        } else {
          deviceNotifying=false;
        }
        Serial.println("*********");
        Serial.print("Received Descriptor Value: ");
        for (int i = 0; i < pDescriptor->getLength(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};


void setup() {
  Serial.begin(115200);
    pinMode(MOT_1F, OUTPUT);
    pinMode(MOT_1R, OUTPUT);
    pinMode(MOT_2F, OUTPUT);
    pinMode(MOT_2R, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pDescriptor = new BLE2902();
  pCharacteristic->addDescriptor(pDescriptor);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pDescriptor->setCallbacks(new MyDisCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  if (deviceConnected && deviceNotifying) {
    Serial.printf("*** Sent Value: %d ***\n", txValue);
    pCharacteristic->setValue(&txValue, 1);
    pCharacteristic->notify();
    txValue++;
  }
  delay(1000);
}
