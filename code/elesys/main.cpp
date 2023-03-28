#include "mainwindow.h"
#include "Elevator.h"
#include "ElevatorSystem.h"
#include "Passenger.h"
#include <QApplication>

void Elevator_Test(){
    /*在Elevator类中，我们主要的算法部分在：
     * 1.run()函数
     * 2.calctime()函数
     * 3.serve()函数
     * 4.moveto()函数
     * 5.waitpeople()函数
     * 6.Scan_EDF_Run()函数
      其他get函数以及set函数中间过程过于简单，只进行简要测试：
    */

    Elevator e_test(1,5,1000,0,0,1,0);
    e_test.setpeoplecount(1);//设置里面有1个人
    e_test.setweightcount(100);//设置里面有100kg
    e_test.setstatus(1);//设置状态为上行

    vector<int> Servefloor = {0,1,2,3,4,5,6,7,8,9,10};//服务-1-10层
    e_test.Set_Serve_Floor(Servefloor);
    e_test.print();
    qDebug()<<"Now PeopleCount:" << e_test.getpeoplecount() << " WeightCount :" << e_test.getweightcount() << " CurrentFloor: " << e_test.getcurrentfloor() << " Status: " << e_test.getstatus() << endl;

    //简单函数的测试到此已经完成

    //calctime的测试
    //测试到4楼的时间：
    qDebug() << e_test.calctime(5) << endl;//ans == 4
    qDebug() << e_test.calctime(20) << endl;//ans == 100000

    vector<int> Up = {6, 9};
    e_test.Set_Up_Sequence(Up);
    qDebug() << e_test.calctime(10) << endl;//ans == 29
    qDebug() << e_test.calctime(0) << endl;//ans == 37

    //现在UP：6，9  || Down：NULL，现在测试serve函数
    e_test.serve(10);
    e_test.serve(0);
    e_test.serve(5);
    e_test.serve(1);

    e_test.Show_Upsequence();//ans:1,5,6,9,10
    e_test.Show_Downsequence();//ans:0

    //moveto
    qDebug() << e_test.getcurrentfloor();//ans = 1
    e_test.moveto(10);
    qDebug() << e_test.getcurrentfloor();//ans = 10
    e_test.moveto(-1);
    qDebug() << e_test.getcurrentfloor();//ans = -1
    e_test.moveto(1);
    qDebug() << e_test.getcurrentfloor();//ans = 1

    //waitpeople
    e_test.setpeoplecount(0);//重新初始化
    e_test.setweightcount(0);
    e_test.setstatus(0);//设置状态为静止

    Passenger p1(1,50,5,1);
    Passenger p2(2,70,10,1);
    Passenger p3(3,65,10,1);
    e_test.in_ele[1].push_back(p1);
    e_test.in_ele[1].push_back(p2);
    e_test.in_ele[1].push_back(p3);
    qDebug() << "==========";
    qDebug() << e_test.getpeoplecount();//ans = 0
    qDebug() << e_test.getweightcount();//ans = 0
    e_test.waitpeople();
    qDebug() << "Currentfloor:" << e_test.getcurrentfloor();//ans = 1
    qDebug() << e_test.getpeoplecount();//ans = 3
    qDebug() << e_test.getweightcount();//ans = 50 + 70 + 65 = 185
    e_test.moveto(5);
    e_test.waitpeople();
    qDebug() << "Currentfloor:" << e_test.getcurrentfloor();//ans = 5
    qDebug() << e_test.getpeoplecount();//ans = 2
    qDebug() << e_test.getweightcount();//ans = 135
    e_test.moveto(10);
    e_test.waitpeople();
    qDebug() << "Currentfloor:" << e_test.getcurrentfloor();//ans = 10
    qDebug() << e_test.getpeoplecount();//ans = 0
    qDebug() << e_test.getweightcount();//ans = 0
    for(int i = 0; i < 20; i++) {
        e_test.in_ele[1].push_back(p2);
    }
    e_test.moveto(1);
    e_test.waitpeople();
    qDebug() << "Currentfloor:" << e_test.getcurrentfloor();//ans = 1
    qDebug() << e_test.getpeoplecount();//ans = 5
    qDebug() << e_test.getweightcount();//ans = 350
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Elevator_Test();
    return a.exec();
}
