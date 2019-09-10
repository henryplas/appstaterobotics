

// Motor 01
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor 02
int motor2Pin1 = 32;
int motor2Pin2 = 33;
int enable2Pin = 25;

// Motor 03
int motor3Pin1 = 22;
int motor3Pin2 = 21;
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
int motor6Pin1 = 2;
int motor6Pin2 = 15;
int enable6Pin = 0;

//Pi pins
int bit0 = 13;
int bit1 = 34;
int bit2 = 35;

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




void setup() {
  



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

  pinMode(bit0, INPUT);
  pinMode(bit1, INPUT);
  pinMode(bit2, INPUT);


  //Initialize PWM for motors
  ledcSetup(pwmChannel, freq, resolution);
  
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);
  ledcAttachPin(enable3Pin, pwmChannel);
  ledcAttachPin(enable4Pin, pwmChannel);
  ledcAttachPin(enable5Pin, pwmChannel);
  ledcAttachPin(enable6Pin, pwmChannel);
  

}

void loop() {
  int state0 = digitalRead(bit0);
  int state1 = digitalRead(bit1);
  int state2 = digitalRead(bit2);

  if (state0 == 0 && state1 == 0 && state2 == 0)
  {
    mot_off();
  }
  else if (state0 == 1 && state1 == 0 && state2 == 0)
  {
    mot_up();
  }
  else if (state0 == 0 && state1 == 1 && state2 == 0)
  {
    mot_down();
  }
  else if (state0 == 1 && state1 == 1 && state2 == 0)
  {
    mot_left();
  }
  else if (state0 == 0 && state1 == 0 && state2 == 1)
  {
    mot_right();
  }


  
  
}
