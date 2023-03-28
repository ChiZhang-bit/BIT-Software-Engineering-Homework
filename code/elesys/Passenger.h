#ifndef PASSENGER_H
#define PASSENGER_H

#include "QtCore"
class Passenger{
private:
    //friend class Elevator;
    int ID;
    int Weight;
    int Destinationfloor;//乘客目标楼层，电梯内按键
    int Currentfloor;//乘客当前位置
    bool Direction;//false: down, true: up 乘客在外的按键

    QDateTime Start_time;//乘客发起上电梯请求的时间

    int elevator_id;//乘坐的几号电梯
public:
    Passenger(int id, int w, int d, int c);
    ~Passenger();
    int GetID();
    int GetWeight();
    int GetDestination();
    int GetCurrentFloor();
    QDateTime GetStart_time();
    //void Outdoor_start(ElevatorSystem ES);
};

#endif // PASSENGER_H
