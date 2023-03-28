#include "Elevator.h"


Ui::MainWindow *ui = nullptr;


void ui_init(Ui::MainWindow *ui_ptr)
{
    ui = ui_ptr;
}

Elevator::Elevator(int id, int mn, int mw, int pc, int wc, int cf, int status) {
    ID = id;
    DoorTime = 10;
    FloorTime = 1;
    MaxNum = mn;
    MaxWeight = mw;
    PeopleCount = pc;
    WeightCount = wc;
    CurrentFloor = cf;
    Status = status;
    qtext[0] = ui->textBrowser_0;
    qtext[1] = ui->textBrowser_1;
    qtext[2] = ui->textBrowser_2;
    qtext[3] = ui->textBrowser_3;
}

Elevator::~Elevator() {}

void Elevator::setpeoplecount(int people) {
    this->PeopleCount = people;
}

void Elevator::setweightcount(int weight) {
    this->WeightCount = weight;
}

void Elevator::setcurrentfloor(int floor) {
    this->CurrentFloor = floor;
}

void Elevator::setstatus(int status) {
    this->Status = status;
}

void Elevator::Set_Serve_Floor(vector<int> num) {
    for (int i = 0; i< 21; i++){
        ServeFloors[i] = false;
    }
    for (int i = 0; i < num.size(); i++) {
        ServeFloors[num[i]] = true;
    }
}

void Elevator::Set_Up_Sequence(vector<int> sequence) {
    upsequence = sequence;
    //for (int i = 0; i < sequence.size(); i++) {
    //    upsequence.push_back(sequence[i]);
    //}
}

void Elevator::Set_Down_Sequence(vector<int> sequence) {
    downsequence = sequence;
    //for (int i = 0; i < sequence.size(); i++) {
    //    downsequence.push_back(sequence[i]);
    //}
}

int Elevator::getpeoplecount() {
    return PeopleCount;
}

int Elevator::getweightcount() {
    return WeightCount;
}

int Elevator::getcurrentfloor() {
    return CurrentFloor;
}

int Elevator::getstatus() {
    return Status;
}

int Elevator::calctime(int passengerfloor) {
    //到此目标楼层计算时间，根据时间进行计算安排的优先级
    int Need_time = 0;
    int temp = CurrentFloor;
    //cout << "temp = " << temp << endl;
    if (ServeFloors[passengerfloor] == false) {
        return 100000;
    }
    if (Status == 0) {
        //电梯只在1楼静止
        Need_time = abs(passengerfloor - 1);
        return Need_time;
    }

    if (CurrentFloor > passengerfloor) {
        if (Status == 1) {
            // 1. 电梯往上走且人在电梯当前楼层的下面
            for (int i = 0; i < upsequence.size(); i++) {
                Need_time += (upsequence[i] - temp);
                temp = upsequence[i];
                //cout << "temp = " << temp << endl;
                Need_time += 10;
            }
            for (int i = 0; i < downsequence.size(); i++) {
                if (downsequence[i] >= passengerfloor) {
                    Need_time += (temp - downsequence[i]);
                    temp = downsequence[i];
                    //cout << "temp = " << temp << endl;
                    Need_time += 10;
                }
                else {
                    break;
                }
            }
            Need_time += (temp - passengerfloor);
        }

        else if (Status == 2) {
            // 2. 电梯往下走且人在电梯当前楼层的下面
            for (int i = 0; i < downsequence.size(); i++) {
                if (downsequence[i] >= passengerfloor) {
                    Need_time += (temp - downsequence[i]);
                    temp = downsequence[i];
                    Need_time += 10;
                }
                else {
                    break;
                }
            }
            Need_time += (temp - passengerfloor);
        }
    }
    else if (CurrentFloor == passengerfloor) {
        Need_time = 0;
    }
    else {
        if (Status == 1) {
            // 3. 电梯往上走且人在电梯当前楼层的上面
            for (int i = 0; i < upsequence.size(); i++) {
                if (upsequence[i] <= passengerfloor) {
                    Need_time += (upsequence[i] - temp);
                    temp = upsequence[i];
                    Need_time += 10;
                }
                else {
                    break;
                }
            }
            Need_time += (passengerfloor - temp);
        }
        else if (Status == 2) {
            // 4. 电梯往下走且人在电梯当前楼层上面
            for (int i = 0; i < downsequence.size(); i++) {
                Need_time += (temp - downsequence[i]);
                temp = downsequence[i];
                Need_time += 10;
            }
            for (int i = 0; i < upsequence.size(); i++) {
                if (upsequence[i] <= passengerfloor) {
                    Need_time += (upsequence[i] - temp);
                    temp = upsequence[i];
                    Need_time += 10;
                }
                else {
                    break;
                }
            }
            Need_time += (passengerfloor - temp);
        }
    }
    return Need_time;
}

void Elevator::serve(int passengerfloor) {
    if (passengerfloor >= CurrentFloor) {
        upsequence.push_back(passengerfloor);
        //对up从小到大排序
        sort(upsequence.begin(), upsequence.end());
    }
    else if (passengerfloor < CurrentFloor) {
        downsequence.push_back(passengerfloor);
        sort(downsequence.begin(), downsequence.end(), greater<int>());
    }
}

void Elevator::run() {
    //emit signal_run(ID);
    /*string str1 = "电梯";
    string str2 = "号开始运行";
    string str = str1 + to_string(ID) + str2;
    qtext[ID]->append(str.c_str());*/

    while (1) {
        //如果没有楼层需求，且不在1层，直接回到1层
        if (Status != 0 && upsequence.empty() && downsequence.empty()) {
            moveto(1);
            Status = 0;//设置状态为静止
            emit signal_static(ID);
            /*str2 = "号静止";
            str = str1 + to_string(ID) + str2;
            qtext[ID]->append(str.c_str());*/
        }

        if (Status == 0) {
            //如果这个时候在1层：
            if (!downsequence.empty()) {
                setstatus(2);//设置状态为下行
                emit signal_down(ID);
                /*str2 = "号下行";
                str = str1 + to_string(ID) + str2;
                qtext[ID]->append(str.c_str());*/
            }
            else if (!upsequence.empty()) {
                setstatus(1);//设置状态为上行
                emit signal_up(ID);
                /*str2 = "号上行";
                str = str1 + to_string(ID) + str2;
                qtext[ID]->append(str.c_str());*/
            }
        }
        if (Status == 1) {
            //如果在上行状态中：
            while (!upsequence.empty()) {
                moveto(upsequence[0]);
                upsequence.erase(upsequence.begin());//upsequence.popfront()
                waitpeople();
            }
            Status = 2;//转为下行
            emit signal_down(ID);
            /*str2 = "号下行";
            str = str1 + to_string(ID) + str2;
            qtext[ID]->append(str.c_str());*/
        }
        if (Status == 2) {
            //如果在下行状态中：
            while (!downsequence.empty()) {
                moveto(downsequence[0]);
                downsequence.erase(downsequence.begin());//downsequence.popfront()
                waitpeople();
            }
            Status = 1;
            emit signal_up(ID);
            /*str2 = "号上行";
            str = str1 + to_string(ID) + str2;
            qtext[ID]->append(str.c_str());*/
        }
    }
}

void Elevator::waitpeople() {
    int floor = CurrentFloor;
    if (CurrentFloor == 0) {
        floor = -1;
    }
    //emit signal_wait(ID, floor);
    /*string str1 = "在第";
    string str2 = "层等待乘客进出电梯";
    string str = str1 + to_string(floor) +str2;
    qtext[ID]->append(str.c_str());*/

    while (!out_ele[CurrentFloor].empty()) {
        Passenger people = out_ele[CurrentFloor].back();
        emit signal_out(ID, people.GetID());
        /*str1 = "乘客";
        str2 = "走出电梯";
        str = str1 + to_string(people.GetID()) +str2 + to_string(ID);
        qtext[ID]->append(str.c_str());*/
        out_ele[CurrentFloor].pop_back();

        PeopleCount--;
        //qDebug() << "Test:" << PeopleCount;
        WeightCount = WeightCount - people.GetWeight();
        //qDebug() << "Test:" << WeightCount;

        emit signal_complete(ID, people.GetID());
        /*str1 = "完成乘客";
        str2 = "的请求" ;
        str = str1 + to_string(people.GetID()) +str2 + to_string(ID);
        qtext[ID]->append(str.c_str());*/
        emit signal_add_one_completed_passenger();
    }

    while (!in_ele[CurrentFloor].empty()) {
        //如果进入这一层的人不为空，就持续让他们进入
        Passenger people = in_ele[CurrentFloor].back();

        //如果进不去，让乘客等下一班
        if(MaxNum >= PeopleCount + 1 && MaxWeight - WeightCount >= people.GetWeight()){
            emit signal_in(ID, people.GetID());
            /*str1 = "乘客";
            str2 = "进入电梯";
            str = str1 + to_string(people.GetID()) + str2 + to_string(ID);
            qtext[ID]->append(str.c_str());*/
            PeopleCount++;
            WeightCount = WeightCount + people.GetWeight();

            in_ele[CurrentFloor].pop_back();

            int Des = people.GetDestination();
            serve(Des);
            out_ele[Des].push_back(people);
        }
        else{
            //不满足的话就跳出循环等待下一班 因为一个人登不上 之后的人都登不上
            break;
        }
    }

    Sleep(10 * 1000);//这里设置等待10s
}

void Elevator::moveto(int floor) {
    //此函数仅限于从一个楼层到另一个楼层的移动（其中不能停靠）
    bool state = false;
    if(CurrentFloor <= floor){
        state = true;//表示电梯往上走
    }

    for(;CurrentFloor!=floor;){
        if(state == true){
            int temp = CurrentFloor;
            if (CurrentFloor == 0) {
                temp = -1;
            }

            int uptemp = CurrentFloor + 1;
            if(uptemp == 0){
                uptemp = -1;
            }
            emit signal_move(ID, temp, uptemp);
            /*string str1 = "电梯从第";
            string str2 = "层移动至第";
            string str3 = "层";
            string str = str1 + to_string(temp) + str2 + to_string(uptemp) + str3;
            qtext[ID]->append(str.c_str());*/
            //表示向上行
            CurrentFloor++;
            Sleep(1000);
            emit signal_arrive_floor(ID, CurrentFloor - 1, uptemp);
        }

        else if(state == false){
            int temp = CurrentFloor;
            if (CurrentFloor == 0) {
                temp = -1;
            }

            int downtemp = CurrentFloor - 1;
            if(downtemp == 0){
                downtemp = -1;
            }
            emit signal_move(ID, temp, downtemp);
            /*string str1 = "电梯从第";
            string str2 = "层移动至第";
            string str3 = "层";
            string str = str1 + to_string(temp) + str2 + to_string(downtemp) + str3;
            qtext[ID]->append(str.c_str());*/
            //表示向下行
            CurrentFloor--;
            Sleep(1000);
            emit signal_arrive_floor(ID, CurrentFloor + 1, downtemp);
        }
    }

    //cout << "电梯从第" << temp << "层移动至第" << floor << "层" << endl;

    return;
}

void Elevator::Scan_EDF_Run(){
    emit signal_run(ID);
    /*string str1 = "电梯";
    string str2 = "号开始运行";
    string str = str1 + to_string(ID) + str2;
    qtext[ID]->append(str.c_str());*/

    vector<Passenger> Scan_Passenger;
    while(1){
        Scan_Passenger.clear();
        //根据EDF算法选择请求列队中哪一个是下一个服务对象
        QDateTime Current_time = QDateTime::currentDateTime();

        while(Scan_Passenger.empty() ||  Current_time.secsTo(Scan_EDF_queue.front().GetStart_time()) >= 60){
            Passenger people = Scan_EDF_queue.front();
            Scan_EDF_queue.pop();
            Scan_Passenger.push_back(people);
        }

        int maxfloor = 0, minfloor = 0;
        for(int i = 0 ; i < Scan_Passenger.size() ; i++){
            int Servefloor = Scan_Passenger[i].GetCurrentFloor();

            in_ele[Servefloor].push_back(Scan_Passenger[i]);

            if(Servefloor > maxfloor){
                maxfloor = Servefloor;
            }
            if(Servefloor < minfloor){
                minfloor = Servefloor;
            }
        }

        if(minfloor < CurrentFloor){
            //需要先下行
            Status = 2;
            for(int i = CurrentFloor; CurrentFloor!=minfloor; CurrentFloor--){
                if(!in_ele[i].empty() || !out_ele[i].empty()){
                    waitpeople();
                }
                moveto(CurrentFloor - 1);
            }
        }


        if(maxfloor >= CurrentFloor){
            Status = 1;
            for(int i = CurrentFloor;CurrentFloor!=maxfloor;CurrentFloor++){
                if(!in_ele[i].empty() || !out_ele[i].empty()){
                    waitpeople();
                }
                moveto(CurrentFloor + 1);
            }
        }

        //运行完这一批后根据Scan算法应该回到1楼
        moveto(1);
        Status = 0;
    }
}
