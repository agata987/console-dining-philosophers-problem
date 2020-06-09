#include "include/Philosopher.h"
#include <ctime>
#include <chrono>
#include <math.h>


Philosopher::Philosopher(std::string name, Fork & left_fork, Fork & right_fork,bool *stop, int * hunger, int number, int * progress, bool * started) : name{name}, left_fork{left_fork}, right_fork{right_fork},thread_(&Philosopher::dine, this)
{
	this->stop = stop;
	this->hunger=hunger;
	this->number=number;
	this->progress=progress;
	this->started=started;

	if(number==4)
	right_neighbor=0;
	else 
	right_neighbor=number+1;

	if(number==0)
	left_neighbor=4;
	else
	left_neighbor=number-1;

	states = new std::string[4];
	states[0]="waiting ";
	states[1]="thinking";
	states[2]="eating  ";


}

Philosopher::~Philosopher()
{
	if(thread_.joinable())
	thread_.join();
}

void Philosopher::think(){
	currentState=1;
	sleep(false);
}


void Philosopher::eat(){
	currentState=2;
	sleep(true);
}

void Philosopher::sleep(bool eating){

	progress[number]=0;

	// seria krotszych uspien
	float time_= randomTime_microseconds();
	float temp =30000; // mikrosekundy

	// ile bedzie serii uspien
	float temp2= time_/temp;

	// do liczenia postepu
	temp2 = 100.0/temp2;
	float temp3_=0;

	if(temp2<1)
	temp2=1;

	do{
		if(*stop)
		break;

		std::this_thread::sleep_for(std::chrono::microseconds((int)temp));

		

		if(time_>temp)
		{
			time_ -= temp;
			if(eating && hunger[number]<100)
			hunger[number] += 1;

			temp3_ += temp2;
			if(temp3_<=100.0)
			progress[number] = (int)temp3_;
		}
		else
		{
			temp = time_;
			time_=0;
			if(eating)
			hunger[number] = 100;
			progress[number]=100;
		}

	} while(time_>0 && !*stop);


}


void Philosopher::dine(){

	std::this_thread::sleep_for(std::chrono::milliseconds(number*1000));
	started[number]=true;

	while(!*stop)
	{
		think();

		currentState=0;

		// sprawdza czy ktorys z sasiadow jest w stanie krytycznym i czeka (chyba ze sam jest w stanie krytycznym gorszym niz oni)
		while(((hunger[left_neighbor]<0.4||hunger[right_neighbor]<0.4))&& !(hunger[number]<0.4 && hunger[number] <= hunger[left_neighbor]&&hunger[number]<=hunger[right_neighbor]))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(300));			
		}

		// don't actually take the locks yet
		std::unique_lock<std::mutex> lock_left_fork(left_fork, std::defer_lock);
		std::unique_lock<std::mutex> lock_right_fork(right_fork, std::defer_lock);

		// lock both unique locks without the deadlock
		std::lock(lock_left_fork,lock_right_fork);

		eat();
	}

}

int Philosopher::randomTime_microseconds()
{
	srand(time(NULL));
	int time_ = (rand()%11 + 25) * 100000;
	time_ += number*100000;

	return time_;
}
