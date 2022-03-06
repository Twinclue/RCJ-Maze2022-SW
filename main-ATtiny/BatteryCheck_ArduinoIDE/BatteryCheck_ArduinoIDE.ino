#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>

#define trigPin 3
#define VIOUTpin A2
#define main_loop_delay 10
#define calibSampleNum 20
#define BAT_resistance 0.0
#define idleCurrent_mA 31

SoftwareSerial mySerial(2,0); // RX, TX
Adafruit_INA219 ina219(0X41);

float voltage=0.0;
const float minCV=10.6;

int VIOUT_offset=0;
const float VIOUT_coef=0.0244;
float current=0.0;

const float LPFcoef=0.1;
	float prevLPF;


bool Trig=false;
bool prevTrig=false;
bool cutoff=false;

int currentOffset(){
	int sample=0;	//should be long enough for adding up all samples
	for(uint8_t i=0; i<calibSampleNum; i++){
		sample+=analogRead(VIOUTpin);
		mySerial.println(sample);
		delay(main_loop_delay);
	}
	return -sample/calibSampleNum;
}

float getCurrent(){
	float data=(analogRead(VIOUTpin)+VIOUT_offset)*VIOUT_coef;
	return data;
}

float LPF(float input){
	float LPF=LPFcoef*input+(1.0-LPFcoef)*prevLPF;
	prevLPF=LPF;
	return LPF;
}

void setup(){

	pinMode(trigPin,OUTPUT);
	digitalWrite(trigPin, LOW);
	mySerial.begin(4800);

	VIOUT_offset=currentOffset();
	// while(!mySerial.available()){
	// 	delay(10);
	// }

	// while(!ina219.begin()){
	// 	// mySerial.println("INA_fail");
	// }
	// ina219.setCalibration_16V_400mA();

	digitalWrite(trigPin, HIGH);	//delete when tests are finished
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

	current=LPF(getCurrent());
	//VIOUT_offset=currentOffset();
	//mySerial.print(analogRead(VIOUTpin));	mySerial.print("\t");
	//mySerial.print(VIOUT_offset);	mySerial.print("\t");
	mySerial.println(current+0.001*idleCurrent_mA);
	mySerial.print("\n");
	delay(main_loop_delay);
}
