#include <Servo.h>

const int buttonPin = 2;
const int ledHijau = 3;
const int ledMerah = 4;
const int servoPin = 9;

int statusPintu = 0;
int buttonState = 0;
int lastButtonState = LOW;   
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;   

void setup() {
  doorServo.attach(servoPin);
  pinMode(buttonPin, INPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(ledMerah, OUTPUT);

  doorServo.write(0);
  digitalWrite(ledMerah, HIGH);
  digitalWrite(ledHijau, LOW);
}

void loop() {
  int mocopin = digitalRead(buttonPin);

  if (mocopin != lastButtonState){
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (mocopin != buttonState) {
      buttonState = mocopin;

      if (buttonState == HIGH) {
        statusPintu = !statusPintu;
        
        if (statusPintu == 1) {
          // Buka Pintu
          doorServo.write(90);
          digitalWrite(ledHijau, HIGH);
          digitalWrite(ledMerah, LOW);
        } else {
          // Tutup Pintu
          doorServo.write(0);
          digitalWrite(ledMerah, HIGH);
          digitalWrite(ledHijau, LOW);
        }
      }
    }
  }
  lastButtonState = mocopin;
}
