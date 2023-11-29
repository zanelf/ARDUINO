#include <Servo.h>
#include <Wire.h>
#include "SparkFunISL29125.h"


SFE_ISL29125 RGB_sensor;

int lista[1500];
int movimientos=0;
bool encontrado = false;

//Motores Izquierda
int E1 = 4;
int M1 = 26;

//Motores Derecha
int E2 =  3;
int M2 = 24;

//infrarojo
int IR = 40;

//Variables sensor Ultrasonico
int pinEcho_cercania = 52;
int pinTrig_cercania = 53;

int pinEcho_radar = 50; 
int pinTrig_radar = 51;

//Variable SERVO
Servo servoMotor1;
Servo servoMotor2;


// Velocidad de motores
int velocidad = 170;//255 max   , (160,80,200)
int velocidadAtras = 110; // 150,
int velocidadDoblar = 180;

//Delay
int espera = 10;  //Milisegundos

void motoresAdelante(bool dev = true) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, velocidad);
  analogWrite(E2, velocidad);
  //Serial.println("Motores hacia adelante");
  delay(espera);
    if(dev){
     Serial.print("sigo");
    lista[movimientos] = 1;
    movimientos+=1;
    }  
}

void motoresDetener(bool dev = true) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  //Serial.println("Motores hacia adelante");
  delay(espera);
  if(dev){
      lista[movimientos] = 2;
  movimientos+=1;
  }
}

void motoresAtras(bool dev = true) {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  analogWrite(E1, velocidadAtras);
  analogWrite(E2, velocidadAtras);
  //Serial.println("Motores hacia atras");
  delay(espera);
  if(dev){
      lista[movimientos] = 3;
  movimientos+=1;

  }
}
void motoresDerecha(bool dev = true) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(E1, velocidadDoblar);
  analogWrite(E2, velocidadDoblar);
  Serial.println("Motores Girando hacia la derecha");
  delay(espera);
if(dev){
    lista[movimientos] = 4;
  movimientos+=1;
}
}
void motoresIzquierda(bool dev = true) {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  analogWrite(E1, velocidadDoblar);
  analogWrite(E2, velocidadDoblar);
  Serial.println("Motores Girando hacia la izquierda");
  delay(espera);
  if(dev){
      lista[movimientos] = 5;
  movimientos+=1;

  }
}
void cerrar() {
  servoMotor1.write(0);
  servoMotor2.write(80);
  delay(1000);
}
void abrir() {
  servoMotor1.write(80);
  servoMotor2.write(0);
  delay(1000);
}
void ajuste() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  analogWrite(E1, velocidadAtras);
  analogWrite(E2, velocidadAtras);
  delay(100);
  for (int i = 0; i < 5; i++) {
    motoresDerecha();
    delay(espera);
  }
}

void devolucion(){
  for(movimientos = movimientos - 1; movimientos >= 0;movimientos--){
    switch(lista[movimientos]){
      case 1:
        motoresAtras(false);
      break;
      case 2:
      
      break;
      case 3:
        motoresAdelante(false);
      break;
      case 4:
      motoresIzquierda(false);
      break;
      case 5:
      motoresDerecha(false);
      break;
    }
  }
  encontrado = !encontrado;
}


float distancia_cercania() {
  float tiempo, distancia;
  float esperaa = 4; //Espera en microsegundos, puede elegir cualqueir valor que desee
  float esperaUS = 10; //El sensor necesita como minimo 10us en alto*/

  digitalWrite(pinTrig_cercania, LOW);
  delayMicroseconds(esperaa);
  digitalWrite(pinTrig_cercania, HIGH);
  delayMicroseconds(esperaUS);
  digitalWrite(pinTrig_cercania, LOW);
  tiempo = pulseIn(pinEcho_cercania, HIGH);
  tiempo = tiempo / 2; //se divide el tiempo total (ida+regreso) por la mitad
  distancia = tiempo / 29.2;
  return distancia;
}

float distancia_radar() {
  float tiempo, distancia;
  float esperaa = 4; //Espera en microsegundos, puede elegir cualqueir valor que desee
  float esperaUS = 10; //El sensor necesita como minimo 10us en alto*/

  digitalWrite(pinTrig_radar, LOW);
  delayMicroseconds(esperaa);
  digitalWrite(pinTrig_radar, HIGH);
  delayMicroseconds(esperaUS);
  digitalWrite(pinTrig_radar, LOW);
  tiempo = pulseIn(pinEcho_radar, HIGH);
  tiempo = tiempo / 2; //se divide el tiempo total (ida+regreso) por la mitad
  distancia = tiempo / 29.2;
  return distancia;
} 

void setup()
{
  pinMode(pinTrig_cercania, OUTPUT);
  pinMode(pinEcho_cercania, INPUT);
  pinMode(pinTrig_radar, OUTPUT);
  pinMode(pinEcho_radar, INPUT);

  pinMode(IR, INPUT);

  servoMotor1.attach(9);
  servoMotor2.attach(10);
  Serial.begin(115200);  // Monitor serial


  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
    abrir();
  }

}

void loop() {

  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();


  int negro = 450;
  int suma = red + green + blue;

  /*
    if(distancia() < 17){
        servoMotor1.write(60);
        Serial.print("color:");Serial.println(suma);
        if(suma < 800){
          servoMotor2.write(70);delay(15);servoMotor2.write(0);delay(15);servoMotor2.write(70);delay(15);servoMotor2.write(0);delay(15);
        }else{

        }
    }else{
      abrir();
      motoresAdelante();
    }

  */

  Serial.print("distancia cercania: " );Serial.println(distancia_cercania());
  Serial.print("distancia_radar: ");Serial.println(distancia_radar());
  delay(15);
}
