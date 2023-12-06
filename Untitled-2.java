distancai[ 0 ]: 22
distancai[ 1 ]: 21
distancai[ 2 ]: 21
distancai[ 3 ]: 21
distancai[ 4 ]: 21
distancai[ 5 ]: 21
distancai[ 6 ]: 21
distancai[ 7 ]: 21
distancai[ 8 ]: 21
distancai[ 9 ]: 22
distancai[ 10 ]: 21
distancai[ 11 ]: 22
distancai[ 12 ]: 21
distancai[ 13 ]: 21
distancai[ 14 ]: 21
distancai[ 15 ]: 21
distancai[ 16 ]: 22
distancai[ 17 ]: 22
distancai[ 18 ]: 22
distancai[ 19 ]: 22
distancai[ 20 ]: 23
distancai[ 21 ]: 22
distancai[ 22 ]: 22




	
	//escanea el radar buscando objetos cercanos
	for(int i=15;i<=165;i++){  
		ServoRadar.write(i);
		delay(30);
		distance = distancia_radar();
	

		//si cumple el parametro solicitado agregara la distancia y los grados		
		if(distance < 30 && distance > 14){
			Serial.print(i);Serial.print(",");Serial.print(distance);Serial.println(".");
			distancias[cant] =  distance;
			grados[cant] = i;
			cant += 1;
		}

	}


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