#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
#define trigPin 3
#define UILED 4

// SoftwareSerial mySerial(1,4); // RX, TX

Adafruit_INA219 ina219(0X41);
float voltage=0.0;
const float minCV=10.6;
bool Trig=false;
bool prevTrig=false;
bool cutoff=false;

void alert(){
		digitalWrite(UILED, HIGH);
		delay(10);
		digitalWrite(UILED, LOW);
		delay(50);
}

void setup(){
	pinMode(trigPin,OUTPUT);
	digitalWrite(trigPin, LOW);
	pinMode(UILED,OUTPUT);
	digitalWrite(UILED, LOW);

	// mySerial.begin(4800);
	// while(!mySerial.available()){
	// 	delay(10);
	// }

	while(!ina219.begin()){
		// mySerial.println("INA_fail");
		alert();
	}

	ina219.setCalibration_16V_400mA();
}

void loop(){
	if(!ina219.success()){
		alert();
	}
	voltage=ina219.getBusVoltage_V();

	// mySerial.println(voltage);
	// mySerial.println(ina219.success());

	if(prevTrig==true){
		Trig=voltage>minCV;
	}else{
		if(voltage>minCV){
			Trig=true;
		}else{
			Trig=false;
			cutoff=true;
			}
	}
	digitalWrite(trigPin, !cutoff);
	digitalWrite(UILED, cutoff);
	while(cutoff);
	prevTrig=Trig;
	delay(1000);
}
