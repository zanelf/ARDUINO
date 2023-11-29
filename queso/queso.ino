#include <Servo.h>
//Motores Izquierda
int E1 = 4;
int M1 = 26;

//Motores Derecha
int E2 =  3;
int M2 = 24;

// Sensor Derecha
int SensorDerecha = 46;

// Sensor Izquierda
int SensorIzquierda = 44;

//Variables sensor Ultrasonico
int pinEcho = 52;
int pinTrig = 53;

//Variable SERVO
Servo myservo;

// Velocidad de motores
int velocidad = 170;//255 max   , (160,80,200)
int velocidadAtras = 110; // 150,
int velocidadDoblar = 180;

//Delay
int espera = 10;  //Milisegundos


int luz = 11;


void setup()
{
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(SensorDerecha, INPUT);    // Entradas PWM
  pinMode(SensorIzquierda, INPUT);  // Entradas PWM
  pinMode(luz,OUTPUT);
  myservo.attach(9, 560, 2320);
  Serial.begin(9600);  // Monitor serial
  digitalWrite(luz,HIGH);
}

void motoresAdelante() {
  digitalWrite(luz,HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, velocidad);
  analogWrite(E2, velocidad);
  //Serial.println("Motores hacia adelante");
  delay(espera);
}
void motoresAtras() {
  digitalWrite(luz,HIGH);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  analogWrite(E1, velocidadAtras);
  analogWrite(E2, velocidadAtras);
  //Serial.println("Motores hacia atras");
  delay(espera);
}
void motoresDerecha() {
  digitalWrite(luz,HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(E1, velocidadDoblar);
  analogWrite(E2, velocidadDoblar);
  Serial.println("Motores Girando hacia la derecha");
  delay(espera);
}
void motoresIzquierda() {
  digitalWrite(luz,HIGH);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  analogWrite(E1, velocidadDoblar);
  analogWrite(E2, velocidadDoblar);
  Serial.println("Motores Girando hacia la izquierda");
  delay(espera);
}
void motoresDetener() {
  digitalWrite(luz,LOW);
  digitalWrite(M1, HIGH);
  digitalWrite(luz,HIGH);
  digitalWrite(M2, HIGH);
   digitalWrite(luz,LOW);
  analogWrite(E1, 0);
  digitalWrite(luz,HIGH);
  analogWrite(E2, 0);
  digitalWrite(luz,LOW);
  myservo.write(0);
  digitalWrite(luz,HIGH);
  delay(500);
  digitalWrite(luz,LOW);
  myservo.write(180);
  //Serial.println("Motores detenidos");
  delay(500);
}

void loop() {

  float tiempo, distancia;
  float esperaa = 4; //Espera en microsegundos, puede elegir cualqueir valor que desee
  float esperaUS = 10; //El sensor necesita como minimo 10us en alto*/
  
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(esperaa);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(esperaUS);
  digitalWrite(pinTrig, LOW);
  tiempo = pulseIn(pinEcho, HIGH);
  tiempo = tiempo / 2; //se divide el tiempo total (ida+regreso) por la mitad
  distancia = tiempo / 29.2;
  Serial.println(distancia);


  if (distancia < 10.0) {
    motoresDetener();
    Serial.println(distancia);
  } else {

    if (digitalRead(SensorIzquierda) == HIGH && digitalRead(SensorDerecha) == HIGH) {
      motoresAdelante();
      Serial.println("adelante");

    } else if (digitalRead(SensorIzquierda) == LOW && digitalRead(SensorDerecha) == HIGH) {
      motoresDerecha();

    }else if (digitalRead(SensorIzquierda) == HIGH && digitalRead(SensorDerecha) == LOW) {
      motoresIzquierda();
    }  else if (digitalRead(SensorIzquierda) == LOW && digitalRead(SensorDerecha) == LOW) {
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      analogWrite(E1, velocidadAtras);
      analogWrite(E2, velocidadAtras);
      delay(100);
        for(int i = 0; i<5; i++){
          motoresDerecha();
          delay(espera);
      }
      //Serial.println("Motores hacia atras");
      
    }
  }
}
