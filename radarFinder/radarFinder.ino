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
Servo ServoRadar;


// Velocidad de motores
int velocidad = 170;   //255 max , (160,80,200)
int velocidadAtras = 110; // 150,
int velocidadDoblar = 180;

//Delay
int espera = 10;  //Milisegundos

long duration;
int distance;




void led(int uno,int dos,int tres){
  digitalWrite(ledRojo,uno);
  digitalWrite(ledVerde,dos);
  digitalWrite(ledAzul,tres);
}

void motoresDetener(bool dev = true) {
	digitalWrite(M1, HIGH);
	digitalWrite(M2, HIGH);
	analogWrite(E1, 0);
	analogWrite(E2, 0);
	delay(espera);
	if(dev){
		lista[movimientos] = 2;
		movimientos+=1;
	}
}


void motoresAdelante(bool dev = true) { //si la Variable es verdadera guardara el realizar el movimiento

	for(int i = 0; i < 15; i++){    //esta modificacion esta hecha para ajustar cuanto se mueve el robot    
		digitalWrite(M1, HIGH);
		digitalWrite(M2, HIGH);
		analogWrite(E1, velocidad);
		analogWrite(E2, velocidad);
		delay(espera);
	}
	delay(10);
	if(dev){ 
		lista[movimientos] = 1;
		movimientos+=1;
    }  
	motoresDetener();	
}


//con los otros movimientos queda revisar si en los otros casos deberia aplicarse la misma logica 


void motoresAtras(bool dev = true) {
	digitalWrite(M1, LOW);
	digitalWrite(M2, LOW);
	analogWrite(E1, velocidadAtras);
	analogWrite(E2, velocidadAtras);
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
	delay(espera);
	if(dev){
		lista[movimientos] = 5;
		movimientos+=1;
	}
}


void devolucion(){
	for(movimientos = movimientos - 1; movimientos >= 0;movimientos--){
		switch(lista[movimientos]){ //switch que revisa que movimiento realizara ahora 
		case 1:
			motoresAtras(false);
		break;
		case 2:
			motoresAdelante(false);
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



//cerrar y abrir aplican especificamente a los brazos para agarrar
void cerrar() {
	servoMotor1.write(95);
	servoMotor2.write(50);
	delay(1000);
}
void abrir() {
	servoMotor1.write(170);
	servoMotor2.write(0);
	delay(1000);
}


void setup(){
	pinMode(pinTrig_cercania, OUTPUT);
	pinMode(pinEcho_cercania, INPUT);
	pinMode(pinTrig_radar, OUTPUT);
	pinMode(pinEcho_radar, INPUT);

	pinMode(IR, INPUT);

	pinMode(ledRojo,OUTPUT);
	pinMode(ledVerde,OUTPUT);
	pinMode(ledAzul,OUTPUT);

	ServoRadar.attach(8);
	servoMotor1.attach(9);
	servoMotor2.attach(10);
	Serial.begin(115200);  // Monitor serial

    led(LOW,LOW,LOW);
	motoresDetener(false);
	abrir();
  delay(500);
	cerrar();
  delay(500);
	abrir();

}

void loop() {

	int distancias[400];
	int grados[400];
	int	cant =0;
	
	//escanea el radar buscando objetos cercanos
	for(int i=15;i<=165;i++){  
		ServoRadar.write(i);
		delay(50);
		distance = distancia_radar();
		//Serial.print(i);Serial.print(",");Serial.print(distance);Serial.println(".");

		//si cumple el parametro solicitado agregara la distancia y los grados		
		if(distance < 30 && distance > 20){
			distancias[cant] =  distance;
			grados[cant] = i;
			cant += 1;
		}

	}

	
	int dir = 0;
	for(int i = 1; i < cant;i++){
		if(distancias[dir] > distancias[i]){
			dir = i;
		}
	}

	if(cant == 0){
		motoresAdelante();
	}else{

	}



	//[DEBUG]
	for(int i = 0;i < cant;i++){
		Serial.print("distancai[ ");Serial.print(i);Serial.print(" ]: ");Serial.println(distancias[i]);
	}	
	for(int i = 0;i < cant;i++){
		Serial.print("grados[ ");Serial.print(i);Serial.print(" ]: ");Serial.println(grados[i]);
	}
	delay(1000);
	delay(1000);



}


int distancia_radar(){
	digitalWrite(pinTrig_radar, LOW); 
	delayMicroseconds(2);
	digitalWrite(pinTrig_radar, HIGH); 
	delayMicroseconds(10);
	digitalWrite(pinTrig_radar, LOW);
	duration = pulseIn(pinEcho_radar, HIGH);
	distance= duration*0.034/2;
	return distance;
}
