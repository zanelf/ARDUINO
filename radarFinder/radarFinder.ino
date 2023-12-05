#include <Servo.h>
#include <Wire.h>
#include "SparkFunISL29125.h"


SFE_ISL29125 RGB_sensor;

int lista[1500]; //pila para poder devolverse 
int movimientos=0; //cantidad de movimientos que realiza 
bool encontrado = false; //verificador si encontro algo 

//Motores Izquierda
int E1 = 4;
int M1 = 26;

//Motores Derecha
int E2 =  3;
int M2 = 24;

//infrarojo
int IR = 40;

//led RGB
int ledRojo = 2;
int ledVerde = 12;
int ledAzul = 13;


//Variables sensor Ultrasonico
int pinEcho_cercania = 52;
int pinTrig_cercania = 53;

int pinEcho_radar = 50; 
int pinTrig_radar = 51;

//Variable SERVO
Servo servoMotor1;
Servo servoMotor2;


// Velocidad de motores
int velocidad = 170;   //255 max , (160,80,200)
int velocidadAtras = 110; // 150,
int velocidadDoblar = 180;

//Delay
int espera = 10;  //Milisegundos



void setup(){
  pinMode(pinTrig_cercania, OUTPUT);
  pinMode(pinEcho_cercania, INPUT);
  pinMode(pinTrig_radar, OUTPUT);
  pinMode(pinEcho_radar, INPUT);

  pinMode(IR, INPUT);

  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);

  servoMotor1.attach(9);
  servoMotor2.attach(10);
  Serial.begin(115200);  // Monitor serial
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
