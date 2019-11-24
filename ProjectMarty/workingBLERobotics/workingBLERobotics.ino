#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


BLECharacteristic *pCharacteristic;
BLEDescriptor *pDescriptor;
bool deviceConnected = false;
bool deviceNotifying = false;
uint8_t txValue = 0;


#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Motor 01
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor 02
int motor2Pin1 = 32;
int motor2Pin2 = 33;
int enable2Pin = 25;

// Motor 03
int motor3Pin1 = 21;
int motor3Pin2 = 22;
int enable3Pin = 23;

// Motor 04
int motor4Pin1 = 5;
int motor4Pin2 = 18;
int enable4Pin = 19;

// Motor 05
int motor5Pin1 = 4;
int motor5Pin2 = 16;
int enable5Pin = 17;

// Motor 06
int motor6Pin1 = 15;
int motor6Pin2 = 2;
int enable6Pin = 0;


// Setting PWM properties
const int pwmChannel = 0;
const int resolution = 8;
const int dutyCycle = 255;
const int freq = 1000;



void mot_up() 
{
    
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor3Pin1, HIGH);
    digitalWrite(motor4Pin1, HIGH);
    digitalWrite(motor5Pin1, HIGH);
    digitalWrite(motor6Pin1, HIGH);

    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin2, LOW); 
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin2, LOW); 
    digitalWrite(motor5Pin2, LOW);
    digitalWrite(motor6Pin2, LOW); 
    
    ledcWrite(pwmChannel, dutyCycle);
    
}

void mot_down() 
{
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(motor3Pin2, HIGH);
    digitalWrite(motor4Pin2, HIGH);
    digitalWrite(motor5Pin2, HIGH);
    digitalWrite(motor6Pin2, HIGH);

    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor2Pin1, LOW); 
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor4Pin1, LOW); 
    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor6Pin1, LOW); 
    
    ledcWrite(pwmChannel, dutyCycle);
  
}
void mot_left() 
{

    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor3Pin1, HIGH);
    digitalWrite(motor4Pin2, HIGH);
    digitalWrite(motor5Pin2, HIGH);
    digitalWrite(motor6Pin2, HIGH);

    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin2, LOW); 
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin1, LOW); 
    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor6Pin1, LOW);
    ledcWrite(pwmChannel, dutyCycle); 
}
void mot_right()
{
  
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(motor3Pin2, HIGH);
    digitalWrite(motor4Pin1, HIGH);
    digitalWrite(motor5Pin1, HIGH);
    digitalWrite(motor6Pin1, HIGH);

    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor2Pin1, LOW); 
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor4Pin2, LOW); 
    digitalWrite(motor5Pin2, LOW);
    digitalWrite(motor6Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle); 
}
void mot_off()
{
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor2Pin1, LOW); 
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor4Pin1, LOW); 
    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor6Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin2, LOW); 
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin2, LOW); 
    digitalWrite(motor5Pin2, LOW);
    digitalWrite(motor6Pin2, LOW); 
    ledcWrite(pwmChannel, 0);
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
          
        //Here is the conditional that drives the motors backwards. 
        if (rxValue=="up") 
        {  
          Serial.print("Received Value: HIGH");
          mot_up();
        }

        //Here is the conditionals that drives the mortors forward. 
        if (rxValue=="down") 
        { 
     
          Serial.print("Received Value: LOW");
          mot_down();
        }

        //Here is the code that drives the motors right. 
        if (rxValue=="left")
        {
          mot_right();
        }
      
        //Here is the conditional that drives the motors Left. 
        if (rxValue=="left") 
        {
          mot_left();
        }

        if (rxValue=="off")
        {
          mot_off();
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



  // Initialize motors to pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(enable3Pin, OUTPUT);
  
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);
  pinMode(enable4Pin, OUTPUT);

  pinMode(motor5Pin1, OUTPUT);
  pinMode(motor5Pin2, OUTPUT);
  pinMode(enable5Pin, OUTPUT);
  
  pinMode(motor6Pin1, OUTPUT);
  pinMode(motor6Pin2, OUTPUT);
  pinMode(enable6Pin, OUTPUT);

  //Initialize PWM for motors
  ledcSetup(pwmChannel, freq, resolution);
  
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);
  ledcAttachPin(enable3Pin, pwmChannel);
  ledcAttachPin(enable4Pin, pwmChannel);
  ledcAttachPin(enable5Pin, pwmChannel);
  ledcAttachPin(enable6Pin, pwmChannel);
  
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  if (deviceConnected && deviceNotifying) {
    Serial.printf("*** Sent Value: %d ***\n", txValue);
    pCharacteristic->setValue(&txValue, 1);
    pCharacteristic->notify();
    txValue++;
  }
  
}
