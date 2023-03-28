#pragma once
#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <windows.h>
#include "Passenger.h"
#include <QThread>
#include "ui_mainwindow.h"
#include <string>
#include <queue>
using namespace std;

void ui_init(Ui::MainWindow *ui_ptr);

class Elevator: public QThread {
    Q_OBJECT

private:
    friend class Passenger;
    bool ServeFloors[21]; //服务为true，不服务为false
    int DoorTime;//开关门的时间
    int FloorTime;//每上升一层需要的时间

    int MaxNum;//乘客人数限制
    int MaxWeight;//乘客重量限制
    int ID;

    int PeopleCount;
    int WeightCount;
    int CurrentFloor;
    int Status;//状态：0-->static  1-->up  2-->down

    //电梯的调度采用LOOK(SC) 或者 EDF_SCAN算法，大于电梯当前层的，放到upsequence数组中，按从小到大顺序排列；
    //小于电梯当前层的，放到downsequence数组中，按从大到小顺序排列；
    vector<int> upsequence;
    vector<int> downsequence;

    /*
    从小到大排序使用 sort(vector.begin(), vector.end());
    从大到小排序使用 sort(vector.begin(), vector.end(),greater<int>());
    */

    QTextBrowser* qtext[4];

public:

    //表示这个乘客在这个楼层中，进入/出去 哪个电梯，三个信息：乘客、楼层、电梯
    vector<Passenger> in_ele[21];//进入电梯的乘客记录;
    vector<Passenger> out_ele[21];//出电梯的乘客记录;

    Elevator(int id, int mn, int mw, int pc, int wc, int cf, int status);
    ~Elevator();
    void operator()() {
            run();
    }

    void setpeoplecount(int people);
    void setweightcount(int weight);
    void setstatus(int status);
    void setcurrentfloor(int floor);
    void Set_Serve_Floor(vector<int> num);
    void Set_Up_Sequence(vector<int> sequence);
    void Set_Down_Sequence(vector<int> sequence);
    void print() {
        cout << "The elevator is No." << ID << "Elevator, " << " Peoplecount:" << PeopleCount << ",WeightCount:" << WeightCount << "KG,Elevator Stay in " << CurrentFloor << " Floor,";
        cout << "MaxWeight:" << WeightCount << "kg，MaxPeople:" << PeopleCount << "" << endl;
        cout << "ServeFloor:";
        for(int i = 0;  i < 21; i++){
            if(ServeFloors[i] == true){
                if(i != 0){
                    cout << i <<",";
                }
                else{
                    cout << "-1,";
                }
            }
        }
        cout << endl << "The elevator is ";
        if (Status == 0) {
            cout << "static status" << endl;
        }
        else if(Status == 1){
            cout << "up status" << endl;
        }
        else {
            cout << "down status" << endl;
        }
    }

    void Show_Upsequence(){
        qDebug() << "Upsequence:" ;
        for(int i = 0; i < upsequence.size(); i++){
            qDebug() << upsequence.at(i) << " ";
        }
    }

    void Show_Downsequence(){
        qDebug() << "Downsequence:" ;
        for(int i = 0; i < downsequence.size(); i++){
            qDebug() << downsequence.at(i) << " ";
        }
    }
    int getpeoplecount();
    int getweightcount();
    int getcurrentfloor();
    int getstatus();

    int calctime(int passengerfloor);
    void serve(int passengerfloor);

    void run();//运行函数

    void moveto(int floor);//运行到指定的楼层
    void waitpeople();//等待乘客进出


    queue<Passenger> Scan_EDF_queue;//所有乘客的等待队列
    //具有相同时限的请求的乘客采用Scan方法的Scan队列
    void Scan_EDF_Run();

signals:
    void signal_arrive_floor(int, int, int);
    void signal_add_one_completed_passenger();
    void signal_run(int);
    void signal_static(int);
    void signal_down(int);
    void signal_up(int);
    void signal_wait(int, int);
    void signal_out(int, int);
    void signal_complete(int, int);
    void signal_in(int, int);
    void signal_move(int, int, int);
};


#endif // ELEVATOR_H
