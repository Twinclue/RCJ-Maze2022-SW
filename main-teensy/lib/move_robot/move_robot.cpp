#include "move_robot.h"

move_robot::move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back,read_imu *_imu,read_light *_light,LiquidCrystal *_disp,Adafruit_NeoPixel *_led,node *_n){
    left = _left;
    right = _right;
    front = _front;
    back = _back;
    imu = _imu;
    light = _light;
    disp = _disp;
    led = _led;
    n = _n;
    pinMode(lTouch,INPUT);
    pinMode(rTouch,INPUT);
    mwall = new detect_wall(front,back);
    flipper->attach(servo);
    flipper->write(140);
    delay(500);
}

short move_robot::fwd(short remDist = 300){
    bool vicFlag=false;
    imu->read();
    if(front->read(fc) < back->read(bc)){
        int startDist = front->read(fc);
        int errorDist = 0;
        imu->read();
        short startAng = imu->getYaw();
        short errorAng = startAng - imu->getYaw();
        fwdPid->init();
        while((errorDist < remDist) && (front->read(fc) > 50)){
            unsigned long start = millis();
            imu->read();
            errorAng = startAng - imu->getYaw();
            // disp->home();
            // disp->clear();
            // Serial.print("Pitch : ");
            // Serial.println(abs(prePitch - imu->getGPitch()));
            errorDist = startDist  - front->read(fc);
            left->on(255 + fwdPid->calcP(errorAng,0));
            right->on(255 - fwdPid->calcP(errorAng,0));

            if(avoidObstacle()){
                remDist = this->fwd(remDist - errorDist);
            }

            int8_t rVic = vic->kitNumOneSide(true);
            int8_t lVic = vic->kitNumOneSide(false);
            if((rVic != -1) &&  mwall->getSingleWall(0)==true && vicFlag == false && n->getNowCount()==0){
                left->on(0);
                right->on(0);
                blink();
                for(int count = 0;count<rVic;count++){
                    drop(false);
                }
                vicFlag = true;
            }
            if((lVic != -1) &&  mwall->getSingleWall(2)==true && vicFlag == false && n->getNowCount()==0){
                left->on(0);
                right->on(0);
                blink();
                for(int count = 0;count<lVic;count++){
                    drop(true);
                }
                vicFlag = true;
            }

            if(light->getFloorColor() == 1){
                remDist = 0;
                this->rev(errorDist);
                left->on(0);
                right->on(0);
                return -1;
            }
            //Serial.println(255 - fwdPid->calcP(errorAng,startAng));
            delay(1);
            Serial.println(millis()-start);
        }
    }
    else{
        int startDist = back->read(bc);
        int errorDist = 0;
        imu->read();
        short startAng = imu->getYaw();
        short errorAng = startAng - imu->getYaw();
        fwdPid->init();
        while((errorDist < remDist) && (front->read(fc) > 50)){
            unsigned long start = millis();
            imu->read();
            errorAng = startAng - imu->getYaw();
            // disp->home();
            // disp->clear();
            // Serial.print("Pitch : ");
            // Serial.println(abs(prePitch - imu->getGPitch()));
            errorDist = back->read(bc) - startDist;
            left->on(255 + fwdPid->calcP(errorAng,0));
            right->on(255 - fwdPid->calcP(errorAng,0));

            if(avoidObstacle()){
                remDist = this->fwd(remDist - errorDist);
            }

            int8_t rVic = vic->kitNumOneSide(true);
            int8_t lVic = vic->kitNumOneSide(false);
            if((rVic != -1) &&  mwall->getSingleWall(0)==true && vicFlag == false && n->getNowCount()==0){
                left->on(0);
                right->on(0);
                blink();
                for(int count = 0;count<rVic;count++){
                    drop(false);
                }
                vicFlag = true;
            }
            if((lVic != -1) &&  mwall->getSingleWall(2)==true && vicFlag == false && n->getNowCount()==0){
                left->on(0);
                right->on(0);
                blink();
                for(int count = 0;count<lVic;count++){
                    drop(true);
                }
                vicFlag = true;
            }

            if(light->getFloorColor() == 1){
                remDist = 0;
                this->rev(errorDist);
                left->on(0);
                right->on(0);
                return -1;
            }
            //Serial.println(255 - fwdPid->calcP(errorAng,startAng));
            delay(1);
            Serial.println(millis()-start);
        }
    }

    left->on(0);
    right->on(0);
    this->corrDir();
    return 0;
}

short move_robot::rev(short remDist = 300){
    int startDist = front->read(fc);
    int errorDist = 0;
    imu->read();
    short startAng = imu->getYaw();
    short errorAng = startAng - imu->getYaw();
    fwdPid->init();
    while(errorDist > -remDist){
        imu->read();
        errorAng = startAng - imu->getYaw();
        errorDist = startDist  - front->read(fc);
        left->on(-255 + fwdPid->calcP(errorAng,0));
        right->on(-255 - fwdPid->calcP(errorAng,0));
        delay(1);
    }
    left->on(0);
    right->on(0);
    return 0;
}

short move_robot::turn(short remAng = 90){
    imu->read();
    short startAng = imu->getYaw();
    short errorAng = 0;
    turnPid->init();
    if(remAng > 0){
        while(errorAng < remAng){
            imu->read();
            errorAng = imu->getYaw() - startAng;
            left->on(-turnPid->calcPI(errorAng,remAng));
            right->on(turnPid->calcPI(errorAng,remAng));
            delay(1);
        }
    }
    else{
        while(errorAng > remAng){
            imu->read();
            errorAng = imu->getYaw() - startAng;
            left->on(turnPid->calcPI(-errorAng,-remAng));
            right->on(-turnPid->calcPI(-errorAng,-remAng));
            //Serial.println(imu->getYaw());
            delay(1);
        }
    }
    left->on(0);
    right->on(0);
    this->corrDir();
    return startAng;
}

short move_robot::goUp(){
    imu->read();
    short prePitch = imu->getGPitch();
    short startAng = imu->getYaw();
    short errorAng = startAng - imu->getYaw();
    fwdPid->init();
    while(abs(prePitch-imu->getGPitch()) <= 15 && front->read(frf) >= 250 && front->read(flf) >= 250){
        imu->read();
        Serial.println(prePitch - imu->getGPitch());
        left->on(255 + fwdPid->calcP(errorAng,0));
        right->on(255 - fwdPid->calcP(errorAng,0));
    }
    left->on(0);
    right->on(0);
    return 0;
}

void move_robot::corrDir(){
    float relativeAng = 0;
    if(front->read(frs) <= 200 && back->read(brs) <= 200){
        relativeAng = readRAngle(front->read(frs),back->read(brs));
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(front->read(frs),back->read(brs));
            left->on(corrDirPid->calcPI(relativeAng,0));//
            right->on(-1 * corrDirPid->calcPI(relativeAng,0));//
        }
    }
    else if(front->read(fls) <= 200 && back->read(bls) <= 200){
        relativeAng = readRAngle(front->read(fls),back->read(bls));
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(front->read(fls),back->read(bls));
            left->on(-1 * corrDirPid->calcPI(relativeAng,0));//
            right->on(corrDirPid->calcPI(relativeAng,0));//
        }
    }
    else if(front->read(flf) <= 200 && front->read(frf) <= 200){
        relativeAng = readRAngle(front->read(flf),front->read(frf),125);
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(front->read(flf),front->read(frf),125);
            left->on(corrDirPid->calcPI(relativeAng,0));//
            right->on(-1 * corrDirPid->calcPI(relativeAng,0));//
        }
    }
    else if(back->read(blf) <= 200 && back->read(brf) <= 200){
        relativeAng = readRAngle(back->read(blf),back->read(brf),125);
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(back->read(blf),back->read(brf),125);
            left->on(-1 * corrDirPid->calcPI(relativeAng,0));//
            right->on(corrDirPid->calcPI(relativeAng,0));//
        }
    }
    left->on(0);
    right->on(0);
}

void move_robot::corrDist(){
    while(((front->read(fls)+back->read(bls))/2) <= 70){
        left->on(255);
        right->on(0);
        delay(70);
        left->on(0);
        right->on(255);
        delay(70);
        left->on(-255);
        right->on(0);
        delay(70);
        left->on(0);
        right->on(-255);
        delay(70);
    }
    left->on(0);
    right->on(0);
    this->corrDir();
    return;
}

bool move_robot::avoidObstacle(){
    if(digitalRead(rTouch)){
        right->on(-255);
        left->on(0);
        delay(500);
        right->on(0);
        left->on(-255);
        delay(500);
        right->on(255);
        left->on(0);
        delay(500);
        right->on(0);
        left->on(255);
        delay(500);
        left->on(0);
        right->on(0);
        return true;
    }
    else if(digitalRead(lTouch)){
        left->on(-255);
        right->on(0);
        delay(500);
        left->on(0);
        right->on(-255);
        delay(500);
        left->on(255);
        right->on(0);
        delay(500);
        left->on(0);
        right->on(255);
        delay(500);
        left->on(0);
        right->on(0);
        return true;
    }
    return false;
}

void move_robot::drop(bool dir){
    if(rescueKitNum > 0){
        if(dir){
            flipper->write(140);
            delay(500);
            flipper->write(172);
            delay(500);
            flipper->write(130);
            delay(200);
            flipper->write(150);
            delay(200);
            flipper->write(140);
            delay(500);
        }
        else{
            flipper->write(140);
            delay(500);
            flipper->write(105);
            delay(500);
            flipper->write(150);
            delay(200);
            flipper->write(130);
            delay(200);
            flipper->write(140);
            delay(500);
        }
        rescueKitNum--;
    }
}

void move_robot::blink(){
    for(int i = 0;i<12;i++){
        tone(23,422,50);
        led->setPixelColor(0,led->Color(255,0,0));
        led->show();
        delay(250);
        tone(23,844,50);
        led->setPixelColor(0,led->Color(0,0,0));
        led->show();
        delay(250);
    }
}