#include <Servo.h>         // Library voor Servo motoren
#include <AFMotor.h>       // Library voor Adafruit motor shield

#define Echo A0            // Echo pin voor ultrasone sensor
#define Trig A1            // Trig pin voor ultrasone sensor
#define motor 10           // Pin voor Servo motor
#define Speed 170          // Motorsnelheid
#define spoint 103         // Servo middenpunt

char value;                // Variabele om serial data op te slaan
int distance;              // Variabele om gemeten afstand op te slaan
int Left;                  
int Right;               
int L = 0;                 
int R = 0;                
int L1 = 0;               
int R1 = 0;                

Servo servo;               // Servo object
AF_DCMotor M1(1);          // DC Motor 1
AF_DCMotor M2(2);          // DC Motor 2
AF_DCMotor M3(3);          // DC Motor 3
AF_DCMotor M4(4);          // DC Motor 4

void setup() {
  Serial.begin(9600);      // Start serial communicatie
  pinMode(Trig, OUTPUT);   // Stel Trig pin in als output
  pinMode(Echo, INPUT);    // Stel Echo pin in als input
  servo.attach(motor);     // Koppel servo motor aan pin
  M1.setSpeed(Speed);      // Stel snelheid van motor 1 in
  M2.setSpeed(Speed);      // Stel snelheid van motor 2 in
  M3.setSpeed(Speed);      // Stel snelheid van motor 3 in
  M4.setSpeed(Speed);      // Stel snelheid van motor 4 in
}

void loop() {
  Obstacle();              // Roep de Obstacle functie aan
}

void Obstacle() {
  distance = ultrasonic(); // Meet de afstand
  if (distance <= 12) {    // Controleer of de afstand minder is dan of gelijk is aan 12 cm
    Stop();              
    backward();           
    delay(100);            
    Stop();               
    L = leftsee();         // Meet afstand aan de linkerkant
    servo.write(spoint);   
    delay(800);           
    R = rightsee();        // Meet afstand aan de rechterkant
    servo.write(spoint);   // Zet servo terug naar middenpunt
    if (L < R) {           // Als linker afstand kleiner is dan rechter afstand
      right();             
      delay(500);          
      Stop();              
      delay(200);          
    } else if (L > R) {    // Als rechter afstand kleiner is dan linker afstand
      left();              
      delay(500);          
      Stop();              
      delay(200);        
    }
  } else {
    forward();             // Ga vooruit als er geen obstakel is
  }
}

int ultrasonic() {
  digitalWrite(Trig, LOW);    // Zet Trig pin laag
  delayMicroseconds(4);      
  digitalWrite(Trig, HIGH);   // Zet Trig pin hoog
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // Zet Trig pin weer laag
  long t = pulseIn(Echo, HIGH); // Meet tijd totdat de echo terugkomt
  long cm = t / 29 / 2;       // Bereken afstand in cm
  return cm;                  // Return de gemeten afstand
}

void forward() {
  M1.run(FORWARD);            
  M2.run(FORWARD);            
  M3.run(FORWARD);            
  M4.run(FORWARD);           
}

void backward() {
  M1.run(BACKWARD);           
  M2.run(BACKWARD);          
  M3.run(BACKWARD);           
  M4.run(BACKWARD);           
}

void right() {
  M1.run(BACKWARD);           
  M2.run(BACKWARD);          
  M3.run(FORWARD);            
  M4.run(FORWARD);           
}

void left() {
  M1.run(FORWARD);            
  M2.run(FORWARD);            
  M3.run(BACKWARD);           
  M4.run(BACKWARD);          
}

void Stop() {
  M1.run(RELEASE);           
  M2.run(RELEASE);        
  M3.run(RELEASE);      
  M4.run(RELEASE);           
}

int rightsee() {
  servo.write(-80);           // Draai servo naar rechts
  delay(800);                 // Wacht 800 milliseconden
  Left = ultrasonic();        // Meet afstand aan de rechterkant
  return Left;                // Return de gemeten afstand
}

int leftsee() {
  servo.write(260);           // Draai servo naar links
  delay(800);                 // Wacht 800 milliseconden
  Right = ultrasonic();       // Meet afstand aan de linkerkant
  return Right;               // Return de gemeten afstand
}
