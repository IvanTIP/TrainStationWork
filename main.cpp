#include <iostream>
#include <thread>
#include <mutex>

std::mutex access;

class Station {
  private:
    bool trainOnStation = false;

  public:
    bool getStatus () {
        return trainOnStation;
    }

    void setStatus (bool status) {
        trainOnStation = status;
    }
};

class Train {
  private:
    char name = '0';
    int time = 0;
    bool status = false;
  public:
    char getName () {
        return name;
    }

    void setName (char inName) {
        name = inName;
    }

    void setTime (int inTime) {
        time = inTime;
    }

    void movement (Station* &station) {
        while (time != 0) {
            time--;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        trainStatus(station);

    }

    void trainStatus (Station* &station) {
        if (station->getStatus()) {
            std::cout << name << " is waiting for an empty place" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        access.lock();
        station->setStatus(true);
        status = true;
        showTrainStatus (station);
        depart();
        status = false;
        showTrainStatus (station);
        station->setStatus(false);
        access.unlock();
    }

    void showTrainStatus (Station* &station) {
        if (station->getStatus() && status) {
            std::cout << name << " arrived at the station" << std::endl;
        } else {
            std::cout << name << " left the station" << std::endl;
        }
    }

    void depart () {
        std::string command = "unknown";
        std::cout << "Waiting for the command \"depart\": " << std::endl;
        std::cin >> command;
        while (command != "depart") {
            std::cout << "Input error! Try again" << std::endl;
            std::cout << "Waiting for the command \"depart\": " << std::endl;
            std::cin >> command;
        }
    }
};

void trainInit (Train* &train) {
    for (int i = 0;i < 3;i++) {
        int time;
        if (i == 0) {
            train[i].setName('A');
        } else if (i == 1) {
            train[i].setName('B');
        } else {
            train[i].setName('C');
        }
        std::cout << "Enter the travel time to the train station for " << train[i].getName() << ": ";
        std::cin >> time;
        train[i].setTime(time);
    }
}

int main() {
    Train* train = new Train[3];
    Station* station = new Station;
    trainInit(train);
    std::thread tr1 (&Train::movement, train[0], std::ref(station));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::thread tr2 (&Train::movement, train[1], std::ref(station));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::thread tr3 (&Train::movement, train[2], std::ref(station));
    tr1.join();
    tr2.join();
    tr3.join();
    delete[] train;
    return 0;
}
