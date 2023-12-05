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
}


//con los otros movimientos queda revisar si en los otros casos deberia aplicarse la misma logica 
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

//cerrar y abrir aplican especificamente a los brazos para agarrar
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

//es la modificacion anterior del giro ajustada para el buscador de linea 
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


//esta funcion empieza a correr todo el arreglo lista, lo que hace que replique sus movimientos en reversa
//por el cambio en como se configuro el buscador, esto probablemente deba cambiarse 
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

//distancia de los ecolocalizadores
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

void led(int uno,int dos,int tres){
  digitalWrite(ledRojo,uno);
  digitalWrite(ledVerde,dos);
  digitalWrite(ledAzul,tres);
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

	servoMotor1.attach(9);
	servoMotor2.attach(10);
	Serial.begin(115200);  // Monitor serial
	

}

void loop() {
	unsigned int red = RGB_sensor.readRed();
	unsigned int green = RGB_sensor.readGreen();
	unsigned int blue = RGB_sensor.readBlue();
	int negro = 450;
	int suma = red + green + blue;

	/*  esta seccion lo que hace es reconocer el borde en base al color queda verificar 
		if(distancia_cercania() < 17){
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


	servoMotor2.write(20); //temporal


	if(!encontrado){ //modo de encontrar 
		//verificando que el sistema funcione bien, con una version simplificada de lo que hace, todo lo que tiene es el comportamiento
		if(distancia_cercania() > 4){ // alcance para agarrar
			//siguiente if es de color
			
		}else{
			cerrar();
			led(HIGH,HIGH,HIGH);
			motoresDetener();
			encontrado = !encontrado;
		}
	}else{
		devolucion();
    motoresDetener();
	}
	delay(15);
  }
