#include <SoftwareSerial.h>
#include <Servo.h>
#include <math.h>   //standard c library
#include "pitches.h"

SoftwareSerial mySerial(10, 11); // RX | TX

Servo gripServo;  
Servo servo1; //gauche
Servo servo2;  //droite

int servo1Pin = 6;
int servo2Pin = 5;
int gripPos = 9;
bool autonomous = false;

char command;

/* Constantes pour les broches */
const byte TRIGGER_PIN = A3; // Broche TRIGGER
const byte ECHO_PIN = A2;    // Broche ECHO
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 1ms = ~20cm à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

//BUZZER
// notes in the melody:
int melody[] = { //using note c5 e5 a5 b4 g5
  NOTE_C5, NOTE_E5, NOTE_A5, NOTE_B4, NOTE_G5
};
// note durations: 8 = eighth note
int noteDurations[] = {
  8,8,8
};


void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   Serial.println("You're connected via Bluetooth");
   servo1.attach(servo1Pin);
   servo2.attach(servo2Pin);
   gripServo.attach(gripPos);
   gripServo.write(50);        //intial positions of servos
   servo1.write(88);
   servo2.write(88);

   /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);

  command = '0';
 }


void loop() {

  //CONTROLLER MODE
  if (mySerial.available())
   {
    command=(mySerial.read());
    if (command=='0') {
      servo1.write(88);
      servo2.write(88);
      Serial.println("pause");
      delay(500);
      autonomous = false;
    }
    if (command=='1') {
      servo1.write(140);
      servo2.write(0);
      Serial.println("front");
      delay(100);
      autonomous = false;
    }
    if (command=='2') {
      servo1.write(40);
      servo2.write(180);
      Serial.println("back");
      delay(500);
      autonomous = false;
    }
    if (command=='3') {
      servo1.write(0);
      servo2.write(0);
      Serial.println("left");
      delay(500);
      autonomous = false;
    }
    if (command=='4') {
      servo1.write(180);
      servo2.write(180);
      Serial.println("rigth");
      delay(500);
      autonomous = false;
    }
    if (command=='5') {
      gripServo.write(90);
      Serial.println("serre");
      delay(500);
      autonomous = false;
    }
    if (command=='6') {
      gripServo.write(0);
      Serial.println("désserre");
      delay(500);
      autonomous = false;
    }
     if (command=='7') {
     // iterate over the notes of the melody:
     // for (int songLoop = 0; songLoop < 3; songLoop++){//loop to play the all the notes several times
          for (int loop1 = 0; loop1 < 8; loop1++){//loop to play notes c5 e5 a5 right after each other

              tone(8, NOTE_C5, 125);
            delay(62.5);
            noTone(8);
            
            tone(8, NOTE_E5, 125);
            delay(62.5);
            noTone(8);
            
            tone(8, NOTE_A5, 125);
            delay(62.5);
            noTone(8);
            
            delay(150);
         
          }
          for (int loop2 = 0; loop2 < 3; loop2++){ //loop to play notes b4 e5 a5
            
            tone(8, NOTE_B4, 125);
            delay(62.5);
            noTone(8);
            
            tone(8, NOTE_E5, 125);
            delay(62.5);
            noTone(8);
            
            tone(8, NOTE_A5, 125);
            delay(62.5);
            noTone(8);
            
            delay(150);
            
          }
          for (int loop3 = 0; loop3 < 5; loop3++){//loop to play b4 e5 g5
            
            tone(8, NOTE_B4, 125);
            delay(62.5);
            noTone(78);
            
            tone(8, NOTE_E5, 125);
            delay(62.5);
            noTone(8);
            
            tone(8, NOTE_G5, 125);
            delay(62.5);
            noTone(8);
            
            delay(150);
            
          }
          
      //}

    }
     if (command=='8') {

      int randomNumber = random(8);
        if (randomNumber == 1){
          //Vador 
          tone(8,LA3,Q); 
      delay(1+Q); //delay duration should always be 1 ms more than the note in order to separate them.
      tone(8,LA3,Q);
      delay(1+Q);
    tone(8,LA3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    
    tone(8,LA3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    tone(8,LA3,H);
    delay(1+H);
    
    tone(8,E4,Q); 
    delay(1+Q); 
    tone(8,E4,Q);
    delay(1+Q);
    tone(8,E4,Q);
    delay(1+Q);
    tone(8,F4,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    
    tone(8,Ab3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    tone(8,LA3,H);
    delay(1+H);
        }
    

     }
    if (command=='9') {
      autonomous = true;
    }

    while (autonomous){
      //AUTONOMOUS MODE
        /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
   
  /* Affiche les résultats en mm, cm et m */
  Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)"));
   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  delay(500);

   int singleRead = 0;
   int allReads = 0;
   for (int i = 0; i< 5; i++){
      singleRead = distance_mm;
      allReads += singleRead;
    }

  if (allReads/5 < 150){
     int randomNumber = random(8);
        if (randomNumber == 1){
            Serial.println("turnright");
            servo1.write(180);
            servo2.write(180);
            delay(200); // delay so that the robot has time to turn far away from obstacle
        }
        if (randomNumber == 2){
            Serial.println("turnleft");
            servo1.write(0);
            servo2.write(0);
            delay(200); // delay so that the robot has time to turn far away from obstacle
        }
         if (randomNumber == 3){
            Serial.println("backup");
            servo1.write(88);
            servo2.write(88);
            delay(200); // delay so that the robot has time to turn far away from obstacle
            servo1.write(0);
            servo2.write(180);
            delay(1000);
        }
        if (randomNumber == 4){
            Serial.println("long right");
            servo1.write(180);
            servo2.write(180);
            delay(500); // delay so that the robot has time to turn far away from obstacle
        }      
        if (randomNumber == 5){
            Serial.println("long left");
            servo1.write(0);
            servo2.write(0);
            delay(500); // delay so that the robot has time to turn far away from obstacle
          }  
        if (randomNumber == 6){
            Serial.println("long left");
            servo1.write(0);
            servo2.write(0);
            delay(3000); // delay so that the robot has time to turn far away from obstacle
          }        
         if (randomNumber == 7 ){
            Serial.println("backup angled");
            servo1.write(88);
            servo2.write(88);
            delay(200);
            servo1.write(40);
            servo2.write(130);
            delay(2000); // delay so that the robot has time to turn far away from obstacle
          }    
  }
  else {
    servo1.write(140);
    servo2.write(0);
  }
    command=(mySerial.read());
  Serial.println(command);
  if (command=='0'){
    autonomous=false;
    break;
  }
    }
   }
   

//Serial.println(autonomous);



  
}
