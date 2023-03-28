#include "Elevator.h"
#include "ElevatorSystem.h"
//#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent>
#include <thread>
#include "QtCore"

//初始化四部电梯
	/*
		| 电梯编号  | 服务楼层        | 乘客人数限制 | 重量限制 |
		|------     |-------------  |--------     |--------|
		| 1         | 所有楼层       | 10          | 800kg  |
		| 2         | 1-10层         | 10          | 800kg  |
		| 3         | -1，1-10层     | 20          | 1600kg |
		| 4         | -1，1，11-20层 | 20          | 2000kg |
	*/
vector<vector<int>> e_floor;
vector<int> e1_floor = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
vector<int> e2_floor = { 1,2,3,4,5,6,7,8,9,10 };
vector<int> e3_floor = { 0,1,2,3,4,5,6,7,8,9,10 };
vector<int> e4_floor = { 0,1,11,12,13,14,15,16,17,18,19,20 };
int max_num[4] = { 10,10,20,20 };
int max_weight[4] = { 800,800,1600,2000 };

ElevatorSystem::ElevatorSystem() {
    completed_passenger_num = 0;
    //e = new Elevator[4]
    e_floor.push_back(e1_floor);
    e_floor.push_back(e2_floor);
    e_floor.push_back(e3_floor);
    e_floor.push_back(e4_floor);

    for (int i = 0; i < 21; i++) {
        floor_request[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        e[i] = new Elevator(i, max_num[i], max_weight[i], 0, 0, 1, 0);
        e[i]->Set_Serve_Floor(e_floor[i]);
    }

    //QtConcurrent::run(this, &Elevator::run);
    e[0]->start();
    e[1]->start();
    e[2]->start();
    e[3]->start();
    connect(e[0], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(Add_one_completed_passenger()));
    connect(e[1], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(Add_one_completed_passenger()));
    connect(e[2], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(Add_one_completed_passenger()));
    connect(e[3], SIGNAL(signal_add_one_completed_passenger()), this, SLOT(Add_one_completed_passenger()));
}

ElevatorSystem::~ElevatorSystem() {}

int ElevatorSystem::Get_completed_passenger_num() {
    return completed_passenger_num;
}

void ElevatorSystem::Add_one_completed_passenger() {
    completed_passenger_num += 1;
}

void ElevatorSystem::Look_Receive_Request(Passenger people){
    int Currentfloor = people.GetCurrentFloor();
    floor_request[Currentfloor] = 1;
    //cout << "Start to " << Currentfloor << " floor request" << endl;
    int choosen = Dispatching(Currentfloor);
    //cout << "The elevator is " << choosen << " th elevator" << endl;
    e[choosen]->serve(Currentfloor);
    e[choosen]->in_ele[Currentfloor].push_back(people);

    //qDebug() << "OK";
}


void ElevatorSystem::Scan_EDF_Receive_Request(Passenger people){
    int Currentfloor = people.GetCurrentFloor();
    floor_request[Currentfloor] = 1;
    //cout << "Start to " << Currentfloor << " floor request" << endl;
    int choosen = Dispatching(Currentfloor);
    //cout << "The elevator is " << choosen << " th elevator" << endl;
    e[choosen]->Scan_EDF_queue.push(people);
}

void ElevatorSystem::Achieve_Floor(int Destinationfloor){
    floor_request[Destinationfloor] = 0;
}

int ElevatorSystem::Dispatching(int Currentfloor) {
    //这一块主要核心是电梯的调度算法，什么时候调用哪一个电梯，决定采用优先度的排序算法，来决定使用哪个电梯
    //优先级的评估使用电梯到达乘客当前楼层
    vector<int> prior;
    for (int i = 0; i < 4; i++) {
        prior.push_back(e[i]->calctime(Currentfloor));
    }

    int maxPosition = min_element(prior.begin(), prior.end()) - prior.begin();
    return maxPosition;
}
