#include "drive_motor.h"

drive_motor::drive_motor(uint8_t _dir, uint8_t _pwm, uint8_t _flt){
    dir=_dir;
    pwm=_pwm;
    flt=_flt;
    pinMode(dir,OUTPUT);
    pinMode(pwm,OUTPUT);
    pinMode(flt,INPUT);
    analogWriteFrequency(pwm, PWMfreq);
}

int drive_motor::on(int power, bool acc = true){
    if(power > 255){
        power = 255;
    }
    else if(power < -255){
        power = -255;
    }
    if(power>0){
        if((power - prePower > 50) && acc){
            power = prePower + 20;
        }
        digitalWrite(dir,HIGH);
        analogWrite(pwm,power);
    }
    else{
        if((power - prePower < -50) && acc){
            power = prePower - 20;
        }
        digitalWrite(dir,LOW);
        analogWrite(pwm,-power);
    }
    prePower = power;
    return digitalRead(flt);    //0: error
}