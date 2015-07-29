#pragma once
//#include <time.h>
#include <chrono>
#include <iostream>
#include <iostream>
using namespace std;

class Timer {
private:
	double begTime;
public:
	Timer()
	{
		begTime = clock();
	}
	void Reset() {
		begTime = clock();
	}

	double Elapsed() {
		return (double)(clock() - begTime);
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= Elapsed();
	}
};

class ChronoTimer
{
private:
	/*char mbstr[100];
	std::chrono::system_clock::time_point startTime, endTime;
	std::time_t now_c;*/
	//typedef std::chrono::high_resolution_clock Time;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

public:
	void StartNow() 
	{
		startTime = std::chrono::high_resolution_clock::now();
	}
	long getNanosecDuration()
	{
		endTime = std::chrono::high_resolution_clock::now();
		//	std::chrono::duration<float> fs = startTime - endTime;
		//std::chrono::milliseconds us = std::chrono::duration_cast<std::chrono::milliseconds> fs;
		return (long)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
	}
	long getMilisecDuration()
	{
		endTime = std::chrono::high_resolution_clock::now();
		//	std::chrono::duration<float> fs = startTime - endTime;
		//std::chrono::milliseconds us = std::chrono::duration_cast<std::chrono::milliseconds> fs;
		return (long)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	}
	/*std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(startTime - std::chrono::hours(24));
	std::strftime(mbstr, sizeof(mbstr), "%A %c", localtime(&now_c));*/

};