#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;


// Setting PWM properties
const int freq = 10;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 100;

void processReceivedValue(char command) {

  //Forward
  if (command == '1')
  {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    ledcWrite(pwmChannel, dutyCycle);
    
  }

  //backwards
  else if (command == '2')
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);1
    ledcWrite(pwmChannel, dutyCycle);
  }


  //stop
  else if (command == '0')
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    ledcWrite(pwmChannel, 0);
  }

  return;
}

void setup() {
  Serial.begin(115200);

  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);

  if (!SerialBT.begin("ESP32")) {
    Serial.println("An error occurred initializing Bluetooth");
  } else {
    Serial.println("Bluetooth initialized");
  }
}

void loop() {

  while (SerialBT.available()) {

    char command = SerialBT.read();
    Serial.println(command);
    processReceivedValue(command);
  }

  delay(50);
}
