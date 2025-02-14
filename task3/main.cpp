#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"

int const threadsCounts = 4;  // number of threads

int const bufferSize = 9;


class Buffer
{
private:
	Semaphore mutex;
	Semaphore notFull;
	Semaphore sumCon;
	Semaphore sizeCon;
	bool Awaiting = false;
	bool Bwaiting = false;
	int consumersWaiting = 0;
	int sum = 0;
	std::vector<int> values;

	bool conditionProdA(){
		if(sum > 20 && values.size() < bufferSize){
			return true;
		}
		return false;
	}

	bool conditionProdB(){
		if(values.size()<bufferSize){
			return true;
		}
		return false;
	}

	bool conditionCon(){
		if(values.size()>3){
			return true;
		}
		return false;
	}


	void print(std::string name, int value)
	{
		std::cout << "\n ### " << name << " Item: "<< value << " Size: " << values.size() << " [";
		for (auto v : values)
			std::cout << v << ",";
		std::cout << "] ###\n";
	}

public:
	Buffer() : mutex(1), notFull(0), sumCon(0), sizeCon(0)
	{
	}

	void prodA(int value)
	{
        mutex.p();
        if (!conditionProdA()) {
            mutex.v();
			Awaiting = true;
            sumCon.p();
        }
		Awaiting = false;
		sum += value;
        values.push_back(value);
		print("A", value);
        if (conditionCon() && consumersWaiting > 0) {
            sizeCon.v();
        } else {
            mutex.v();
        }
	}

    void prodB(int value)
	{
		mutex.p();

        if (!conditionProdB()) {
            mutex.v();
			Bwaiting = true;
            notFull.p();
        }
		Bwaiting = false;
		sum += value;
        values.push_back(value);
		print("B", value);
        if (conditionCon() && consumersWaiting > 0) {
            sizeCon.v();
        } else {
            mutex.v();
        }
	}

	int consume()
	{
		mutex.p();

        if (!conditionCon()) {
            mutex.v();
			consumersWaiting++;
            sizeCon.p();
			consumersWaiting--;
        }
        auto value = values.front();
		sum -= value;
        values.erase(values.begin());
		print("C", value);
        if (conditionProdA() && Awaiting) {
            sumCon.v();
        } else if (conditionProdB() && Bwaiting) {
            notFull.v();
		} else{
			mutex.v();
		}
		return value;
	}
};

Buffer buffer;

void* threadProdA(void* arg)
{
	int *iterations = static_cast<int*>(arg);

	for (int i = 0; i < *iterations; ++i)
	{
		buffer.prodA(rand() % 10 + 1);
	}
	std::cout<<"A done"<<std::endl;
	return NULL;
}

void* threadProdB(void* arg)
{
	int *iterations = static_cast<int*>(arg);

	for (int i = 0; i < *iterations; ++i)
	{
		buffer.prodB(rand() % 15 + 1);
	}
	std::cout<<"B done"<<std::endl;
	return NULL;
}

void* threadCons(void* arg)
{
	int *iterations = static_cast<int*>(arg);

	for (int i = 0; i < *iterations; ++i)
	{
		auto value = buffer.consume();
	}
	std::cout<<"C done"<<std::endl;
	return NULL;
}


int main(int argc, char *argv[])
{
	int producerIterations;
    int consumerIterations;

    if (argc == 3) {
        producerIterations = std::stoi(argv[1]);
        consumerIterations = std::stoi(argv[2]);
    } else {
        producerIterations = 10000;
        consumerIterations = 9997;
    }
#ifdef _WIN32
	HANDLE tid[threadsCounts];
	DWORD id;

	// create threads
	tid[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProdA, 0, 0, &id);
	tid[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProdB, 0, 0, &id);
	tid[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadCons, 0, 0, &id);
	tid[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadCons, 0, 0, &id);

	// wait for threads to finish
	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else
	pthread_t tid[threadsCounts];

	// create threads
	pthread_create(&(tid[0]), NULL, threadProdA, &producerIterations);
	pthread_create(&(tid[1]), NULL, threadProdB, &producerIterations);
	pthread_create(&(tid[2]), NULL, threadCons, &consumerIterations);
	pthread_create(&(tid[3]), NULL, threadCons, &consumerIterations);

	// wait for threads to finish
	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
#endif
	return 0;
}