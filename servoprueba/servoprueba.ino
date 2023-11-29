/*
  Creado: Luis del Valle (ldelvalleh@programarfacil.com)
  https://programarfacil.com
*/
 
// Incluímos la librería para poder controlar el servo
#include <Servo.h>
 
// Declaramos la variable para controlar el servo
Servo servoMotor1;
Servo servoMotor2;
 
void setup() {
  // Iniciamos el monitor serie para mostrar el resultado
  Serial.begin(9600);
 
  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoMotor1.attach(9);
  servoMotor2.attach(10);
}
 
void loop() {
 
  servoMotor1.write(0);
  servoMotor2.write(80);
  delay(1000);
  servoMotor1.write(80); 
  servoMotor2.write(0);
  delay(1000);

}
