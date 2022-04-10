#include "solver.h"

solver::solver(read_BNO055 *_imu,read_light *_light,move_robot *_move,detect_wall *_wall,node *_node,LiquidCrystal *_lcd){
    imu = _imu;
    light = _light;
    move = _move;
    wall = _wall;
    n = _node;
    disp = _lcd;
}

int solver::rightHand(){
    if(!wall->getSingleWall(right)){
        move->turn();
        move->fwd();
    }
    else if(!wall->getSingleWall(front)){
        move->fwd();
    }
    else if(!wall->getSingleWall(left)){
        move->turn(-90);
        move->fwd();
    }
    else{
        move->turn(180);
        move->fwd();
    }
    return 0;
}

int solver::EXrightHand(){
    short moveResult = 0;
    String debug_buf;
    for(int i =0;i < 4;i++){
        walls[i] = wall->getSingleWall(i);
    }
    n->searchAroundNodes(walls[front],walls[left],walls[back],walls[right]);
    moveto = n->getMinCountDir();


    block debug = n->getNowNode();
    Serial.println("===================================");
    debug_buf = "Now Node Num      : " + String(n->getNowNodeNum()) + " / X : " + String(debug.p.x) + " / Y : " + String(debug.p.y) + " / count : " + String(debug.count);
    Serial.println(debug_buf);
    Serial.println("-----------------------------------");
    debug_buf = "Wall              : R : " + String(walls[right]) + " / F : " + String(walls[front]) + " / L : " + String(walls[left]) + " / B : " + String(walls[back]);
    Serial.println(debug_buf);
    debug_buf = "Related Node Num   : R : " + String(n->getTempNodeNum(right)) + " / F : " + String(n->getTempNodeNum(front)) + " / L : " + String(n->getTempNodeNum(left)) + " / B : " + String(n->getTempNodeNum(back));
    Serial.println(debug_buf);
    debug_buf = "Related Node Count : R : " + String(n->getRelatedNodeCount(right)) + " / F : " + String(n->getRelatedNodeCount(front)) + " / L : " + String(n->getRelatedNodeCount(left)) + " / B : " + String(n->getRelatedNodeCount(back));
    Serial.println(debug_buf);
    Serial.println("-----------------------------------");
    debug_buf = "Slope State       : " + String(slopeState);
    Serial.println(debug_buf);
    debug_buf = "Move to           : " + String(moveto);
    Serial.println(debug_buf);

    //coordinate disp
    disp->clear();
    disp->print("X : ");
    disp->print(debug.p.x);
    disp->setCursor(0,1);
    disp->print("Y : ");
    disp->print(debug.p.y);

    switch (moveto){
        case front:
            if(slopeState == GOUP){
                moveResult = move->goUp();
            }
            else if(slopeState == GODOWN){
                moveResult = move->goDown();
            }
            else{
                moveResult = move->fwd();
            }
            break;
        case left:
            move->turn(-90);
            moveResult =  move->fwd();
            break;
        case back:
            move->turn(180);
            moveResult =  move->fwd();
            break;
        case right:
            move->turn();
            moveResult =  move->fwd();
            break;
        default:
            break;
    }
    if(moveResult == 0){
        n->updatePosition(moveto);
        n->setTile(light->getFloorColor());
        slopeState = FLAT;
    }
    else if(moveResult == -2){//slope up
        n->updatePosition(moveto);
        //n->setTile(3);
        slopeState = GOUP;
    }
    else if(moveResult == -3){//slope down
        n->updatePosition(moveto);
        slopeState = GODOWN;
    }
    else{
        n->setTile(black,moveto);
        n->updateRotation(moveto);
        slopeState = FLAT;
    }
    debug_buf = "Move Result       : " + String(moveResult);
    Serial.println(debug_buf);
    return n->isHome();
}