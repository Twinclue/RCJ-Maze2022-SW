#include "drive_motor.h"

drive_motor::drive_motor(uint8_t _in1, uint8_t _in2, uint8_t _pwm, uint8_t _lo1, uint8_t _lo2){
    in1 = _in1;
    in2 = _in2;
    pwm = _pwm;
    lo1 = _lo1;
    lo1 = _lo1;
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(pwm,OUTPUT);
    pinMode(lo1,INPUT);
    pinMode(lo2,INPUT);
}

int drive_motor::on(int power){
    if(power>0){
            digitalWrite(in1,HIGH);
            digitalWrite(in2,LOW);
            analogWrite(pwm,power);
        }
        else{
            digitalWrite(in1,LOW);
            digitalWrite(in2,HIGH);
            analogWrite(pwm,-power);
        }
}