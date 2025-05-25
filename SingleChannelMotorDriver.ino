// Single Channel Motor Driver
// Created by The Machine Shop
// Author John Lawrence
// Copyright May 2025

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10,OUTPUT);
  Serial.begin(9600);
}

int address = -128;
int command;
int speed;
int checksum;
int packet[4];
char numOfBytes;
String string, string1, string2, string3, string4;
char c;
bool led;
bool led2;
bool valid = 3;
bool stop = 1;

void serialFlush(){
  while(Serial.available() > 0) {
    Serial.read();
    digitalWrite(6, HIGH);
  }
  digitalWrite(6, LOW);
}

void loop() {
  digitalWrite(5, led);
  led = !led;

  if(Serial.available() > 3){    
    c = (char)Serial.read();
    if (c == address){
      packet[1] = Serial.read();
      packet[2] = Serial.read();
      packet[3] = Serial.read();
      if (((c + packet[1] + packet[2])& 0x7f) == packet[3]){
        command = packet[1];
        speed = packet[2];
        Serial.print("c:");
        Serial.print(command);
        Serial.print(" s:");
        Serial.println(speed);
        valid = 100;
        stop = 0;
      }
      else{
        valid -= 1;
        
      }
    }
  }
  else{
    valid -= 1;
    Serial.print("valid: ");
    Serial.println(valid);
  }
  if (valid < 1){
      stop = 1;
      valid = 0;
    }

  serialFlush();
  delay(20);
  if(stop){
    analogWrite(9, 0);
    analogWrite(10, 0);
    //Serial.println("stopped");
  }
  else if (valid){ // checksum good
    if (speed == 0){
      analogWrite(9, 0);
      analogWrite(10, 0);
    }
    if (command){ // reverse
      analogWrite(9, 0);
      analogWrite(10, int(constrain(speed*2.35, 0, 255)));
    }
    else{ // forwards
      analogWrite(10, 0);
      analogWrite(9, int(constrain(speed*2.35, 0, 255)));
    }
  }
}