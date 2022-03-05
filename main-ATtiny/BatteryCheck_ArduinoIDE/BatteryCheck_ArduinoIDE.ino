#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
#define trigPin 3
#define VIOUTpin 4
#define main_loop_delay 10

SoftwareSerial mySerial(2,0); // RX, TX
Adafruit_INA219 ina219(0X41);

float voltage=0.0;
const float minCV=10.6;

const int VIOUT_offset=0;
const float VIOUT_coef=1.0;
float current=0.0;

bool Trig=false;
bool prevTrig=false;
bool cutoff=false;

float getCurrent(){
	current=(analogRead(VIOUTpin)+VIOUT_offset)*VIOUT_coef;
	return current;
}

void setup(){
	pinMode(trigPin,OUTPUT);
	digitalWrite(trigPin, HIGH);
	mySerial.begin(4800);
	// while(!mySerial.available()){
	// 	delay(10);
	// }
	// while(!ina219.begin()){
	// 	// mySerial.println("INA_fail");
	// }
	// ina219.setCalibration_16V_400mA();
}

void loop(){
	// while(!ina219.success()){
	// 	digitalWrite(trigPin, false);
	// }
	// voltage=ina219.getBusVoltage_V();

	// // mySerial.println(voltage);
	// // mySerial.println(ina219.success());

	// if(prevTrig==true){		//last time voltage was high enough
	// 	Trig=voltage>minCV;
	// }else{
	// 	if(voltage>minCV){	//voltage recovered
	// 		Trig=true;
	// 	}else{				//voltage is lower than Vth for 1 second
	// 		Trig=false;
	// 		cutoff=true;
	// 		}
	// }
	// digitalWrite(trigPin, !cutoff);
	// while(cutoff);
	// prevTrig=Trig;

	float current=getCurrent();
	mySerial.println(current);
	delay(main_loop_delay);
}
