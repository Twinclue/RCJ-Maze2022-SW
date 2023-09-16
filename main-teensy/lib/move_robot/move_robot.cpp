#include "move_robot.h"


//interrupts
volatile bool Rvicflag = false;
volatile bool Lvicflag = false;
volatile byte Rkitnum = 0;
volatile byte Lkitnum = 0;

extern _gameMode gameMode;

void intrR(){
    Rvicflag = true;
    digitalWrite(RE_LED_R,HIGH);
    Rkitnum = digitalRead(R_COMM_2) || (digitalRead(R_COMM_3) << 1);
    return;
}

void intrL(){
    Lvicflag = true;
    Lkitnum = digitalRead(L_COMM_2) || (digitalRead(L_COMM_3) << 1);
    digitalWrite(RE_LED_B,HIGH);
    return;
}



move_robot::move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back,read_BNO055 *_imu,read_light *_light,LiquidCrystal *_disp,Adafruit_NeoPixel *_led){
    left = _left;
    right = _right;
    front = _front;
    back = _back;
    imu = _imu;
    light = _light;
    disp = _disp;
    led = _led;
    pinMode(lTouch,INPUT);
    pinMode(rTouch,INPUT);
    mwall = new detect_wall(front,back);
    flipper->attach(servo);
    flipper->write(145);
    delay(500);
}

short move_robot::fwd(short remDist = 300){
    bool frontAnker = false;
    int startDist;
    bool outOfRange = false;
    bool vicFound = false;
    
    if(front->read(fc) < back->read(bc) && front->read(fc) != 8190){
        frontAnker = true;
        startDist = front->read(fc);
    }
    else if(front->read(fc) > back->read(bc) && (back->read(bc) < 800)){
        startDist = back->read(bc);
    }
    else{
        outOfRange = true;
    }


    int errorDist = 0;
    short startAng = imu->getYaw();
    short errorAng = startAng - imu->getYaw();
    short startPitch = imu->getPitch();

    fwdPid->init();
    attachInterrupts();
    uint32_t startTime = millis();
    while((errorDist < remDist) && (front->read(fc) > 50) && outOfRange == false){
        errorAng = startAng - imu->getYaw();
        if(frontAnker){
            errorDist = startDist  - front->read(fc);
        }
        else{
            errorDist = back->read(bc) - startDist;
        }
        left->on(250 + fwdPid->calcP(errorAng,0));
        right->on(-250 + fwdPid->calcP(errorAng,0));

        if(imu->getPitch()>15){  //temporary threshold
            digitalWrite(RE_LED_R, HIGH);
            delay(500);
            left->on(0);
            right->on(0);
            delay(1000);
            digitalWrite(RE_LED_R, LOW);
            return -2;  //making slopeState GOUP
        }
        else if(imu->getPitch()<-15){
            digitalWrite(RE_LED_G, HIGH);
            for(int i = 255; i > 100;i--){
                left->on(i);
                right->on(-i);
                delay(5);
            }
            //delay(1000);
            digitalWrite(RE_LED_G, LOW);
            return -3;  //making slopeState GODOWN
        }
        if(avoidObstacle()){
            remDist = this->fwd(remDist - errorDist);
        }
        if(light->getFloorColor() == 1){
            remDist = 0;
            this->rev(errorDist);
            left->on(0,false);
            right->on(0,false);
            return -1;
        }
        if(!vicFound){vicFound=victim();}
        delay(1);
    }

    Serial.println(millis() - startTime);

    while(millis() - startTime < 1000 && outOfRange){//秒
        errorAng = startAng - imu->getYaw();
        if(frontAnker){
            errorDist = startDist  - front->read(fc);
        }
        else{
            errorDist = back->read(bc) - startDist;
        }
        left->on(250 + fwdPid->calcP(errorAng,0));
        right->on(-250 + fwdPid->calcP(errorAng,0));

        if(imu->getPitch()>15){  //temporary threshold
            digitalWrite(RE_LED_R, HIGH);
            delay(500);
            left->on(0);
            right->on(0);
            delay(1000);
            digitalWrite(RE_LED_R, LOW);
            return -2;  //making slopeState GOUP
        }
        else if(imu->getPitch()<-15){
            digitalWrite(RE_LED_G, HIGH);
            for(int i = 255; i > 100;i--){
                left->on(i);
                right->on(-i);
                delay(5);
            }
            //delay(1000);
            digitalWrite(RE_LED_G, LOW);
            return -3;  //making slopeState GODOWN
        }
        if(avoidObstacle()){
            remDist = this->fwd(remDist - errorDist);
        }
        if(light->getFloorColor() == 1){
            remDist = 0;
            this->rev(errorDist);
            left->on(0,false);
            right->on(0,false);
            return -1;
        }
        if(!vicFound){vicFound=victim();}
        delay(1);
    }
    digitalWrite(RE_LED_B,LOW);
    digitalWrite(RE_LED_G,LOW);
    detachInterrups();

    left->on(0);
    right->on(0);
    this->corrDir();
    return 0;
}

int move_robot::rev(int remDist = 300){
    int startDist = front->read(fc);
    int errorDist = 0;
    short startAng = imu->getYaw();
    short errorAng = startAng - imu->getYaw();
    fwdPid->init();
    while(errorDist > -remDist){
        errorAng = startAng - imu->getYaw();
        errorDist = startDist  - front->read(fc);
        left->on(-255 + fwdPid->calcP(errorAng,0));
        right->on(255 - fwdPid->calcP(errorAng,0));
        delay(1);
    }
    left->on(0);
    right->on(0);
    return 0;
}

short move_robot::turn(short remAng = 90){
    uint16_t loopcount = 0;
    short startAng = imu->getYaw();
    short errorAng = 0;
    bool turnRight = false;
    int power = 0;
    turnPid->init();
    attachInterrupts();
    if(remAng > 0){
        turnRight = true;
    }
    while(true){
        loopcount++;
        if(((errorAng>remAng) && turnRight) || ((errorAng<remAng) && !turnRight)){
            break;
        }
        errorAng = imu->getYaw() - startAng;
        if(turnRight){
            power = -turnPid->calcPI(-errorAng,-remAng);
        }
        else{
            power = turnPid->calcPI(errorAng,remAng);
        }
        if(remAng > 0){//CWのとき
            if(digitalRead(SW_L) == true){//バンパー引っかかり検知
                left->on(-250,false);
                right->on(0,false);
                delay(300);
                left->on(-250,false);
                right->on(250,false);
                delay(100);

            }
            else if(digitalRead(SW_R) == true){//壁衝突
                left->on(-250,false);
                right->on(250,false);
                delay(100);
            }
        }
        else{//CCWのとき
            if(digitalRead(SW_L) == true){
                left->on(-250,false);
                right->on(250,false);
                delay(100);
            }
            else if(digitalRead(SW_R) == true){
                left->on(0,false);
                right->on(250,false);
                delay(300);
                left->on(-250,false);
                right->on(250,false);
                delay(100);
            }
        }
        left->on(-power);
        right->on(-power);
        //victim();
        delay(1);
    }
    detachInterrups();
    left->on(0);
    right->on(0);
    this->corrDir();
    return startAng;
}

short move_robot::goUp(){
    short startAng = imu->getYaw();
    short errorAng = startAng - imu->getYaw();
    fwdPid->init();
    while(!(imu->getPitch() < 5 && imu->getPitch() > -5)){
        errorAng = startAng - imu->getYaw();
        left->on(180 + fwdPid->calcP(errorAng,0));
        right->on(-180 + fwdPid->calcP(errorAng,0));
    }
    left->on(180);
    right->on(-180);
    delay(500);
    left->on(0);
    right->on(0);
    corrDir();
    digitalWrite(RE_LED_B,HIGH);
    delay(300);
    digitalWrite(RE_LED_B,LOW);
    return 0;
}

short move_robot::goDown(){
    short startAng = imu->getYaw();
    short errorAng = startAng - imu->getYaw();
    fwdPid->init();
    while(!(imu->getPitch() < 5 && imu->getPitch() > -5)){
        errorAng = startAng - imu->getYaw();
        left->on(100 + fwdPid->calcP(errorAng,0));
        right->on(-100 + fwdPid->calcP(errorAng,0));
    }
    left->on(180);
    right->on(-180);
    delay(300);
    left->on(0);
    right->on(0);
    corrDir();
    digitalWrite(RE_LED_B,HIGH);
    delay(500);
    digitalWrite(RE_LED_B,LOW);
    return 0;
}

void move_robot::corrDir(){
    float relativeAng = 0;
    int lenghCompare[4];
    lenghCompare[0] = (front->read(frs) + back->read(brs));
    lenghCompare[2] = (front->read(fls) + back->read(bls));
    lenghCompare[1] = (front->read(frf) + front->read(flf));
    lenghCompare[3] = (back->read(brf) + back->read(blf));

    uint8_t minlengh = 0;

    for(int i = 0; i < 4; i++){
        if(lenghCompare[i] < lenghCompare[minlengh]){
            minlengh = i;
        }
    }

    if(minlengh == 0){
        relativeAng = readRAngle(front->read(frs),back->read(brs));
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(front->read(frs),back->read(brs));
            left->on(corrDirPid->calcPI(relativeAng,0));//
            right->on(corrDirPid->calcPI(relativeAng,0));//
        }
    }
    else if(minlengh == 2){
        relativeAng = readRAngle(front->read(fls),back->read(bls));
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(front->read(fls),back->read(bls));
            left->on(-corrDirPid->calcPI(relativeAng,0));//
            right->on(-corrDirPid->calcPI(relativeAng,0));//
        }
    }
    else if(minlengh == 1){
        relativeAng = readRAngle(front->read(flf),front->read(frf),125);
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(front->read(flf),front->read(frf),125);
            left->on(corrDirPid->calcPI(relativeAng,0));//
            right->on(corrDirPid->calcPI(relativeAng,0));//
        }
    }
    else if(minlengh == 3){
        relativeAng = readRAngle(back->read(blf),back->read(brf),125);
        corrDirPid->init();
        while(relativeAng <= -1 || 1 <= relativeAng){
            relativeAng = readRAngle(back->read(blf),back->read(brf),125);
            left->on(-corrDirPid->calcPI(relativeAng,0));//
            right->on(-corrDirPid->calcPI(relativeAng,0));//
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
    if(digitalRead(SW_R)){
        right->on(255,false);
        left->on(0,false);
        delay(250);
        right->on(0,false);
        left->on(-255,false);
        delay(250);
        right->on(-255,false);
        left->on(0,false);
        delay(250);
        right->on(0,false);
        left->on(255,false);
        delay(250);
        left->on(0,false);
        right->on(0,false);
        return true;
    }
    else if(digitalRead(SW_L)){
        left->on(-255,false);
        right->on(0,false);
        delay(250);
        left->on(0,false);
        right->on(255,false);
        delay(250);
        left->on(255,false);
        right->on(0,false);
        delay(250);
        left->on(0,false);
        right->on(-255,false);
        delay(250);
        left->on(0,false);
        right->on(0,false);
        return true;
    }
    return false;
}

void move_robot::drop(bool dir){
    if(rescueKitNum > 0){
        if(dir){
            flipper->write(145);
            delay(500);
            flipper->write(180);
            delay(500);
            flipper->write(130);
            delay(200);
            flipper->write(150);
            delay(200);
            flipper->write(145);
            delay(500);
        }
        else{
            flipper->write(145);
            delay(500);
            flipper->write(105);
            delay(500);
            flipper->write(150);
            delay(200);
            flipper->write(130);
            delay(200);
            flipper->write(145);
            delay(500);
        }
        rescueKitNum--;
    }
}

void move_robot::blink(bool color=0){
    if(!color){
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
    }else{
        for(int i = 0;i<12;i++){
            tone(23,422,50);
            led->setPixelColor(0,led->Color(0,255,0));
            led->show();
            delay(250);
            tone(23,844,50);
            led->setPixelColor(0,led->Color(0,0,0));
            led->show();
            delay(250);
        }
    }
    
}

void move_robot::attachInterrupts(){//ピン番号は仮
    attachInterrupt(digitalPinToInterrupt(R_COMM_ITR),intrR,RISING);
    attachInterrupt(digitalPinToInterrupt(L_COMM_ITR),intrL,RISING);
}

void move_robot::detachInterrups(){//ピン番号は仮
    detachInterrupt(digitalPinToInterrupt(R_COMM_ITR));
    detachInterrupt(digitalPinToInterrupt(L_COMM_ITR));
}

bool move_robot::victim(){
    switch (gameMode){  // グローバル変数gameMode
        case _gameMode::WORLD: //ワールドリーグ仕様
            if(Rvicflag){
                if(mwall->getSingleWall(0)){
                    left->on(0);
                    right->on(0);
                    blink();
                    for(int count = 0;count<Rkitnum;count++){
                        drop(false);
                    }
                }
                Rvicflag = false;
                digitalWrite(RE_LED_R,LOW);
                return true;

            }else if(Lvicflag){
                if(mwall->getSingleWall(2)==true){
                    left->on(0);
                    right->on(0);
                    blink();
                    for(int count = 0;count<Lkitnum;count++){
                        drop(true);
                    }
                }
                Lvicflag = false;
                digitalWrite(RE_LED_B,LOW);
                return true;
                
            }else{
                return false;
            }
        
        case _gameMode::ENTRY: //エントリーリーグ仕様，床面の色で判定
            if(light->getFloorColor()==2){  //ここも単なる変数じゃなくて読んで意味が分かるようにしたい
                //ちょっと進んで停止？
                delay(80);
                left->on(0,false);
                right->on(0,false);
                //計測
                light->setColor(150, 0, 0);
                delay(100);
                red=light->read();
                light->setColor(0,150,0);
                delay(100);
                green=light->read();
                light->setColor(80, 80, 80);
                //出力
                if(red>r_th&&green<g_th){   //赤
                    blink(0);
                    return true;
                }else if(red<r_th&&green>g_th){//緑
                    blink(1);
                    return true;
                }else{  //判定不可
                    left->on(100,false);
                    right->on(100,false);
                    delay(100);
                    left->on(0,false);
                    right->on(0,false);
                }
                return false;
            }
        
        default:
        return false; //ここ適当なので，修正すること
    }
}