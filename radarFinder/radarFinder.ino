//Librerias
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

int repeticiones=0;

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


// led para saber el estado de movimiento
void led(int uno,int dos,int tres){
  digitalWrite(ledRojo,uno);  //derecha
  digitalWrite(ledVerde,dos); //adelante
  digitalWrite(ledAzul,tres); //izquierda
}


void motoresDetener(bool dev = true) {
	digitalWrite(M1, HIGH);
	digitalWrite(M2, HIGH);
	analogWrite(E1, 0);
	analogWrite(E2, 0);
	delay(espera);
	if(dev){
		lista[movimientos] = 2;  //guarda movimiento realizado
		movimientos+=1;
	}
}


void motoresAdelante(bool dev = true) { //si la Variable es verdadera guardara el realizar el movimiento

	for(int i = 0; i < 35; i++){    //esta modificacion esta hecha para ajustar cuanto se mueve el robot    
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
		Serial.println("ADELANTE");
    }  
	motoresDetener();	
}


//con los otros movimientos queda revisar si en los otros casos deberia aplicarse la misma logica 


void motoresAtras(bool dev = true) {
	for(int i = 0; i < 35; i++){
		digitalWrite(M1, LOW);
		digitalWrite(M2, LOW);
		analogWrite(E1, velocidadAtras);
		analogWrite(E2, velocidadAtras);
		delay(espera);
	}
	delay(10);

	if(dev){
		lista[movimientos] = 3;
		movimientos+=1;
		Serial.println("ATRAS");
	}
}
void motoresDerecha(bool dev = true) {
	for(int i = 0; i < 30; i++){
		digitalWrite(M1, HIGH);
		digitalWrite(M2, LOW);
		analogWrite(E1, velocidadDoblar);
		analogWrite(E2, velocidadDoblar);
		delay(espera);
	}
	delay(10);
	if(dev){
		lista[movimientos] = 4;
		movimientos+=1;
		Serial.println("DERECHA");
	}
}
void motoresIzquierda(bool dev = true) {
	for(int i = 0; i < 30; i++){
		digitalWrite(M1, LOW);
		digitalWrite(M2, HIGH);
		analogWrite(E1, velocidadDoblar);
		analogWrite(E2, velocidadDoblar);
		delay(espera);
	}
	delay(10);
	if(dev){
		lista[movimientos] = 5;
		movimientos+=1;
		Serial.println("IZQUIERDA");
	}
}


void devolucion(){
	for(movimientos = movimientos - 1; movimientos >= 0;movimientos--){
		switch(lista[movimientos]){ //switch que revisa que movimiento realizara ahora 
		case 1:
			motoresAtras(false);
			Serial.println("1: atras");
		break;
		case 2:
			motoresDetener(false);
			Serial.println("2: detener");
		break;
		case 3:
			motoresAdelante(false);
			Serial.println("3: adelante");
		break;
		case 4:
		motoresIzquierda(false);
		Serial.println("4: Izquierda");
		break;
		case 5:
		motoresDerecha(false);
		Serial.println("5: Izquierda");
		break;
		}
		delay(30);
		motoresDetener(false);
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
	servoMotor1.write(180);
	servoMotor2.write(-10);
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

void loop() {

	int distancias[400];
	int grados[400];
	int	cant =0;
	 motoresDetener(false);

	


	if(!encontrado){ //modo de encontrar 
		//verificando que el sistema funcione bien, con una version simplificada de lo que hace, todo lo que tiene es el comportamiento
		if(distancia_cercania() < 6){ // alcance para agarrar
			motoresDetener(false);
			cerrar();
      
			encontrado = !encontrado;
			
		}else if(distancia_cercania() < 35){

			motoresAdelante();
			delay(500);
			motoresDetener(false);
			// servoMotor1.write(120);
			// delay(1000);

			unsigned int red = RGB_sensor.readRed();
			unsigned int green = RGB_sensor.readGreen();
			unsigned int blue = RGB_sensor.readBlue();
			int negro = 450;
			int suma = red + green + blue;
      Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
			Serial.print("color:");Serial.println(suma);
			
		/*
			if(suma < 800){ //AJUSTE IN SITU 
				motoresIzquierda();motoresIzquierda();motoresIzquierda();motoresIzquierda();
				servoMotor1.write(170);
			}else{
				//abrir();
				
			}
		*/

		}else{
			

	//escanea el radar buscando objetos cercanos
  if(repeticiones<10){
	for(int i=70;i<=115;i++){  
		ServoRadar.write(i);
		delay(30);
		distance = distancia_radar();
	

		//si cumple el parametro solicitado agregara la distancia y los grados		
		if(distance < 30 && distance > 10){
			Serial.print(i);Serial.print(",");Serial.print(distance);Serial.println(".");
			distancias[cant] =  distance;
			grados[cant] = i;
			cant += 1;
		}
    }
	repeticiones=repeticiones+1;
  }

	if(cant == 0){
		motoresAdelante();
    led(HIGH,LOW,HIGH);
	}else{
    led(LOW,HIGH,LOW);
		int dir = 0;
		for(int i = 1; i < cant;i++){
			if(distancias[dir] > distancias[i]){
				dir = i;
			}
		}
    
		if(grados[dir] >= 125 ){
      led(HIGH,HIGH,LOW);
			motoresIzquierda();
			motoresIzquierda();
		}else if(grados[dir] >= 83){
      led(HIGH,HIGH,LOW);
			motoresIzquierda();
		}else if(grados[dir] >= 42){
      led(LOW,HIGH,HIGH);
			motoresDerecha();
		}else{
      led(LOW,HIGH,HIGH);
			motoresDerecha();
			motoresDerecha();
		}
	}

		}
	}else{
		devolucion();
    	motoresDetener(false);
		delay(500);
		if(digitalRead(IR) == HIGH){
			abrir();
			delay(500);
		}

		
	}
	 
	delay(15);


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
