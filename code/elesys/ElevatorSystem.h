#ifndef ELEVATORSYSTEM_H
#define ELEVATORSYSTEM_H

#include "Elevator.h"
#include "Passenger.h"
using namespace std;

//不应该让Elesys当作passenger的友元函数，应该让Ele作为友元函数

class ElevatorSystem: public QObject{
    Q_OBJECT

private:
    int floor_request[21];//0表示-1层, 其中1表示那个楼层有请求
    int completed_passenger_num;

public:
    Elevator* e[4];

    ElevatorSystem();
    ~ElevatorSystem();
    int Get_completed_passenger_num();
    //下一行的函数指在这个系统中收到那一层的请求
    void Look_Receive_Request(Passenger people);
    void Scan_EDF_Receive_Request(Passenger people);
    void Achieve_Floor(int);
    //int Dispatching(Passenger people);
    int Dispatching(int Currentfloor);

private slots:
    void Add_one_completed_passenger();
};

#endif // ELEVATORSYSTEM_H
