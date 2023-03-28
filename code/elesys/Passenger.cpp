#include "Passenger.h"

Passenger::Passenger(int id, int w, int d, int c){
    ID = id;
    Weight = w;
    if(d == -1){
        Destinationfloor = 0;
    }
    else{
        Destinationfloor = d;
    }

    if(c == -1){
        Currentfloor = 0;
    }
    else{
        Currentfloor = c;
    }
    elevator_id = 0;
    Direction = (d - c);
    Start_time = QDateTime::currentDateTime();
}

Passenger::~Passenger(){}

int Passenger::GetID() {
    return ID;
}

int Passenger::GetDestination(){
    return Destinationfloor;
}

int Passenger::GetCurrentFloor(){
    return Currentfloor;
}

int Passenger::GetWeight(){
    return Weight;
}

QDateTime Passenger::GetStart_time(){
    return Start_time;
}

//void Passenger::Outdoor_start(ElevatorSystem ES){
//    //这个模块指乘客在电梯外按下按键（上或下）
//    //向电梯运行系统中发送相应的信号,加入到电梯系统的队列之中
//    ES.Receive_Request(this->Currentfloor);
//}

