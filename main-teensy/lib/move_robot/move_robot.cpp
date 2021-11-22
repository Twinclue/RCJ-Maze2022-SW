#include "move_robot.h"

move_robot::move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back,read_imu *_imu){
    left = _left;
    right = _right;
    front = _front;
    back = _back;
    imu = _imu;
}

short move_robot::fwd(short remDist = 300){
    int startDist = front->read(fc);
    while(startDist - front->read(fc) < remDist){
        left->on(255);
        right->on(255);
    }
    left->on(0);
    right->on(0);
    return 1;
}

short move_robot::turn(short remAng = 90){
    imu->read();
    short startAng = imu->getYaw();
    short errorAng = imu->getYaw() - startAng;
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
            Serial.println(imu->getYaw());
            delay(1);
        }
    }
    left->on(0);
    right->on(0);
    return startAng;
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
    while(((front->read(fls)+back->read(bls))/2) <= 50){
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
    delay(100);
    return;
}