#include <iostream>
#include <queue>
#include <thread>
#include "monitor.h"
#include <cstdlib>
#include <ctime>

class BufferMonitor : public Monitor {
private:
    std::queue<int> buffer;
    int sum = 0;
    const int BUFFER_SIZE;
    Condition Full, ACondition,ConCondition;
    bool Awaiting = false;
	bool Bwaiting = false;
	int consumersWaiting = 0;
    bool conditionProdA(){
		if(sum > 20 && buffer.size() < BUFFER_SIZE){
			return true;
		}
		return false;
	}

	bool conditionProdB(){
		if(buffer.size()<BUFFER_SIZE){
			return true;
		}
		return false;
	}

	bool conditionCon(){
		if(buffer.size()>3){
			return true;
		}
		return false;
	}

public:
    BufferMonitor(int size) : BUFFER_SIZE(size) {}

    void produceA(int value) {
        enter();
        if (!conditionProdA()) {
            Awaiting = true;
            wait(ACondition);
        }
        Awaiting = false;
        sum += value;
        buffer.push(value);
        std::cout << "A produced: " << value << ". Buffer size: " << buffer.size() << std::endl;
        if(conditionCon() && consumersWaiting > 0)
        {
            signal(ConCondition);
        }
        leave();
    }

    void produceB(int value) {
        enter();
        if(!conditionProdB())
        {
            Bwaiting = true;
            wait(Full);
        }
        Bwaiting = false;
        sum += value;
        buffer.push(value);
        std::cout << "B produced: " << value << ". Buffer size: " << buffer.size() << std::endl;
        if(conditionCon() && consumersWaiting > 0)
        {
            signal(ConCondition);
        }
        leave();
    }

    void consume(int id) {
        enter();
        if(!conditionCon())
        {
            consumersWaiting++;
            wait(ConCondition);
            consumersWaiting--;
        }
        int item = buffer.front();
        sum -= item;
        buffer.pop();
        std::cout << "Consumer " << id << " consumed: " << item << ". Buffer size: " << buffer.size() << std::endl;
        if(conditionProdA() && Awaiting)
        {
            signal(ACondition);
        }
        else if(conditionProdB() && Bwaiting)
        {
            signal(Full);
        }
        leave();
    }
};

void producerAThread(BufferMonitor &monitor, int iterations) {
    while(iterations > 0){
        monitor.produceA(rand() % 10 + 5 );
        iterations--;
    }
}

void producerBThread(BufferMonitor &monitor, int iterations) {
    while(iterations > 0){
        monitor.produceB(rand() % 10 + 5 );
        iterations--;
    }
}

void consumerThread(BufferMonitor &monitor, int id, int iterations) {
    while(iterations > 0){
        monitor.consume(id);
        iterations--;
    }
}

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));

    int producerIterations;
    int consumerIterations;

    if (argc == 3) {
        producerIterations = std::stoi(argv[1]);
        consumerIterations = std::stoi(argv[2]);
    } else {
        producerIterations = 10000;
        consumerIterations = 10000;
    }

    BufferMonitor monitor(9);

    std::thread producerA(producerAThread, std::ref(monitor), producerIterations);
    std::thread producerB(producerBThread, std::ref(monitor), producerIterations);
    std::thread consumer1(consumerThread, std::ref(monitor), 1, consumerIterations);
    std::thread consumer2(consumerThread, std::ref(monitor), 2, consumerIterations);

    producerA.join();
    producerB.join();
    consumer1.join();
    consumer2.join();

    return 0;
}